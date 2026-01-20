#include "Smelt.h"

#include "Distributor.h"

namespace CRAFT
{
	void SMELT::InitData()
	{
		forgeKywd = RE::TESForm::LookupByID<RE::BGSKeyword>(0x00088105);        //CraftingSmithingForge
		smeltKywd = RE::TESForm::LookupByID<RE::BGSKeyword>(0x000A5CCE);        //CraftingSmithingSmelter
		tanningRackKywd = RE::TESForm::LookupByID<RE::BGSKeyword>(0x0007866A);  //CraftingSmithingTanningRack

		CraftingBase::InitData(rawMap);
	}

	bool SMELT::CreateRecipe(TYPE a_type, RE::TESBoundObject* a_item, std::int32_t a_numRequired)
	{
		auto formID = a_item->GetFormID();

		auto it = formidMap.find(formID);
		if (it == formidMap.end()) {
			return false;
		}

		return CreateRecipe(a_type, a_item, it->second.form, it->second.count, a_numRequired);
	}

	bool SMELT::CreateRecipe(TYPE a_type, RE::TESBoundObject* a_item, RE::TESForm* a_ingot, std::uint16_t a_numConstructed, std::int32_t a_numRequired)
	{
		if (IsBlacklisted(a_item)) {
			return false;
		}

		const auto factory = RE::IFormFactory::GetConcreteFormFactoryByType<RE::BGSConstructibleObject>();

		if (auto constructibleObj = factory ? factory->Create() : nullptr) {
			constructibleObj->benchKeyword = a_ingot->GetFormID() == tanningRackMat ? tanningRackKywd : smeltKywd;
			constructibleObj->requiredItems.AddObjectToContainer(a_item, a_numRequired, nullptr);
			constructibleObj->createdItem = a_ingot;

			RE::TESConditionItem* equippedNode = nullptr;
			if (a_type != TYPE::kClutter) {
				equippedNode = new RE::TESConditionItem;
				equippedNode->next = nullptr;
				equippedNode->data.flags.isOR = true;
				equippedNode->data.comparisonValue.f = 0.0f;
				equippedNode->data.functionData.function = RE::FUNCTION_DATA::FunctionID::kGetEquipped;
				equippedNode->data.functionData.params[0] = a_item;
			}
			auto itemCountNode = new RE::TESConditionItem;
			itemCountNode->next = equippedNode;
			itemCountNode->data.comparisonValue.f = static_cast<float>(a_numRequired);
			itemCountNode->data.flags.opCode = RE::CONDITION_ITEM_DATA::OpCode::kGreaterThanOrEqualTo;
			itemCountNode->data.functionData.function = RE::FUNCTION_DATA::FunctionID::kGetItemCount;
			itemCountNode->data.functionData.params[0] = a_item;

			constructibleObj->conditions.head = itemCountNode;

			std::uint16_t numConstructed = a_numConstructed;
			if (numConstructed == 0) {
				RE::TESBoundObject* item = a_item;
				if (auto armor = item->As<RE::TESObjectARMO>(); armor && armor->templateArmor) {
					while (armor && armor->templateArmor) {
						armor = armor->templateArmor;
					}
					item = armor;
				} else if (auto weap = item->As<RE::TESObjectWEAP>(); weap && weap->templateWeapon) {
					while (weap && weap->templateWeapon) {
						weap = weap->templateWeapon;
					}
					item = weap;
				}

				Manager::GetSingleton()->ForEachConstructible([&](const auto& cobj) {
					if (cobj && cobj->benchKeyword == forgeKywd && cobj->createdItem == item) {
						numConstructed = static_cast<std::uint16_t>(cobj->requiredItems.GetObjectCount(static_cast<RE::TESBoundObject*>(a_ingot)));
						return RE::BSContainer::ForEachResult::kStop;
					}
					return RE::BSContainer::ForEachResult::kContinue;
				});

				if (numConstructed == 0) {
					auto itemGold = item->GetGoldValue();
					auto ingotGold = a_ingot->GetGoldValue();

					auto itemWeight = item->GetWeight();
					auto ingotWeight = a_ingot->GetWeight();

					if (itemGold > 0 && ingotGold > 0 && itemWeight > 0.0f && ingotWeight > 0.0f) {
						numConstructed = static_cast<std::uint16_t>(std::min(itemWeight / ingotWeight, itemGold * 0.5f / ingotGold));
					} else if (itemWeight > 0.0f && ingotWeight > 0.0f) {
						numConstructed = static_cast<std::uint16_t>(itemWeight / ingotWeight);
					} else if (itemGold > 0 && ingotGold > 0) {
						numConstructed = static_cast<std::uint16_t>(itemGold * 0.5f / ingotGold);
					}
				}
			}

			std::uint16_t cap = 0;
			switch (a_type) {
			case TYPE::kArmor:
				cap = maxArmorAmount;
				break;
			case TYPE::kWeap:
				cap = maxWeapAmount;
				break;
			case TYPE::kJewel:
				cap = maxJewelryAmount;
				break;
			case TYPE::kClutter:
				cap = maxClutterAmount;
				break;
			default:
				std::unreachable();
			}
			if (cap > 0) {
				numConstructed = std::clamp(numConstructed, static_cast<uint16_t>(1), cap);
			} else if (numConstructed == 0) {
				numConstructed = 1;
			}

			constructibleObj->data.numConstructed = numConstructed;

			Manager::GetSingleton()->AddGeneratedConstructible(constructibleObj);

			switch (a_type) {
			case TYPE::kArmor:
				armorCount++;
				break;
			case TYPE::kWeap:
				weapCount++;
				break;
			case TYPE::kJewel:
				jewelryCount++;
				break;
			case TYPE::kClutter:
				miscObjCount++;
				break;
			default:
				std::unreachable();
			}

			return true;
		}

		return false;
	}
}
