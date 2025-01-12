#include "Distributor.h"

namespace CRAFT
{
	void Manager::LoadOverwrites()
	{
		logger::info("{:*^30}", "OVERWRITES");

		std::vector<std::string> configs = clib_util::distribution::get_configs(R"(Data\)", "_CRD"sv);

		if (configs.empty()) {
			logger::warn("No .ini files with _CRD suffix were found within the Data folder, aborting...");
			return;
		}

		logger::info("{} matching inis found", configs.size());

		for (auto& path : configs) {
			logger::info("\tINI : {}", path);

			CSimpleIniA ini;
			ini.SetUnicode();
			ini.SetMultiKey();
			ini.SetAllowKeyOnly();

			if (const auto rc = ini.LoadFile(path.c_str()); rc < 0) {
				logger::error("\t\tcouldn't read INI");
				continue;
			}

			smelt.LoadINIData(ini, "SMELT");
			temper.LoadINIData(ini, "TEMPER");

			smelt.LoadBlackList(ini, "SMELT_BlackList");
			temper.LoadBlackList(ini, "TEMPER_BlackList");
		}
	}

	void Manager::LoadSettings()
	{
		logger::info("{:*^30}", "SETTINGS");

		constexpr auto path = L"Data/SKSE/Plugins/po3_CraftingRecipeDistributor.ini";

		CSimpleIniA ini;
		ini.SetUnicode();

		ini.LoadFile(path);

		ini::get_value(ini, smelt.maxWeapAmount, "SMELT", "Weapon cap", ";maximum amount of ingots (or other materials) recieved when smelting weapons");
		ini::get_value(ini, smelt.maxArmorAmount, "SMELT", "Armor cap", ";maximum amount of ingots (or other materials) recieved when smelting armor");
		ini::get_value(ini, smelt.maxJewelryAmount, "SMELT", "Jewelry cap", ";maximum amount of ingots (or other materials) recieved when smelting jewelry");
		ini::get_value(ini, smelt.maxClutterAmount, "SMELT", "Clutter cap", ";maximum amount of ingots (or other materials) recieved when smelting clutter");

		logger::info("SMELT");
		logger::info("\tmax weapon cap : {}", smelt.maxWeapAmount);
		logger::info("\tmax armor cap : {}", smelt.maxArmorAmount);
		logger::info("\tmax jewelry cap : {}", smelt.maxJewelryAmount);
		logger::info("\tmax clutter cap : {}", smelt.maxClutterAmount);

		void(ini.SaveFile(path));
	}

	void Manager::AddGeneratedConstructible(RE::BGSConstructibleObject* a_obj)
	{
		generatedConstructibles.push_back(a_obj);
	}

	void Manager::InitData()
	{
		ironIngot = RE::TESForm::LookupByID<RE::TESObjectMISC>(0x5ACE4);
		dwemerIngot = RE::TESForm::LookupByID<RE::TESObjectMISC>(0xDB8A2);
		goldIngot = RE::TESForm::LookupByID<RE::TESObjectMISC>(0x5AD9E);
		silverIngot = RE::TESForm::LookupByID<RE::TESObjectMISC>(0x5ACE3);

		smelt.InitData();
		temper.InitData();
	}

	void Manager::CreateStandardRecipes(TYPE a_type, RE::TESBoundObject* a_form)
	{
		bool didFormID = false;

		if (smelt.CreateRecipe(a_type, a_form)) {
			didFormID = true;
		}
		if (temper.CreateRecipe(a_form)) {
			didFormID = true;
		}

		if (didFormID) {
			return;
		}

		if (auto formCount = smelt.keywordMap.GetData(a_form)) {
			smelt.CreateRecipe(a_type, a_form, formCount->form, formCount->count);
		}
		if (auto formCount = temper.keywordMap.GetData(a_form)) {
			temper.CreateRecipe(a_form, formCount->form, formCount->count);
		}
	}

	void Manager::CreateJewelryRecipes(RE::TESObjectARMO* a_armor)
	{
		if (smelt.CreateRecipe(TYPE::kJewel, a_armor)) {
			return;
		}

		RE::TESBoundObject* ingot = nullptr;
		std::uint16_t       numConstructed = 0;

		if (auto formCount = smelt.keywordMap.GetData(a_armor)) {
			ingot = static_cast<RE::TESBoundObject*>(formCount->form);
			numConstructed = formCount->count;
		} else {
			std::string_view name = a_armor->GetName();
			if (const auto templateArmor = a_armor->templateArmor; templateArmor) {
				name = templateArmor->GetName();
			}
			if (RE::ArmorContainsModel(a_armor, "gold") || string::icontains(name, "gold")) {
				ingot = goldIngot;
			} else if (RE::ArmorContainsModel(a_armor, "silver") || string::icontains(name, "silver")) {
				ingot = silverIngot;
			}
		}

		if (ingot) {
			std::int32_t numRequired = 1;

			auto itemWeight = a_armor->GetWeight();
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

			smelt.CreateRecipe(TYPE::kJewel, a_armor, ingot, numConstructed, numRequired);
		}
	}

