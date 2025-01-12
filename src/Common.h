#pragma once

namespace CRAFT
{
	enum class TYPE
	{
		kArmor = 0,
		kWeap,
		kJewel,
		kClutter
	};
	
	struct FormCount
	{
		FormCount() = default;
		FormCount(RE::TESForm* a_form, std::uint16_t a_count);
		explicit FormCount(RE::TESForm* a_form);

		// members
		RE::TESForm*  form;
		std::uint16_t count;
	};

	using RawMap = frozen::map<std::string_view, RE::FormID, 61>;
	using FormIDMap = Map<RE::FormID, FormCount>;

	struct KeywordMap
	{
		void                     Init(const RawMap& a_rawMap);
		std::optional<FormCount> GetData(RE::TESBoundObject* a_form);

		// members
		StringMap<FormCount> map{};
	};

	struct CustomINIData
	{
		void LoadData(const CSimpleIniA& ini, const char* a_type);
		void LookupData(KeywordMap& a_keywordMap, FormIDMap& a_formidMap) const;

		// members
		std::vector<std::string> data;
	};

	std::variant<RE::TESForm*, std::string> ParseFormType(const std::string& a_str);
	RE::TESForm*                            ParseForm(const std::string& a_str);
}
