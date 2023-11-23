-- 
SET @CGUID := 9900000;
SET @OGUID := 9900000;
SET @NPCTEXTID := 599000;

DELETE FROM `creature_equip_template` WHERE (`ID`=1 AND `CreatureID` IN (198870, 201668, 203133, 204162, 205620, 205622));
INSERT INTO `creature_equip_template` (`CreatureID`, `ID`, `ItemID1`, `AppearanceModID1`, `ItemVisual1`, `ItemID2`, `AppearanceModID2`, `ItemVisual2`, `ItemID3`, `AppearanceModID3`, `ItemVisual3`, `VerifiedBuild`) VALUES
(198870, 1, 191778, 0, 0, 194930, 0, 0, 0, 0, 0, 52106), -- Sundered Preserver
(201668, 1, 202374, 0, 0, 0, 0, 0, 0, 0, 0, 52106), -- Neltharion
(203133, 1, 202379, 0, 0, 0, 0, 0, 0, 0, 0, 52106), -- Neltharion
(204162, 1, 116837, 0, 0, 0, 0, 0, 0, 0, 0, 52106), -- Sundered Body
(205620, 1, 191867, 0, 0, 0, 0, 0, 0, 0, 0, 52106), -- Malgosa Spellbinder
(205622, 1, 200727, 0, 0, 200727, 0, 0, 0, 0, 0, 52106); -- Krono Sandtongue

DELETE FROM `areatrigger_template` WHERE (`IsServerSide`=0 AND `Id` IN (32106, 32509, 32559, 32593));
INSERT INTO `areatrigger_template` (`Id`, `IsServerSide`, `Type`, `Flags`, `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`, `Data6`, `Data7`, `VerifiedBuild`) VALUES
(32106, 0, 0, 0, 5, 5, 0, 0, 0, 0, 0, 0, 52106),
(32509, 0, 0, 4, 150, 150, 0, 0, 0, 0, 0, 0, 52106),
(32559, 0, 4, 0, 24, 24, 10, 10, 0.300000011920928955, 0.300000011920928955, 0, 0, 52106),
(32593, 0, 0, 4, 150, 150, 0, 0, 0, 0, 0, 0, 52106);

DELETE FROM `areatrigger_create_properties` WHERE `Id` IN (28968, 28697, 29356, 28785);
INSERT INTO `areatrigger_create_properties` (`Id`, `AreaTriggerId`, `MoveCurveId`, `ScaleCurveId`, `MorphCurveId`, `FacingCurveId`, `AnimId`, `AnimKitId`, `DecalPropertiesId`, `TimeToTarget`, `TimeToTargetScale`, `Shape`, `ShapeData0`, `ShapeData1`, `ShapeData2`, `ShapeData3`, `ShapeData4`, `ShapeData5`, `ShapeData6`, `ShapeData7`, `VerifiedBuild`) VALUES
(28968, 32106, 0, 0, 0, 0, -1, 0, 521, 0, 600000, 0, 5, 5, 0, 0, 0, 0, 0, 0, 52106), -- Spell: 403908 (Sunder Reality)
(28697, 32509, 0, 0, 0, 0, -1, 0, 0, 0, 12000, 0, 150, 150, 0, 0, 0, 0, 0, 0, 52106), -- Spell: 408420 (Ebon Destruction)
(29356, 32559, 0, 0, 0, 0, -1, 0, 492, 0, 94000, 4, 24, 24, 10, 10, 0.300000011920928955, 0.300000011920928955, 0, 0, 52106), -- Spell: 403288 (Echoing Fissure)
(28785, 32593, 0, 0, 0, 0, -1, 0, 0, 0, 5000, 0, 150, 150, 0, 0, 0, 0, 0, 0, 52106); -- Spell: 409313 (Raze the Earth)

DELETE FROM `conversation_line_template` WHERE `Id` IN (56784, 56785, 56786, 56787, 56788, 56789, 56790, 56791, 56792, 56793);
INSERT INTO `conversation_line_template` (`Id`, `UiCameraID`, `ActorIdx`, `Flags`, `ChatType`, `VerifiedBuild`) VALUES
(56784, 0, 0, 0, 0, 52106),
(56785, 0, 1, 0, 0, 52106),
(56786, 0, 2, 0, 0, 52106),
(56787, 0, 0, 0, 0, 52106),
(56788, 0, 1, 0, 0, 52106),
(56789, 0, 2, 0, 0, 52106),
(56790, 0, 0, 0, 0, 52106),
(56791, 0, 0, 0, 0, 52106),
(56792, 0, 0, 0, 0, 52106),
(56793, 0, 3, 0, 0, 52106);

