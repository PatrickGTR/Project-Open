#include <YSI_Coding\y_hooks>

#define T_BAN       "bans"
#define F_IP        "ipadress"
#define F_GPCI      "gpci"
#define F_REASON    "reason"
#define F_ADMIN     "by_admin"
#define F_DATE      "timestamp_date"
#define F_UDATE     "timestamp_udate"

forward OnRemovePlayerBan(username[MAX_PLAYER_NAME], unbannerid);

hook OnGameModeInit()
{

    mysql_tquery(GetMySQLHandle(),
    	"CREATE TABLE IF NOT EXISTS "T_BAN" (\
    	"F_USERNAME" VARCHAR(24), \
    	"F_IP" VARCHAR(17), \
    	"F_GPCI" VARCHAR(60), \
    	"F_REASON" VARCHAR(32), \
        "F_ADMIN" VARCHAR(24), \
        "F_DATE" datetime NOT NULL DEFAULT CURRENT_TIMESTAMP, \
        "F_UDATE" datetime NOT NULL DEFAULT '0000-00-00 00:00:00')"
   );
}

hook OnPlayerLogin(playerid) {
    IsPlayerBanned(playerid);
}

AddPlayerBan(playerid, byid = INVALID_PLAYER_ID, reason[], interval_type, banduration) {
    new
        plyIP[16 + 1],
        plyName[MAX_PLAYER_NAME],
        name[MAX_PLAYER_NAME],
        query[512],
        plyGPCI[60]
    ;

    GetPlayerIp(playerid, plyIP, sizeof(plyIP));
    gpci(playerid, plyGPCI, sizeof(plyGPCI));
    GetPlayerName(playerid, plyName, sizeof(plyName));

    if (byid == INVALID_PLAYER_ID) {
        name = "SERVER";
    }
    else {
        strcpy(name, plyName, MAX_PLAYER_NAME);
    }

    new
        interval[6]
    ;
    switch(interval_type) {
        case 0: strcpy(interval, "DAY", sizeof(interval));
        case 1: strcpy(interval, "MONTH", sizeof(interval));
        case 2: strcpy(interval, "YEAR", sizeof(interval));
    }

    mysql_format(GetMySQLHandle(), query, sizeof(query), "\
        INSERT INTO "T_BAN" ("F_USERNAME","F_IP", "F_GPCI", "F_REASON", "F_ADMIN", "F_DATE", "F_UDATE" ) VALUES \
        ('%e', '%e', '%e', '%e', '%e',  CURRENT_TIMESTAMP(), DATE_ADD(CURRENT_TIMESTAMP(), INTERVAL %i %s))", plyName, plyIP, plyGPCI, reason, name, banduration, interval);
    mysql_tquery(GetMySQLHandle(), query, "", "");
    return Kick(playerid);
}

RemovePlayerBan(username[], playerid = INVALID_PLAYER_ID) {
    new
        query[45 + MAX_PLAYER_NAME];

    mysql_format(GetMySQLHandle(), query, sizeof(query), "SELECT "F_USERNAME" FROM "T_BAN" WHERE "F_USERNAME" = '%e'", username);
    mysql_tquery(GetMySQLHandle(), query, "OnRemovePlayerBan", "si", username, playerid);
}

public OnRemovePlayerBan(username[MAX_PLAYER_NAME], unbannerid) {
    new
        query[40 + MAX_PLAYER_NAME]
    ;
    if (!cache_num_rows()) {
        if (unbannerid != INVALID_PLAYER_ID) {
            SendServerMsgF(unbannerid, "The account "C_GREY"(%s) "C_WHITE"you are trying to unban was not found in the database.", username);
        }
    }
    else {
        mysql_format(GetMySQLHandle(), query, sizeof(query), "DELETE FROM "T_BAN" WHERE "F_USERNAME" = '%e'", username);
        mysql_tquery(GetMySQLHandle(), query, "", "");
        if (unbannerid != INVALID_PLAYER_ID) {
            SendServerMsgF(unbannerid, "You successfully unbanned "C_GREY"%s's "C_WHITE"account.", username);
        }
    }
}

IsPlayerBanned(playerid) {
    new
        retrieveGCPI[60],
        plyName[MAX_PLAYER_NAME],
        plyIP[16 + 1]
    ;
    gpci(playerid, retrieveGCPI, sizeof(retrieveGCPI));
    GetPlayerName(playerid, plyName, sizeof(plyName));
    GetPlayerIp(playerid, plyIP, sizeof(plyIP));

    inline OnPlayerBanLoad() {
        new
            banip[16 + 1],
            name[MAX_PLAYER_NAME],
            banby[MAX_PLAYER_NAME],
            banreason[MAX_BAN_REASON],
            bandate[40],
            unbandate[40],
            expired
        ;

        if (cache_num_rows()) {
            cache_get_value_name(0, F_USERNAME, name, sizeof(name));
            cache_get_value_name(0, F_IP,       banip, sizeof(banip));
            cache_get_value_name(0, F_ADMIN,    banby, sizeof(banby));
            cache_get_value_name(0, F_REASON,   banreason, sizeof(banreason));

            cache_get_value_name(0, "date_ban",     bandate, sizeof(bandate));
            cache_get_value_name(0, "date_unban",   unbandate, sizeof(unbandate));

            cache_get_value_name_int(0, "expired", expired);

            if (expired <= 0) {
                SendMsgF(playerid, COLOR_WHITE, "Your ban expired on "C_GREY"%s "C_WHITE"\nWelcome back %p(%i), don't break any rules again", unbandate, playerid, playerid);
                RemovePlayerBan(name, INVALID_PLAYER_ID);
            }
            else {
                new string[512];
                format(string, sizeof(string), "\
                    "C_WHITE"Username: "C_GREY"%s\n\
                    "C_WHITE"Banned IP: "C_GREY"%s\n\
                    "C_WHITE"Current IP: "C_GREY"%s\n\
                    "C_WHITE"Banned By: "C_GREY"%s\n\
                    "C_WHITE"Reason of Ban: "C_GREY"%s\n\
                    "C_WHITE"Date of Ban: "C_GREY"%s\n\
                    "C_WHITE"Date of Unban: "C_GREY"%s", name, banip, plyIP, banby, banreason, bandate, unbandate);
                Dialog_Show(playerid, DIALOG_STYLE_MSGBOX, "Ban Information", string, "Close", "");
                Kick(playerid);
            }
            return 1;
        }
    }
    MySQL_PQueryInline(
        GetMySQLHandle(),
        using inline OnPlayerBanLoad,
        "SELECT *, DATE_FORMAT("F_DATE",'"SQL_DATETIME_FORMAT"') AS date_ban, \
        DATE_FORMAT("F_UDATE",'"SQL_DATETIME_FORMAT"') AS date_unban, \
        TIMESTAMPDIFF(SECOND, CURRENT_TIMESTAMP(), "F_UDATE") AS expired \
        FROM "T_BAN" WHERE "F_USERNAME" = '%e' OR "F_GPCI" = '%e' OR "F_IP" = '%e' LIMIT 1", plyName, retrieveGCPI, plyIP
    );
    return 0;
}

