#include "Distributor.h"

namespace CRAFT
{
	void LoadOverwrites()
	{
		logger::info("{:*^30}", "OVERWRITES");

		std::vector<std::string> configs;

		auto constexpr folder = R"(Data\)";
		for (const auto& entry : std::filesystem::directory_iterator(folder)) {
			if (entry.exists() && entry.path().extension() == ".ini"sv) {
				if (const auto path = entry.path().string(); path.find("_CRD") != std::string::npos) {
					configs.push_back(path);
				}
			}
		}

		if (configs.empty()) {
			logger::warn("No .ini files with _CRD suffix were found within the Data folder, aborting...");
			return;
		}

		logger::info("{} matching inis found", configs.size());

		const auto get_ini_data = [](CSimpleIniA& ini, const char* a_type, CustomINIData& a_iniData) {
			CSimpleIniA::TNamesDepend values;
			ini.GetAllValues(a_type, "Recipe", values);
			values.sort(CSimpleIniA::Entry::LoadOrder());

			if (const auto size = values.size(); size > 0) {
				logger::info("	{} entries found : {}", a_type, size);

				a_iniData.reserve(values.size());
				for (auto& value : values) {
					a_iniData.emplace_back(value.pItem);
				}
			} else {
				logger::error("	no {} entries found", a_type);
			}
		};

		for (auto& path : configs) {
			logger::info("ini : {}", path);

			CSimpleIniA ini;
			ini.SetUnicode();
			ini.SetMultiKey();

			if (const auto rc = ini.LoadFile(path.c_str()); rc < 0) {
				logger::error("	couldn't read path");
				continue;
			}

			get_ini_data(ini, "SMELT", SMELT::customINIData);
			get_ini_data(ini, "TEMPER", TEMPER::customINIData);
		}
	}

	void LoadSettings()
	{
		logger::info("{:*^30}", "SETTINGS");

		constexpr auto path = L"Data/SKSE/Plugins/po3_CraftingRecipeDistributor.ini";

		CSimpleIniA ini;
		ini.SetUnicode();

		ini.LoadFile(path);

		SMELT::maxWeapAmount = string::lexical_cast<std::uint16_t>(ini.GetValue("SMELT", "Weapon cap", "0"));
		ini.SetValue("SMELT", "Weapon cap", std::to_string(SMELT::maxWeapAmount).c_str(), ";maximum amount of ingots (or other materials) recieved when smelting weapons", true);

		SMELT::maxArmorAmount = string::lexical_cast<std::uint16_t>(ini.GetValue("SMELT", "Armor cap", "0"));
		ini.SetValue("SMELT", "Armor cap", std::to_string(SMELT::maxArmorAmount).c_str(), ";maximum amount of ingots (or other materials) recieved when smelting armor", true);

		SMELT::maxJewelryAmount = string::lexical_cast<std::uint16_t>(ini.GetValue("SMELT", "Jewelry cap", "0"));
		ini.SetValue("SMELT", "Jewelry cap", std::to_string(SMELT::maxJewelryAmount).c_str(), ";maximum amount of ingots (or other materials) recieved when smelting jewelry", true);

		SMELT::maxClutterAmount = string::lexical_cast<std::uint16_t>(ini.GetValue("SMELT", "Clutter cap", "0"));
		ini.SetValue("SMELT", "Clutter cap", std::to_string(SMELT::maxClutterAmount).c_str(), ";maximum amount of ingots (or other materials) recieved when smelting clutter", true);

		logger::info("SMELT");
		logger::info("	max weapon cap : {}", SMELT::maxWeapAmount);
		logger::info("	max armor cap : {}", SMELT::maxArmorAmount);
		logger::info("	max jewelry cap : {}", SMELT::maxJewelryAmount);
		logger::info("	max clutter cap : {}", SMELT::maxClutterAmount);

		ini.SaveFile(path);
	}

