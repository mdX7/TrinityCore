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

#include "AreaTriggerAI.h"
#include "GameObject.h"
#include "InstanceScript.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "TemporarySummon.h"
#include "aberrus_the_shadowed_crucible.h"

// @TODO: Calamitous Strike / Rushing Darkness arent destroying walls when you are already inside the areatrigger

// next session
// @TODO: fix intro not being triggered (sai bug?)
// @TODO: rescheduling stuff

enum EchoOfNeltharionSpells
{
    // Intro
    SPELL_TWISTED_SECRET                        = 401003,
    SPELL_P1_ENERGY_COLOR                       = 410672,
    SPELL_NELTHARION_STATS                      = 409970,
    SPELL_PRE_RP_TRANSFORM                      = 412812,
    SPELL_EARTH_DISSOLVE_IN                     = 413400,
    SPELL_EARTHEN_GRASP                         = 409724,
    SPELL_CALAMITOUS_STRIKE_DEZRAN              = 410848,
    SPELL_RP_TRANSFORM                          = 412813,

    // Combat
    SPELL_ENABLE_TWISTED_EARTH                  = 405772,
    SPELL_TWISTED_EARTH_INITIAL                 = 401480,
    SPELL_TWISTED_EARTH_MISSILE                 = 402830,
    SPELL_TWISTED_EARTH_AREATRIGGER             = 401796,
    SPELL_ENABLE_RUSHING_DARKNESS               = 407265,
    SPELL_RUSHING_DARKNESS_CAST                 = 407207,
    SPELL_RUSHING_DARKNESS_SELECTOR             = 407160,
    SPELL_RUSHING_DARKNESS_ARROW                = 407182,
    SPELL_RUSHING_DARKNESS_KNOCKBACK_LFR        = 407221,
    SPELL_VOLCANIC_BLAST                        = 402831,
    SPELL_VOLCANIC_HEART_SELECTOR               = 410968,
    SPELL_VOLCANIC_HEARTBEAT                    = 410966,
    SPELL_VOLCANIC_HEART_EXPLOSION              = 410953,

    SPELL_ENABLE_CALAMITOUS_STRIKE              = 405773,
    SPELL_CALAMITOUS_STRIKE_KNOCKBACK           = 401022,
    SPELL_CALAMITOUS_STRIKE_DAMAGE_TAKEN_AURA   = 401998,

    SPELL_SHATTER                               = 401825,
    SPELL_SHATTER_DAMAGE_TAKEN_AURA             = 411430,
    SPELL_SHATTER_DOT                           = 407329,
    SPELL_SHATTERED_ROCK_MISSILE                = 401873,

    // Wrathion and Sabellian
    SPELL_ANCHOR_HERE                           = 45313,
    SPELL_EARTHEN_GRASP_PLAYERS                 = 409724,
    SPELL_EARTHEN_GRASP_NPCS                    = 410836,
    SPELL_START_INTRO_CONVERSATION              = 408211, // casted via smart_scripts @ Sabellian

    // Dezran
    SPELL_INCANTATION_OF_FREEDOM                = 409721,
    SPELL_UNCONSCIOUS                           = 197827,
};

enum EchoOfNeltharionEvents
{
    EVENT_ENABLE_TWISTED_EARTH = 1,
    EVENT_CAST_TWISTED_EARTH,
    EVENT_ENERGIZE,
    EVENT_ENABLE_RUSHING_DARKNESS,
    EVENT_CAST_RUSHING_DARKNESS,
    EVENT_VOLCANIC_HEART,
    EVENT_ENABLE_CALAMITOUS_STRIKE,
    EVENT_CAST_CALAMITOUS_STRIKE,
};

enum EchoOfNeltharionActions
{
    ACTION_TRIGGER_DEZRAN_KNOCKBACK = 1,
    ACTION_TRIGGER_RP_TRANSFORM,
    ACTION_TWISTED_EARTH_P1,
};