DELETE FROM `conversation_template` WHERE `Id` IN (21533);
INSERT INTO `conversation_template` (`Id`, `FirstLineID`, `TextureKitId`, `ScriptName`, `VerifiedBuild`) VALUES
(21533, 56784, 0, 'conversation_echo_of_neltharion_intro', 52106);

DELETE FROM `creature` WHERE `guid` BETWEEN @CGUID+0 AND @CGUID+17;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnDifficulties`, `PhaseId`, `PhaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `wander_distance`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `unit_flags2`, `unit_flags3`, `StringId`, `VerifiedBuild`) VALUES
(@CGUID+0, 198874, 2569, 14663, 14663, '14,15,16,17', '0', 0, 0, 1, 2451.3681640625, 2512.174560546875, 582.5765380859375, 5.885982036590576171, 604800, 0, 0, 5338272, 0, 0, NULL, NULL, NULL, NULL, 'neltharion_trash', 52106), -- Sundered Siegemaster (Area: Aberrus, the Shadowed Crucible - Difficulty: 0) CreateObject1 (Auras: )
(@CGUID+1, 198874, 2569, 14663, 14663, '14,15,16,17', '0', 0, 0, 1, 2461.692626953125, 2478.526123046875, 582.5765380859375, 6.183114528656005859, 604800, 0, 0, 5111340, 0, 0, NULL, NULL, NULL, NULL, 'neltharion_trash', 52106), -- Sundered Siegemaster (Area: Aberrus, the Shadowed Crucible - Difficulty: 0) CreateObject1 (Auras: )
(@CGUID+2, 198869, 2569, 14663, 14663, '14,15,16,17', '0', 0, 0, 1, 2465.635498046875, 2531.940185546875, 582.4029541015625, 5.533048629760742187, 604800, 0, 0, 4893416, 0, 0, NULL, NULL, NULL, NULL, 'neltharion_trash', 52106), -- Sundered Devastator (Area: Aberrus, the Shadowed Crucible - Difficulty: 0) CreateObject1 (Auras: )
(@CGUID+3, 198871, 2569, 14663, 14663, '14,15,16,17', '0', 0, 0, 1, 2462.5400390625, 2488.213623046875, 582.5765380859375, 6.183114528656005859, 604800, 0, 0, 5537285, 0, 0, NULL, NULL, NULL, NULL, 'neltharion_trash', 52106), -- Sundered Scaleguard (Area: Aberrus, the Shadowed Crucible - Difficulty: 0) CreateObject1 (Auras: )
(@CGUID+4, 198870, 2569, 14663, 14663, '14,15,16,17', '0', 0, 0, 1, 2449.944580078125, 2503.759521484375, 582.5765380859375, 5.885982036590576171, 604800, 0, 0, 4893416, 0, 0, NULL, NULL, NULL, NULL, 'neltharion_trash', 52106), -- Sundered Preserver (Area: Aberrus, the Shadowed Crucible - Difficulty: 0) CreateObject1 (Auras: )
(@CGUID+5, 198873, 2569, 14663, 14663, '14,15,16,17', '0', 0, 0, 1, 2458.494873046875, 2532.049560546875, 582.4029541015625, 5.533048629760742187, 604800, 0, 0, 5338272, 0, 0, NULL, NULL, NULL, NULL, 'neltharion_trash', 52106), -- Sundered Edgelord (Area: Aberrus, the Shadowed Crucible - Difficulty: 0) CreateObject1 (Auras: )
(@CGUID+6, 198872, 2569, 14663, 14663, '14,15,16,17', '0', 0, 0, 1, 2451.507080078125, 2453.4462890625, 582.5765380859375, 0.589351177215576171, 604800, 0, 0, 4685395, 0, 0, NULL, NULL, NULL, NULL, 'neltharion_trash', 52106), -- Sundered Manaweaver (Area: Aberrus, the Shadowed Crucible - Difficulty: 0) CreateObject1 (Auras: )
(@CGUID+7, 198873, 2569, 14663, 14663, '14,15,16,17', '0', 0, 0, 1, 2464.671875, 2433.73095703125, 582.5765380859375, 0.777049660682678222, 604800, 0, 0, 5111340, 0, 0, NULL, NULL, NULL, NULL, 'neltharion_trash', 52106), -- Sundered Edgelord (Area: Aberrus, the Shadowed Crucible - Difficulty: 0) CreateObject1 (Auras: )
(@CGUID+8, 198874, 2569, 14663, 14663, '14,15,16,17', '0', 0, 0, 1, 2448.132080078125, 2465.060791015625, 582.5765380859375, 0.164373502135276794, 604800, 0, 0, 5111340, 0, 0, NULL, NULL, NULL, NULL, 'neltharion_trash', 52106), -- Sundered Siegemaster (Area: Aberrus, the Shadowed Crucible - Difficulty: 0) CreateObject1 (Auras: )
(@CGUID+9, 198874, 2569, 14663, 14663, '14,15,16,17', '0', 0, 0, 1, 2467.635498046875, 2441.359375, 582.5765380859375, 0.777049660682678222, 604800, 0, 0, 5111340, 0, 0, NULL, NULL, NULL, NULL, 'neltharion_trash', 52106), -- Sundered Siegemaster (Area: Aberrus, the Shadowed Crucible - Difficulty: 0) CreateObject1 (Auras: )
(@CGUID+10, 198874, 2569, 14663, 14663, '14,15,16,17', '0', 0, 0, 1, 2460.803955078125, 2524.97216796875, 582.4029541015625, 5.533048629760742187, 604800, 0, 0, 5111340, 0, 0, NULL, NULL, NULL, NULL, 'neltharion_trash', 52106), -- Sundered Siegemaster (Area: Aberrus, the Shadowed Crucible - Difficulty: 0) CreateObject1 (Auras: )
(@CGUID+11, 198874, 2569, 14663, 14663, '14,15,16,17', '0', 0, 0, 1, 2456.194580078125, 2483.467041015625, 582.5765380859375, 6.183114528656005859, 604800, 0, 0, 5111340, 0, 0, NULL, NULL, NULL, NULL, 'neltharion_trash', 52106), -- Sundered Siegemaster (Area: Aberrus, the Shadowed Crucible - Difficulty: 0) CreateObject1 (Auras: )
(@CGUID+12, 198873, 2569, 14663, 14663, '14,15,16,17', '0', 0, 0, 1, 2472.01220703125, 2432.08154296875, 582.5765380859375, 0.644618153572082519, 604800, 0, 0, 5111340, 0, 0, NULL, NULL, NULL, NULL, 'neltharion_trash', 52106), -- Sundered Edgelord (Area: Aberrus, the Shadowed Crucible - Difficulty: 0) CreateObject1 (Auras: )
(@CGUID+13, 201575, 2569, 14663, 14663, '14,15,16,17', '0', 0, 0, 0, 2532.7587890625, 2476.48779296875, 584.00372314453125, 3.05888223648071289, 604800, 0, 0, 72041600, 3387, 0, NULL, NULL, NULL, NULL, 52106), -- Sabellian (Area: Aberrus, the Shadowed Crucible - Difficulty: Looking For Raid) CreateObject1 (Auras: 410836 - Earthen Grasp)
(@CGUID+14, 201574, 2569, 14663, 14663, '14,15,16,17', '0', 0, 0, 0, 2532.83251953125, 2486.69091796875, 584.03533935546875, 3.05888223648071289, 604800, 0, 0, 54031200, 0, 0, NULL, NULL, NULL, NULL, 52106), -- Wrathion (Area: Aberrus, the Shadowed Crucible - Difficulty: Looking For Raid) CreateObject1 (Auras: 410836 - Earthen Grasp)
(@CGUID+15, 205620, 2569, 14663, 14663, '14,15,16,17', '0', 0, 0, 1, 2443.927001953125, 2458.454833984375, 582.5765380859375, 0.210782811045646667, 604800, 0, 0, 17889690, 0, 0, NULL, NULL, NULL, NULL, 'neltharion_trash', 52106), -- Malgosa Spellbinder (Area: Aberrus, the Shadowed Crucible - Difficulty: Looking For Raid) CreateObject1 (Auras: 411710 - Primal Senses)
(@CGUID+16, 205622, 2569, 14663, 14663, '14,15,16,17', '0', 0, 0, 1, 2445.553955078125, 2509.205810546875, 582.5765380859375, 5.885982036590576171, 604800, 0, 0, 17889690, 0, 0, NULL, NULL, NULL, NULL, 'neltharion_trash', 52106), -- Krono Sandtongue (Area: Aberrus, the Shadowed Crucible - Difficulty: Looking For Raid) CreateObject1 (Auras: 42459 - Dual Wield, 411710 - Primal Senses)
(@CGUID+17, 201668, 2569, 14663, 14663, '14,15,16,17', '0', 0, 0, 1, 2422.723876953125, 2481.954833984375, 582.4029541015625, 0, 604800, 0, 0, 210695472, 100, 0, NULL, NULL, NULL, NULL, 52106); -- Neltharion (Area: Aberrus, the Shadowed Crucible - Difficulty: Looking For Raid) CreateObject2 (Auras: 401003 - Twisted Secret)
-- (@CGUID+998, 202610, 2569, 14663, 14663, '14,15,16,17', '0', 0, 0, 0, 2379.741455078125, 2482.317626953125, 582.58880615234375, 0, 604800, 0, 0, 18010400, 3155, 0, NULL, NULL, NULL, NULL, 52106), -- Winglord Dezran (Area: Aberrus, the Shadowed Crucible - Difficulty: Looking For Raid) CreateObject2 (Auras: 197827 - Unconscious)
-- (@CGUID+1233, 203812, 2569, 14663, 14663, '14,15,16,17', '0', 0, 0, 0, 2450.94091796875, 2518.053955078125, 582.4029541015625, 5.18841552734375, 604800, 0, 0, 11566256, 3231, 0, NULL, NULL, NULL, NULL, 52106), -- Voice From Beyond (Area: Aberrus, the Shadowed Crucible - Difficulty: Looking For Raid) CreateObject2 (Auras: 407036 - Hidden in Void)
-- (@CGUID+1234, 203812, 2569, 14663, 14663, '14,15,16,17', '0', 0, 0, 0, 2448.44970703125, 2454.713623046875, 582.4029541015625, 1.121640324592590332, 604800, 0, 0, 11566256, 3231, 0, NULL, NULL, NULL, NULL, 52106), -- Voice From Beyond (Area: Aberrus, the Shadowed Crucible - Difficulty: Looking For Raid) CreateObject2 (Auras: 407036 - Hidden in Void)
-- (@CGUID+1235, 203812, 2569, 14663, 14663, '14,15,16,17', '0', 0, 0, 0, 2504.48095703125, 2476.475830078125, 582.403076171875, 2.979046106338500976, 604800, 0, 0, 11566256, 3231, 0, NULL, NULL, NULL, NULL, 52106), -- Voice From Beyond (Area: Aberrus, the Shadowed Crucible - Difficulty: Looking For Raid) CreateObject2 (Auras: 407036 - Hidden in Void)

