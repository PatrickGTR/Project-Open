
#include <a_mysql>


#include <YSI_Visual\y_dialog>
#include <YSI_Coding\y_inline>

#include <formatex>
#include <strlib>

#define TABLE_ACCOUNT       "accounts"
#define FIELD_ACCOUNTID     "accountID"
#define FIELD_USERNAME      "username"
#define FIELD_PASSWORD      "password"
#define FIELD_SALT          "salt"
#define FIELD_MONEY         "money"
#define FIELD_KILLS         "kills"
#define FIELD_DEATHS        "deaths"
#define FIELD_JOB           "jobID"
#define FIELD_SCORE         "score"
#define FIELD_EXP           "experience"
#define FIELD_WANTEDLVL     "wantedlevel"
#define FIELD_REG_DATE      "registerdate"
#define FIELD_LAST_LOG      "lastlogin"

#define MAX_PASSWORD        (65)
#define MAX_HASH            (11)


#include <login>
#include <register>
#include <skills>
#include <items>
#include <vip>

static
    playerAccountID[MAX_PLAYERS],
    playerKills[MAX_PLAYERS],
    playerDeaths[MAX_PLAYERS],
    playerRegisterDate[MAX_PLAYERS][40],
    playerLastLoginDate[MAX_PLAYERS][40];

forward OnPlayerLogin(playerid);