	void Distribute()
	{
		const auto dataHandler = RE::TESDataHandler::GetSingleton();
		if (dataHandler) {
			logger::info("{:*^30}", "PROCESSING");

			auto ironIngot = RE::TESForm::LookupByID<RE::TESObjectMISC>(0x5ACE4);
			auto dwemerIngot = RE::TESForm::LookupByID<RE::TESObjectMISC>(0xDB8A2);
			auto goldIngot = RE::TESForm::LookupByID<RE::TESObjectMISC>(0x5AD9E);
			auto silverIngot = RE::TESForm::LookupByID<RE::TESObjectMISC>(0x5ACE3);

			FORGE::kywd = RE::TESForm::LookupByID<RE::BGSKeyword>(0x00088105);             //CraftingSmithingForge
			SMELT::smeltKywd = RE::TESForm::LookupByID<RE::BGSKeyword>(0x000A5CCE);        //CraftingSmithingSmelter
			SMELT::tanningRackKywd = RE::TESForm::LookupByID<RE::BGSKeyword>(0x0007866A);  //CraftingSmithingTanningRack
			TEMPER::armorKywd = RE::TESForm::LookupByID<RE::BGSKeyword>(0x000ADB78);       //CraftingSmithingArmorTable
			TEMPER::weapKywd = RE::TESForm::LookupByID<RE::BGSKeyword>(0x00088108);        //CraftingSmithingSharpeningWheel
			TEMPER::arcanePerk = RE::TESForm::LookupByID<RE::BGSPerk>(0x0005218E);         //ArcaneBlacksmith

			get_data(SMELT::rawMap, SMELT::keywordMap);
			get_custom_data(SMELT::customINIData, SMELT::keywordMap, SMELT::formidMap);

			get_data(TEMPER::rawMap, TEMPER::keywordMap);
			get_custom_data(TEMPER::customINIData, TEMPER::keywordMap, TEMPER::formidMap);

			for (auto& constructibleObj : dataHandler->GetFormArray<RE::BGSConstructibleObject>()) {
				if (constructibleObj && constructibleObj->benchKeyword == SMELT::smeltKywd) {
					const auto obj = constructibleObj->requiredItems.GetContainerObjectAt(0);
					if (obj.has_value() && constructibleObj->conditions.head == nullptr) {
						auto itemCountNode = new RE::TESConditionItem;
						itemCountNode->next = nullptr;
						itemCountNode->data.comparisonValue.f = static_cast<float>(obj.value()->count);
						itemCountNode->data.flags.opCode = RE::CONDITION_ITEM_DATA::OpCode::kGreaterThanOrEqualTo;
						itemCountNode->data.functionData.function = RE::FUNCTION_DATA::FunctionID::kGetItemCount;
						itemCountNode->data.functionData.params[0] = obj.value()->obj;

						constructibleObj->conditions.head = itemCountNode;
					}
				}
			}

			const auto get_keyword = [](RE::TESBoundObject* a_form, const KeywordMap& a_map) {
				if (const auto keywordForm = a_form->As<RE::BGSKeywordForm>(); keywordForm && keywordForm->keywords) {
					std::span<RE::BGSKeyword*> keywords(keywordForm->keywords, keywordForm->numKeywords);
					for (auto& keyword : keywords | std::views::reverse) {
						if (auto result = a_map.find(std::string(keyword->formEditorID)); result != a_map.end()) {
							return result;
						}
					}
				}
				return a_map.end();
			};

			const auto create_recipes = [&](RE::TESBoundObject* a_form, TYPE a_type) {
				bool didFormID = false;
		
				auto formid = a_form->GetFormID();
				if (SMELT::formidMap.find(formid) != SMELT::formidMap.end()) {
					if (SMELT::create_recipe(a_type, a_form, SMELT::formidMap[formid].first, SMELT::formidMap[formid].second)) {
						didFormID = true;
					}
				}
				if (TEMPER::formidMap.find(formid) != TEMPER::formidMap.end()) {
					if (TEMPER::create_recipe(a_form, TEMPER::formidMap[formid].first, TEMPER::formidMap[formid].second)) {
						didFormID = true;
					}
				}

				if (didFormID) {
					return;
				}

				if (auto result = get_keyword(a_form, SMELT::keywordMap); result != SMELT::keywordMap.end()) {
					auto& [keyword, ingotCount] = (*result);
					auto& [ingot, count] = ingotCount;
					SMELT::create_recipe(a_type, a_form, ingot, count);
				}
				if (auto result = get_keyword(a_form, TEMPER::keywordMap); result != TEMPER::keywordMap.end()) {
					auto& [keyword, matCount] = (*result);
					auto& [mat, count] = matCount;
					TEMPER::create_recipe(a_form, mat, count);
				}
			};

			for (auto& weapon : dataHandler->GetFormArray<RE::TESObjectWEAP>()) {
				if (weapon && weapon->GetPlayable()) {
					if (auto name = std::string(weapon->GetName()); !name.empty()) {
						create_recipes(weapon, TYPE::kWeap);
					}
				}
			}
			logger::info("Finished WEAP");

			for (auto& armor : dataHandler->GetFormArray<RE::TESObjectARMO>()) {
				if (armor && armor->GetPlayable()) {
					auto name = std::string(armor->GetName());
					if (!name.empty()) {
						if (!armor->HasKeywordString("ArmorJewelry"sv)) {
							create_recipes(armor, TYPE::kArmor);
						} else {
							bool didFormID = false;
							auto formid = armor->GetFormID();
							if (SMELT::formidMap.find(formid) != SMELT::formidMap.end()) {
								if (SMELT::create_recipe(TYPE::kJewel, armor, SMELT::formidMap[formid].first, SMELT::formidMap[formid].second)) {
									didFormID = true;
								}
							}
							if (didFormID) {
								continue;
							}
							RE::TESBoundObject* ingot = nullptr;
							std::uint16_t numConstructed = 0;
							if (auto result = get_keyword(armor, SMELT::keywordMap); result != SMELT::keywordMap.end()) {
								auto& [keyword, ingotCount] = (*result);
								ingot = static_cast<RE::TESBoundObject*>(ingotCount.first);
								numConstructed = ingotCount.second;
							} else {
								if (const auto templateArmor = armor->templateArmor; templateArmor) {
									name = std::string(templateArmor->GetName());
								}
								if (string::icontains(armor->worldModels[0].model, "gold") || string::icontains(name, "gold")) {
									ingot = goldIngot;
								} else if (string::icontains(armor->worldModels[0].model, "silver") || string::icontains(name, "silver")) {
									ingot = silverIngot;
								}
							}
							if (ingot) {
								std::int32_t numRequired = 1;
								
								auto itemWeight = armor->GetWeight();
								auto ingotWeight = ingot->GetWeight();
								if (itemWeight == 0.0f) {
									itemWeight = 0.1f;
								}
								if (ingotWeight == 0.0f) {
									ingotWeight = 0.1f;
								}
								if (itemWeight < ingotWeight) {
									numRequired = static_cast<std::int32_t>(ingotWeight / itemWeight);
								}
								
								SMELT::create_recipe(TYPE::kJewel, armor, ingot, numConstructed, numRequired);
							}
						}
					}
				}
			}
			logger::info("Finished ARMOR");

			for (auto& miscObj : dataHandler->GetFormArray<RE::TESObjectMISC>()) {
				if (miscObj && miscObj->GetPlayable()) {
					if (auto name = std::string(miscObj->GetName()); !name.empty()) {
						bool didFormID = false;
						auto formid = miscObj->GetFormID();
						if (SMELT::formidMap.find(formid) != SMELT::formidMap.end()) {
							if (SMELT::create_recipe(TYPE::kClutter, miscObj, SMELT::formidMap[formid].first, SMELT::formidMap[formid].second)) {
								didFormID = true;
							}
						}
						if (didFormID) {
							continue;
						}
						RE::TESBoundObject* ingot = nullptr;
						std::uint16_t numConstructed = 0;
						if (auto result = get_keyword(miscObj, SMELT::keywordMap); result != SMELT::keywordMap.end()) {
							auto& [keyword, ingotCount] = (*result);
							ingot = static_cast<RE::TESBoundObject*>(ingotCount.first);
							numConstructed = ingotCount.second;
						} else {
							if (!miscObj->HasKeywordString("VendorItemOreIngot"sv) && miscObj->HasKeywordString("VendorItemClutter"sv) || miscObj->HasKeywordString("VendorItemTool"sv)) {
								if (string::icontains(miscObj->model, "gold") || string::icontains(miscObj->model, "coin")) {
									ingot = goldIngot;
								} else if (string::icontains(miscObj->model, "silver")) {
									ingot = silverIngot;
								} else if (string::icontains(miscObj->model, "dwemer")) {
									ingot = dwemerIngot;
								} else if (std::ranges::any_of(SMELT::ironMats, [&](const auto& str) { return string::icontains(miscObj->model, str); })) {
									ingot = ironIngot;
								}
							}
						}
						if (ingot) {
							std::int32_t numRequired = 1;
							if (numConstructed == 0) {
								auto itemWeight = miscObj->GetWeight();
								auto ingotWeight = ingot->GetWeight();
								if (itemWeight == 0.0f) {
									itemWeight = miscObj->IsGold() ? 0.01f : 0.1f;
								}
								if (ingotWeight == 0.0f) {
									ingotWeight = 0.1f;
								}
								if (itemWeight < ingotWeight) {
									numRequired = static_cast<std::int32_t>(ingotWeight / itemWeight);
								} else {
									numConstructed = static_cast<std::uint16_t>(itemWeight / ingotWeight);
								}
							}
							SMELT::create_recipe(TYPE::kClutter, miscObj, ingot, numConstructed, numRequired);
						}
					}
				}
			}
			logger::info("Finished CLUTTER");

			std::copy(generatedConstructibles.begin(), generatedConstructibles.end(), std::back_inserter(dataHandler->GetFormArray<RE::BGSConstructibleObject>()));
			generatedConstructibles.clear();

			logger::info("{:*^30}", "RESULT");
			logger::info("SMELT");
			logger::info("	{} weapon recipes added", SMELT::weapCount);
			logger::info("	{} armor recipes added", SMELT::armorCount);
			logger::info("	{} jewelry recipes added", SMELT::jewelryCount);
			logger::info("	{} clutter recipes added", SMELT::miscObjCount);
			logger::info("TEMPER");
			logger::info("	{} weapon recipes added", TEMPER::weapCount);
			logger::info("	{} armor recipes added", TEMPER::armorCount);
		}
	}
}
