#pragma once
#include "Settings.h"
#include <xbyak/xbyak.h>

namespace ExperienceAdjustment
{
	bool InstallDisenchantHook();
	void AddDisenchantSkill(RE::PlayerCharacter* player, RE::ActorValue actorVal, float fAmount, RE::InventoryEntryData* item);
	void AddEnchantSkill(RE::PlayerCharacter* player, RE::ActorValue actorVal, float fAmount, RE::TESForm* soulGem);

	REL::Relocation<std::uintptr_t> disFuncBase_Hook{ REL::RelocationID(50459, 51363) };
	REL::Relocation<std::uintptr_t> encFuncBase_Hook{ REL::RelocationID(50450, 51355) };

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
			dq(ExperienceAdjustment::disFuncBase_Hook.address() + 0xC0);
		}
	};

	struct EnchantPatch : Xbyak::CodeGenerator
	{
		EnchantPatch()
		{
			Xbyak::Label xpFuncLabel;
			Xbyak::Label returnLabel;

			mov(r11, ptr[rbx + 0x18]);
			mov(r9, ptr[r11]);
			sub(rsp, 0x20);
			call(ptr[rip + xpFuncLabel]);
			add(rsp, 0x20);

			jmp(ptr[rip + returnLabel]);

			L(xpFuncLabel);
			dq(reinterpret_cast<std::uintptr_t>(ExperienceAdjustment::AddEnchantSkill));

			L(returnLabel);
			dq(ExperienceAdjustment::encFuncBase_Hook.address() + 0x279);
		}
	};

	void AddDisenchantSkill(RE::PlayerCharacter* player, RE::ActorValue actorVal, float fAmount, RE::InventoryEntryData* item)
	{
		if (item && (item->object->GetFormType() == RE::FormType::Weapon))
		{
			auto* weapon = item->object->As<RE::TESObjectWEAP>();
			auto* baseEnchant = weapon->formEnchanting->data.baseEnchantment;

			float xpAmount = 1.0f;
			if (baseEnchant) {
				std::int32_t cost = baseEnchant->data.costOverride;
				xpAmount = cost * Settings::fDisenchantingWeaponExpMult;
			}

			logger::info("Disenchant weapon: Adding {} xp", xpAmount);

			player->AddSkillExperience(actorVal, xpAmount);
		}
		else
		{
			auto* form = item->object->As<RE::TESEnchantableForm>();
			if (form) {
				auto* baseEnchant = form->formEnchanting->data.baseEnchantment;
				float xpAmount = 1.0f;
				if (baseEnchant) {
					std::int32_t cost = baseEnchant->data.costOverride;
					xpAmount = cost * Settings::fDisenchantingArmorExpMult;
				}

				logger::info("Disenchant armor: Adding {} xp", xpAmount);

				player->AddSkillExperience(actorVal, xpAmount);
			} else {
				logger::info("Not weapon or armor");
				player->AddSkillExperience(actorVal, fAmount);
			}

		}
	}

	void AddEnchantSkill(RE::PlayerCharacter* player, RE::ActorValue actorVal, float fAmount,RE::TESForm* soulGem)
	{	
		float xpMult = 1.0f;

		if (soulGem && soulGem->GetFormType() == RE::FormType::SoulGem) {
			logger::info("Soul gem soul level used {}", soulGem->GetName());
		
			auto* gemForm = soulGem->As<RE::TESSoulGem>();
			auto currentSoul = gemForm->GetContainedSoul();
			
			switch (currentSoul) {
			case RE::SOUL_LEVEL::kPetty:
				xpMult = Settings::fPettyMult;
				break;
			case RE::SOUL_LEVEL::kLesser:
				xpMult = Settings::fLesserMult;
				break;
			case RE::SOUL_LEVEL::kCommon:
				xpMult = Settings::fCommonMult;
				break;
			case RE::SOUL_LEVEL::kGreater:
				xpMult = Settings::fGreaterMult;
				break;
			case RE::SOUL_LEVEL::kGrand:
				xpMult = Settings::fGrandMult;
				break;
			}

			logger::info("Adding {} XP", xpMult*fAmount);
			player->AddSkillExperience(actorVal, fAmount*xpMult);	

		} else {
			logger::info("Not soul gem");

			player->AddSkillExperience(actorVal, xpMult);	
		}
	}

	bool InstallDisenchantHook()
	{
		ExperienceAdjustment::ExpPatch code;
		code.ready();

		auto& trampoline = SKSE::GetTrampoline();
		trampoline.write_branch<6>(disFuncBase_Hook.address() + 0xBA, trampoline.allocate(code));
		logger::info("Disenchant hook installed");
		return true;
	}

	bool InstallEnchantHook()
	{
		ExperienceAdjustment::EnchantPatch code;
		code.ready();
		auto& trampoline = SKSE::GetTrampoline();
		trampoline.write_branch<6>(encFuncBase_Hook.address() + 0x273, trampoline.allocate(code));
		logger::info("Enchant hook installed");
		return true;
	}

};
