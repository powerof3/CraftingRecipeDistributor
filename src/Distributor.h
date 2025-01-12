#pragma once

#include "Common.h"
#include "Smelt.h"
#include "Temper.h"

namespace CRAFT
{
	class Manager : public REX::Singleton<Manager>
	{
	public:
		void LoadSettings();
		void LoadOverwrites();
		void Distribute();

		void AddGeneratedConstructible(RE::BGSConstructibleObject* a_obj);

	private:
		void InitData();

		void CreateStandardRecipes(TYPE a_type, RE::TESBoundObject* a_form);
		void CreateJewelryRecipes(RE::TESObjectARMO* a_armor);
		void CreateClutterRecipes(RE::TESObjectMISC* a_miscObj);

		void Clear();

		// members
		RE::TESObjectMISC*                       ironIngot{};
		RE::TESObjectMISC*                       dwemerIngot{};
		RE::TESObjectMISC*                       goldIngot{};
		RE::TESObjectMISC*                       silverIngot{};
		TEMPER                                   temper;
		SMELT                                    smelt;
		std::vector<RE::BGSConstructibleObject*> generatedConstructibles{};

		static constexpr std::array<std::string_view, 7> ironMats = { "iron", "pick", "kettle", "lantern", "knife", "scissor", "scapel" };
	};
}