enum EchoOfNeltharionAnimKitIDs
{
    ANIM_KIT_KNEELING = 27493,

    ANIM_KIT_WALL = 18309,
};

enum EchoOfNeltharionSpellVisualKitIDs
{
    SPELL_VISUAL_KIT_RP_UNKNOWN = 181075,

    SPELL_VISUAL_SHATTERED_ROCK = 126173,
};

enum EchoOfNeltharionPaths
{
    PATH_DEZRAN = 202610 * 100,
};

enum EchoOfNeltharionTexts
{
    SAY_AGGRO = 0,
};

struct TwistedEarthData
{
    Position MissilePosition;
    Milliseconds TimeOffset;
    Position GobPosition;
    QuaternionData GobRotation;
    QuaternionData GobParentRotation;
};

std::vector<TwistedEarthData> TwistedEarthP1Positions = {
    {
        { 2461.6162f, 2463.5781f, 582.32f, 5.8417773f },
        328ms,
        { 2461.6162f, 2463.5781f, 582.32f, 4.576287f },
        { 0.0f, 0.0f, -0.75355244f, 0.6573878f },
        { 0.0f, 0.0f, -0.7535527f, 0.65738755f },
    },
    {
        { 2451.97f, 2462.53f, 582.32f, 5.696903f },
        9ms,
        { 2451.97f, 2462.53f, 582.32f, 5.0649767f },
        { 0.0f, 0.0f, -0.57213306f, 0.8201608f },
        { 0.0f, 0.0f, -0.57213306f, 0.82016087f },
    },
    {
        { 2471.1143f, 2464.9854f, 582.32f, 5.945906f },
        581ms,
        { 2471.1143f, 2464.9854f, 582.32f, 5.142692f },
        { 0.0f, 0.0f, -0.53983974f, 0.8417678f },
        { 0.0f, 0.0f, -0.53984064f, 0.84176725f },
    },
    {
        { 2479.276f, 2470.5225f, 582.32f, 6.083717f },
        842ms,
        { 2479.276f, 2470.5225f, 582.32f, 5.474305f },
        { 0.0f, 0.0f, -0.39350414f, 0.91932285f },
        { 0.0f, 0.0f, -0.39350477f, 0.9193226f },
    },
    {
        { 2487.5884f, 2475.6938f, 582.32f, 6.1869593f },
        1082ms,
        { 2487.5884f, 2475.6938f, 582.32f, 5.0634904f },
        { 0.0f, 0.0f, -0.57274246f, 0.81973535f },
        { 0.0f, 0.0f, -0.57274264f, 0.8197352f },
    },
    {
        { 2497.2363f, 2476.7278f, 582.32f, 6.21315f },
        1332ms,
        { 2497.2363f, 2476.7278f, 582.32f, 4.574795f },
        { 0.0f, 0.0f, -0.7540426f, 0.6568255f },
        { 0.0f, 0.0f, -0.7540429f, 0.6568251f },
    },
    {
        { 2477.6162f, 2481.2864f, 582.32f, 6.2710085f },
        1332ms,
        { 2477.6162f, 2481.2864f, 582.32f, 5.942043f },
        { 0.0f, 0.0f, -0.16974545f, 0.98548794f },
        { 0.0f, 0.0f, -0.16974628f, 0.9854878f },
    },
    {
        { 2475.8958f, 2471.5747f, 582.32f, 6.0903916f },
        998ms,
        { 2475.8958f, 2471.5747f, 582.32f, 6.2736545f },
        { 0.0f, 0.0f, -0.0047655106f, 0.9999886f },
        { 0.0f, 0.0f, -0.0047656437f, 0.9999886f },
    },
    {
        { 2506.4294f, 2478.6914f, 582.32f, 6.2442183f },
        1581ms,
        { 2506.4294f, 2478.6914f, 582.32f, 5.270807f },
        { 0.0f, 0.0f, -0.48484802f, 0.8745984f },
        { 0.0f, 0.0f, -0.48484856f, 0.87459815f },
    },
    {
        { 2479.0203f, 2490.9912f, 582.32f, 0.15915672f },
        1581ms,
        { 2479.0203f, 2490.9912f, 582.32f, 0.053786084f },
        { 0.0f, 0.0f, 0.026889801f, 0.9996384f },
        { 0.0f, 0.0f, 0.026890708f, 0.99963844f },
    },
    {
        { 2514.3271f, 2484.7502f, 582.32f, 0.03050702f },
        1836ms,
        { 2514.3271f, 2484.7502f, 582.32f, 5.4627867f },
        { 0.0f, 0.0f, -0.39879227f, 0.9170413f },
        { 0.0f, 0.0f, -0.39879277f, 0.9170411f },
    },
    {
        { 2477.6196f, 2500.854f, 582.32f, 0.33156446f },
        1836ms,
        { 2477.6196f, 2500.854f, 582.32f, 0.22832079f },
        { 0.0f, 0.0f, 0.11391258f, 0.9934908f },
        { 0.0f, 0.0f, 0.11391337f, 0.9934907f },
    },
    {
        { 2522.984f, 2488.1133f, 582.32f, 0.061347686f },
        2080ms,
        { 2522.984f, 2488.1133f, 582.32f, 4.7030296f },
        { 0.0f, 0.0f, -0.7104082f, 0.7037899f },
        { 0.0f, 0.0f, -0.7104089f, 0.70378923f },
    },
    {
        { 2476.4932f, 2510.724f, 582.32f, 0.4912933f },
        2080ms,
        { 2476.4932f, 2510.724f, 582.32f, 6.282079f },
        { 0.0f, 0.0f, -0.0005531311f, 0.9999998f },
        { 0.0f, 0.0f, -0.0005532821f, 0.9999998f },
    },
    {
        { 2477.4583f, 2520.6313f, 582.32f, 0.6151565f },
        2327ms,
        { 2477.4583f, 2520.6313f, 582.32f, 6.0900927f },
        { 0.0f, 0.0f, -0.096396446f, 0.995343f },
        { 0.0f, 0.0f, -0.09639735f, 0.9953429f },
    },
    {
        { 2468.1694f, 2498.2334f, 582.321f, 0.3439605f },
        2327ms,
        { 2468.1694f, 2498.2334f, 582.321f, 0.9387494f },
        { 0.0f, 0.0f, 0.45232868f, 0.8918513f },
        { 0.0f, 0.0f, 0.452329f, 0.8918511f },
    },
    {
        { 2477.0503f, 2494.052f, 582.321f, 0.21910095f },
        1996ms,
        { 2477.0503f, 2494.052f, 582.321f, 1.3227234f },
        { 0.0f, 0.0f, 0.614192f, 0.7891566f },
        { 0.0f, 0.0f, 0.6141924f, 0.7891563f },
    },
    {
        { 2459.2139f, 2502.0708f, 582.321f, 0.5038205f },
        2575ms,
        { 2459.2139f, 2502.0708f, 582.321f, 1.3931813f },
        { 0.0f, 0.0f, 0.64160633f, 0.7670341f },
        { 0.0f, 0.0f, 0.6416071f, 0.76703346f },
    },
    {
        { 2477.6274f, 2530.4756f, 582.32f, 0.7237615f },
        2575ms,
        { 2477.6274f, 2530.4756f, 582.32f, 0.1587321f },
        { 0.0f, 0.0f, 0.07928276f, 0.99685216f },
        { 0.0f, 0.0f, 0.07928348f, 0.9968521f },
    },
    {
        { 2449.293f, 2502.8823f, 582.321f, 0.66717297f },
        2835ms,
        { 2449.293f, 2502.8823f, 582.321f, 1.5851707f },
        { 0.0f, 0.0f, 0.7121706f, 0.70200646f },
        { 0.0f, 0.0f, 0.71217066f, 0.7020064f },
    },
    {
        { 2476.0464f, 2540.3499f, 582.32f, 0.8307718f },
        2835ms,
        { 2476.0464f, 2540.3499f, 582.32f, 0.15882587f },
        { 0.0f, 0.0f, 0.07932949f, 0.99684846f },
        { 0.0f, 0.0f, 0.07932966f, 0.99684846f },
    },
    {
        { 2439.4307f, 2503.9731f, 582.321f, 0.92170733f },
        3087ms,
        { 2439.4307f, 2503.9731f, 582.321f, 1.3361071f },
        { 0.0f, 0.0f, 0.61945915f, 0.7850289f },
        { 0.0f, 0.0f, 0.6194594f, 0.7850287f },
    },
    {
        { 2449.611f, 2499.7083f, 582.32f, 0.5835767f },
        2999ms,
        { 2449.611f, 2499.7083f, 582.32f, 2.4455097f },
        { 0.0f, 0.0f, 0.9400425f, 0.3410574f },
        { 0.0f, 0.0f, 0.940043f, 0.34105608f },
    },
    {
        { 2442.526f, 2492.7168f, 582.32f, 0.49782255f },
        3329ms,
        { 2442.526f, 2492.7168f, 582.32f, 2.2535226f },
        { 0.0f, 0.0f, 0.9030256f, 0.4295867f },
        { 0.0f, 0.0f, 0.9030261f, 0.42958567f },
    },
    {
        { 2430.1714f, 2507.5176f, 582.321f, 1.2873011f },
        3329ms,
        { 2430.1714f, 2507.5176f, 582.321f, 1.0743017f },
        { 0.0f, 0.0f, 0.51169014f, 0.8591701f },
        { 0.0f, 0.0f, 0.5116902f, 0.8591701f },
    },
    {
        { 2420.7812f, 2510.1448f, 582.321f, 1.6395996f },
        3577ms,
        { 2420.7812f, 2510.1448f, 582.321f, 1.5217046f },
        { 0.0f, 0.0f, 0.68953896f, 0.7242486f },
        { 0.0f, 0.0f, 0.68953955f, 0.7242481f },
    },
    {
        { 2435.6055f, 2485.5938f, 582.32f, 0.27531585f },
        3577ms,
        { 2435.6055f, 2485.5938f, 582.32f, 2.4877102f },
        { 0.0f, 0.0f, 0.9470291f, 0.32114777f },
        { 0.0f, 0.0f, 0.94702923f, 0.32114732f },
    },
    {
        { 2410.7935f, 2510.6353f, 582.321f, 1.9650004f },
        3825ms,
        { 2410.7935f, 2510.6353f, 582.321f, 1.5217203f },
        { 0.0f, 0.0f, 0.6895447f, 0.72424316f },
        { 0.0f, 0.0f, 0.68954515f, 0.72424275f },
    },
    {
        { 2430.898f, 2476.9106f, 582.32f, 5.73029f },
        3825ms,
        { 2430.898f, 2476.9106f, 582.32f, 2.8018572f },
        { 0.0f, 0.0f, 0.98560715f, 0.169052f },
        { 0.0f, 0.0f, 0.985608f, 0.16904692f },
    },
    {
        { 2425.7388f, 2468.619f, 582.32f, 4.9347243f },
        4082ms,
        { 2425.7388f, 2468.619f, 582.32f, 2.368108f },
        { 0.0f, 0.0f, 0.9261427f, 0.37717333f },
        { 0.0f, 0.0f, 0.9261429f, 0.37717286f },
    },
    {
        { 2417.6667f, 2463.0332f, 582.32f, 4.451226f },
        4330ms,
        { 2417.6667f, 2463.0332f, 582.32f, 1.984137f },
        { 0.0f, 0.0f, 0.83715916f, 0.54695934f },
        { 0.0f, 0.0f, 0.83716f, 0.54695815f },
    },
    {
        { 2440.8452f, 2474.029f, 582.32f, 5.8708816f },
        4330ms,
        { 2440.8452f, 2474.029f, 582.32f, 3.5097048f },
        { 0.0f, 0.0f, -0.9831095f, 0.18301854f },
        { 0.0f, 0.0f, 0.9831101f, -0.18301524f },
    },
    {
        { 2435.6301f, 2482.3455f, 582.32f, 0.030257108f },
        4000ms,
        { 2435.6301f, 2482.3455f, 582.32f, 3.893673f },
        { 0.0f, 0.0f, -0.9301262f, 0.3672401f },
        { 0.0f, 0.0f, 0.9301267f, -0.36723876f },
    },
    {
        { 2446.3145f, 2465.9683f, 582.32f, 5.6876054f },
        4583ms,
        { 2446.3145f, 2465.9683f, 582.32f, 3.9658072f },
        { 0.0f, 0.0f, -0.91627884f, 0.40054104f },
        { 0.0f, 0.0f, 0.91627926f, -0.40054f },
    },
    {
        { 2411.807f, 2456.192f, 582.32f, 4.3115797f },
        4583ms,
        { 2411.807f, 2456.192f, 582.32f, 2.8824916f },
        { 0.0f, 0.0f, 0.99162006f, 0.12918846f },
        { 0.0f, 0.0f, 0.9916208f, 0.1291829f },
    },
    {
        { 2454.5947f, 2460.6367f, 582.32f, 5.6936445f },
        4845ms,
        { 2454.5947f, 2460.6367f, 582.32f, 4.3148756f },
        { 0.0f, 0.0f, -0.8328047f, 0.55356693f },
        { 0.0f, 0.0f, -0.83280486f, 0.55356675f },
    },
    {
        { 2462.5005f, 2454.941f, 582.32f, 5.686597f },
        5083ms,
        { 2462.5005f, 2454.941f, 582.32f, 3.8612132f },
        { 0.0f, 0.0f, -0.93596363f, 0.35209665f },
        { 0.0f, 0.0f, 0.9359639f, -0.35209587f },
    },
    {
        { 2467.2769f, 2446.405f, 582.32f, 5.6097126f },
        5329ms,
        { 2467.2769f, 2446.405f, 582.32f, 3.44234f },
        { 0.0f, 0.0f, -0.9887152f, 0.14980748f },
        { 0.0f, 0.0f, 0.988716f, -0.14980236f },
    },
    {
        { 2472.9917f, 2438.9692f, 582.32f, 5.57572f },
        5586ms,
        { 2472.9917f, 2438.9692f, 582.32f, 4.1513834f },
        { 0.0f, 0.0f, -0.87522507f, 0.4837159f },
        { 0.0f, 0.0f, 0.8752252f, -0.4837157f },
    },
    {
        { 2481.459f, 2433.649f, 582.32f, 5.5949125f },
        5828ms,
        { 2481.459f, 2433.649f, 582.32f, 4.151372f },
        { 0.0f, 0.0f, -0.8752279f, 0.48371074f },
        { 0.0f, 0.0f, 0.87522876f, -0.48370928f },
    },
};

