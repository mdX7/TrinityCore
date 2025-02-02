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

#include "Containers.h"
#include "CreatureAIImpl.h"
#include "ObjectAccessor.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "TemporarySummon.h"
#include "Vehicle.h"
#include <MovementPackets.h>

enum ViolentImpactData
{
    SPELL_RIDE_VEHICLE_HARDCODED    = 46598
};

// 214298 - Pile of Rubble
struct npc_pile_of_rubble_violent_impact : public ScriptedAI
{
    npc_pile_of_rubble_violent_impact(Creature* creature) : ScriptedAI(creature) { }

    void IsSummonedBy(WorldObject* summoner) override
    {
        if (Player* player = summoner->ToPlayer())
        {
            player->SetDisableGravity(true, false);

            summoner->CastSpell(me, SPELL_RIDE_VEHICLE_HARDCODED, CastSpellExtraArgsInit{
                .TriggerFlags = TRIGGERED_IGNORE_CAST_IN_PROGRESS | TRIGGERED_DONT_REPORT_CAST_ERROR
            });

            // required for laying animation, depends on MovementFlags2 PreventChangePitch or InterpolatePitching
            //me->m_Events.AddEventAtOffset([this, summoner]()
            //{
            //    WorldPackets::Movement::MoveUpdate moveUpdate;
            //    moveUpdate.Status = &summoner->m_movementInfo;
            //    summoner->SendMessageToSet(moveUpdate.Write(), true);
            //}, 500ms);
        }
    }

    void PassengerBoarded(Unit* /*passenger*/, int8 /*seatId*/, bool apply) override
    {
        if (!apply)
            return;

        Vehicle* veh = me->GetVehicleKit();
        if (!veh)
            return;

        if (veh->GetAvailableSeatCount())
            return;

        // set flags after all seats are in use. (Initially removed in VehicleJoinEvent::Execute)
        // on retail its also added after all passengers entered
        me->SetNpcFlag(UNIT_NPC_FLAG_SPELLCLICK);
    }
};

void AddSC_chapter_breaking_point()
{
    // Creature
    RegisterCreatureAI(npc_pile_of_rubble_violent_impact);
}
