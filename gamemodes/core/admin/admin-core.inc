#include <YSI_Coding\y_hooks>
#include <YSI_Data\y_bit>
#include <strlib>

#define MAX_ADMIN_LEVELS (5)

enum
{
    E_ADM_LVL_1 = 1,
    E_ADM_LVL_2,
    E_ADM_LVL_3,
    E_ADM_LVL_4,
    E_ADM_LVL_5
}

new
		  adminLevel[MAX_PLAYERS],
		  adminCommands[5][2048],
BitArray: adminOnDuty<MAX_PLAYERS>
;

#define T_ADM_ACC		"admins"
#define F_ADM_ACCID		"userID"
#define F_ADM_LEVEL		"adminLevel"



hook OnGameModeInit() {
    static const admin_createQuery[] = "\
        CREATE TABLE IF NOT EXISTS "T_ADM_ACC" ( \
    	"F_ADM_ACCID" INT(11) NOT NULL, \
    	"F_ADM_LEVEL" TINYINT(2) NOT NULL DEFAULT 0, \
    	PRIMARY KEY ("F_ADM_ACCID"), \
    	FOREIGN KEY ("F_ADM_ACCID") REFERENCES "TABLE_ACCOUNT"("FIELD_ACCOUNTID") ON DELETE CASCADE ON UPDATE CASCADE)\
    ";
    mysql_tquery(GetMySQLHandle(), admin_createQuery);
    return 1;
}

hook OnPlayerLogin(playerid)
{
    inline OnPlayerAdminLoad()
	{
		if(cache_num_rows())
		{

			new
				adm_level;

			cache_get_value_name_int(0, F_ADM_LEVEL, adm_level);

			SetPlayerAdminLevel(playerid, adm_level);
		}
	}
	MySQL_TQueryInline(GetMySQLHandle(), using inline OnPlayerAdminLoad, "SELECT "F_ADM_LEVEL" FROM "T_ADM_ACC" WHERE "F_ADM_ACCID" = %i", GetPlayerAccountID(playerid));
}

/*

    Admin Messaging
        - SendAdminMessage
        - SendAdminMessageF(fmat[], {Float,_}:...);
*/

SendAdminMessage(const fmat[])
{
    foreach(new i : Player)
    {
        if (i != INVALID_PLAYER_ID && GetPlayerAdminLevel(i) > 0)
        {
            SendMsgF(i, COLOR_LIGHTGREEN, "** ADMIN ** "C_WHITE"- %s", fmat);
        }
    }
    return 1;
}

SendAdminMessageF(const fmat[], {Float,_}:...)
{
    new
        out[144];

    foreach(new i : Player)
    {
        if (i != INVALID_PLAYER_ID && GetPlayerAdminLevel(i) > 0)
        {
            formatex(out, sizeof(out), fmat, ___(1));
            SendAdminMessage(out);
        }
    }
    return 1;
}

AddAdminCommandToList(level, const string[])
{
    if (!(E_ADM_LVL_1 <= level <= E_ADM_LVL_5))
    {
        printf("ERROR: Cannot register admin command for level %d", level);
        return 0;
    }

    strcat(adminCommands[level - 1], string);
    strcat(adminCommands[level - 1], "\n");
    return 1;
}

SetPlayerAdminLevel(playerid, level)
{
    adminLevel[playerid] = level;

    mysql_tquery(
        GetMySQLHandle(),
        sprintf(
            "INSERT INTO "T_ADM_ACC" ("F_ADM_ACCID", "F_ADM_LEVEL") VALUES (%i, %i) \
            ON DUPLICATE KEY UPDATE "F_ADM_LEVEL" = %i", GetPlayerAccountID(playerid), level, level
        )
    );
}

GetAdminRankName(playerid)
{
    new admin_RankNames[6][24] =
    {
        "None",          // 0
        "Junior Admin",  // 1
        "General Admin", // 2
        "Senior Admin",  // 3
        "Head Admin",    // 4
        "Developer"      // 5
    };

    return admin_RankNames[GetPlayerAdminLevel(playerid)];
}

GetPlayerAdminLevel(playerid)
    return adminLevel[playerid];


SetPlayerOnDuty(playerid, bool:toggle)
{
    // TODO: Change colour when player is on duty.

    Bit_Set(adminOnDuty, playerid, toggle);
    return 1;
}

IsAdminOnDuty(playerid)
    return Bit_Get(adminOnDuty, playerid);