// 201668 - Neltharion
struct boss_echo_of_neltharion : public BossAI
{
    boss_echo_of_neltharion(Creature* creature) : BossAI(creature, DATA_ECHO_OF_NELTHARION) { }

    void JustAppeared() override
    {
        scheduler.ClearValidator();

        uint8 introState = instance->GetData(DATA_ECHO_OF_NELTHARION_INTRO_STATE);
        DoCastSelf(SPELL_TWISTED_SECRET);
        DoCastSelf(SPELL_P1_ENERGY_COLOR);
        DoCastSelf(SPELL_NELTHARION_STATS);

        if (introState == DONE)
        {
            me->SetFaction(FACTION_MONSTER_2);
        }
        else
        {
            DoCastSelf(SPELL_PRE_RP_TRANSFORM);
            DoCastSelf(SPELL_EARTH_DISSOLVE_IN);
        }
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

        Talk(SAY_AGGRO);

        events.ScheduleEvent(EVENT_ENABLE_TWISTED_EARTH, 1s);
        events.ScheduleEvent(EVENT_ENERGIZE, 1s);
        events.ScheduleEvent(EVENT_ENABLE_RUSHING_DARKNESS, 10s);
        events.ScheduleEvent(EVENT_VOLCANIC_HEART, 16s);
        events.ScheduleEvent(EVENT_ENABLE_CALAMITOUS_STRIKE, 24s);
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
                case EVENT_ENABLE_TWISTED_EARTH:
                    DoCastAOE(SPELL_ENABLE_TWISTED_EARTH);
                    events.ScheduleEvent(EVENT_CAST_TWISTED_EARTH, 200ms);
                    break;
                case EVENT_CAST_TWISTED_EARTH:
                    DoCastAOE(SPELL_TWISTED_EARTH_INITIAL);
                    break;
                case EVENT_ENERGIZE:
                    me->SetPower(POWER_ENERGY, me->GetPower(POWER_ENERGY) + 3); // likely handled by some serverside aura, sadge
                    events.Repeat(1s);
                    break;
                case EVENT_ENABLE_RUSHING_DARKNESS:
                    DoCastAOE(SPELL_ENABLE_RUSHING_DARKNESS);
                    events.ScheduleEvent(EVENT_CAST_RUSHING_DARKNESS, 700ms);
                    break;
                case EVENT_CAST_RUSHING_DARKNESS:
                    DoCastAOE(SPELL_RUSHING_DARKNESS_CAST);
                    break;
                case EVENT_VOLCANIC_HEART:
                    DoCastAOE(SPELL_VOLCANIC_HEART_SELECTOR);
                    break;
                case EVENT_ENABLE_CALAMITOUS_STRIKE:
                    DoCastAOE(SPELL_ENABLE_CALAMITOUS_STRIKE);
                    events.ScheduleEvent(EVENT_CAST_CALAMITOUS_STRIKE, 3s + 500ms);
                    break;
                case EVENT_CAST_CALAMITOUS_STRIKE:
                    DoCastVictim(SPELL_CALAMITOUS_STRIKE_KNOCKBACK);
                    break;
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

                    me->CastSpell(dezran, SPELL_CALAMITOUS_STRIKE_DEZRAN);
                });
                break;
            case ACTION_TRIGGER_RP_TRANSFORM:
                me->SetOrientation(me->GetHomePosition().GetOrientation());
                DoCastAOE(SPELL_RP_TRANSFORM);
                me->SetFaction(FACTION_MONSTER_2);
                me->SendCancelSpellVisualKit(SPELL_VISUAL_KIT_RP_UNKNOWN);
                instance->SetData(DATA_ECHO_OF_NELTHARION_INTRO_STATE, DONE);
                break;
            case ACTION_TWISTED_EARTH_P1:
            {
                for (TwistedEarthData const& data : TwistedEarthP1Positions)
                {
                    scheduler.Schedule(data.TimeOffset, [this, data](TaskContext /*task*/)
                    {
                        CastSpellExtraArgs args(TRIGGERED_FULL_MASK);
                        args.SetCustomArg(data);
                        me->CastSpell(data.MissilePosition, SPELL_TWISTED_EARTH_MISSILE, args);
                    });
                }
                break;
            }
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

                dezran->SetWalk(false);
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

