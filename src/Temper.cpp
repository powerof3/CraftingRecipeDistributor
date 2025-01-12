#include "Temper.h"

#include "Distributor.h"

namespace CRAFT
{
	void TEMPER::InitData()
	{
		armorKywd = RE::TESForm::LookupByID<RE::BGSKeyword>(0x000ADB78);  //CraftingSmithingArmorTable
		weapKywd = RE::TESForm::LookupByID<RE::BGSKeyword>(0x00088108);   //CraftingSmithingSharpeningWheel
		arcanePerk = RE::TESForm::LookupByID<RE::BGSPerk>(0x0005218E);    //ArcaneBlacksmith
		
		CraftingBase::InitData(rawMap);
	}

	bool TEMPER::CreateRecipe(RE::TESBoundObject* a_item)
	{
		auto formID = a_item->GetFormID();

		auto it = formidMap.find(formID);
		if (it == formidMap.end()) {
			return false;
		}

		return CreateRecipe(a_item, it->second.form, it->second.count);
	}

	bool TEMPER::CreateRecipe(RE::TESBoundObject* a_item, RE::TESForm* a_mat, std::uint16_t a_requiredNum)
	{
		if (a_item == a_mat || IsBlacklisted(a_item)) {
			return false;
		}

		bool isArmor = a_item->IsArmor();

		if (auto& arr = RE::TESDataHandler::GetSingleton()->GetFormArray<RE::BGSConstructibleObject>(); std::ranges::any_of(arr, [&](const auto& constructibleObj) {
				return constructibleObj && constructibleObj->benchKeyword == (isArmor ? armorKywd : weapKywd) && constructibleObj->createdItem == a_item;
			})) {
			return false;
		}

		if (!isArmor) {
			if (const auto weap = static_cast<RE::TESObjectWEAP*>(a_item); weap && (weap->templateWeapon || weap->IsStaff())) {
				return false;
			}
		} else if (const auto armor = static_cast<RE::TESObjectARMO*>(a_item); armor && armor->templateArmor) {
			return false;
		}

		const auto factory = RE::IFormFactory::GetConcreteFormFactoryByType<RE::BGSConstructibleObject>();

		if (auto constructibleObj = factory ? factory->Create() : nullptr) {
			constructibleObj->benchKeyword = isArmor ? armorKywd : weapKywd;
			constructibleObj->requiredItems.AddObjectToContainer(static_cast<RE::TESBoundObject*>(a_mat), a_requiredNum, nullptr);
			constructibleObj->createdItem = a_item;

			auto perkNode = new RE::TESConditionItem;
			perkNode->next = nullptr;
			perkNode->data.comparisonValue.f = 1.0f;
			perkNode->data.functionData.function = RE::FUNCTION_DATA::FunctionID::kHasPerk;
			perkNode->data.functionData.params[0] = arcanePerk;

			auto enchantNode = new RE::TESConditionItem;
			enchantNode->next = perkNode;
			enchantNode->data.comparisonValue.f = 1.0f;
			enchantNode->data.flags.isOR = true;
			enchantNode->data.flags.opCode = RE::CONDITION_ITEM_DATA::OpCode::kNotEqualTo;
			enchantNode->data.functionData.function = RE::FUNCTION_DATA::FunctionID::kEPTemperingItemIsEnchanted;

			constructibleObj->conditions.head = enchantNode;

			Manager::GetSingleton()->AddGeneratedConstructible(constructibleObj);

			isArmor ? armorCount++ : weapCount++;

			return true;
		}

		return false;
	}
}