DELETE FROM `creature_addon` WHERE `guid` BETWEEN @CGUID+0 AND @CGUID+17;
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `StandState`, `AnimTier`, `VisFlags`, `SheathState`, `PvpFlags`, `emote`, `aiAnimKit`, `movementAnimKit`, `meleeAnimKit`, `visibilityDistanceType`, `auras`) VALUES
(@CGUID+0, 0, 0, 0, 0, 0, 1, 0, 425, 0, 0, 0, 4, ''), -- Sundered Siegemaster
(@CGUID+1, 0, 0, 0, 0, 0, 1, 0, 425, 0, 0, 0, 4, ''), -- Sundered Siegemaster
(@CGUID+2, 0, 0, 0, 0, 0, 1, 0, 425, 0, 0, 0, 4, ''), -- Sundered Devastator
(@CGUID+3, 0, 0, 0, 0, 0, 1, 0, 425, 0, 0, 0, 4, ''), -- Sundered Scaleguard
(@CGUID+4, 0, 0, 0, 0, 0, 1, 0, 425, 0, 0, 0, 4, ''), -- Sundered Preserver
(@CGUID+5, 0, 0, 0, 0, 0, 1, 0, 425, 0, 0, 0, 4, ''), -- Sundered Edgelord
(@CGUID+6, 0, 0, 0, 0, 0, 1, 0, 425, 0, 0, 0, 4, ''), -- Sundered Manaweaver
(@CGUID+7, 0, 0, 0, 0, 0, 1, 0, 425, 0, 0, 0, 4, ''), -- Sundered Edgelord
(@CGUID+8, 0, 0, 0, 0, 0, 1, 0, 425, 0, 0, 0, 4, ''), -- Sundered Siegemaster
(@CGUID+9, 0, 0, 0, 0, 0, 1, 0, 425, 0, 0, 0, 4, ''), -- Sundered Siegemaster
(@CGUID+10, 0, 0, 0, 0, 0, 1, 0, 425, 0, 0, 0, 4, ''), -- Sundered Siegemaster
(@CGUID+11, 0, 0, 0, 0, 0, 1, 0, 425, 0, 0, 0, 4, ''), -- Sundered Siegemaster
(@CGUID+12, 0, 0, 0, 0, 0, 1, 0, 425, 0, 0, 0, 4, ''), -- Sundered Edgelord
(@CGUID+15, 0, 0, 0, 0, 0, 1, 0, 425, 0, 0, 0, 4, ''), -- Malgosa Spellbinder 
(@CGUID+16, 0, 0, 0, 0, 0, 1, 0, 425, 0, 0, 0, 4, ''), -- Krono Sandtongue
-- (@CGUID+13, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, '410836'), -- Sabellian - 410836 - Earthen Grasp
-- (@CGUID+14, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, '410836'), -- Wrathion - 410836 - Earthen Grasp
(@CGUID+17, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, '401003'); -- Neltharion - 401003 - Twisted Secret

