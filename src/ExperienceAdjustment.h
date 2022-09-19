#pragma once
#include "Settings.h"
#include <xbyak/xbyak.h>

namespace ExperienceAdjustment
{
	bool InstallXbyakHook();
	void AddDisenchantSkill(RE::PlayerCharacter* player, RE::ActorValue actorVal, float fAmount, RE::InventoryEntryData* item);

	REL::Relocation<std::uintptr_t> funcBase_Hook{ REL::RelocationID(50459, 51363) };

	struct ExpPatch : Xbyak::CodeGenerator
	{
		ExpPatch()
		{
			Xbyak::Label xpFuncLabel;
			Xbyak::Label returnLabel;

			mov(r9, r14);
			sub(rsp, 0x20);
			call(ptr[rip + xpFuncLabel]);
			add(rsp, 0x20);

			jmp(ptr[rip + returnLabel]);

			L(xpFuncLabel);
			dq(reinterpret_cast<std::uintptr_t>(ExperienceAdjustment::AddDisenchantSkill));

			L(returnLabel);
			dq(ExperienceAdjustment::funcBase_Hook.address() + 0xC0);
		}
	};

	void AddDisenchantSkill(RE::PlayerCharacter* player, RE::ActorValue actorVal, float fAmount, RE::InventoryEntryData* item)
	{
		if (item && (item->object->GetFormType() == RE::FormType::Weapon))
		{
			player->AddSkillExperience(actorVal, fAmount * Settings::fDisenchantingExpMult);
		}
		else
		{
			player->AddSkillExperience(actorVal, fAmount);
		}
	}

	bool InstallXbyakHook()
	{
		ExperienceAdjustment::ExpPatch code;
		code.ready();

		auto& trampoline = SKSE::GetTrampoline();
		trampoline.write_branch<6>(funcBase_Hook.address() + 0xBA, trampoline.allocate(code));
		logger::info("Disenchant hook installed");
		return true;
	}

};
