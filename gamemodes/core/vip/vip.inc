#include <YSI_Coding\y_hooks>

#define T_VIP               "vip_players"       //Tabe
#define F_VIP_OWNERID       "fk_account_id"     //00
#define F_VIP_LEVEL         "vip_level"         //01
#define F_VIP_EXPIRE        "vip_expireDate"    //02

static
    playerVIPLevel[MAX_PLAYERS];

hook OnGameModeInit()
{
    mysql_tquery(
        GetSQLHandle(), 
        "CREATE TABLE IF NOT EXISTS "T_VIP"(\
        "F_VIP_OWNERID" INT(11) AUTO_INCREMENT, \
        "F_VIP_LEVEL" TINYINT(2) NOT NULL DEFAULT 0, \
        "F_VIP_EXPIRE" datetime NOT NULL DEFAULT '0000-00-00 00:00:00', \
        PRIMARY KEY ("F_VIP_OWNERID"), \
        FOREIGN KEY ("F_VIP_OWNERID") REFERENCES "T_ACCOUNT"("F_ACCOUNTID") ON DELETE CASCADE ON UPDATE RESTRICT)"
    );
}

hook OnPlayerLogin(playerid) {
    inline OnPlayerVIPLoad() {
        if(cache_num_rows()) {
            new
                expire,
                expiry_date[20];

            cache_get_value_name_int(0, F_VIP_LEVEL, playerVIPLevel[playerid]);
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
        GetSQLHandle(), using inline OnPlayerVIPLoad, 
        "SELECT *, DATE_FORMAT("F_VIP_EXPIRE", '"SQL_DATETIME_FORMAT"') as vip_expire_data, TIMESTAMPDIFF(SECOND, CURRENT_TIMESTAMP(), "F_VIP_EXPIRE") \
        FROM "T_VIP" WHERE "F_VIP_OWNERID" = %i LIMIT 1", GetPlayerAccountID(playerid)
    );
}

GivePlayerVIP(playerid, level, interval_type, duration) {
    new 
        interval[6]
    ;
	interval = interval_type == 0 ? ("MONTH") : ("YEAR");

    mysql_tquery(
        GetSQLHandle(), 
        sprintf("INSERT INTO "T_VIP" ("F_VIP_OWNERID", "F_VIP_LEVEL", "F_VIP_EXPIRE") \
	    VALUES(%i, %i, DATE_ADD(CURRENT_TIMESTAMP(), INTERVAL %i %s))", GetPlayerAccountID(playerid), level, duration, interval)
    );

    playerVIPLevel[playerid] = level;
}

RemovePlayerVIP(playerid) {
	new 
		query[45 + MAX_PLAYER_NAME];

	mysql_format(GetSQLHandle(), query, sizeof(query), "DELETE FROM "T_VIP" WHERE "F_VIP_OWNERID" = %i", GetPlayerAccountID(playerid));
	mysql_tquery(GetSQLHandle(), query);
}

GetPlayerVIPLevel(playerid) {
    return playerVIPLevel[playerid];
}