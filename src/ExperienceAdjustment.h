#pragma once
#include "Settings.h"

class ExperienceAdjustment
{
public:
	static bool InstallDisenchantHook()
	{
		REL::Relocation<std::uintptr_t> funcBase_Hook(REL::ID(50459));		//1.5
		std::uint8_t noopPatch[] = { 0x90 };

		auto& trampoline = SKSE::GetTrampoline();
		trampoline.write_call<5>(funcBase_Hook.address() + 0xBA, AddDisenchantSkill);
		REL::safe_write(funcBase_Hook.address() + 0xBA + 0x5, noopPatch, 1);

		logger::info("InstallDisenchantHook loaded");

		return true;
	}

	static void AddDisenchantSkill(RE::PlayerCharacter *player, RE::ActorValue actorVal, float fAmount)
	{
		player->AddSkillExperience(actorVal, fAmount * Settings::fDisenchantingExpMult);
	}
};
