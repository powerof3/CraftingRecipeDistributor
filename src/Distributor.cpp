#include "Distributor.h"

namespace CRAFT
{
	void Manager::LoadOverwrites()
	{
		REX::INFO("{:*^30}", "OVERWRITES");

		std::vector<std::string> configs = clib_util::distribution::get_configs(R"(Data\)", "_CRD"sv);

		if (configs.empty()) {
			REX::WARN("No .ini files with _CRD suffix were found within the Data folder, aborting...");
			return;
		}

		REX::INFO("{} matching inis found", configs.size());

		for (auto& path : configs) {
			REX::INFO("\tINI : {}", path);

			CSimpleIniA ini;
			ini.SetUnicode();
			ini.SetMultiKey();
			ini.SetAllowKeyOnly();

			if (const auto rc = ini.LoadFile(path.c_str()); rc < 0) {
				REX::ERROR("\t\tcouldn't read INI");
				continue;
			}

			smelt.LoadINIData(ini, "SMELT");
			smelt.LoadBlackList(ini, "SMELT_BlackList");

			temper.LoadINIData(ini, "TEMPER");
			temper.LoadBlackList(ini, "TEMPER_BlackList");
		}
	}

	void Manager::LoadSettings()
	{
		REX::INFO("{:*^30}", "SETTINGS");

		const auto store = REX::FIniSettingStore::GetSingleton();
		store->Init(configPath.data(), "");

		store->Load();
		store->Save();

		REX::INFO("SMELT");
		REX::INFO("\tmax weapon cap : {}", smelt.maxWeapAmount.GetValue());
		REX::INFO("\tmax armor cap : {}", smelt.maxArmorAmount.GetValue());
		REX::INFO("\tmax jewelry cap : {}", smelt.maxJewelryAmount.GetValue());
		REX::INFO("\tmax clutter cap : {}", smelt.maxClutterAmount.GetValue());
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

		const auto& cobjArray = RE::TESDataHandler::GetSingleton()->GetFormArray<RE::BGSConstructibleObject>();
		vanillaConstructibles.reserve(cobjArray.size());
		for (const auto& cobj : cobjArray) {
			vanillaConstructibles.emplace_back(cobj);
		}
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
			if (RE::ArmorContainsModel(a_armor, "gold") || REX::STR::ICONTAINS(name, "gold")) {
				ingot = goldIngot;
			} else if (RE::ArmorContainsModel(a_armor, "silver") || REX::STR::ICONTAINS(name, "silver")) {
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
				if (REX::STR::ICONTAINS(a_miscObj->model, "gold") || REX::STR::ICONTAINS(a_miscObj->model, "coin")) {
					ingot = goldIngot;
				} else if (REX::STR::ICONTAINS(a_miscObj->model, "silver")) {
					ingot = silverIngot;
				} else if (REX::STR::ICONTAINS(a_miscObj->model, "dwemer")) {
					ingot = dwemerIngot;
				} else if (std::ranges::any_of(ironMats, [&](const auto& str) { return REX::STR::ICONTAINS(a_miscObj->model, str); })) {
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
		vanillaConstructibles.clear();
		vanillaConstructibles.shrink_to_fit();

		generatedConstructibles.clear();
		generatedConstructibles.shrink_to_fit();

		smelt.Clear();
		temper.Clear();
	}

	void Manager::Distribute()
	{
		if (const auto dataHandler = RE::TESDataHandler::GetSingleton(); dataHandler) {
			InitData();

			// patch cobjs from being shown unless player has required items?
			for (auto& cobj : vanillaConstructibles) {
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

			constexpr auto is_valid_form = [](RE::TESForm* a_form) {
				return a_form != nullptr && a_form->GetPlayable() && !REX::STR::IS_EMPTY(a_form->GetName());
			};

			// generate recipes
			for (auto& weapon : dataHandler->GetFormArray<RE::TESObjectWEAP>()) {
				if (!is_valid_form(weapon)) {
					continue;
				}
				CreateStandardRecipes(TYPE::kWeap, weapon);
			}

			for (auto& armor : dataHandler->GetFormArray<RE::TESObjectARMO>()) {
				if (!is_valid_form(armor)) {
					continue;
				}
				if (!armor->HasKeywordString("ArmorJewelry"sv)) {
					CreateStandardRecipes(TYPE::kArmor, armor);
				} else {
					CreateJewelryRecipes(armor);
				}
			}

			for (auto& miscObj : dataHandler->GetFormArray<RE::TESObjectMISC>()) {
				if (!is_valid_form(miscObj)) {
					continue;
				}
				CreateClutterRecipes(miscObj);
			}

			std::ranges::copy(generatedConstructibles, std::back_inserter(dataHandler->GetFormArray<RE::BGSConstructibleObject>()));

			REX::INFO("{:*^30}", "RESULT");
			REX::INFO("SMELT");
			REX::INFO("\t{} weapon recipes added", smelt.weapCount);
			REX::INFO("\t{} armor recipes added", smelt.armorCount);
			REX::INFO("\t{} jewelry recipes added", smelt.jewelryCount);
			REX::INFO("\t{} clutter recipes added", smelt.miscObjCount);
			REX::INFO("TEMPER");
			REX::INFO("\t{} weapon recipes added", temper.weapCount);
			REX::INFO("\t{} armor recipes added", temper.armorCount);

			Clear();
		}
	}

	void Manager::ForEachConstructible(const std::function<RE::BSContainer::ForEachResult(RE::BGSConstructibleObject*)>& a_fn)
	{
		for (auto& cobj : vanillaConstructibles) {
			if (a_fn(cobj) == RE::BSContainer::ForEachResult::kStop) {
				break;
			}
		}
	}
}
