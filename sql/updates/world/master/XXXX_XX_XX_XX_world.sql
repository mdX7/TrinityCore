SET @CGUID := 11500000;

-- Creature
DELETE FROM `creature` WHERE `guid` BETWEEN @CGUID+0 AND @CGUID+1;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnDifficulties`, `PhaseId`, `PhaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `wander_distance`, `currentwaypoint`, `MovementType`, `npcflag`, `unit_flags`, `unit_flags2`, `unit_flags3`, `VerifiedBuild`) VALUES
(@CGUID+0, 217930, 2552, 14717, 14774, '0', 22876, 0, 0, 0, 2034.3472900390625, -808.328125, -0.77019119262695312, 4.697892665863037109, 120, 0, 0, 0, NULL, NULL, NULL, NULL, 56311), -- Pile of Rubble (Area: Tranquil Strand - Difficulty: 0) CreateObject1 (Auras: )
(@CGUID+1, 214449, 2552, 14717, 14774, '0', 22711, 0, 0, 0, 2032.93408203125, -819.23785400390625, -0.42339858412742614, 1.706894516944885253, 120, 0, 0, 0, NULL, NULL, NULL, NULL, 58867); -- Kirin Tor Mage (Area: Tranquil Strand - Difficulty: 0) CreateObject1 (Auras: 159474 - Permanent Feign Death (NO Stun, Untrackable, Immune))

-- Template Addon
DELETE FROM `creature_template_addon` WHERE `entry` IN (214449);
INSERT INTO `creature_template_addon` (`entry`, `PathId`, `mount`, `StandState`, `AnimTier`, `VisFlags`, `SheathState`, `PvpFlags`, `emote`, `aiAnimKit`, `movementAnimKit`, `meleeAnimKit`, `visibilityDistanceType`, `auras`) VALUES
(214449, 0, 0, 0, 0, 4, 1, 0, 0, 0, 0, 0, 0, '159474'); -- 214449 (Kirin Tor Mage) - Permanent Feign Death (NO Stun, Untrackable, Immune)

-- Template
UPDATE `creature_template` SET `faction`=35, `speed_run`=1, `BaseAttackTime`=2000, `unit_flags`=512, `unit_flags2`=67110912, `unit_flags3`=1090519040, `VehicleId`=8297 WHERE `entry`=214298; -- Pile of Rubble
UPDATE `creature_template` SET `faction`=35, `speed_run`=1, `BaseAttackTime`=2000, `unit_flags`=33555200, `unit_flags2`=67110912, `unit_flags3`=1090519040 WHERE `entry` IN (214302, 214300); -- Rubble
UPDATE `creature_template` SET `faction`=35, `npcflag`=2, `speed_run`=1, `BaseAttackTime`=2000, `unit_flags`=512, `unit_flags2`=67110912, `unit_flags3`=1090519040, `VehicleId`=8297 WHERE `entry`=217930; -- Pile of Rubble

UPDATE `creature_template` SET `AIName` = '', `ScriptName` = 'npc_pile_of_rubble_violent_impact' WHERE `entry` = 214298;

-- Difficulty
UPDATE `creature_template_difficulty` SET `ContentTuningID`=2797, `StaticFlags1`=0x30000100, `VerifiedBuild`=58867 WHERE (`Entry`=214302 AND `DifficultyID`=0); -- 214302 (Rubble) - Sessile, CanSwim, Floating
UPDATE `creature_template_difficulty` SET `ContentTuningID`=2797, `StaticFlags1`=0x20000100, `StaticFlags3`=0x2000000, `VerifiedBuild`=58867 WHERE (`Entry`=217930 AND `DifficultyID`=0); -- 217930 (Pile of Rubble) - Sessile, Floating - CannotTurn
UPDATE `creature_template_difficulty` SET `ContentTuningID`=2797, `StaticFlags1`=0x30000100, `VerifiedBuild`=58867 WHERE (`Entry`=214300 AND `DifficultyID`=0); -- 214300 (Rubble) - Sessile, CanSwim, Floating
UPDATE `creature_template_difficulty` SET `ContentTuningID`=2797, `StaticFlags1`=0x10000000, `VerifiedBuild`=58867 WHERE (`Entry`=214449 AND `DifficultyID`=0); -- 214449 (Kirin Tor Mage) - CanSwim

-- NPC Spellclick
DELETE FROM `npc_spellclick_spells` WHERE `npc_entry` IN (214298, 217930);
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES
(214298, 429488, 1, 0),
(217930, 46598, 1, 0);

