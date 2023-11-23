/*
 * This file is part of the TrinityCore Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "GameObject.h"
#include "InstanceScript.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "TemporarySummon.h"
#include "aberrus_the_shadowed_crucible.h"

enum EchoOfNeltharionSpells
{
    // Intro
    SPELL_TWISTED_SECRET            = 401003,
    SPELL_P1_ENERGY_COLOR           = 410672,
    SPELL_NELTHARION_STATS          = 409970,
    SPELL_PRE_RP_TRANSFORM          = 412812,
    SPELL_EARTH_DISSOLVE_IN         = 413400,
    SPELL_EARTHEN_GRASP             = 409724,
    SPELL_CALAMITOUS_STRIKE         = 410848,
    SPELL_RP_TRANSFORM              = 412813,

    // Wrathion and Sabellian
    SPELL_ANCHOR_HERE               = 45313,
    SPELL_EARTHEN_GRASP_WRATHION    = 410836,
    SPELL_START_INTRO_CONVERSATION  = 408211, // casted via smart_scripts @ Sabellian
};

enum EchoOfNeltharionEvents
{

};

enum EchoOfNeltharionDisplayIds
{
    DISPLAYID_NELTHARION_INITIAL = 112765,
};

// 201668 - Neltharion
struct boss_echo_of_neltharion : public BossAI
{
    boss_echo_of_neltharion(Creature* creature) : BossAI(creature, DATA_ECHO_OF_NELTHARION)
    {
        me->SetDisplayId(DISPLAYID_NELTHARION_INITIAL, true);
    }

    void JustAppeared() override
    {
        DoCastSelf(SPELL_TWISTED_SECRET);
        DoCastSelf(SPELL_P1_ENERGY_COLOR);
        DoCastSelf(SPELL_NELTHARION_STATS);
        DoCastSelf(SPELL_PRE_RP_TRANSFORM);
        DoCastSelf(SPELL_EARTH_DISSOLVE_IN);

        scheduler.Schedule(16s, [this](TaskContext /*task*/)
        {
            DoCastSelf(SPELL_EARTHEN_GRASP);
        });
    }

    void JustDied(Unit* /*killer*/) override
    {
        instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

        _JustDied();
    }

    void EnterEvadeMode(EvadeReason /*why*/) override
    {
        instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

        summons.DespawnAll();
        _EnterEvadeMode();
        _DespawnAtEvade();
    }

    void JustEngagedWith(Unit* who) override
    {
        BossAI::JustEngagedWith(who);
        instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me, 1);
    }

    void UpdateAI(uint32 diff) override
    {
        if (!UpdateVictim())
            return;

        events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = events.ExecuteEvent())
        {
            switch (eventId)
            {
                default:
                    break;
            }

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;
        }

        DoMeleeAttackIfReady();
    }
};

Position const DezranSpawnPosition = { 2379.7415f, 2482.3176f, 582.5888f, 0.0f };

// 21533 - Echo of Neltharion Intro
class conversation_echo_of_neltharion_intro : public ConversationScript
{
public:
    conversation_echo_of_neltharion_intro() : ConversationScript("conversation_echo_of_neltharion_intro") { }

    enum ActorData
    {
        ACTOR_NELTHARION    = 90020,
        ACTOR_WRATHION      = 89965,
        ACTOR_SABELLIAN     = 89966,
        ACTOR_DEZRAN        = 0,
    };

    enum EventData
    {
        EVENT_CAST_EARTHEN_GRASP = 1,
        EVENT_SPAWN_DEZRAN,
    };

    void OnConversationCreate(Conversation* conversation, Unit* /*creator*/) override
    {
        InstanceScript* instance = conversation->GetInstanceScript();
        if (!instance)
            return;

        if (Creature* neltharion = instance->GetCreature(DATA_ECHO_OF_NELTHARION))
            conversation->AddActor(ACTOR_NELTHARION, 0, neltharion->GetGUID());
        if (Creature* sabellian = instance->GetCreature(DATA_ECHO_OF_NELTHARION_SABELLIAN))
            conversation->AddActor(ACTOR_SABELLIAN, 1, sabellian->GetGUID());
        if (Creature* wrathion = instance->GetCreature(DATA_ECHO_OF_NELTHARION_WRATHION))
            conversation->AddActor(ACTOR_WRATHION, 2, wrathion->GetGUID());
    }

    /*void OnConversationStart(Conversation* conversation) override
    {

    }*/

    void OnConversationUpdate(Conversation* conversation, uint32 diff) override
    {
        _events.Update(diff);

        switch (_events.ExecuteEvent())
        {
            case EVENT_CAST_EARTHEN_GRASP:
                break;
            case EVENT_SPAWN_DEZRAN:
            {
                TempSummon* dezran = conversation->GetMap()->SummonCreature(NPC_ECHO_OF_NELTHARION_WINGLORD_DEZRAN, DezranSpawnPosition);
                if (!dezran)
                    break;

                _dezranGUID = dezran->GetGUID();
                conversation->AddActor(ACTOR_DEZRAN, 3, _dezranGUID);
                // @TODO: continue here
                break;
            }
            default:
                break;
        }
    }

private:
    EventMap _events;
    ObjectGuid _dezranGUID;
};

void AddSC_boss_echo_of_neltharion()
{
    RegisterAberrusTheShadowedCrucibleCreatureAI(boss_echo_of_neltharion);

    new conversation_echo_of_neltharion_intro();
}
