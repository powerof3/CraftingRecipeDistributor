#include "Common.h"

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

	void CustomINIData::LoadData(const CSimpleIniA& ini, const char* a_type)
	{
		CSimpleIniA::TNamesDepend values;
		ini.GetAllValues(a_type, "Recipe", values);
		values.sort(CSimpleIniA::Entry::LoadOrder());

		if (const auto size = values.size(); size > 0) {
			logger::info("\t{} entries found : {}", a_type, size);

			data.reserve(values.size());
			for (auto& value : values) {
				data.emplace_back(value.pItem);
			}
		} else {
			logger::error("\tno {} entries found", a_type);
		}
	}

	void CustomINIData::LookupData(KeywordMap& a_keywordMap, FormIDMap& a_formidMap) const
	{
		for (const auto& value : data) {
			auto sections = string::split(value, "|");
			std::ranges::for_each(sections, [](auto& str) { string::trim(str); });

			//[FORMID ~ ESP]
			RE::TESForm* createdItem = nullptr;
			if (!sections.empty()) {
				createdItem = ParseForm(sections[0]);
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
										   a_formidMap[a_item->GetFormID()] = formCount;
									   }
								   },
								   [&](const std::string& edid) {
									   a_keywordMap.map[edid] = formCount;
								   } },
						ParseFormType(str));
				}
			}
		}
	}

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
}