// 401480 - Twisted Earth
class spell_twisted_earth_initial : public SpellScript
{
    void HandleHitTarget(SpellEffIndex /*effIndex*/)
    {
        Creature* caster = GetCaster()->ToCreature();
        if (!caster)
            return;

        caster->AI()->DoAction(ACTION_TWISTED_EARTH_P1);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_twisted_earth_initial::HandleHitTarget, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

// 402831 - Volcanic Blast
class spell_volcanic_blast_summon_wall : public SpellScript
{
    void HandleHit(SpellEffIndex /*effIndex*/)
    {
        TwistedEarthData const* data = std::any_cast<TwistedEarthData>(&GetSpell()->m_customArg);
        if (!data)
            return;

        GameObject* wall = GetCaster()->SummonGameObject(GO_ROCK_WALL, data->GobPosition, data->GobRotation, 1440min, GO_SUMMON_TIMED_OR_CORPSE_DESPAWN);
        if (!wall)
            return;

        wall->SetParentRotation(data->GobParentRotation);

        wall->SetAnimKitId(ANIM_KIT_WALL, false);

        GetCaster()->CastSpell(data->GobPosition, SPELL_TWISTED_EARTH_AREATRIGGER, true);
    }

    void Register() override
    {
        OnEffectHit += SpellEffectFn(spell_volcanic_blast_summon_wall::HandleHit, EFFECT_2, SPELL_EFFECT_DUMMY);
    }
};

// 401796 - Twisted Earth
struct at_twisted_earth : AreaTriggerAI
{
    at_twisted_earth(AreaTrigger* areatrigger) : AreaTriggerAI(areatrigger) { }

    static constexpr float ROCK_PLAYER_RANGE = 4.0f;
    static constexpr uint32 ROCK_PLAYER_NUM = 15;

    void OnCreate(Spell const* /*creatingSpell*/) override
    {
        GameObject* wall = at->FindNearestGameObject(GO_ROCK_WALL, 1.0f);
        if (!wall)
            return;

        _wallGUID = wall->GetGUID();

        _scheduler.Schedule(500ms, [this](TaskContext task)
        {
            if (_destroyed)
                return;

            for (ObjectGuid const& guid : at->GetInsideUnits())
            {
                Unit* unit = ObjectAccessor::GetUnit(*at, guid);
                if (!unit)
                    continue;
                OnUnitEnter(unit);
            }
            task.Repeat(500ms);
        });
    }

    void CreateShatteredRock(Unit* caster, Position pos)
    {
        at->SendPlayOrphanSpellVisual(pos, SPELL_VISUAL_SHATTERED_ROCK, 2.0f, true);

        caster->CastSpell(pos, SPELL_SHATTERED_ROCK_MISSILE, true);
    }

    void HandleShatteredRocks()
    {
        Unit* caster = at->GetCaster();
        if (!caster)
            return;

        Creature* casterCreature = caster->ToCreature();
        if (!casterCreature)
            return;

        std::list<Unit*> targetList;
        casterCreature->AI()->SelectTargetList(targetList, ROCK_PLAYER_NUM, SelectTargetMethod::Random);
        if (targetList.empty())
            return;

        for (Unit* target : targetList)
            CreateShatteredRock(caster, target->GetRandomNearPosition(ROCK_PLAYER_RANGE));
    }

    void DestroyWall(Unit* triggeringUnit)
    {
        Unit* caster = at->GetCaster();
        if (!caster)
            return;

        caster->CastSpell(triggeringUnit, SPELL_SHATTER, true);
        HandleShatteredRocks();

        GameObject* wall = ObjectAccessor::GetGameObject(*at, _wallGUID);
        if (!wall)
            return;

        wall->DespawnOrUnsummon(); // respawn when?
        at->Remove();
        _destroyed = true;
    }

    void OnUnitEnter(Unit* unit) override
    {
        if (!unit->IsPlayer())
            return;

        if (unit->HasAura(SPELL_RUSHING_DARKNESS_KNOCKBACK_LFR) || unit->HasAura(SPELL_CALAMITOUS_STRIKE_KNOCKBACK))
            DestroyWall(unit);
    }

    void OnUpdate(uint32 diff) override
    {
        _scheduler.Update(diff);
    }

private:
    ObjectGuid _wallGUID;
    TaskScheduler _scheduler;
    bool _destroyed;
};

// 407207 - Rushing Darkness
class spell_rushing_darkness_initial_cast : public SpellScript
{
    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_RUSHING_DARKNESS_SELECTOR });
    }

    void OnPrecast() override
    {
        GetCaster()->CastSpell(nullptr, SPELL_RUSHING_DARKNESS_SELECTOR, true);
    }

    void Register() override
    {

    }
};

