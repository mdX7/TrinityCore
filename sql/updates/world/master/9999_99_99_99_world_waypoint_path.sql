--
DROP PROCEDURE IF EXISTS waypoint_data_refactor;

DELIMITER ;;
CREATE PROCEDURE waypoint_data_refactor() BEGIN
  IF EXISTS (SELECT * FROM `information_schema`.`columns` WHERE `table_schema`=SCHEMA() AND `table_name`='waypoint_data') THEN
    DROP TABLE IF EXISTS `waypoint_path`;
    CREATE TABLE `waypoint_path`  (
      `PathId` bigint UNSIGNED NOT NULL,
      `Type` tinyint UNSIGNED NOT NULL DEFAULT 0,
      `Flags` tinyint UNSIGNED NOT NULL DEFAULT 0,
      `FinalOrientation` float NULL DEFAULT NULL,
      `NextPathId` bigint UNSIGNED NOT NULL,
      `NextPathDelay` int UNSIGNED NOT NULL DEFAULT 0,
      `Comment` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NULL DEFAULT NULL,
      PRIMARY KEY (`PathId`) USING BTREE
    ) ENGINE = InnoDB CHARACTER SET = utf8mb4 COLLATE = utf8mb4_unicode_ci ROW_FORMAT = DYNAMIC;

    DROP TABLE IF EXISTS `waypoint_path_node`;
    CREATE TABLE `waypoint_path_node`  (
      `PathId` bigint UNSIGNED NOT NULL DEFAULT 0,
      `NodeId` int UNSIGNED NOT NULL DEFAULT 0 ,
      `PositionX` float NOT NULL DEFAULT 0,
      `PositionY` float NOT NULL DEFAULT 0,
      `PositionZ` float NOT NULL DEFAULT 0,
      PRIMARY KEY (`PathId`, `NodeId`) USING BTREE
    ) ENGINE = InnoDB CHARACTER SET = utf8mb4 COLLATE = utf8mb4_unicode_ci ROW_FORMAT = DYNAMIC;
    
    ALTER TABLE `creature_addon` CHANGE COLUMN `path_id` `PathId` bigint UNSIGNED NOT NULL DEFAULT 0 AFTER `guid`;
    ALTER TABLE `creature_template_addon` CHANGE COLUMN `path_id` `PathId` bigint UNSIGNED NOT NULL DEFAULT 0 AFTER `entry`;

    DELETE FROM `command` WHERE `name` IN('reload waypoint_data', 'reload waypoint_path');
    INSERT INTO `command` (`name`, `help`) VALUES 
    ('reload waypoint_path', 'Syntax: .reload waypoint_path will reload waypoint_path and waypoint_path_node tables.');
    
    -- insert migrated data
    
    DROP TABLE IF EXISTS `waypoint_data`;
  END IF;
END;;

DELIMITER ;
CALL waypoint_data_refactor();

DROP PROCEDURE IF EXISTS waypoint_data_refactor;
