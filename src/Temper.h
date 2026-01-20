#pragma once

#include "Common.h"

namespace CRAFT
{
	class TEMPER : public CraftingBase
	{
	public:
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
			{ "ArmorMaterialBearStormcloak"sv, "Leather01"sv },
			{ "ArmorMaterialBlades"sv, "IngotSteel"sv },
			{ "ArmorMaterialDaedric"sv, "IngotEbony"sv },
			{ "ArmorMaterialDragonPlate"sv, "DragonBone"sv },
			{ "ArmorMaterialDragonscale"sv, "DragonScales"sv },
			{ "ArmorMaterialDwarven"sv, "IngotDwarven"sv },
			{ "ArmorMaterialEbony"sv, "IngotEbony"sv },
			{ "ArmorMaterialElven"sv, "IngotIMoonstone"sv },
			{ "ArmorMaterialElvenGilded"sv, "IngotQuicksilver"sv },
			{ "ArmorMaterialFalmer"sv, "ChaurusChitin"sv },
			{ "ArmorMaterialForsworn"sv, "Leather01"sv },
			{ "ArmorMaterialGlass"sv, "IngotMalachite"sv },
			{ "ArmorMaterialHide"sv, "Leather01"sv },
			{ "ArmorMaterialImperialHeavy"sv, "IngotSteel"sv },
			{ "ArmorMaterialImperialLight"sv, "Leather01"sv },
			{ "ArmorMaterialImperialStudded"sv, "Leather01"sv },
			{ "ArmorMaterialIron"sv, "IngotIron"sv },
			{ "ArmorMaterialIronBanded"sv, "IngotCorundum"sv },
			{ "ArmorMaterialLeather"sv, "Leather01"sv },
			{ "ArmorMaterialMS02Forsworn"sv, "Leather01"sv },
			{ "ArmorMaterialOrcish"sv, "IngotOrichalcum"sv },
			{ "ArmorMaterialPenitus"sv, "Leather01"sv },
			{ "ArmorMaterialScaled"sv, "IngotCorundum"sv },
			{ "ArmorMaterialSteel"sv, "IngotSteel"sv },
			{ "ArmorMaterialSteelPlate"sv, "IngotSteel"sv },
			{ "ArmorMaterialStormcloak"sv, "Leather01"sv },
			{ "ArmorMaterialStudded"sv, "IngotIron"sv },
			{ "ArmorMaterialThievesGuild"sv, "Leather01"sv },
			{ "ArmorMaterialThievesGuildLeader"sv, "Leather01"sv },
			{ "DLC1ArmorMaterialDawnguard"sv, "IngotSteel"sv },
			{ "DLC1ArmorMaterialFalmerHardened"sv, "ChaurusChitin"sv },
			{ "DLC1ArmorMaterialHunter"sv, "IngotSteel"sv },
			{ "DLC1ArmorMaterialVampire"sv, "Leather01"sv },
			{ "DLC1LD_CraftingMaterialAetherium"sv, "IngotDwarven"sv },
			{ "DLC1WeapMaterialDragonbone"sv, "DragonBone"sv },
			{ "DLC2ArmorMaterialBonemoldHeavy"sv, "BoneMeal"sv },
			{ "DLC2ArmorMaterialChitinHeavy"sv, "DLC2ChitinPlate"sv },
			{ "DLC2ArmorMaterialChitinLight"sv, "DLC2ChitinPlate"sv },
			{ "DLC2ArmorMaterialMoragTong"sv, "Leather01"sv },
			{ "DLC2ArmorMaterialNordicHeavy"sv, "IngotQuicksilver"sv },
			{ "DLC2ArmorMaterialStalhrimHeavy"sv, "DLC2OreStalhrim"sv },
			{ "DLC2ArmorMaterialStalhrimLight"sv, "DLC2OreStalhrim"sv },
			{ "DLC2WeaponMaterialNordic"sv, "IngotQuicksilver"sv },
			{ "DLC2WeaponMaterialStalhrim"sv, "DLC2OreStalhrim"sv },
			{ "USKPArmorMaterialLinwe"sv, "Leather01"sv },
			{ "USLEEPArmorMaterialBlackguard"sv, "Leather01"sv },
			{ "WeapMaterialDaedric"sv, "IngotEbony"sv },
			{ "WeapMaterialDraugr"sv, "IngotSteel"sv },
			{ "WeapMaterialDraugrHoned"sv, "IngotSteel"sv },
			{ "WeapMaterialDwarven"sv, "IngotDwarven"sv },
			{ "WeapMaterialEbony"sv, "IngotEbony"sv },
			{ "WeapMaterialElven"sv, "IngotIMoonstone"sv },
			{ "WeapMaterialFalmer"sv, "ChaurusChitin"sv },
			{ "WeapMaterialFalmerHoned"sv, "ChaurusChitin"sv },
			{ "WeapMaterialGlass"sv, "IngotMalachite"sv },
			{ "WeapMaterialImperial"sv, "IngotSteel"sv },
			{ "WeapMaterialIron"sv, "IngotIron"sv },
			{ "WeapMaterialOrcish"sv, "IngotOrichalcum"sv },
			{ "WeapMaterialSilver"sv, "ingotSilver"sv },
			{ "WeapMaterialSteel"sv, "IngotSteel"sv },
			{ "WeapMaterialWood"sv, "Firewood01"sv },
		};
	};
}
