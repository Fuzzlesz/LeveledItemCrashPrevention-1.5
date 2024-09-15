#include "hooks.h"

#include "settings.h"

namespace Hooks {
    void DebugLeveledList(RE::TESLeveledList* a_list, RE::TESForm* a_problem) {
        auto* form = skyrim_cast<RE::TESBoundObject*>(a_list);
        _loggerInfo("Caught problematic insertion of form {} to leveled list {}.", _debugEDID(a_problem), _debugEDID(form));
        _loggerInfo("The form has not been inserted. For ease of review,\nhere are the current contents of the list:\n");
        int i = 1;

        for (auto& entry : a_list->entries) {
            if (!entry.form) {
                _loggerInfo("{}. Null form. This is a problem, do not ignore it.", i);
            }
            else {
                _loggerInfo("{}. {}", i, _debugEDID(entry.form));
            }
            ++i;
        }
        _loggerInfo("___________________________________________________");
        if (Settings::Holder::GetSingleton()->ShouldWarn()) {
            RE::DebugMessageBox("Warning: Caught bad AddForm. Check <My Games/Skyrim Special Edition/SKSE/LeveledListCrashPrevention.log> for more information. This message is safe to ignore.");
        }
    }

    //Leveled Items
    void ProtectLevItems::AddForm(RE::TESLeveledList* a_this, RE::TESBoundObject* a_list, unsigned short a_level, unsigned long long a_count, RE::TESForm* a_form) {
        if (a_this->numEntries > 254) {
            DebugLeveledList(a_this, a_form);
        }
        else {
            _originalCall(a_this, a_list, a_level, a_count, a_form);
        }
    }

    bool ProtectLevItems::Install() {
        SKSE::AllocTrampoline(14);
        auto& trampoline = SKSE::GetTrampoline();

        REL::Relocation<std::uintptr_t> target{ REL::ID(55401), 0x56 };
        if (!REL::make_pattern<"E8 55 ED 80 FF">().match(target.address())) {
            _loggerInfo("Failed to validate hook address for ProtectLevItems. Aborting load.");
            return false;
        }
        _loggerInfo("Installed leveled item patch.");
        _originalCall = trampoline.write_call<5>(target.address(), &AddForm);
        return true;
    }

    //Leveled Actors

    void ProtectLeveledActors::AddForm(RE::TESLeveledList* a_this, RE::TESBoundObject* a_list, unsigned short a_level, unsigned long long a_count, RE::TESForm* a_form) {
        if (a_this->numEntries > 254) {
            DebugLeveledList(a_this, a_form);
        }
        else {
            _originalCall(a_this, a_list, a_level, a_count, a_form);
        }
    }

    bool ProtectLeveledActors::Install() {
        SKSE::AllocTrampoline(14);
        auto& trampoline = SKSE::GetTrampoline();

        REL::Relocation<std::uintptr_t> target{ REL::ID(55387), 0x56 };
        if (!REL::make_pattern<"E8 05 F5 80 FF">().match(target.address())) {
            _loggerInfo("Failed to validate hook address for ProtectLeveledActors. Aborting load.");
            return false;
        }

        _loggerInfo("Installed leveled actor patch.");
        _originalCall = trampoline.write_call<5>(target.address(), &AddForm);
        return true;
    }

    //Leveled Spells

    void ProtectLeveledSpells::AddForm(RE::TESLeveledList* a_this, RE::TESBoundObject* a_list, unsigned short a_level, unsigned long long a_count, RE::TESForm* a_form) {
        if (a_this->numEntries > 254) {
            DebugLeveledList(a_this, a_form);
        }
        else {
            _originalCall(a_this, a_list, a_level, a_count, a_form);
        }
    }

    bool ProtectLeveledSpells::Install() {
        SKSE::AllocTrampoline(14);
        auto& trampoline = SKSE::GetTrampoline();

        REL::Relocation<std::uintptr_t> target{ REL::ID(55415), 0x56 };
        if (!REL::make_pattern<"E8 25 E5 80 FF">().match(target.address())) {
            _loggerInfo("Failed to validate hook address for ProtectLeveledSpells. Aborting load.");
            return false;
        }

        _loggerInfo("Installed leveled spell patch.");
        _originalCall = trampoline.write_call<5>(target.address(), &AddForm);
        return true;
    }

    //Install

    bool Install() {
        if (!ProtectLevItems::Install() || !ProtectLeveledActors::Install() || !ProtectLeveledSpells::Install()) {
            return false;
        }
        return true;
    }
}