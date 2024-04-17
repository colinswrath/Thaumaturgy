#pragma once
#include <SimpleIni.h>

class Settings
{
public:
	inline static float fDisenchantingArmorExpMult;
	inline static float fDisenchantingWeaponExpMult;
	inline static float fPettyMult;
	inline static float fLesserMult;
	inline static float fCommonMult;
	inline static float fGreaterMult;
	inline static float fGrandMult;

	inline static void LoadSettings()
	{
		logger::info("Loading settings");

		CSimpleIniA ini;
		ini.SetUnicode();
		ini.LoadFile(R"(.\Data\SKSE\Plugins\Thaumaturgy.ini)");

		auto xpArmorMult = (float)ini.GetDoubleValue("Thaumaturgy", "fDisenchantingArmorExpMult", 0.01);
		auto xpWeaponMult = (float)ini.GetDoubleValue("Thaumaturgy", "fDisenchantingWeaponExpMult", 0.15);

		fPettyMult = (float)ini.GetDoubleValue("Thaumaturgy", "fEnchantPettyMult", 1.0);
		fLesserMult = (float)ini.GetDoubleValue("Thaumaturgy", "fEnchantLesserMult", 1.25);
		fCommonMult = (float)ini.GetDoubleValue("Thaumaturgy", "fEnchantCommonMult", 1.5);
		fGreaterMult = (float)ini.GetDoubleValue("Thaumaturgy", "fEnchantGreaterMult", 1.75);
		fGrandMult = (float)ini.GetDoubleValue("Thaumaturgy", "fEnchantGrandMult", 2.0);

		fDisenchantingArmorExpMult = xpArmorMult;
		fDisenchantingWeaponExpMult = xpWeaponMult;

		logger::info("fDisenchantingArmorExpMult: " + std::to_string(fDisenchantingArmorExpMult));
		logger::info("fDisenchantingWeaponExpMult: " + std::to_string(fDisenchantingWeaponExpMult));
		logger::info("fEnchantPettyMult: " + std::to_string(fPettyMult));
		logger::info("fEnchantLesserMult: " + std::to_string(fLesserMult));
		logger::info("fEnchantCommonMult: " + std::to_string(fCommonMult));
		logger::info("fEnchantGreaterMult: " + std::to_string(fGreaterMult));
		logger::info("fEnchantGrandMult: " + std::to_string(fGrandMult));
	}
};