DELETE FROM `creature_template_addon` WHERE `entry` IN (203812, 205622, 205620);
INSERT INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `StandState`, `AnimTier`, `VisFlags`, `SheathState`, `PvpFlags`, `emote`, `aiAnimKit`, `movementAnimKit`, `meleeAnimKit`, `visibilityDistanceType`, `auras`) VALUES
(203812, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 3, '407036'), -- 203812 (Voice From Beyond) - Hidden in Void
(205622, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, '42459 411710'), -- 205622 (Krono Sandtongue) - Dual Wield, Primal Senses
(205620, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, '411710'); -- 205620 (Malgosa Spellbinder) - Primal Senses

DELETE FROM `creature_template_difficulty` WHERE (`DifficultyID`=17 AND `Entry` IN (201668, 202610, 202814, 203459, 203809, 203812, 204162, 205340, 205620, 205622, 205638, 205662, 205880));
INSERT INTO `creature_template_difficulty` (`Entry`, `DifficultyID`, `LevelScalingDeltaMin`, `LevelScalingDeltaMax`, `ContentTuningID`, `HealthScalingExpansion`, `HealthModifier`, `ManaModifier`, `CreatureDifficultyID`, `TypeFlags`, `TypeFlags2`, `VerifiedBuild`) VALUES
(201668, 17, 3, 3, 2648, 9, 432, 1, 234229, 2097228, 128, 52106), -- Neltharion
(202610, 17, 0, 0, 2648, 9, 100, 1, 231226, 2097260, 65536, 52106), -- Winglord Dezran
(202814, 17, 0, 0, 2648, 9, 1.35000002384185791, 1, 234233, 2097160, 144, 52106), -- Twisted Aberration
(203459, 17, 0, 0, 81, 9, 1, 1, 232556, 0, 0, 52106), -- Aberrus
(203809, 17, 1, 1, 2648, 9, 12, 1, 232913, 2097224, 0, 52106), -- Entropic Hatred
(203812, 17, 1, 1, 2648, 9, 26, 1, 234237, 2097224, 128, 52106), -- Voice From Beyond
(204162, 17, 0, 0, 2648, 9, 2.5, 1, 233262, 4096, 65664, 52106), -- Sundered Body
(205340, 17, 0, 0, 2648, 9, 1, 1, 234516, 1610612752, 117440646, 52106), -- Expired Experiment
(205620, 17, 2, 2, 2648, 9, 42, 1, 234791, 2097224, 0, 52106), -- Malgosa Spellbinder
(205622, 17, 2, 2, 2648, 9, 42, 1, 234793, 2097224, 0, 52106), -- Krono Sandtongue
(205638, 17, 0, 0, 2648, 9, 3, 1, 234809, 16778240, 16, 52106), -- Sundered Flame Banner
(205662, 17, 0, 0, 2648, 9, 1, 1, 234836, 1610612752, 6, 52106), -- Focus Crystal
(205880, 17, 0, 0, 2648, 9, 1, 1, 235080, 0, 128, 52106); -- [DNT] Sword Vehicle

