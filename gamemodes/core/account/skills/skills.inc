#include <YSI_Coding\y_hooks>
#include <strlib>

#define TABLE_SKILL_TYPE 		"skills_type"
#define FIELD_SKILL_ID 			"skill_id"
#define FIELD_SKILL_NAME 		"skill_name"

#define TABLE_PLAYER_SKILLS     "skills_player"
#define FIELD_FK_ACCOUNT_ID     "fk_account_id"
#define FIELD_FK_SKILLID        "fk_skill_id"
#define FIELD_SKILL_VALUE       "value"

enum _: E_MYSQL_SKILL_ID {
	TOTAL_RAPE_SKILL_ID = 0,
	TOTAL_BRAPE_SKILL_ID,
	TOTAL_ROB_SKILL_ID,
	TOTAL_BROB_SKILL_ID,
	TOTAL_STOREROB_SKILL_ID,
	TOTAL_CUFFESCAPE_SKILL_ID,
	TOTAL_ARREST_SKILL_ID,
	TOTAL_BARREST_SKILL_ID
}


enum E_PLAYER_SKILL_DATA {
    TTL_RAPE,
    TTL_BEENRAPED,
    TTL_ROB,
    TTL_BEENROBBED,
    TTL_STOREROBBED,
    TTL_CUFFESCAPE,
    TTL_ARREST,
    TTL_BEENARRESTED
}

static playerSkills[MAX_PLAYERS][E_PLAYER_SKILL_DATA];

hook OnGameModeInit() {
	mysql_tquery(
        GetMySQLHandle(),
        "CREATE TABLE IF NOT EXISTS "TABLE_SKILL_TYPE"("FIELD_SKILL_ID" SMALLINT(6) NOT NULL, "FIELD_SKILL_NAME" VARCHAR(24) NOT NULL, PRIMARY KEY("FIELD_SKILL_ID"))"
    );

	inline checkSkillsRow() {
		new rows;
		cache_get_value_index_int(0, 0, rows);

		if(!rows) {
			mysql_tquery(
                GetMySQLHandle(),
                "INSERT INTO "TABLE_SKILL_TYPE" ("FIELD_SKILL_ID", "FIELD_SKILL_NAME") VALUES \
                (0, 'TotalRape'), \
                (1, 'TotalBeenRaped'), \
                (2, 'TotalRob'), \
                (3, 'TotalBeenRobbed'), \
                (4, 'TotalStoreRobbed'), \
                (5, 'TotalCuffEscape'), \
                (6, 'TotalArrest'), \
                (7, 'TotalBeenArrested')"
            );
		}
	}
	MySQL_TQueryInline(
        GetMySQLHandle(),
        using inline checkSkillsRow,
        "SELECT COUNT("FIELD_SKILL_ID") FROM "TABLE_SKILL_TYPE""
    );

	mysql_tquery(
        GetMySQLHandle(),
        "CREATE TABLE IF NOT EXISTS "TABLE_PLAYER_SKILLS"(\
		"FIELD_FK_ACCOUNT_ID" INT(11) NOT NULL, \
		"FIELD_FK_SKILLID" SMALLINT(6) NOT NULL, \
		"FIELD_SKILL_VALUE" INT(11) NOT NULL, \
		PRIMARY KEY("FIELD_FK_ACCOUNT_ID", "FIELD_FK_SKILLID"), \
		FOREIGN KEY("FIELD_FK_ACCOUNT_ID") REFERENCES "TABLE_ACCOUNT"("FIELD_ACCOUNTID") ON DELETE CASCADE ON UPDATE CASCADE, \
        FOREIGN KEY("FIELD_FK_SKILLID") REFERENCES "TABLE_SKILL_TYPE"("FIELD_SKILL_ID") ON DELETE CASCADE ON UPDATE CASCADE)"
    );
    return 1;
}

hook OnPlayerDisconnect(playerid, reason)
{
    new
        empty_data[E_PLAYER_SKILL_DATA];
    playerSkills[playerid] = empty_data;
    return 1;
}

