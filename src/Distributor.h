#pragma once

namespace CRAFT
{
	enum TYPE : std::uint32_t
	{
		kArmor = 0,
		kWeap = 1,
		kJewel = 2,
		kClutter = 3
	};

	using RawMap = frozen::map<std::string_view, RE::FormID, 61>;
	using KeywordMap = std::map<std::string, std::pair<RE::TESForm*, std::uint16_t>>;
	using FormIDMap = std::map<RE::FormID, std::pair<RE::TESForm*, std::uint16_t>>;

	using CustomINIData = std::vector<std::string>;

	inline std::vector<RE::BGSConstructibleObject*> generatedConstructibles;

	inline void get_data(const RawMap& a_rawMap, KeywordMap& a_map)
	{
		for (auto& [keyword, formID] : a_rawMap) {
			if (auto form = RE::TESForm::LookupByID(formID); form) {
				a_map[std::string{ keyword }] = { form, static_cast<std::uint16_t>(0) };
			}
		}
	}

	inline void get_custom_data(const CustomINIData& a_customVec, KeywordMap& a_keywordMap, FormIDMap& a_formidMap)
	{
		for (auto& value : a_customVec) {
			auto sections = string::split(value, "|");
			std::ranges::for_each(sections, [](auto& str) { string::trim(str); });

			//[FORMID ~ ESP]
			RE::TESForm* createdItem = nullptr;
			try {
				if (auto str = sections.at(0); str.contains(" ~ ")) {
					if (auto formIDpair = string::split(str, " ~ "); !formIDpair.empty()) {
						auto formID = string::lexical_cast<RE::FormID>(formIDpair.at(0), true);
						auto esp = formIDpair.at(1);

						createdItem = RE::TESDataHandler::GetSingleton()->LookupForm(formID, esp);
					}
				} else {
					auto formID = string::lexical_cast<RE::FormID>(str, true);
					createdItem = RE::TESForm::LookupByID(formID);
				}
			} catch (...) {
			}

			if (!createdItem) {
				continue;
			}

			//COUNT
			std::uint16_t count = 0;
			try {
				count = string::lexical_cast<std::uint16_t>(sections.at(2));
			} catch (...) {
			}

			//KEYWORDS
			try {
				auto split_str = string::split(sections.at(1), ",");
				std::ranges::for_each(split_str, [](auto& str) { string::trim(str); });

				for (auto& str : split_str) {
					if (str.find(" ~ ") != std::string::npos) {
						if (auto formIDpair = string::split(str, " ~ "); !formIDpair.empty()) {
							auto formID = string::lexical_cast<RE::FormID>(formIDpair.at(0), true);
							auto esp = formIDpair.at(1);

							if (auto item = RE::TESDataHandler::GetSingleton()->LookupForm(formID, esp); item) {
								a_formidMap[item->GetFormID()] = { createdItem, count };
							}
						}
					} else if (str.contains("0X") || str.contains("0x")) {
						auto formID = string::lexical_cast<RE::FormID>(str, true);
						if (auto item = RE::TESForm::LookupByID(formID); item) {
							a_formidMap[item->GetFormID()] = { createdItem, count };
						}
					} else {
						a_keywordMap[str] = { createdItem, count };
					}
				}
			} catch (...) {
			}
		}
	}

	void LoadOverwrites();

	void LoadSettings();

	void Distribute();

	namespace FORGE
	{
		inline RE::BGSKeyword* kywd;
	}