UPDATE `creature_template_difficulty` SET `LevelScalingDeltaMin`=1, `LevelScalingDeltaMax`=1, `ContentTuningID`=2648, `HealthScalingExpansion`=9, `VerifiedBuild`=52106 WHERE (`Entry`=198869 AND `DifficultyID`=17); -- Sundered Devastator
UPDATE `creature_template_difficulty` SET `LevelScalingDeltaMin`=1, `LevelScalingDeltaMax`=1, `ContentTuningID`=2648, `HealthScalingExpansion`=9, `VerifiedBuild`=52106 WHERE (`Entry`=198870 AND `DifficultyID`=17); -- Sundered Preserver
UPDATE `creature_template_difficulty` SET `LevelScalingDeltaMin`=1, `LevelScalingDeltaMax`=1, `ContentTuningID`=2648, `HealthScalingExpansion`=9, `VerifiedBuild`=52106 WHERE (`Entry`=198871 AND `DifficultyID`=17); -- Sundered Scaleguard
UPDATE `creature_template_difficulty` SET `LevelScalingDeltaMin`=1, `LevelScalingDeltaMax`=1, `ContentTuningID`=2648, `HealthScalingExpansion`=9, `VerifiedBuild`=52106 WHERE (`Entry`=198872 AND `DifficultyID`=17); -- Sundered Manaweaver
UPDATE `creature_template_difficulty` SET `LevelScalingDeltaMin`=1, `LevelScalingDeltaMax`=1, `ContentTuningID`=2648, `HealthScalingExpansion`=9, `VerifiedBuild`=52106 WHERE (`Entry`=198873 AND `DifficultyID`=17); -- Sundered Edgelord
UPDATE `creature_template_difficulty` SET `LevelScalingDeltaMin`=1, `LevelScalingDeltaMax`=1, `ContentTuningID`=2648, `HealthScalingExpansion`=9, `VerifiedBuild`=52106 WHERE (`Entry`=198874 AND `DifficultyID`=17); -- Sundered Siegemaster

