#include <YSI_Coding\y_hooks>

#define TABLE_VIP               "vip_players"       //Tabe
#define FIELD_VIP_OWNERID       "fk_account_id"     //00
#define FIELD_VIP_LEVEL         "vip_level"         //01
#define FIELD_VIP_EXPIRE        "vip_expireDate"    //02

static
    playerVIPLevel[MAX_PLAYERS];

hook OnGameModeInit()
{
    mysql_tquery(
        GetMySQLHandle(),
        "CREATE TABLE IF NOT EXISTS "TABLE_VIP"(\
        "FIELD_VIP_OWNERID" INT(11) AUTO_INCREMENT, \
        "FIELD_VIP_LEVEL" TINYINT(2) NOT NULL DEFAULT 0, \
        "FIELD_VIP_EXPIRE" datetime NOT NULL DEFAULT '0000-00-00 00:00:00', \
        PRIMARY KEY ("FIELD_VIP_OWNERID"), \
        FOREIGN KEY ("FIELD_VIP_OWNERID") REFERENCES "TABLE_ACCOUNT"("FIELD_ACCOUNTID") ON DELETE CASCADE ON UPDATE RESTRICT)"
    );
    return 1;
}

hook OnPlayerLogin(playerid) {
    inline OnPlayerVIPLoad() {
        if(cache_num_rows()) {
            new
            expire,
            expiry_date[20];

            cache_get_value_name_int(0, FIELD_VIP_LEVEL, playerVIPLevel[playerid]);
            cache_get_value_name_int(0, "expire", expire);
            cache_get_value_name(0, "vip_expire_data", expiry_date, 20);

            if (expire > 0) {
                RemovePlayerVIP(playerid);
                SendServerMsgF(playerid, "Your VIP expired on "C_GREY"%s", expiry_date);
                playerVIPLevel[playerid] = 0;
            }
        }
    }

	MySQL_TQueryInline(
        GetMySQLHandle(), using inline OnPlayerVIPLoad,
        "SELECT *, DATE_FORMAT("FIELD_VIP_EXPIRE", '"SQL_DATETIME_FORMAT"') as vip_expire_data, TIMESTAMPDIFF(SECOND, CURRENT_TIMESTAMP(), "FIELD_VIP_EXPIRE") \
        FROM "TABLE_VIP" WHERE "FIELD_VIP_OWNERID" = %i LIMIT 1", GetPlayerAccountID(playerid)
    );
    return 1;
}

GivePlayerVIP(playerid, level, interval_type, duration) {
    new
        interval[6]
    ;
	interval = interval_type == 0 ? ("MONTH") : ("YEAR");

    mysql_tquery(
        GetMySQLHandle(),
        sprintf("INSERT INTO "TABLE_VIP" ("FIELD_VIP_OWNERID", "FIELD_VIP_LEVEL", "FIELD_VIP_EXPIRE") \
	    VALUES(%i, %i, DATE_ADD(CURRENT_TIMESTAMP(), INTERVAL %i %s))", GetPlayerAccountID(playerid), level, duration, interval)
    );

    playerVIPLevel[playerid] = level;
}

RemovePlayerVIP(playerid) {
	new
		query[45 + MAX_PLAYER_NAME];

	mysql_format(GetMySQLHandle(), query, sizeof(query), "DELETE FROM "TABLE_VIP" WHERE "FIELD_VIP_OWNERID" = %i", GetPlayerAccountID(playerid));
	mysql_tquery(GetMySQLHandle(), query);
}

//
//
GetPlayerVIPLevel(playerid) {
    return playerVIPLevel[playerid];
}