	void Manager::CreateClutterRecipes(RE::TESObjectMISC* a_miscObj)
	{
		if (smelt.CreateRecipe(TYPE::kClutter, a_miscObj)) {
			return;
		}

		RE::TESBoundObject* ingot = nullptr;
		std::uint16_t       numConstructed = 0;

		if (auto formCount = smelt.keywordMap.GetData(a_miscObj)) {
			ingot = static_cast<RE::TESBoundObject*>(formCount->form);
			numConstructed = formCount->count;
		} else {
			if (!a_miscObj->HasKeywordString("VendorItemOreIngot"sv) && (a_miscObj->HasKeywordString("VendorItemClutter"sv) || a_miscObj->HasKeywordString("VendorItemTool"sv))) {
				if (string::icontains(a_miscObj->model, "gold") || string::icontains(a_miscObj->model, "coin")) {
					ingot = goldIngot;
				} else if (string::icontains(a_miscObj->model, "silver")) {
					ingot = silverIngot;
				} else if (string::icontains(a_miscObj->model, "dwemer")) {
					ingot = dwemerIngot;
				} else if (std::ranges::any_of(ironMats, [&](const auto& str) { return string::icontains(a_miscObj->model, str); })) {
					ingot = ironIngot;
				}
			}
		}

		if (ingot) {
			std::int32_t numRequired = 1;

			if (numConstructed == 0) {
				auto itemWeight = a_miscObj->GetWeight();
				auto ingotWeight = ingot->GetWeight();
				if (itemWeight == 0.0f) {
					itemWeight = a_miscObj->IsGold() ? 0.01f : 0.1f;
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

			smelt.CreateRecipe(TYPE::kClutter, a_miscObj, ingot, numConstructed, numRequired);
		}
	}

	void Manager::Clear()
	{
		generatedConstructibles.clear();

		smelt.Clear();
		temper.Clear();
	}

	void Manager::Distribute()
	{
		if (const auto dataHandler = RE::TESDataHandler::GetSingleton(); dataHandler) {
			InitData();

			// patch cobjs from being shown unless player has required items?
			for (auto& cobj : dataHandler->GetFormArray<RE::BGSConstructibleObject>()) {
				if (cobj && cobj->benchKeyword == smelt.smeltKywd) {
					const auto obj = cobj->requiredItems.GetContainerObjectAt(0);
					if (obj.has_value() && cobj->conditions.head == nullptr) {
						auto itemCountNode = new RE::TESConditionItem;
						itemCountNode->next = nullptr;
						itemCountNode->data.comparisonValue.f = static_cast<float>(obj.value()->count);
						itemCountNode->data.flags.opCode = RE::CONDITION_ITEM_DATA::OpCode::kGreaterThanOrEqualTo;
						itemCountNode->data.functionData.function = RE::FUNCTION_DATA::FunctionID::kGetItemCount;
						itemCountNode->data.functionData.params[0] = obj.value()->obj;

						cobj->conditions.head = itemCountNode;
					}
				}
			}

			// generate recipes
			for (auto& weapon : dataHandler->GetFormArray<RE::TESObjectWEAP>()) {
				if (std::string_view name = weapon && weapon->GetPlayable() ? weapon->GetName() : std::string_view{}; !name.empty()) {
					CreateStandardRecipes(TYPE::kWeap, weapon);
				}
			}

			for (auto& armor : dataHandler->GetFormArray<RE::TESObjectARMO>()) {
				if (std::string_view name = armor && armor->GetPlayable() ? armor->GetName() : std::string_view{}; !name.empty()) {
					if (!armor->HasKeywordString("ArmorJewelry"sv)) {
						CreateStandardRecipes(TYPE::kArmor, armor);
					} else {
						CreateJewelryRecipes(armor);
					}
				}
			}

			for (auto& miscObj : dataHandler->GetFormArray<RE::TESObjectMISC>()) {
				if (std::string_view name = miscObj && miscObj->GetPlayable() ? miscObj->GetName() : std::string_view{}; !name.empty()) {
					CreateClutterRecipes(miscObj);
				}
			}

			std::ranges::copy(generatedConstructibles, std::back_inserter(dataHandler->GetFormArray<RE::BGSConstructibleObject>()));

			logger::info("{:*^30}", "RESULT");
			logger::info("SMELT");
			logger::info("\t{} weapon recipes added", smelt.weapCount);
			logger::info("\t{} armor recipes added", smelt.armorCount);
			logger::info("\t{} jewelry recipes added", smelt.jewelryCount);
			logger::info("\t{} clutter recipes added", smelt.miscObjCount);
			logger::info("TEMPER");
			logger::info("\t{} weapon recipes added", temper.weapCount);
			logger::info("\t{} armor recipes added", temper.armorCount);

			Clear();
		}
	}
}
