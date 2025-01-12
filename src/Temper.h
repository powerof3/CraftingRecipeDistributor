#pragma once

#include "Common.h"

namespace CRAFT
{
	class TEMPER : public CraftingBase
	{
	public:
		void LookupForms();
		void InitData();
		bool CreateRecipe(RE::TESBoundObject* a_item);
		bool CreateRecipe(RE::TESBoundObject* a_item, RE::TESForm* a_mat, std::uint16_t a_requiredNum);

		// members
		RE::BGSKeyword* armorKywd{};
		RE::BGSKeyword* weapKywd{};
		RE::BGSPerk*    arcanePerk{};

		std::uint32_t weapCount{ 0 };
		std::uint32_t armorCount{ 0 };

		static constexpr RawMap rawMap{
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
	};
}