hook OnPlayerLogin(playerid) {
	inline OnPlayerSkillsLoad() {
		new
			rows = cache_num_rows(),
			skill_id,
			skill_val;

		if(rows) {
			for(new i = 0; i < rows; i++) {
				cache_get_value_name_int(i, FIELD_SKILL_ID, skill_id);
				cache_get_value_name_int(i, FIELD_SKILL_VALUE, skill_val);

				switch(skill_id) {
					case TOTAL_RAPE_SKILL_ID: {
						playerSkills[playerid][TTL_RAPE] = skill_val;
                    }
					case TOTAL_BRAPE_SKILL_ID: {
						playerSkills[playerid][TTL_BEENRAPED] = skill_val;
                    }
					case TOTAL_ROB_SKILL_ID: {
						playerSkills[playerid][TTL_ROB] = skill_val;
                    }
					case TOTAL_BROB_SKILL_ID: {
						playerSkills[playerid][TTL_BEENROBBED] = skill_val;
                    }
					case TOTAL_STOREROB_SKILL_ID: {
						playerSkills[playerid][TTL_STOREROBBED] = skill_val;
                    }
					case TOTAL_CUFFESCAPE_SKILL_ID: {
						playerSkills[playerid][TTL_CUFFESCAPE] = skill_val;
                    }
					case TOTAL_ARREST_SKILL_ID: {
						playerSkills[playerid][TTL_ARREST] = skill_val;
                    }
					case TOTAL_BARREST_SKILL_ID: {
						playerSkills[playerid][TTL_BEENARRESTED] = skill_val;
                    }
				}
			}
		}
	}

	MySQL_TQueryInline(
        GetMySQLHandle(),
        using inline OnPlayerSkillsLoad,
        "SELECT p."FIELD_FK_SKILLID", p."FIELD_SKILL_VALUE", s."FIELD_SKILL_ID" \
        FROM "TABLE_PLAYER_SKILLS" as p INNER JOIN "TABLE_SKILL_TYPE" as s \
        ON p."FIELD_FK_SKILLID" = s."FIELD_SKILL_ID" \
        WHERE p."FIELD_FK_ACCOUNT_ID" = %i", GetPlayerAccountID(playerid)
    );
    return 1;
}


/*
    [USAGE]

    SetPlayerTotalRape(playerid, value)
    GivePlayerTotalRape(playerid, value)
    GetPlayerTotalRape(playerid)
*/

SetPlayerTotalRape(playerid, value, skill_id = TOTAL_RAPE_SKILL_ID)
{
    mysql_tquery(GetMySQLHandle(), sprintf("INSERT INTO "TABLE_PLAYER_SKILLS" ("FIELD_FK_ACCOUNT_ID", "FIELD_FK_SKILLID", "FIELD_SKILL_VALUE") \
       VALUES (%i, %i, %i) ON DUPLICATE KEY UPDATE "FIELD_SKILL_VALUE" = %i", GetPlayerAccountID(playerid), skill_id, value, value));
    return playerSkills[playerid][TTL_RAPE] = value;
}

GivePlayerTotalRape(playerid, value)
    return SetPlayerTotalRape(playerid, GetPlayerTotalRape(playerid) + value);

GetPlayerTotalRape(playerid)
    return playerSkills[playerid][TTL_RAPE];

/*
    [USAGE]

    SetPlayerTotalBeenRape(playerid, value)
    GivePlayerTotalBeenRape(playerid, value)
    GetPlayerTotalBeenRape(playerid)
*/

SetPlayerTotalBeenRape(playerid, value, skill_id = TOTAL_BRAPE_SKILL_ID)
{
    mysql_tquery(GetMySQLHandle(), sprintf("INSERT INTO "TABLE_PLAYER_SKILLS" ("FIELD_FK_ACCOUNT_ID", "FIELD_FK_SKILLID", "FIELD_SKILL_VALUE") \
       VALUES (%i, %i, %i) ON DUPLICATE KEY UPDATE "FIELD_SKILL_VALUE" = %i", GetPlayerAccountID(playerid), skill_id, value, value));
    return playerSkills[playerid][TTL_BEENRAPED] = value;
}

GivePlayerTotalBeenRape(playerid, value)
    return SetPlayerTotalBeenRape(playerid, GetPlayerTotalBeenRape(playerid) + value);

GetPlayerTotalBeenRape(playerid)
    return playerSkills[playerid][TTL_BEENRAPED];

/*
    [USAGE]

    SetPlayerTotalRob(playerid, value)
    GivePlayerTotalRob(playerid, value)
    GetPlayerTotalRob(playerid)
*/

SetPlayerTotalRob(playerid, value, skill_id = TOTAL_ROB_SKILL_ID)
{
    mysql_tquery(GetMySQLHandle(), sprintf("INSERT INTO "TABLE_PLAYER_SKILLS" ("FIELD_FK_ACCOUNT_ID", "FIELD_FK_SKILLID", "FIELD_SKILL_VALUE") \
       VALUES (%i, %i, %i) ON DUPLICATE KEY UPDATE "FIELD_SKILL_VALUE" = %i", GetPlayerAccountID(playerid), skill_id, value, value));
    return playerSkills[playerid][TTL_ROB] = value;
}

GivePlayerTotalRob(playerid, value)
    return SetPlayerTotalRob(playerid, GetPlayerTotalRob(playerid) + value);
GetPlayerTotalRob(playerid)
    return playerSkills[playerid][TTL_ROB];

/*
    [USAGE]

    SetPlayerTotalBeenRob(playerid, value)
    GivePlayerTotalBeenRob(playerid, value)
    GetPlayerTotalBeenRob(playerid)
*/

SetPlayerTotalBeenRob(playerid, value, skill_id = TOTAL_BROB_SKILL_ID)
{
    mysql_tquery(GetMySQLHandle(), sprintf("INSERT INTO "TABLE_PLAYER_SKILLS" ("FIELD_FK_ACCOUNT_ID", "FIELD_FK_SKILLID", "FIELD_SKILL_VALUE") \
       VALUES (%i, %i, %i) ON DUPLICATE KEY UPDATE "FIELD_SKILL_VALUE" = %i", GetPlayerAccountID(playerid), skill_id, value, value));
    return playerSkills[playerid][TTL_BEENROBBED] = value;
}

