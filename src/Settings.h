#pragma once
#include <SimpleIni.h>

class Settings
{
public:
	inline static float fDisenchantingExpMult;

	inline static void LoadSettings()
	{
		logger::info("Loading settings");

		CSimpleIniA ini;
		ini.SetUnicode();
		ini.LoadFile(R"(.\Data\SKSE\Plugins\Thaumaturgy.ini)");

		auto xpMult = (float)ini.GetDoubleValue("Thaumaturgy", "fDisenchantingExpMult", 15.0);

		(xpMult < 0.0 || xpMult > 100.0) ? fDisenchantingExpMult = 0.15f : fDisenchantingExpMult = xpMult;
		logger::info("fDisenchantingExpMult: " + std::to_string(fDisenchantingExpMult));
	}
};