DELETE FROM `creature_model_info` WHERE `DisplayID` IN (112943, 112335, 112631, 111965, 111490, 99448, 112807, 112360, 112696, 112698, 112699, 112803, 113211, 107355, 110832, 111002, 110835, 109269, 115766, 113871, 115816, 114214, 114216);
INSERT INTO `creature_model_info` (`DisplayID`, `BoundingRadius`, `CombatReach`, `DisplayID_Other_Gender`, `VerifiedBuild`) VALUES
(112943, 0.5, 1, 0, 52106),
(112335, 1.288985729217529296, 2, 0, 52106),
(112631, 1.156360983848571777, 0, 0, 52106),
(111965, 0.300000011920928955, 1.026000022888183593, 0, 52106),
(111490, 2.106381893157958984, 3.60000014305114746, 0, 52106),
(99448, 0.375, 0.75, 0, 52106),
(112807, 1.141746759414672851, 1, 0, 52106),
(112360, 3.652988195419311523, 6, 0, 52106),
(112696, 0.241117060184478759, 0, 0, 52106),
(112698, 0.283680140972137451, 0, 0, 52106),
(112699, 0.422498524188995361, 0, 0, 52106),
(112803, 0.488889604806900024, 11.20000076293945312, 0, 52106),
(113211, 0.305999994277954101, 1.5, 0, 52106),
(107355, 0.347000002861022949, 2, 0, 52106),
(110832, 0.305999994277954101, 1.5, 0, 52106),
(111002, 1, 1.5, 0, 52106),
(110835, 0.347222000360488891, 1.5, 0, 52106),
(109269, 0.347000002861022949, 1.5, 0, 52106),
(115766, 1.755318164825439453, 3, 0, 52106),
(113871, 0.236000001430511474, 1.5, 0, 52106),
(115816, 0.347222000360488891, 1.5, 0, 52106),
(114214, 0.542177200317382812, 1, 0, 52106),
(114216, 1.755318164825439453, 2.999999761581420898, 0, 52106);

DELETE FROM `creature_template_gossip` WHERE (`CreatureID`=201574 AND `MenuID`=31614);
INSERT INTO `creature_template_gossip` (`CreatureID`, `MenuID`, `VerifiedBuild`) VALUES
(201574, 31614, 52106); -- Wrathion

