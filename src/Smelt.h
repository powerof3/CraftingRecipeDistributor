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

		std::vector<std::string> bannedKeywords{ "MagicDisallowEnchanting" };

		std::uint32_t weapCount{ 0 };
		std::uint32_t armorCount{ 0 };
		std::uint32_t jewelryCount{ 0 };
		std::uint32_t miscObjCount{ 0 };

		static constexpr RawMap rawMap = {
			{ "ArmorMaterialBearStormcloak"sv, 0x800E4 },
			{ "ArmorMaterialBlades"sv, 0x5ACE5 },
			{ "ArmorMaterialDaedric"sv, 0x5AD9D },
			{ "ArmorMaterialDragonPlate"sv, 0x3ADA4 },
			{ "ArmorMaterialDragonscale"sv, 0x3ADA3 },
			{ "ArmorMaterialDwarven"sv, 0xDB8A2 },
			{ "ArmorMaterialEbony"sv, 0x5AD9D },
			{ "ArmorMaterialElven"sv, 0x5AD9F },
			{ "ArmorMaterialElvenGilded"sv, 0x5AD9F },
			{ "ArmorMaterialFalmer"sv, 0x3AD57 },
			{ "ArmorMaterialForsworn"sv, 0x800E4 },
			{ "ArmorMaterialGlass"sv, 0x5ADA1 },
			{ "ArmorMaterialHide"sv, 0x800E4 },
			{ "ArmorMaterialImperialHeavy"sv, 0x5ACE5 },
			{ "ArmorMaterialImperialLight"sv, 0x800E4 },
			{ "ArmorMaterialImperialStudded"sv, 0x800E4 },
			{ "ArmorMaterialIron"sv, 0x5ACE4 },
			{ "ArmorMaterialIronBanded"sv, 0x5ACE4 },
			{ "ArmorMaterialLeather"sv, 0x800E4 },
			{ "ArmorMaterialMS02Forsworn"sv, 0x800E4 },
			{ "ArmorMaterialOrcish"sv, 0x5AD99 },
			{ "ArmorMaterialPenitus"sv, 0x800E4 },
			{ "ArmorMaterialScaled"sv, 0x5ACE5 },
			{ "ArmorMaterialSteel"sv, 0x5ACE5 },
			{ "ArmorMaterialSteelPlate"sv, 0x5ACE5 },
			{ "ArmorMaterialStormcloak"sv, 0x800E4 },
			{ "ArmorMaterialStudded"sv, 0x5ACE4 },
			{ "ArmorMaterialThievesGuild"sv, 0x800E4 },
			{ "ArmorMaterialThievesGuildLeader"sv, 0x800E4 },
			{ "DLC1ArmorMaterialDawnguard"sv, 0x5ACE5 },
			{ "DLC1ArmorMaterialFalmerHardened"sv, 0x3AD57 },
			{ "DLC1ArmorMaterialHunter"sv, 0x5ACE5 },
			{ "DLC1ArmorMaterialVampire"sv, 0x800E4 },
			{ "DLC1LD_CraftingMaterialAetherium"sv, 0xDB8A2 },
			{ "DLC1WeapMaterialDragonbone"sv, 0x3ADA4 },
			{ "DLC2ArmorMaterialBonemoldHeavy"sv, 0x34CDD },
			{ "DLC2ArmorMaterialChitinHeavy"sv, 0x402B04E },
			{ "DLC2ArmorMaterialChitinLight"sv, 0x402B04E },
			{ "DLC2ArmorMaterialMoragTong"sv, 0x800E4 },
			{ "DLC2ArmorMaterialNordicHeavy"sv, 0x5ACE5 },
			{ "DLC2ArmorMaterialStalhrimHeavy"sv, 0x402B06B },
			{ "DLC2ArmorMaterialStalhrimLight"sv, 0x402B06B },
			{ "DLC2WeaponMaterialNordic"sv, 0x5ACE5 },
			{ "DLC2WeaponMaterialStalhrim"sv, 0x402B06B },
			{ "USKPArmorMaterialLinwe"sv, 0x800E4 },
			{ "USLEEPArmorMaterialBlackguard"sv, 0x800E4 },
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
		static constexpr RE::FormID tanningRackMat = 0x800E4;
	};
}
