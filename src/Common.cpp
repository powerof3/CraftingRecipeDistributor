#include "Common.h"

namespace RE
{
	std::variant<RE::TESForm*, std::string> ParseFormType(const std::string& a_str)
	{
		if (a_str.contains(" ~ ")) {
			if (auto formIDpair = string::split(a_str, " ~ "); !formIDpair.empty()) {
				auto        formID = string::to_num<RE::FormID>(formIDpair[0], true);
				const auto& esp = formIDpair[1];

				if (g_mergeMapperInterface) {
					const auto [mergedModName, mergedFormID] = g_mergeMapperInterface->GetNewFormID(esp.c_str(), formID);
					return RE::TESDataHandler::GetSingleton()->LookupForm(mergedFormID, mergedModName);
				} else {
					return RE::TESDataHandler::GetSingleton()->LookupForm(formID, esp);
				}
			}
		} else if (string::is_only_hex(a_str)) {
			return RE::TESForm::LookupByID(string::to_num<RE::FormID>(a_str, true));
		}

		return a_str;
	}

	RE::TESForm* ParseForm(const std::string& a_str)
	{
		RE::TESForm* form = nullptr;

		std::visit(overload{
					   [&](RE::TESForm* a_form) {
						   form = a_form;
					   },
					   [&](const std::string& edid) {
						   form = RE::TESForm::LookupByEditorID(edid);
					   } },
			ParseFormType(a_str));

		return form;
	}

	bool ArmorContainsModel(RE::TESObjectARMO* a_armor, std::string_view a_str)
	{
		return string::icontains(a_armor->worldModels[0].GetModel(), a_str);
	}
}

namespace CRAFT
{
	FormCount::FormCount(RE::TESForm* a_form, std::uint16_t a_count) :
		form(a_form),
		count(a_count)
	{}

	FormCount::FormCount(RE::TESForm* a_form) :
		FormCount(a_form, 0)
	{}

	void KeywordMap::Init(const RawMap& a_rawMap)
	{
		for (const auto& [keyword, formID] : a_rawMap) {
			if (auto form = RE::TESForm::LookupByID(formID)) {
				map.insert_or_assign(keyword, FormCount(form));
			}
		}
	}
	std::optional<FormCount> KeywordMap::GetData(RE::TESBoundObject* a_form)
	{
		if (const auto keywordForm = a_form->As<RE::BGSKeywordForm>(); keywordForm && keywordForm->keywords && keywordForm->numKeywords > 0) {
			std::span keywords(keywordForm->keywords, keywordForm->numKeywords);
			for (auto& keyword : keywords | std::views::reverse) {
				if (auto result = map.find(keyword->GetFormEditorID()); result != map.end()) {
					return result->second;
				}
			}
		}
		return std::nullopt;
	}

	void CraftingBase::LoadBlackList(const CSimpleIniA& ini, const char* a_type)
	{
		CSimpleIniA::TNamesDepend keys;
		ini.GetAllKeys(a_type, keys);
		for (const auto& key : keys) {
			blackList.emplace_back(string::trim_copy(key.pItem));
		}
		logger::info("\t\t{} entries : {}", a_type, blackList.size());
	}

	void CraftingBase::LoadINIData(const CSimpleIniA& ini, const char* a_type)
	{
		CSimpleIniA::TNamesDepend values;
		ini.GetAllValues(a_type, "Recipe", values);
		values.sort(CSimpleIniA::Entry::LoadOrder());

		if (const auto size = values.size(); size > 0) {
			logger::info("\t\t{} entries : {}", a_type, size);

			customINIData.reserve(values.size());
			for (auto& value : values) {
				customINIData.emplace_back(value.pItem);
			}
		} else {
			logger::error("\t\t{} entries: 0", a_type);
		}
	}

	void CraftingBase::InitINIData()
	{
		for (const auto& value : customINIData) {
			auto sections = string::split(value, "|");
			std::ranges::for_each(sections, [](auto& str) { string::trim(str); });

			//[FORMID ~ ESP]
			RE::TESForm* createdItem = nullptr;
			if (!sections.empty()) {
				createdItem = RE::ParseForm(sections[0]);
			}

			if (!createdItem) {
				continue;
			}

			//COUNT
			std::uint16_t count = 0;
			if (sections.size() > 2) {
				count = string::to_num<std::uint16_t>(sections[2]);
			}

			//KEYWORDS
			if (sections.size() > 1) {
				auto split_str = string::split(sections[1], ",");
				std::ranges::for_each(split_str, [](auto& str) { string::trim(str); });

				auto formCount = FormCount(createdItem, count);

				for (const auto& str : split_str) {
					std::visit(overload{
								   [&](const RE::TESForm* a_item) {
									   if (a_item) {
										   formidMap[a_item->GetFormID()] = formCount;
									   }
								   },
								   [&](const std::string& edid) {
									   keywordMap.map[edid] = formCount;
								   } },
						RE::ParseFormType(str));
				}
			}
		}
	}

	void CraftingBase::InitBlackList()
	{
		std::erase_if(blackList, [&](const auto& entry) {
			bool erase = false;
			std::visit(overload{
						   [&](const RE::TESForm* a_item) {
							   if (a_item && a_item->IsNot(RE::FormType::Keyword)) {
								   blackListForms.emplace(a_item->GetFormID());
								   erase = true;
							   }
						   },
						   [&](const std::string&) {
						   } },
				RE::ParseFormType(entry));
			return erase;
		});
	}

	void CraftingBase::InitData(const RawMap& a_rawMap)
	{
		keywordMap.Init(a_rawMap);

		InitINIData();
		InitBlackList();
	}

	bool CraftingBase::IsBlacklisted(RE::TESBoundObject* a_item) const
	{
		return !blackListForms.empty() && blackListForms.contains(a_item->GetFormID()) || !blackList.empty() && a_item->HasAnyKeywordByEditorID(blackList);
	}

	void CraftingBase::Clear()
	{
		customINIData.clear();
		keywordMap.map.clear();
		formidMap.clear();
		blackList.clear();
		blackListForms.clear();
	}
}