DELETE FROM `npc_text` WHERE `ID`=@NPCTEXTID+0;
INSERT INTO `npc_text` (`ID`, `Probability0`, `Probability1`, `Probability2`, `Probability3`, `Probability4`, `Probability5`, `Probability6`, `Probability7`, `BroadcastTextId0`, `BroadcastTextId1`, `BroadcastTextId2`, `BroadcastTextId3`, `BroadcastTextId4`, `BroadcastTextId5`, `BroadcastTextId6`, `BroadcastTextId7`, `VerifiedBuild`) VALUES
(@NPCTEXTID+0, 1, 0, 0, 0, 0, 0, 0, 0, 242351, 0, 0, 0, 0, 0, 0, 0, 52106); -- 201574 (Wrathion)

DELETE FROM `gossip_menu` WHERE (`MenuID`=31614 AND `TextID`=@NPCTEXTID+0);
INSERT INTO `gossip_menu` (`MenuID`, `TextID`, `VerifiedBuild`) VALUES
(31614, @NPCTEXTID+0, 52106); -- 201574 (Wrathion)

DELETE FROM `creature_questitem` WHERE (`CreatureEntry`=201668 AND `DifficultyID`=17 AND `Idx`=2) OR (`CreatureEntry`=201668 AND `DifficultyID`=17 AND `Idx`=1) OR (`CreatureEntry`=201668 AND `DifficultyID`=17 AND `Idx`=0);
INSERT INTO `creature_questitem` (`CreatureEntry`, `DifficultyID`, `Idx`, `ItemId`, `VerifiedBuild`) VALUES
(201668, 17, 2, 205997, 52106), -- Neltharion
(201668, 17, 1, 205996, 52106), -- Neltharion
(201668, 17, 0, 205994, 52106); -- Neltharion

DELETE FROM `gameobject` WHERE `guid` BETWEEN @OGUID+0 AND @OGUID+1;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnDifficulties`, `PhaseId`, `PhaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `VerifiedBuild`) VALUES
(@OGUID+0, 390080, 2569, 14663, 14663, '14,15,16,17', '0', 0, 2389.558349609375, 2481.978759765625, 582.4398193359375, 3.141592741012573242, 0, 0, -1, 0, 604800, 255, 1, 52106), -- Door (Area: Aberrus, the Shadowed Crucible - Difficulty: 0) CreateObject1
(@OGUID+1, 397995, 2569, 14663, 14663, '14,15,16,17', '0', 0, 2527.007080078125, 2482.306640625, 582.0560302734375, 3.141592741012573242, 0, 0, -1, 0, 604800, 255, 0, 52106); -- Door (Area: Aberrus, the Shadowed Crucible - Difficulty: 0) CreateObject1

DELETE FROM `gameobject_addon` WHERE `guid` BETWEEN @OGUID+0 AND @OGUID+1;
INSERT INTO `gameobject_addon` (`guid`, `parent_rotation0`, `parent_rotation1`, `parent_rotation2`, `parent_rotation3`, `WorldEffectID`, `AIAnimKitID`) VALUES
(@OGUID+0, 0, 0, 1, -0.00000004371138828, 0, 0), -- Door
(@OGUID+1, 0, 0, 1, -0.00000004371138828, 0, 0); -- Door

-- ------------------------------------------------------------------------
-- Script related

UPDATE `creature_template` SET `faction`=14, `BaseAttackTime`=2000, `unit_flags2`=2099200 WHERE `entry`=203812; -- Voice From Beyond
UPDATE `creature_template` SET `unit_flags`=33554432 WHERE `entry`=202610; -- Winglord Dezran
UPDATE `creature_template` SET `faction`=16, `speed_walk`=1.60000002384185791, `speed_run`=2.571428537368774414, `BaseAttackTime`=2000, `unit_flags2`=2097152 WHERE `entry`=201668; -- Neltharion
UPDATE `creature_template` SET `faction`=16, `speed_run`=1, `BaseAttackTime`=2000, `unit_flags2`=32768 WHERE `entry`=205638; -- Sundered Flame Banner
UPDATE `creature_template` SET `faction`=16, `BaseAttackTime`=2000, `unit_flags`=32768, `unit_flags2`=4196352 WHERE `entry`=198870; -- Sundered Preserver
UPDATE `creature_template` SET `faction`=16, `speed_walk`=1.60000002384185791, `speed_run`=1.857142806053161621, `BaseAttackTime`=2000, `unit_flags`=32832, `unit_flags2`=2048 WHERE `entry`=205622; -- Krono Sandtongue
UPDATE `creature_template` SET `faction`=16, `speed_walk`=1.60000002384185791, `speed_run`=1.857142806053161621, `BaseAttackTime`=2000, `unit_flags`=32832, `unit_flags2`=2048 WHERE `entry`=205620; -- Malgosa Spellbinder
UPDATE `creature_template` SET `faction`=35, `speed_walk`=1.20000004768371582, `BaseAttackTime`=2000, `unit_flags`=256 WHERE `entry`=203133; -- Neltharion
UPDATE `creature_template` SET `VehicleId`=8178 WHERE `entry`=198874; -- Sundered Siegemaster