#include <YSI_Coding\y_hooks>
hook OnGameModeInit()
{

    mysql_tquery(GetMySQLHandle(),
        "CREATE TABLE IF NOT EXISTS "TABLE_ACCOUNT" \
        ("FIELD_ACCOUNTID" INTEGER AUTO_INCREMENT, \
        "FIELD_USERNAME" VARCHAR(24) NOT NULL, \
        "FIELD_PASSWORD" VARCHAR(65) NOT NULL, \
        "FIELD_SALT" VARCHAR(11) NOT NULL, \
        "FIELD_MONEY" INT(11) NOT NULL, \
        "FIELD_KILLS" SMALLINT(6) NOT NULL, \
        "FIELD_DEATHS" SMALLINT(6) NOT NULL, \
        "FIELD_JOB" TINYINT(2) NOT NULL, \
        "FIELD_SCORE" INT(11) NOT NULL, \
        "FIELD_EXP" INT(11) NOT NULL, \
        "FIELD_WANTEDLVL" SMALLINT(6) NOT NULL, \
        "FIELD_LAST_LOG" DATETIME NOT NULL, \
        "FIELD_REG_DATE" DATETIME NOT NULL, \
        PRIMARY KEY ("FIELD_ACCOUNTID"))"
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
    // reset player data
    // ints
    playerAccountID[playerid] =
    playerKills[playerid] =
    playerDeaths[playerid] = 0;
    // arrays/strings
    playerRegisterDate[playerid][0] =
    playerLastLoginDate[playerid][0] = EOS;

    inline const LoadAccountData() {
        if(cache_num_rows() > 0) {

            new
                playerSalt[MAX_HASH],
                playerPassword[MAX_PASSWORD];
            // retrieve accountid, salt and password from database.
            // store accountID to playerAccountID
            // store salt to playerSalt
            // store password to playerPassword
            cache_get_value_name_int(0, FIELD_ACCOUNTID, playerAccountID[playerid]);
            cache_get_value_name(0, FIELD_SALT, playerSalt, MAX_HASH);
            cache_get_value_name(0, FIELD_PASSWORD, playerPassword, MAX_PASSWORD);

            // show the login dialog and pass the password and salt for
            printf("OnPlayerConnect: salt: %s | password: %s", playerSalt, playerPassword);
            PromptLogin(playerid, playerSalt, playerPassword);

        } else {

            // show the register dialog
            PromptRegister(playerid);
        }
    }

    // execute statement
    new
        playerName[MAX_PLAYER_NAME];

    GetPlayerName(playerid, playerName, sizeof(playerName));
    MySQL_PQueryInline(GetMySQLHandle() , using inline LoadAccountData, "SELECT "FIELD_ACCOUNTID", "FIELD_PASSWORD", "FIELD_SALT" FROM "TABLE_ACCOUNT" WHERE "FIELD_USERNAME" ='%e' LIMIT 1", playerName);
    return 1;
}

LoadAccount(playerid, const input_password[], const db_salt[], const db_password[]) {
    new
        buf[65];
    // hash password
    SHA256_PassHash(input_password, db_salt, buf, sizeof(buf));

    // compare password
    // if password from database isn't the same as the user input = wrong
    // if password is empty = wrong
    if (!isequal(buf, db_password) && !isnull(input_password)) {
        SendErrorMsg(playerid, "Incorrect password");
        PromptLogin(playerid, db_salt, db_password);
        return 1;
    }

    inline const AccountDataLoaded() {
        new
            playerWantedLevel,
            playerJob,
            playerMoney,
            playerScore,
            playerExperience
        ;

        cache_get_value_name(0, "reg_date", playerRegisterDate[playerid], 40);
        cache_get_value_name(0, "last_log", playerLastLoginDate[playerid], 40);

        cache_get_value_name_int(0, FIELD_MONEY, playerMoney);
        GivePlayerMoney(playerid, playerMoney);

        cache_get_value_name_int(0, FIELD_SCORE, playerScore);
        SetPlayerScore(playerid, playerScore);

        cache_get_value_name_int(0, FIELD_EXP, playerExperience);
        SetPlayerXP(playerid, playerExperience, .first_spawn=true, .update=false);

        cache_get_value_name_int(0, FIELD_JOB, playerJob);
        SetPlayerJob(playerid, playerJob);

        cache_get_value_name_int(0, FIELD_KILLS, playerKills[playerid]);
        cache_get_value_name_int(0, FIELD_DEATHS, playerDeaths[playerid]);
        cache_get_value_name_int(0, FIELD_WANTEDLVL, playerWantedLevel);

        // if the player left with wanted level we give him his last wanted level.
        if(playerWantedLevel > 0) {
            SetPlayerWantedLevel(playerid, playerWantedLevel);
            SendServerMsgF(playerid, "Continuing your last wanted level when you left - Wanted Level: "C_GREY"%i", playerWantedLevel);
        }

        // notify player that
        SendServerMsg(playerid, "You have successfully logged in to your account! Welcome back!");

        // called when player has fully signed in.
        CallLocalFunction("OnPlayerLogin", "i", playerid);

        // update player's last login date.
        mysql_pquery(GetMySQLHandle(), sprintf("UPDATE "TABLE_ACCOUNT" SET "FIELD_LAST_LOG" = CURRENT_TIMESTAMP() WHERE "FIELD_ACCOUNTID" = %i", GetPlayerAccountID(playerid)));
    }

    // execute statement
    MySQL_TQueryInline(GetMySQLHandle(), using inline AccountDataLoaded,
        "SELECT "FIELD_MONEY", "FIELD_KILLS", "FIELD_DEATHS", "FIELD_JOB", "FIELD_SCORE", "FIELD_EXP", "FIELD_WANTEDLVL", DATE_FORMAT("FIELD_REG_DATE", '"SQL_DATE_FORMAT"') as reg_date, \
        DATE_FORMAT("FIELD_LAST_LOG", '"SQL_DATETIME_FORMAT"') as last_log \
        FROM "TABLE_ACCOUNT" WHERE "FIELD_ACCOUNTID" = %i LIMIT 1", GetPlayerAccountID(playerid));
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

GetPlayerRegisterDate(playerid, date[], length) {
    return strcpy(date, playerRegisterDate[playerid], length);
}

stock hk_SetPlayerScore(playerid, score)
{
    mysql_tquery(GetMySQLHandle(), sprintf("UPDATE "TABLE_ACCOUNT" SET "FIELD_SCORE" = %i WHERE "FIELD_ACCOUNTID" = %i", score, GetPlayerAccountID(playerid)));
    return SetPlayerScore(playerid, score);
}
#if defined _ALS_SetPlayerScore
    #undef SetPlayerScore
#else
    #define _ALS_SetPlayerScore
#endif

#define SetPlayerScore hk_SetPlayerScore

GivePlayerScore(playerid, score)
    return SetPlayerScore(playerid, GetPlayerScore(playerid) + score);