// 407160 - Rushing Darkness
class spell_rushing_darkness_selector : public SpellScript
{
    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_RUSHING_DARKNESS_ARROW });
    }

    void HandleHitTarget(SpellEffIndex /*effIndex*/)
    {
        GetCaster()->CastSpell(GetHitUnit(), SPELL_RUSHING_DARKNESS_ARROW);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_rushing_darkness_selector::HandleHitTarget, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

// 407182 - Rushing Darkness
class spell_rushing_darkness_arrow : public AuraScript
{
    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_RUSHING_DARKNESS_KNOCKBACK_LFR });
    }

    void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Unit* caster = GetCaster();
        if (!caster)
            return;

        caster->CastSpell(GetTarget(), SPELL_RUSHING_DARKNESS_KNOCKBACK_LFR, true);
    }

    void Register() override
    {
        AfterEffectRemove += AuraEffectApplyFn(spell_rushing_darkness_arrow::HandleRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

// 410968 - Volcanic Heart
class spell_volcanic_heart_selector : public SpellScript
{
    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_VOLCANIC_HEARTBEAT });
    }

    void HandleHitTarget(SpellEffIndex /*effIndex*/)
    {
        GetCaster()->CastSpell(GetHitUnit(), SPELL_VOLCANIC_HEARTBEAT);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_volcanic_heart_selector::HandleHitTarget, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

// 410966 - Volcanic Heartbeat
class spell_volcanic_heartbeat : public AuraScript
{
    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_VOLCANIC_HEART_EXPLOSION });
    }

    void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Unit* caster = GetCaster();
        if (!caster)
            return;

        caster->CastSpell(GetTarget(), SPELL_VOLCANIC_HEART_EXPLOSION, true);
    }

    void Register() override
    {
        AfterEffectRemove += AuraEffectApplyFn(spell_volcanic_heartbeat::HandleRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

// 401022 - Calamitous Strike
class spell_calamitous_strike_knockback : public SpellScript
{
    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_CALAMITOUS_STRIKE_DAMAGE_TAKEN_AURA });
    }

    void HandleHitTarget(SpellEffIndex /*effIndex*/)
    {
        GetCaster()->CastSpell(GetHitUnit(), SPELL_CALAMITOUS_STRIKE_DAMAGE_TAKEN_AURA);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_calamitous_strike_knockback::HandleHitTarget, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

// 401825 - Shatter
class spell_shatter_echo_of_neltharion : public SpellScript
{
    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_SHATTER_DOT });
    }

    void HandleHitTarget(SpellEffIndex /*effIndex*/)
    {
        GetCaster()->CastSpell(GetHitUnit(), SPELL_SHATTER_DOT);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_shatter_echo_of_neltharion::HandleHitTarget, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

void AddSC_boss_echo_of_neltharion()
{
    RegisterAberrusTheShadowedCrucibleCreatureAI(boss_echo_of_neltharion);
    RegisterAberrusTheShadowedCrucibleCreatureAI(npc_winglord_dezran_echo_of_neltharion);

    new conversation_echo_of_neltharion_intro();

    RegisterSpellScript(spell_neltharion_earthen_grasp_players);
    RegisterSpellScript(spell_twisted_earth_initial);
    RegisterSpellScript(spell_volcanic_blast_summon_wall);
    RegisterAreaTriggerAI(at_twisted_earth);

    RegisterSpellScript(spell_rushing_darkness_initial_cast);
    RegisterSpellScript(spell_rushing_darkness_selector);
    RegisterSpellScript(spell_rushing_darkness_arrow);

    RegisterSpellScript(spell_volcanic_heart_selector);
    RegisterSpellScript(spell_volcanic_heartbeat);

    RegisterSpellScript(spell_calamitous_strike_knockback);

    RegisterSpellScript(spell_shatter_echo_of_neltharion);
}