	namespace TEMPER
	{
		inline constexpr RawMap rawMap = {
			{ "ArmorMaterialBearStormcloak"sv, 0xDB5D2 },
			{ "ArmorMaterialBlades"sv, 0x5ACE5 },
			{ "ArmorMaterialDaedric"sv, 0x5AD9D },
			{ "ArmorMaterialDragonPlate"sv, 0x3ADA4 },
			{ "ArmorMaterialDragonscale"sv, 0x3ADA3 },
			{ "ArmorMaterialDwarven"sv, 0xDB8A2 },
			{ "ArmorMaterialEbony"sv, 0x5AD9D },
			{ "ArmorMaterialElven"sv, 0x5AD9F },
			{ "ArmorMaterialElvenGilded"sv, 0x5ADA0 },
			{ "ArmorMaterialFalmer"sv, 0x3AD57 },
			{ "ArmorMaterialForsworn"sv, 0xDB5D2 },
			{ "ArmorMaterialGlass"sv, 0x5ADA1 },
			{ "ArmorMaterialHide"sv, 0xDB5D2 },
			{ "ArmorMaterialImperialHeavy"sv, 0x5ACE5 },
			{ "ArmorMaterialImperialLight"sv, 0xDB5D2 },
			{ "ArmorMaterialImperialStudded"sv, 0xDB5D2 },
			{ "ArmorMaterialIron"sv, 0x5ACE4 },
			{ "ArmorMaterialIronBanded"sv, 0x5AD93 },
			{ "ArmorMaterialLeather"sv, 0xDB5D2 },
			{ "ArmorMaterialMS02Forsworn"sv, 0xDB5D2 },
			{ "ArmorMaterialOrcish"sv, 0x5AD99 },
			{ "ArmorMaterialPenitus"sv, 0xDB5D2 },
			{ "ArmorMaterialScaled"sv, 0x5AD93 },
			{ "ArmorMaterialSteel"sv, 0x5ACE5 },
			{ "ArmorMaterialSteelPlate"sv, 0x5ACE5 },
			{ "ArmorMaterialStormcloak"sv, 0xDB5D2 },
			{ "ArmorMaterialStudded"sv, 0x5ACE4 },
			{ "ArmorMaterialThievesGuild"sv, 0xDB5D2 },
			{ "ArmorMaterialThievesGuildLeader"sv, 0xDB5D2 },
			{ "DLC1ArmorMaterialDawnguard"sv, 0x5ACE5 },
			{ "DLC1ArmorMaterialFalmerHardened"sv, 0x3AD57 },
			{ "DLC1ArmorMaterialHunter"sv, 0x5ACE5 },
			{ "DLC1ArmorMaterialVampire"sv, 0xDB5D2 },
			{ "DLC1LD_CraftingMaterialAetherium"sv, 0xDB8A2 },
			{ "DLC1WeapMaterialDragonbone"sv, 0x3ADA4 },
			{ "DLC2ArmorMaterialBonemoldHeavy"sv, 0x34CDD },
			{ "DLC2ArmorMaterialChitinHeavy"sv, 0x402B04E },
			{ "DLC2ArmorMaterialChitinLight"sv, 0x402B04E },
			{ "DLC2ArmorMaterialMoragTong"sv, 0xDB5D2 },
			{ "DLC2ArmorMaterialNordicHeavy"sv, 0x5ADA0 },
			{ "DLC2ArmorMaterialStalhrimHeavy"sv, 0x402B06B },
			{ "DLC2ArmorMaterialStalhrimLight"sv, 0x402B06B },
			{ "DLC2WeaponMaterialNordic"sv, 0x5ADA0 },
			{ "DLC2WeaponMaterialStalhrim"sv, 0x402B06B },
			{ "USKPArmorMaterialLinwe"sv, 0xDB5D2 },
			{ "USLEEPArmorMaterialBlackguard"sv, 0xDB5D2 },
			{ "WeapMaterialDaedric"sv, 0x5AD9D },
			{ "WeapMaterialDraugr"sv, 0x5ACE5 },
			{ "WeapMaterialDraugrHoned"sv, 0x5ACE5 },
			{ "WeapMaterialDwarven"sv, 0xDB8A2 },
			{ "WeapMaterialEbony"sv, 0x5AD9D },
			{ "WeapMaterialElven"sv, 0x5AD9F },
			{ "WeapMaterialFalmer"sv, 0x3AD57 },
			{ "WeapMaterialFalmerHoned"sv, 0x3AD57 },
			{ "WeapMaterialGlass"sv, 0x5ADA1 },
			{ "WeapMaterialImperial"sv, 0x5ACE5 },
			{ "WeapMaterialIron"sv, 0x5ACE4 },
			{ "WeapMaterialOrcish"sv, 0x5AD99 },
			{ "WeapMaterialSilver"sv, 0x5ACE3 },
			{ "WeapMaterialSteel"sv, 0x5ACE5 },
			{ "WeapMaterialWood"sv, 0x6F993 }
		};

		inline CustomINIData customINIData;
		inline KeywordMap keywordMap;
		inline FormIDMap formidMap;

		inline RE::BGSKeyword* armorKywd;
		inline RE::BGSKeyword* weapKywd;
		inline RE::BGSPerk* arcanePerk;

		inline std::uint32_t weapCount = 0;
		inline std::uint32_t armorCount = 0;

