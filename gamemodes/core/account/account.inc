
#include <a_mysql>


#include <YSI_Visual\y_dialog>
#include <YSI_Coding\y_inline>

#include <formatex>
#include <strlib>

#define TABLE_ACCOUNT       "accounts"
#define FIELD_ACCOUNTID     "accountID"
#define FIELD_USERNAME      "username"


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
        new
            playerSalt[MAX_HASH],
            playerPassword[MAX_PASSWORD];

        MySQL_BindResultInt(stmt_checkPlayerAccount, 0, playerAccountID[playerid]);
        MySQL_BindResult(stmt_checkPlayerAccount, 1, playerPassword, sizeof (playerPassword));
        MySQL_BindResult(stmt_checkPlayerAccount, 2, playerSalt, sizeof (playerSalt));

        if(MySQL_Statement_FetchRow(stmt_checkPlayerAccount)) {
            // show the login dialog and pass the password and salt for
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

    MySQL_Bind(stmt_checkPlayerAccount, 0, playerName);
    MySQL_ExecuteParallel_Inline(stmt_checkPlayerAccount, using inline LoadAccountData);
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

        MySQL_BindResultInt(stmt_loadPlayerAccount, 0, playerMoney);
        MySQL_BindResultInt(stmt_loadPlayerAccount, 1, playerKills[playerid]);
        MySQL_BindResultInt(stmt_loadPlayerAccount, 2, playerDeaths[playerid]);
        MySQL_BindResultInt(stmt_loadPlayerAccount, 3, playerJob);
        MySQL_BindResultInt(stmt_loadPlayerAccount, 4, playerScore);
        MySQL_BindResultInt(stmt_loadPlayerAccount, 5, playerExperience);
        MySQL_BindResultInt(stmt_loadPlayerAccount, 6, playerWantedLevel);

        MySQL_BindResult(stmt_loadPlayerAccount, 7, playerRegisterDate[playerid], 40);
        MySQL_BindResult(stmt_loadPlayerAccount, 8, playerLastLoginDate[playerid], 40);

        if(MySQL_Statement_FetchRow(stmt_loadPlayerAccount)) {
            GivePlayerMoney(playerid, playerMoney);
            SetPlayerScore(playerid, playerScore);
            SetPlayerXP(playerid, playerExperience, .first_spawn=true, .update=false);
            SetPlayerJob(playerid, playerJob);

            // if the player left with wanted level we give him his last wanted level.
            if(playerWantedLevel > 0) {
                SetPlayerWantedLevel(playerid, playerWantedLevel);
                SendServerMsgF(playerid, "Continuing your last wanted level when you left - Wanted Level: "C_GREY"%i", playerWantedLevel);
            }
        }
    }

    // notify player
    SendServerMsg(playerid, "You have successfully logged in to your account! Welcome back!");

    // call OnPlayerLogin
    CallLocalFunction("OnPlayerLogin", "i", playerid);

    new accountid = GetPlayerAccountID(playerid);

    // update player's last login date.
    MySQL_BindInt(stmt_updateLastLogin, 0, accountid);
    MySQL_ExecuteParallel(stmt_updateLastLogin);

    // execute statement
    MySQL_BindInt(stmt_loadPlayerAccount, 0, accountid);
    MySQL_ExecuteThreaded_Inline(stmt_loadPlayerAccount, using inline AccountDataLoaded);
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
    MySQL_BindInt(stmt_updateScore, 0, score);
    MySQL_BindInt(stmt_updateScore, 1, GetPlayerAccountID(playerid));
    MySQL_ExecuteThreaded(stmt_updateScore);
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
