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

#ifndef TRINITY_WAYPOINTDEFINES_H
#define TRINITY_WAYPOINTDEFINES_H

#include "Define.h"
#include "EnumFlag.h"
#include "Optional.h"
#include <vector>

#define WAYPOINT_PATH_FLAG_FOLLOW_PATH_BACKWARDS_MINIMUM_NODES 2

enum class WaypointPathType : uint8
{
    Normal = 0,
    TakeOff = 1,
    Landing = 2,
};

enum class WaypointPathFlags : uint8
{
    None                                = 0x00,
    FollowPathBackwardsFromEndToStart   = 0x01,
    Running                             = 0x02,
    DisableGravity                      = 0x04,
};

DEFINE_ENUM_FLAG(WaypointPathFlags);

enum WaypointMoveType
{
    WAYPOINT_MOVE_TYPE_WALK,
    WAYPOINT_MOVE_TYPE_RUN,
    WAYPOINT_MOVE_TYPE_LAND,
    WAYPOINT_MOVE_TYPE_TAKEOFF,

    WAYPOINT_MOVE_TYPE_MAX
};

struct WaypointNode
{
    WaypointNode() : Id(0), X(0.f), Y(0.f), Z(0.f) { }
    WaypointNode(uint32 id, float x, float y, float z)
    {
        Id = id;
        X = x;
        Y = y;
        Z = z;
    }

    uint32 Id;
    float X;
    float Y;
    float Z;
};

struct WaypointPath
{
    WaypointPath() : Id(0), Type(WaypointPathType::Normal), Flags(WaypointPathFlags::None) { }
    WaypointPath(uint32 id, std::vector<WaypointNode>&& nodes, WaypointPathFlags flags = WaypointPathFlags::None)
    {
        Id = id;
        Nodes = nodes;
        Flags = flags;
    }

    uint64 Id;
    WaypointPathType Type;
    EnumFlag<WaypointPathFlags> Flags = WaypointPathFlags::None;
    Optional<float> FinalOrientation;
    uint64 NextPathId;
    uint32 NextPathDelay;

    std::vector<WaypointNode> Nodes;
};

#endif