GivePlayerTotalBeenRob(playerid, value)
    return SetPlayerTotalBeenRob(playerid, GetPlayerTotalRob(playerid) + value);

GetPlayerTotalBeenRob(playerid)
    return playerSkills[playerid][TTL_BEENROBBED];

/*
    [USAGE]

    SetPlayerTotalStoreRob(playerid, value)
    GivePlayerTotalStoreRob(playerid, value)
    GetPlayerTotalStoreRob(playerid)
*/

SetPlayerTotalStoreRob(playerid, value, skill_id = TOTAL_STOREROB_SKILL_ID)
{
    mysql_tquery(GetMySQLHandle(), sprintf("INSERT INTO "TABLE_PLAYER_SKILLS" ("FIELD_FK_ACCOUNT_ID", "FIELD_FK_SKILLID", "FIELD_SKILL_VALUE") \
       VALUES (%i, %i, %i) ON DUPLICATE KEY UPDATE "FIELD_SKILL_VALUE" = %i", GetPlayerAccountID(playerid), skill_id, value, value));
    return playerSkills[playerid][TTL_STOREROBBED] = value;
}

GivePlayerTotalStoreRob(playerid, value)
    return SetPlayerTotalStoreRob(playerid, GetPlayerTotalStoreRob(playerid) + value);

GetPlayerTotalStoreRob(playerid)
    return playerSkills[playerid][TTL_STOREROBBED];

/*
    [USAGE]

    SetPlayerTotalCuffEscape(playerid, value)
    GivePlayerTotalCuffEscape(playerid, value)
    GetPlayerTotalCuffEscape(playerid)
*/

SetPlayerTotalCuffEscape(playerid, value, skill_id = TOTAL_CUFFESCAPE_SKILL_ID)
{
    mysql_tquery(GetMySQLHandle(), sprintf("INSERT INTO "TABLE_PLAYER_SKILLS" ("FIELD_FK_ACCOUNT_ID", "FIELD_FK_SKILLID", "FIELD_SKILL_VALUE") \
       VALUES (%i, %i, %i) ON DUPLICATE KEY UPDATE "FIELD_SKILL_VALUE" = %i", GetPlayerAccountID(playerid), skill_id, value, value));
    return playerSkills[playerid][TTL_CUFFESCAPE] = value;
}

GivePlayerTotalCuffEscape(playerid, value)
    return SetPlayerTotalCuffEscape(playerid, GetPlayerTotalCuffEscape(playerid) + value);

GetPlayerTotalCuffEscape(playerid)
    return playerSkills[playerid][TTL_CUFFESCAPE];

/*
    [USAGE]

    SetPlayerTotalArrest(playerid, value)
    GivePlayerTotalArrest(playerid, value)
    GetPlayerTotalArrest(playerid)
*/


SetPlayerTotalArrest(playerid, value, skill_id = TOTAL_ARREST_SKILL_ID)
{
    mysql_tquery(GetMySQLHandle(), sprintf("INSERT INTO "TABLE_PLAYER_SKILLS" ("FIELD_FK_ACCOUNT_ID", "FIELD_FK_SKILLID", "FIELD_SKILL_VALUE") \
       VALUES (%i, %i, %i) ON DUPLICATE KEY UPDATE "FIELD_SKILL_VALUE" = %i", GetPlayerAccountID(playerid), skill_id, value, value));
    return playerSkills[playerid][TTL_ARREST] = value;
}

GivePlayerTotalArrest(playerid, value)
    return SetPlayerTotalArrest(playerid, GetPlayerTotalArrest(playerid) + value);

GetPlayerTotalArrest(playerid)
    return playerSkills[playerid][TTL_ARREST];


/*
    [USAGE]

    SetPlayerTotalBeenArrest(playerid, value)
    GivePlayerTotalBeenArrest(playerid, value)
    GetPlayerTotalBeenArrest(playerid)
*/

SetPlayerTotalBeenArrest(playerid, value, skill_id = TOTAL_BARREST_SKILL_ID)
{
    mysql_tquery(GetMySQLHandle(), sprintf("INSERT INTO "TABLE_PLAYER_SKILLS" ("FIELD_FK_ACCOUNT_ID", "FIELD_FK_SKILLID", "FIELD_SKILL_VALUE") \
       VALUES (%i, %i, %i) ON DUPLICATE KEY UPDATE "FIELD_SKILL_VALUE" = %i", GetPlayerAccountID(playerid), skill_id, value, value));
    return playerSkills[playerid][TTL_BEENARRESTED] = value;
}

GivePlayerTotalBeenArrest(playerid, value)
    return SetPlayerTotalBeenArrest(playerid, GetPlayerTotalBeenArrest(playerid) + value);

GetPlayerTotalBeenArrest(playerid)
    return playerSkills[playerid][TTL_BEENARRESTED];