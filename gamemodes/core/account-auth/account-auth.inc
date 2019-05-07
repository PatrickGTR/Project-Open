
#include <a_mysql>

#include <YSI_Visual\y_dialog>
#include <YSI_Coding\y_inline>
#include <YSI_Coding\y_hooks>

#include <formatex>
#include <strlib>

#define T_ACCOUNT       "accounts"
#define F_ACCOUNTID     "accountID"
#define F_USERNAME      "username"
#define F_PASSWORD      "password"
#define F_HASH          "hash"
#define F_MONEY         "money"
#define F_KILLS         "kills"
#define F_DEATHS        "deaths"
#define F_JOB           "jobID"
#define F_SCORE         "score"
#define F_EXP           "experience"
#define F_WANTEDLVL     "wantedlevel"
#define F_REG_DATE      "registerdate"
#define F_LAST_LOG      "lastlogin"

#define MAX_PASSWORD    (65)
#define MAX_HASH        (11)

#include <login>
#include <register>

static
    playerAccountID[MAX_PLAYERS],
    playerKills[MAX_PLAYERS],
    playerDeaths[MAX_PLAYERS],
    playerPassword[MAX_PLAYERS][MAX_PASSWORD],
    playerSalt[MAX_PLAYERS][MAX_HASH],
    playerRegisterDate[MAX_PLAYERS][40],
    playerLastLoginDate[MAX_PLAYERS][40];

forward OnPlayerLogin(playerid);

hook OnGameModeInit()
{

    mysql_tquery(GetSQLHandle(),
        "CREATE TABLE IF NOT EXISTS "T_ACCOUNT" \
        ("F_ACCOUNTID" INTEGER AUTO_INCREMENT, \
        "F_USERNAME" VARCHAR(24) NOT NULL, \
        "F_PASSWORD" VARCHAR(65) NOT NULL, \
        "F_HASH" VARCHAR(11) NOT NULL, \
        "F_MONEY" INT(11) NOT NULL, \
        "F_KILLS" SMALLINT(6) NOT NULL, \
        "F_DEATHS" SMALLINT(6) NOT NULL, \
        "F_JOB" TINYINT(2) NOT NULL, \
        "F_SCORE" INT(11) NOT NULL, \
        "F_EXP" INT(11) NOT NULL, \
        "F_WANTEDLVL" SMALLINT(6) NOT NULL, \
        "F_REG_DATE" DATETIME NOT NULL, \
        "F_LAST_LOG" DATETIME NOT NULL, \
        PRIMARY KEY ("F_ACCOUNTID"))"
    );
}

hook OnPlayerDeath(playerid, killerid, reason) {

    if(killerid != INVALID_PLAYER_ID){
        playerKills[killerid] ++;
    }
    playerDeaths[playerid] ++;
    return 1;
}

hook OnPlayerConnect(playerid) {
    playerAccountID[playerid] =
    playerKills[playerid] =
    playerDeaths[playerid] = 0;

    playerPassword[playerid][0] =
    playerSalt[playerid][0] =
    playerRegisterDate[playerid][0] =
    playerLastLoginDate[playerid][0] = EOS;

    new 
        playerName[MAX_PLAYER_NAME];

    GetPlayerName(playerid, playerName, sizeof(playerName));

    inline const LoadAccountData() {

        if(cache_num_rows() > 0) {
            cache_get_value_name_int(0, F_ACCOUNTID, playerAccountID[playerid]);
            cache_get_value_name(0, F_HASH, playerSalt[playerid], MAX_HASH);
            cache_get_value_name(0, F_PASSWORD, playerPassword[playerid], MAX_PASSWORD);



            PromptLogin(playerid);

        } else {
            PromptRegister(playerid);
        }

    }
    MySQL_TQueryInline(GetSQLHandle() , using inline LoadAccountData, "SELECT "F_ACCOUNTID", "F_PASSWORD", "F_HASH" FROM "T_ACCOUNT" WHERE "F_USERNAME" ='%e' LIMIT 1", playerName);
    return 1;
}

LoadAccount(playerid, const password[]) {
    new
        buf[65]
    ;

    SHA256_PassHash(password, playerSalt[playerid], buf, sizeof(buf));

    if (!isequal(buf, playerPassword[playerid]) && !isnull(password)) {
        SendErrorMsg(playerid, "Incorrect password");
        PromptLogin(playerid);
        return 1;
    }
    
    inline const AccountDataLoaded() {
        new
            wantedLevel,
            playerJob,
            playerMoney,
            playerScore,
            playerExperience
        ;   

        cache_get_value_name(0, "reg_date", playerRegisterDate[playerid], 40);
        cache_get_value_name(0, "last_log", playerLastLoginDate[playerid], 40);
        
        cache_get_value_name_int(0, F_MONEY, playerMoney);
        GivePlayerMoney(playerid, playerMoney);

        cache_get_value_name_int(0, F_SCORE, playerScore);
        SetPlayerScore(playerid, playerScore);

        cache_get_value_name_int(0, F_EXP, playerExperience);
        //SetPlayerXP(playerid, playerExperience, true);
       
        cache_get_value_name_int(0, F_JOB, playerJob);
        //SetPlayerJob(playerid, playerJob);

        cache_get_value_name_int(0, F_KILLS, playerKills[playerid]);
        cache_get_value_name_int(0, F_DEATHS, playerDeaths[playerid]);

        cache_get_value_name_int(0, F_WANTEDLVL, wantedLevel);
       

        if(wantedLevel > 0) {
            SetPlayerWantedLevel(playerid, wantedLevel);
            SendServerMsgF(playerid, "Continuing your last wanted level when you left - Wanted Level: "C_GREY"%i", wantedLevel);
        }
        mysql_tquery(GetSQLHandle(), sprintf("UPDATE "T_ACCOUNT" SET "F_LAST_LOG" = CURRENT_TIMESTAMP() WHERE "F_ACCOUNTID" = %i", GetPlayerAccountID(playerid)));

        CallLocalFunction("OnPlayerLogin", "i", playerid);
        
        SendServerMsg(playerid, "You have successfully logged in to your account! Welcome back!");
    }

    MySQL_PQueryInline(GetSQLHandle(), using inline AccountDataLoaded,
        "SELECT *, DATE_FORMAT("F_REG_DATE", '"SQL_DATE_FORMAT"') as reg_date, \
        DATE_FORMAT("F_LAST_LOG", '"SQL_DATETIME_FORMAT"') as last_log \
        FROM "T_ACCOUNT" WHERE "F_ACCOUNTID" = %i LIMIT 1", GetPlayerAccountID(playerid)); 
    return 1;

}

GetPlayerAccountID(playerid) {
    return playerAccountID[playerid];
}

SetPlayerAccountID(playerid, accid) {
    return playerAccountID[playerid] = accid;
}

GetPlayerKills(playerid) {
    return playerKills[playerid];
}

GetPlayerDeaths(playerid) {
    return playerDeaths[playerid];
}

GetplayerRegisterDate(playerid, date[], length) {
    return strcpy(date, playerRegisterDate[playerid], length);
}