		inline bool create_recipe(RE::TESBoundObject* a_item, RE::TESForm* a_mat, std::uint16_t a_requiredNum)
		{
			if (a_item == a_mat) {
				return false;
			}

		    bool isArmor = a_item->IsArmor();

			auto& arr = RE::TESDataHandler::GetSingleton()->GetFormArray<RE::BGSConstructibleObject>();
			if (std::ranges::any_of(arr, [&](const auto& constructibleObj) {
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
			auto constructibleObj = factory ? factory->Create() : nullptr;

			if (constructibleObj) {
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

				generatedConstructibles.push_back(constructibleObj);

				isArmor ? armorCount++ : weapCount++;

				return true;
			}

			return false;
		}
	}

    namespace SMELT
	{
		inline constexpr RawMap rawMap = {
			{ "ArmorMaterialBearStormcloak"sv, 0x800E4 },
			{ "ArmorMaterialBlades"sv, 0x5ACE5 },
			{ "ArmorMaterialDaedric"sv, 0x5AD9D },
			{ "ArmorMaterialDragonPlate"sv, 0x3ADA4 },
			{ "ArmorMaterialDragonscale"sv, 0x3ADA3 },
			{ "ArmorMaterialDwarven"sv, 0xDB8A2 },
			{ "ArmorMaterialEbony"sv, 0x5AD9D },
			{ "ArmorMaterialElven"sv, 0x5AD9F },
			{ "ArmorMaterialElvenGilded"sv, 0x5AD9F },
			{ "ArmorMaterialFalmer"sv, 0x3AD57 },
			{ "ArmorMaterialForsworn"sv, 0x800E4 },
			{ "ArmorMaterialGlass"sv, 0x5ADA1 },
			{ "ArmorMaterialHide"sv, 0x800E4 },
			{ "ArmorMaterialImperialHeavy"sv, 0x5ACE5 },
			{ "ArmorMaterialImperialLight"sv, 0x800E4 },
			{ "ArmorMaterialImperialStudded"sv, 0x800E4 },
			{ "ArmorMaterialIron"sv, 0x5ACE4 },
			{ "ArmorMaterialIronBanded"sv, 0x5ACE4 },
			{ "ArmorMaterialLeather"sv, 0x800E4 },
			{ "ArmorMaterialMS02Forsworn"sv, 0x800E4 },
			{ "ArmorMaterialOrcish"sv, 0x5AD99 },
			{ "ArmorMaterialPenitus"sv, 0x800E4 },
			{ "ArmorMaterialScaled"sv, 0x5ACE5 },
			{ "ArmorMaterialSteel"sv, 0x5ACE5 },
			{ "ArmorMaterialSteelPlate"sv, 0x5ACE5 },
			{ "ArmorMaterialStormcloak"sv, 0x800E4 },
			{ "ArmorMaterialStudded"sv, 0x5ACE4 },
			{ "ArmorMaterialThievesGuild"sv, 0x800E4 },
			{ "ArmorMaterialThievesGuildLeader"sv, 0x800E4 },
			{ "DLC1ArmorMaterialDawnguard"sv, 0x5ACE5 },
			{ "DLC1ArmorMaterialFalmerHardened"sv, 0x3AD57 },
			{ "DLC1ArmorMaterialHunter"sv, 0x5ACE5 },
			{ "DLC1ArmorMaterialVampire"sv, 0x800E4 },
			{ "DLC1LD_CraftingMaterialAetherium"sv, 0xDB8A2 },
			{ "DLC1WeapMaterialDragonbone"sv, 0x3ADA4 },
			{ "DLC2ArmorMaterialBonemoldHeavy"sv, 0x34CDD },
			{ "DLC2ArmorMaterialChitinHeavy"sv, 0x402B04E },
			{ "DLC2ArmorMaterialChitinLight"sv, 0x402B04E },
			{ "DLC2ArmorMaterialMoragTong"sv, 0x800E4 },
			{ "DLC2ArmorMaterialNordicHeavy"sv, 0x5ACE5 },
			{ "DLC2ArmorMaterialStalhrimHeavy"sv, 0x402B06B },
			{ "DLC2ArmorMaterialStalhrimLight"sv, 0x402B06B },
			{ "DLC2WeaponMaterialNordic"sv, 0x5ACE5 },
			{ "DLC2WeaponMaterialStalhrim"sv, 0x402B06B },
			{ "USKPArmorMaterialLinwe"sv, 0x800E4 },
			{ "USLEEPArmorMaterialBlackguard"sv, 0x800E4 },
			{ "WeapMaterialDaedric"sv, 0x5AD9D },
			{ "WeapMaterialDraugr"sv, 0x5ACE5 },
			{ "WeapMaterialDraugrHoned"sv, 0x5ACE5 },
			{ "WeapMaterialDwarven"sv, 0xDB8A2 },
			{ "WeapMaterialEbony"sv, 0x5AD9D },
			{ "WeapMaterialElven"sv, 0x5AD9F },
			{ "WeapMaterialFalmer"sv, 0x3AD57 },
			{ "WeapMaterialFalmerHoned"sv, 0x3AD57 },
			{ "WeapMaterialGlass"sv, 0x5ADA1 },
			{ "WeapMaterialImperial"sv, 0x5ACE5 },
			{ "WeapMaterialIron"sv, 0x5ACE4 },
			{ "WeapMaterialOrcish"sv, 0x5AD99 },
			{ "WeapMaterialSilver"sv, 0x5ACE3 },
			{ "WeapMaterialSteel"sv, 0x5ACE5 },
			{ "WeapMaterialWood"sv, 0x6F993 }
		};

		inline constexpr RE::FormID tanningRackMat = 0x800E4;

		inline constexpr std::array<std::string_view, 7> ironMats = { "iron", "pick", "kettle", "lantern", "knife", "scissor", "scapel" };

		inline CustomINIData customINIData;
		inline KeywordMap keywordMap;
		inline FormIDMap formidMap;

		inline RE::BGSKeyword* smeltKywd;
		inline RE::BGSKeyword* tanningRackKywd;

		inline std::uint16_t maxWeapAmount;
		inline std::uint16_t maxArmorAmount;
		inline std::uint16_t maxJewelryAmount;
		inline std::uint16_t maxClutterAmount;

		inline std::uint32_t weapCount = 0;
		inline std::uint32_t armorCount = 0;
		inline std::uint32_t jewelryCount = 0;
		inline std::uint32_t miscObjCount = 0;

		inline bool create_recipe(TYPE a_type, RE::TESBoundObject* a_item, RE::TESForm* a_ingot, std::uint16_t a_numConstructed, std::int32_t a_numRequired = 1)
		{
			const auto factory = RE::IFormFactory::GetConcreteFormFactoryByType<RE::BGSConstructibleObject>();
			auto constructibleObj = factory ? factory->Create() : nullptr;

			if (constructibleObj) {
				constructibleObj->benchKeyword = a_ingot->GetFormID() == tanningRackMat ? tanningRackKywd : smeltKywd;
				constructibleObj->requiredItems.AddObjectToContainer(a_item, a_numRequired, nullptr);
				constructibleObj->createdItem = a_ingot;

				RE::TESConditionItem* equippedNode = nullptr;
				if (a_type != kClutter) {
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

					auto& arr = RE::TESDataHandler::GetSingleton()->GetFormArray<RE::BGSConstructibleObject>();
					auto result = std::ranges::find_if(arr,
						[&](const auto& cobj) { return cobj && cobj->benchKeyword == FORGE::kywd && cobj->createdItem == item; });
					if (result != arr.end()) {
						auto craftRecipe = *result;
						numConstructed = static_cast<std::uint16_t>(craftRecipe->requiredItems.CountObjectsInContainer(static_cast<RE::TESBoundObject*>(a_ingot)));
					}

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

						std::uint16_t cap = 0;
						switch (a_type) {
						case kArmor:
							cap = maxArmorAmount;
							break;
						case kWeap:
							cap = maxWeapAmount;
							break;
						case kJewel:
							cap = maxJewelryAmount;
							break;
						case kClutter:
							cap = maxClutterAmount;
							break;
						}
						if (cap > 0) {
							numConstructed = std::clamp(numConstructed, static_cast<uint16_t>(1), cap);
						} else if (numConstructed == 0) {
							numConstructed = 1;
						}
					}
				}
				constructibleObj->data.numConstructed = numConstructed;

				generatedConstructibles.push_back(constructibleObj);

				switch (a_type) {
				case kArmor:
                    armorCount++;
					break;
				case kWeap:
                    weapCount++;
					break;
				case kJewel:
                    jewelryCount++;
					break;
				case kClutter:
                    miscObjCount++;
					break;
				}

				return true;
			}

			return false;
		}
	}
}
