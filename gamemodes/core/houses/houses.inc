
#define MAX_HOUSE_INTERIORS     (10)
#define MAX_H_DESCRIPTION       (32)
#define MAX_HOUSE_PER_PLAYER    (1)

#define INVALID_HOUSE_ID        (-1)
#define INVALID_OWNER_ID        (-1)
#define HOUSES_STREAMER_IDENTIFIER (20)

new
Iterator: houseIndex<MAX_HOUSES>,
          ply_InHouseID[MAX_PLAYERS] = {INVALID_HOUSE_ID, ...},
          ply_houseCheckpointID[MAX_PLAYERS];


#include <houses-impl>
#include <furnitures_main>
#include <houses_cmd>

#include <YSI_Coding\y_hooks>

/* Hooked callbacks */

hook OnGameModeInit()
{
    mysql_tquery(GetSQLHandle(),
        "CREATE TABLE IF NOT EXISTS "TABLE_HOUSEINT"(\
        "FIELD_HOUSE_LEVEL" TINYINT(4) NOT NULL,\
        "FIELD_HOUSE_INFO" VARCHAR(24) NOT NULL, \
        "FIELD_HOUSE_EXT_X" FLOAT NOT NULL, \
        "FIELD_HOUSE_EXT_Y" FLOAT NOT NULL, \
        "FIELD_HOUSE_EXT_Z" FLOAT NOT NULL, \
        "FIELD_HOUSE_EXT_A" FLOAT NOT NULL, \
        "FIELD_HOUSE_INTID" MEDIUMINT NOT NULL DEFAULT 0,\
        PRIMARY KEY("FIELD_HOUSE_LEVEL"))"
    );

    inline checkHouseIntRows()
    {
        new rows;
        cache_get_value_index_int(0, 0, rows);

        if(!rows)
        {
            mysql_tquery(GetSQLHandle(), "INSERT INTO "TABLE_HOUSEINT" (\
                "FIELD_HOUSE_LEVEL", "FIELD_HOUSE_INFO",  "FIELD_HOUSE_EXT_X", "FIELD_HOUSE_EXT_Y", "FIELD_HOUSE_EXT_Z", "FIELD_HOUSE_EXT_A", "FIELD_HOUSE_INTID") \
                VALUES \
                (1, 'Level 1', -42.6186, 1412.7570, 1084.4297, 179.4052, 8), \
                (2, 'Level 2', 260.7346, 1237.2487, 1084.2578, 0.1768, 9), \
                (3, 'Level 3', -68.7526, 1351.3705, 1080.2109, 358.68036, 6), \
                (4, 'Level 4', 22.9072, 1403.3308, 1084.4370, 359.0405, 5), \
                (5, 'Level 5', 226.3071, 1114.1798, 1080.9930, 271.0070, 5), \
                (6, 'Level 6', 24.0796, 1340.2196, 1084.3750, 358.4605, 10), \
                (7, 'Level 7', 2324.4849, -1149.3196, 1050.7101, 0.0039, 12), \
                (8, 'Level 8', 140.2810, 1366.1149, 1083.8594, 0.8971, 5), \
                (9, 'Level 9', 1260.8743, -785.4063, 1091.9063, 271.2455, 5), \
                (10, 'Level 10', 1727.0060, -1637.9930, 20.2230, 182.4255, 18)"
            );
        }
    }
    MySQL_TQueryInline(GetSQLHandle(), using inline checkHouseIntRows, "SELECT COUNT("FIELD_HOUSE_LEVEL") FROM "TABLE_HOUSEINT"");

    LoadHouseInteriors();

    mysql_tquery(GetSQLHandle(),
        "CREATE TABLE IF NOT EXISTS "TABLE_HOUSE"(\
        "FIELD_HOUSE_ID" SMALLINT NOT NULL DEFAULT 0, \
        "FIELD_HOUSE_OWNERID" INT(11) NULL DEFAULT NULL, \
        "FIELD_HOUSE_INTERIOR" TINYINT(4) NOT NULL, \
        "FIELD_HOUSE_DESCRIPTION" VARCHAR(32) DEFAULT 'Hello', \
        "FIELD_HOUSE_PRICE" MEDIUMINT NOT NULL, \
        "FIELD_HOUSE_CASH_STORED" MEDIUMINT NOT NULL, \
        "FIELD_HOUSE_ENT_X" FLOAT NOT NULL, \
        "FIELD_HOUSE_ENT_Y" FLOAT NOT NULL, \
        "FIELD_HOUSE_ENT_Z" FLOAT NOT NULL, \
        "FIELD_HOUSE_ENT_A" FLOAT NOT NULL, \
        "FIELD_HOUSE_VWID" MEDIUMINT NOT NULL, \
        PRIMARY KEY ("FIELD_HOUSE_ID"), \
        FOREIGN KEY("FIELD_HOUSE_INTERIOR") REFERENCES "TABLE_HOUSEINT"("FIELD_HOUSE_LEVEL") ON DELETE CASCADE ON UPDATE CASCADE, \
        FOREIGN KEY("FIELD_HOUSE_OWNERID") REFERENCES "T_ACCOUNT"("F_ACCOUNTID") ON DELETE SET NULL ON UPDATE CASCADE)"
    );
    LoadHouses();
}



hook OnPlayerEnterDynamicCP(playerid, checkpointid)
{
    new
        arr[2];

    Streamer_GetArrayData(STREAMER_TYPE_CP, checkpointid, E_STREAMER_EXTRA_ID, arr);

    if (arr[0] == HOUSES_STREAMER_IDENTIFIER)
    {
        new i = arr[1];
        if (checkpointid == GetHouseEntranceCP(i))
        {
            ply_InHouseID[playerid] = i;
            ply_houseCheckpointID[playerid] = checkpointid;
        }
    }
    return 1;
}

