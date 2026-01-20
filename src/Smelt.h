#pragma once

#include "Common.h"

namespace CRAFT
{
	class SMELT : public CraftingBase
	{
	public:
		void InitData();
		bool CreateRecipe(TYPE a_type, RE::TESBoundObject* a_item, std::int32_t a_numRequired = 1);
		bool CreateRecipe(TYPE a_type, RE::TESBoundObject* a_item, RE::TESForm* a_ingot, std::uint16_t a_numConstructed, std::int32_t a_numRequired = 1);

		// members
		RE::BGSKeyword* smeltKywd{};
		RE::BGSKeyword* tanningRackKywd{};
		RE::BGSKeyword* forgeKywd{};

		std::uint16_t maxWeapAmount{ 0 };
		std::uint16_t maxArmorAmount{ 0 };
		std::uint16_t maxJewelryAmount{ 0 };
		std::uint16_t maxClutterAmount{ 0 };

		std::uint32_t weapCount{ 0 };
		std::uint32_t armorCount{ 0 };
		std::uint32_t jewelryCount{ 0 };
		std::uint32_t miscObjCount{ 0 };

		static constexpr RawMap rawMap = {
			{ "ArmorMaterialBearStormcloak"sv, "LeatherStrips"sv },
			{ "ArmorMaterialBlades"sv, "IngotSteel"sv },
			{ "ArmorMaterialDaedric"sv, "IngotEbony"sv },
			{ "ArmorMaterialDragonPlate"sv, "DragonBone"sv },
			{ "ArmorMaterialDragonscale"sv, "DragonScales"sv },
			{ "ArmorMaterialDwarven"sv, "IngotDwarven"sv },
			{ "ArmorMaterialEbony"sv, "IngotEbony"sv },
			{ "ArmorMaterialElven"sv, "IngotIMoonstone"sv },
			{ "ArmorMaterialElvenGilded"sv, "IngotIMoonstone"sv },
			{ "ArmorMaterialFalmer"sv, "ChaurusChitin"sv },
			{ "ArmorMaterialForsworn"sv, "LeatherStrips"sv },
			{ "ArmorMaterialGlass"sv, "IngotMalachite"sv },
			{ "ArmorMaterialHide"sv, "LeatherStrips"sv },
			{ "ArmorMaterialImperialHeavy"sv, "IngotSteel"sv },
			{ "ArmorMaterialImperialLight"sv, "LeatherStrips"sv },
			{ "ArmorMaterialImperialStudded"sv, "LeatherStrips"sv },
			{ "ArmorMaterialIron"sv, "IngotIron"sv },
			{ "ArmorMaterialIronBanded"sv, "IngotIron"sv },
			{ "ArmorMaterialLeather"sv, "LeatherStrips"sv },
			{ "ArmorMaterialMS02Forsworn"sv, "LeatherStrips"sv },
			{ "ArmorMaterialOrcish"sv, "IngotOrichalcum"sv },
			{ "ArmorMaterialPenitus"sv, "LeatherStrips"sv },
			{ "ArmorMaterialScaled"sv, "IngotSteel"sv },
			{ "ArmorMaterialSteel"sv, "IngotSteel"sv },
			{ "ArmorMaterialSteelPlate"sv, "IngotSteel"sv },
			{ "ArmorMaterialStormcloak"sv, "LeatherStrips"sv },
			{ "ArmorMaterialStudded"sv, "IngotIron"sv },
			{ "ArmorMaterialThievesGuild"sv, "LeatherStrips"sv },
			{ "ArmorMaterialThievesGuildLeader"sv, "LeatherStrips"sv },
			{ "DLC1ArmorMaterialDawnguard"sv, "IngotSteel"sv },
			{ "DLC1ArmorMaterialFalmerHardened"sv, "ChaurusChitin"sv },
			{ "DLC1ArmorMaterialHunter"sv, "IngotSteel"sv },
			{ "DLC1ArmorMaterialVampire"sv, "LeatherStrips"sv },
			{ "DLC1LD_CraftingMaterialAetherium"sv, "IngotDwarven"sv },
			{ "DLC1WeapMaterialDragonbone"sv, "DragonBone"sv },
			{ "DLC2ArmorMaterialBonemoldHeavy"sv, "BoneMeal"sv },
			{ "DLC2ArmorMaterialChitinHeavy"sv, "DLC2ChitinPlate"sv },
			{ "DLC2ArmorMaterialChitinLight"sv, "DLC2ChitinPlate"sv },
			{ "DLC2ArmorMaterialMoragTong"sv, "LeatherStrips"sv },
			{ "DLC2ArmorMaterialNordicHeavy"sv, "IngotSteel"sv },
			{ "DLC2ArmorMaterialStalhrimHeavy"sv, "DLC2OreStalhrim"sv },
			{ "DLC2ArmorMaterialStalhrimLight"sv, "DLC2OreStalhrim"sv },
			{ "DLC2WeaponMaterialNordic"sv, "IngotSteel"sv },
			{ "DLC2WeaponMaterialStalhrim"sv, "DLC2OreStalhrim"sv },
			{ "USKPArmorMaterialLinwe"sv, "LeatherStrips"sv },
			{ "USLEEPArmorMaterialBlackguard"sv, "LeatherStrips"sv },
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
			{ "WeapMaterialWood"sv, "Firewood01"sv }
		};
		static constexpr RE::FormID tanningRackMat = 0x800E4;
	};
}
