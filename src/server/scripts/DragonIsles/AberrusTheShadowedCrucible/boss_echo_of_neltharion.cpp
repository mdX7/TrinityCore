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
    SPELL_EARTHEN_GRASP_PLAYERS     = 409724,
    SPELL_EARTHEN_GRASP_NPCS        = 410836,
    SPELL_START_INTRO_CONVERSATION  = 408211, // casted via smart_scripts @ Sabellian

    // Dezran
    SPELL_INCANTATION_OF_FREEDOM    = 409721,
    SPELL_UNCONSCIOUS               = 197827,
};

enum EchoOfNeltharionEvents
{

};

enum EchoOfNeltharionActions
{
    ACTION_TRIGGER_DEZRAN_KNOCKBACK = 1,
    ACTION_TRIGGER_RP_TRANSFORM,
};

enum EchoOfNeltharionAnimKitIDs
{
    ANIM_KIT_KNEELING = 27493,
};

enum EchoOfNeltharionSpellVisualKitIDs
{
    SPELL_VISUAL_KIT_RP_UNKNOWN = 181075,
};

enum EchoOfNeltharionDisplayIds
{
    DISPLAYID_NELTHARION_INITIAL = 112765,
};

enum EchoOfNeltharionPaths
{
    PATH_DEZRAN = 202610 * 100,
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
        scheduler.ClearValidator();

        DoCastSelf(SPELL_TWISTED_SECRET);
        DoCastSelf(SPELL_P1_ENERGY_COLOR);
        DoCastSelf(SPELL_NELTHARION_STATS);
        DoCastSelf(SPELL_PRE_RP_TRANSFORM);
        DoCastSelf(SPELL_EARTH_DISSOLVE_IN);
    }

    void JustDied(Unit* /*killer*/) override
    {
        instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

        _JustDied();

        Creature* dezran = instance->GetCreature(DATA_ECHO_OF_NELTHARION_DEZRAN);
        if (!dezran)
            return;

        dezran->SetTemplateRooted(false);
        dezran->SetAIAnimKitId(ANIM_KIT_KNEELING);
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
        scheduler.Update(diff);

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

    void DoAction(int32 param)
    {
        switch (param)
        {
            case ACTION_TRIGGER_DEZRAN_KNOCKBACK:
                scheduler.Schedule(100ms, [this](TaskContext /*task*/)
                {
                    Creature* dezran = instance->GetCreature(DATA_ECHO_OF_NELTHARION_DEZRAN);
                    if (!dezran)
                        return;

                    me->CastSpell(dezran, SPELL_CALAMITOUS_STRIKE);
                });
                break;
            case ACTION_TRIGGER_RP_TRANSFORM:
                me->SetOrientation(me->GetHomePosition().GetOrientation());
                DoCastAOE(SPELL_RP_TRANSFORM);
                me->SetFaction(FACTION_MONSTER_2);
                me->SendCancelSpellVisualKit(SPELL_VISUAL_KIT_RP_UNKNOWN);
                break;
            default:
                break;
        }
    }
};

// 202610 - Winglord Dezran
struct npc_winglord_dezran_echo_of_neltharion : public ScriptedAI
{
    npc_winglord_dezran_echo_of_neltharion(Creature* creature) : ScriptedAI(creature) { }

    void WaypointPathEnded(uint32 /*nodeId*/, uint32 pathId)
    {
        if (pathId != PATH_DEZRAN)
            return;

        me->SetHomePosition(me->GetPosition());
        DoCastAOE(SPELL_INCANTATION_OF_FREEDOM);
    }

    void MovementInform(uint32 type, uint32 id) override
    {
        if (type != EFFECT_MOTION_TYPE)
            return;

        if (id != 0)
            return;

        me->SetHomePosition(me->GetHomePosition());
        me->SetTemplateRooted(true);
        DoCastAOE(SPELL_UNCONSCIOUS);
        me->SetStandState(UNIT_STAND_STATE_DEAD);
        me->SetImmuneToAll(true);
        me->SetUnitFlag(UNIT_FLAG_PREVENT_EMOTES_FROM_CHAT_TEXT | UNIT_FLAG_UNINTERACTIBLE);

        Creature* neltharion = me->GetInstanceScript()->GetCreature(DATA_ECHO_OF_NELTHARION);
        if (!neltharion)
            return;

        neltharion->AI()->DoAction(ACTION_TRIGGER_RP_TRANSFORM);
    }

