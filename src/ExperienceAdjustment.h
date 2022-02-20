#pragma once
#include "Settings.h"

class ExperienceAdjustment
{
public:

	static void AddDisenchantSkill(RE::PlayerCharacter* player, RE::ActorValue actorVal, float fAmount, RE::InventoryEntryData* item)
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

	static bool InstallXbyakHook()
	{
		REL::Relocation<std::uintptr_t> funcBase_Hook(REL::ID(50459));		//1.5

		struct ExpPatch : Xbyak::CodeGenerator
		{
			ExpPatch(std::uintptr_t xpFuncAddress, std::uintptr_t returnAddress)
			{
				Xbyak::Label xpFuncLabel;
				Xbyak::Label returnLabel;

				mov(r9,r14);
				sub(rsp, 0x20);
				call(ptr[rip+ xpFuncLabel]);
				add(rsp, 0x20);

				jmp(ptr[rip + returnLabel]);

				L(xpFuncLabel);
				dq(xpFuncAddress);

				L(returnLabel);
				dq(returnAddress);
			}
		};

		ExpPatch code(reinterpret_cast<std::uintptr_t>(AddDisenchantSkill), funcBase_Hook.address() + 0xC0);

		auto& trampoline = SKSE::GetTrampoline();
		trampoline.write_branch<6>(funcBase_Hook.address() + 0xBA, trampoline.allocate(code));
		logger::info("Disenchant hook installed");
		return true;
	}

};