-- Spelltarget pos.
DELETE FROM `spell_target_position` WHERE (`EffectIndex`=0 AND `ID` IN (440763,429500,440302,440518)) OR (`EffectIndex`=5 AND `ID`=429498);
INSERT INTO `spell_target_position` (`ID`, `EffectIndex`, `MapID`, `PositionX`, `PositionY`, `PositionZ`, `VerifiedBuild`) VALUES
(440763, 0, 2552, 2034.3800048828125, -808.1500244140625, -0.30000001192092895, 56311), -- Spell: 440763 (Teleport to Rubble [DNT]) Efffect: 252 (SPELL_EFFECT_TELEPORT_UNITS)
(429500, 0, 2552, 2034.3800048828125, -808.1500244140625, -0.30000001192092895, 56311), -- Spell: 429500 (Teleport to Rubble [DNT]) Efffect: 252 (SPELL_EFFECT_TELEPORT_UNITS)
(440302, 0, 2552, 2035.7900390625, -809.030029296875, -0.27000001072883605, 56311), -- Spell: 440302 (<DNT> Teleport) Efffect: 252 (SPELL_EFFECT_TELEPORT_UNITS)
(429498, 5, 2552, 2034.3800048828125, -808.1500244140625, -0.30000001192092895, 56311), -- Spell: 429498 (Trapped) Efffect: 252 (SPELL_EFFECT_TELEPORT_UNITS)
(440518, 0, 2731, 2395.2099609375, -1395.489990234375, 2032.0699462890625, 56311); -- Spell: 440518 (<DNT> Play Scene) Efffect: 45 (SPELL_EFFECT_PLAY_MOVIE)

-- Vehicle
DELETE FROM `vehicle_template_accessory` WHERE (`entry`=214298 AND `seat_id` IN (3,2,1));
INSERT INTO `vehicle_template_accessory` (`entry`, `accessory_entry`, `seat_id`, `minion`, `description`, `summontype`, `summontimer`, `RideSpellID`) VALUES
(214298, 214302, 3, 0, 'Pile of Rubble - Rubble', 8, 0, 46598), -- Pile of Rubble - Rubble
(214298, 214300, 2, 0, 'Pile of Rubble - Rubble', 8, 0, 46598), -- Pile of Rubble - Rubble
(214298, 214300, 1, 0, 'Pile of Rubble - Rubble', 8, 0, 46598); -- Pile of Rubble - Rubble

-- Phase
DELETE FROM `phase_name` WHERE `ID` IN (22876, 22711);
INSERT INTO `phase_name` (`ID`, `Name`) VALUES
(22876, 'Cosmetic - See Pile of Rubble (Questgiver)'),
(22711, 'Cosmetic - See killed Kirin Tor Mage');

DELETE FROM `phase_area` WHERE `PhaseId` IN (22876,22711); 
INSERT INTO `phase_area` (`AreaId`, `PhaseId`, `Comment`) VALUES
(14774, 22876, 'See Pile of Rubble (Questgiver)'),
(14774, 22711, 'Cosmetic - See killed Kirin Tor Mage');

DELETE FROM `conditions` WHERE (`SourceTypeOrReferenceId`=26 AND `SourceGroup` = 22876 AND `SourceEntry` = 0);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `Comment`) VALUE
(26, 22876, 0, 0, 0, 47, 0, 78529, 2|8|64, 0, 1, 'Apply Phase 22876 if Quest 78529 is not taken | complete | rewarded');

-- Quest
DELETE FROM `quest_details` WHERE `ID`=78529;
INSERT INTO `quest_details` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `VerifiedBuild`) VALUES
(78529, 0, 0, 0, 0, 0, 0, 0, 0, 56311); -- Violent Impact

DELETE FROM `creature_queststarter` WHERE (`id`=217930 AND `quest`=78529);
INSERT INTO `creature_queststarter` (`id`, `quest`, `VerifiedBuild`) VALUES
(217930, 78529, 56311); -- Violent Impact offered by Pile of Rubble

DELETE FROM `quest_description_conditional` WHERE (`QuestId`=82680 AND `PlayerConditionId`=125262 AND `QuestgiverCreatureId`=224394 AND `locale`='enUS') OR (`QuestId`=80176 AND `PlayerConditionId`=41725 AND `QuestgiverCreatureId`=219135 AND `locale`='enUS') OR (`QuestId`=80175 AND `PlayerConditionId`=41725 AND `QuestgiverCreatureId`=215335 AND `locale`='enUS');
INSERT INTO `quest_description_conditional` (`QuestId`, `PlayerConditionId`, `QuestgiverCreatureId`, `locale`, `Text`, `OrderIndex`, `VerifiedBuild`) VALUES
(82680, 125262, 224394, 'enUS', 'Well, hey! It\'s you again, $n.\n\nShame I don\'t have any elementals for you to get in shape...\n\nBut I do have another favor to ask you, if you\'re up for it.\n\nA friend of mine, Olbarig, lives out at Three Shields. He studies the wild cloudrooks there, and he and I talk about them from time to time.\n\nBut I haven\'t heard from him in a long time. Would you check on him for me?', 0, 56311), -- Birdman of the Three Shields
(80176, 41725, 219135, 'enUS', 'I\'ve almost got it! And you, a fellow mage, are perfect to help me!\n\nI\'m making a new kind of teleportation spell to get us back home to the Empire.\n\nI think these crabs are the key. They\'re practically radiating arcane teleportation magic!\n\nWe need more samples to study. \n\nTake this focus, it will let you see traces of the arcane. There\'s not much other arcane sources around here, so any you see should lead you straight to the affected crabs!', 0, 56311), -- Arcane Trickles
(80175, 41725, 215335, 'enUS', 'I want you to check in on someone. He\'s a mage... the only one of us who is. All the others died in the crash.\n\nHe\'s trying to make a portal to the Empire, so we can get reinforcements and have a way to and from home. \n\nWe all know it\'s a lost cause, but he still hasn\'t given up. \n\nMaybe meeting an experienced mage like you will give him the reality check he needs. Check on him, will you?', 0, 56311); -- The Last Mage