    void OnSpellCast(SpellInfo const* spell)
    {
        if (spell->Id != SPELL_INCANTATION_OF_FREEDOM)
            return;

        Creature* neltharion = me->GetInstanceScript()->GetCreature(DATA_ECHO_OF_NELTHARION);
        if (!neltharion)
            return;

        neltharion->AI()->DoAction(ACTION_TRIGGER_DEZRAN_KNOCKBACK);
    }

    void DamageTaken(Unit* /*attacker*/, uint32& damage, DamageEffectType /*damageType*/, SpellInfo const* /*spellInfo = nullptr*/) override
    {
        if (me->GetHealth() <= damage)
            damage = me->GetHealth() - 1;
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

    enum LineData
    {
        CONVO_LINE_EARTHEN_GRASP = 56787,
        CONVO_LINE_DEZRAN_SPAWN  = 56792,
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

    void OnConversationStart(Conversation* conversation) override
    {
        if (Milliseconds const* earthenGraspTime = conversation->GetLineStartTime(DEFAULT_LOCALE, CONVO_LINE_EARTHEN_GRASP))
            _events.ScheduleEvent(EVENT_CAST_EARTHEN_GRASP, *earthenGraspTime);

        if (Milliseconds const* dezranSpawnTime = conversation->GetLineStartTime(DEFAULT_LOCALE, CONVO_LINE_DEZRAN_SPAWN))
            _events.ScheduleEvent(EVENT_SPAWN_DEZRAN, *dezranSpawnTime);
    }

    void OnConversationUpdate(Conversation* conversation, uint32 diff) override
    {
        _events.Update(diff);

        switch (_events.ExecuteEvent())
        {
            case EVENT_CAST_EARTHEN_GRASP:
            {
                InstanceScript* instance = conversation->GetInstanceScript();
                if (!instance)
                    return;

                Creature* neltharion = instance->GetCreature(DATA_ECHO_OF_NELTHARION);
                if (!neltharion)
                    return;

                neltharion->CastSpell(nullptr, SPELL_EARTHEN_GRASP_PLAYERS, false);
                break;
            }
            case EVENT_SPAWN_DEZRAN:
            {
                InstanceScript* instance = conversation->GetInstanceScript();
                if (!instance)
                    return;

                TempSummon* dezran = conversation->GetMap()->SummonCreature(NPC_ECHO_OF_NELTHARION_WINGLORD_DEZRAN, DezranSpawnPosition);
                if (!dezran)
                    break;

                conversation->AddActor(ACTOR_DEZRAN, 3, dezran->GetGUID());

                GameObject* exitDoor = instance->GetGameObject(DATA_ECHO_OF_NELTHARION_EXIT_DOOR);
                if (!exitDoor)
                    return;

                exitDoor->UseDoorOrButton(2 * IN_MILLISECONDS);

                dezran->GetMotionMaster()->MovePath(PATH_DEZRAN, false);
                break;
            }
            default:
                break;
        }
    }

private:
    EventMap _events;
};

// 409724 - Earthen Grasp
class spell_neltharion_earthen_grasp_players : public SpellScript
{
    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_EARTHEN_GRASP_NPCS });
    }

    void HandleAfterCast()
    {
        InstanceScript* instance = GetCaster()->GetInstanceScript();
        if (!instance)
            return;

        if (Creature* sabellian = instance->GetCreature(DATA_ECHO_OF_NELTHARION_SABELLIAN))
            sabellian->CastSpell(nullptr, SPELL_EARTHEN_GRASP_NPCS, true);

        if (Creature* wrathion = instance->GetCreature(DATA_ECHO_OF_NELTHARION_WRATHION))
            wrathion->CastSpell(nullptr, SPELL_EARTHEN_GRASP_NPCS, true);
    }

    void Register() override
    {
        AfterCast += SpellCastFn(spell_neltharion_earthen_grasp_players::HandleAfterCast);
    }
};

void AddSC_boss_echo_of_neltharion()
{
    RegisterAberrusTheShadowedCrucibleCreatureAI(boss_echo_of_neltharion);
    RegisterAberrusTheShadowedCrucibleCreatureAI(npc_winglord_dezran_echo_of_neltharion);

    new conversation_echo_of_neltharion_intro();

    RegisterSpellScript(spell_neltharion_earthen_grasp_players);
}