DELETE FROM `creature_text` WHERE `CreatureID`=201668;
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `BroadcastTextId`, `TextRange`, `comment`) VALUES
(201668, 0, 0, 'Your usefulness has ended.', 14, 0, 100, 0, 0, 223622, 238933, 0, 'Neltharion'),
(201668, 1, 1, 'Buried in rubble.', 14, 0, 100, 0, 0, 223615, 238946, 0, 'Neltharion'),
(201668, 2, 2, 'Impudent mortals! If death is what you seek I shall grant it.', 14, 0, 100, 0, 0, 223642, 238958, 0, 'Neltharion to Neltharion'),
(201668, 3, 3, 'Your remains will scatter across the endless dark.', 14, 0, 100, 0, 0, 223612, 238963, 0, 'Neltharion to Player'),
(201668, 4, 4, 'Fall in shadow.', 14, 0, 100, 0, 0, 223603, 238951, 0, 'Neltharion to Neltharion'),
(201668, 5, 5, 'Devouring shade.', 14, 0, 100, 0, 0, 223604, 238952, 0, 'Neltharion to Neltharion'),
(201668, 6, 6, 'Your pathetic resistance is no longer amusing. Embrace oblivion!', 14, 0, 100, 0, 0, 223643, 238959, 0, 'Neltharion to Neltharion'),
(201668, 7, 7, 'Come and feast, my pets!', 14, 0, 100, 0, 0, 223620, 238954, 0, 'Neltharion'),
(201668, 8, 8, 'My will is destruction!', 14, 0, 100, 0, 0, 223610, 238961, 0, 'Neltharion'),
(201668, 9, 9, '|TInterface\\ICONS\\INV_Misc_Head_Dragon_Black_Nightmare.BLP:20|t Neltharion begins casting |cFFFF0000|Hspell:407917|h[Ebon Destruction]|h|r', 42, 0, 100, 0, 0, 223610, 0, 0, 'Neltharion to Player'),
(201668, 10, 10, 'The hungering abyss opens!', 14, 0, 100, 0, 0, 223621, 238955, 0, 'Neltharion'),
(201668, 11, 11, 'The whispers... never... fade...', 14, 0, 100, 0, 0, 223621, 238936, 0, 'Neltharion to Player');

UPDATE `creature_template` SET `AIName`='', `ScriptName`='boss_echo_of_neltharion' WHERE `entry`=201668;

 -- Creature 201574 smart ai
SET @ENTRY := 201574;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryOrGuid` = @ENTRY;
UPDATE `creature_template` SET `AIName` = 'SmartAI', `ScriptName` = '' WHERE `entry` = @ENTRY;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `Difficulties`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(@ENTRY, 0, 0, 0, '', 58, 0, 100, 0, 0, 0, 0, 0, 101, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'On any waypoint of any path ended - Self: Set home position to current position');

 -- Creature 201575 smart ai
SET @ENTRY := 201575;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryOrGuid` = @ENTRY;
UPDATE `creature_template` SET `AIName` = 'SmartAI', `ScriptName` = '' WHERE `entry` = @ENTRY;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `Difficulties`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(@ENTRY, 0, 0, 1, '', 58, 0, 100, 0, 0, 0, 0, 0, 101, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'On any waypoint of any path ended - Self: Set home position to current position'),
(@ENTRY, 0, 1, 0, '', 61, 0, 100, 0, 0, 0, 0, 0, 11, 408211, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'On any waypoint of any path ended - Self: Cast spell 408211 on Self');