DELETE FROM `quest_completion_log_conditional` WHERE (`QuestId`=78529 AND `PlayerConditionId`=122973 AND `QuestgiverCreatureId`=0 AND `locale`='enUS') OR (`QuestId`=78529 AND `PlayerConditionId`=122972 AND `QuestgiverCreatureId`=0 AND `locale`='enUS');
INSERT INTO `quest_completion_log_conditional` (`QuestId`, `PlayerConditionId`, `QuestgiverCreatureId`, `locale`, `Text`, `OrderIndex`, `VerifiedBuild`) VALUES
(78529, 122973, 0, 'enUS', 'Find Jaina at the Tranquil Strand.', 1, 56311), -- Violent Impact
(78529, 122972, 0, 'enUS', 'Find Thrall at the Tranquil Strand.', 0, 56311); -- Violent Impact

-- Model
DELETE FROM `creature_model_info` WHERE `DisplayID`=116109;
INSERT INTO `creature_model_info` (`DisplayID`, `BoundingRadius`, `CombatReach`, `DisplayID_Other_Gender`, `VerifiedBuild`) VALUES
(116109, 0.300000011920928955, 0, 0, 58867);

UPDATE `creature_model_info` SET `VerifiedBuild`=58867 WHERE `DisplayID` IN (106700, 114107, 115979, 115980, 106691, 115978, 114826, 106699, 121969, 55233, 74160, 68019, 121983, 117186, 16259, 93415, 121970, 121979, 115747, 99451, 121971, 121973, 115495, 121981, 88316, 46607, 121987, 121974, 74161, 99452, 121972, 71445, 121991, 121977, 115745, 71599, 109350, 121982, 121988, 3257, 121978, 121980, 65187, 74159, 121985, 121990, 121976, 58289, 121984, 114531, 114532, 115110, 121975);
UPDATE `creature_model_info` SET `CombatReach`=9, `VerifiedBuild`=58867 WHERE `DisplayID`=114662;
UPDATE `creature_model_info` SET `BoundingRadius`=2.212586402893066406, `CombatReach`=2.03125, `VerifiedBuild`=58867 WHERE `DisplayID`=114512;

-- Quest: Violent Impact
SET @ENTRY := 78529;
INSERT IGNORE INTO `quest_template_addon` (`ID`) VALUES (@ENTRY);
UPDATE `quest_template_addon` SET `ScriptName` = 'SmartQuest' WHERE `ID` = @ENTRY;
DELETE FROM `smart_scripts` WHERE `source_type` = 5 AND `entryOrGuid` = @ENTRY;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `action_param7`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`, `Difficulties`) VALUES
(@ENTRY, 5, 0, 1, 47, 0, 100, 0, 0, 0, 0, 0, 0, 85, 429500, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'On quest accepted - Player who accepted: Cast spell 429500 on self', ''),
(@ENTRY, 5, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 85, 436050, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'On quest accepted - Player who accepted: Cast spell 436050 on self', ''),
(@ENTRY, 5, 2, 0, 51, 0, 100, 0, 0, 0, 0, 0, 0, 85, 429497, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'On quest failed - Player: Cast spell 429497 on self', '');

-- Scene 3324
SET @ENTRY := 3324;
UPDATE `scene_template` SET `ScriptName` = 'SmartScene' WHERE `SceneId` = @ENTRY;
DELETE FROM `smart_scripts` WHERE `source_type` = 10 AND `entryOrGuid` = @ENTRY;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `action_param7`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`, `Difficulties`) VALUES
(@ENTRY, 10, 0, 0, 81, 0, 100, 0, 0, 0, 0, 0, 0, 33, 214450, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'On Scene Complete - Self: Give Kill Credit: Spider Scene (214450)', '');
