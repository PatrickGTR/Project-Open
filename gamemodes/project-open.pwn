/*
    Contributor:
        PatrickGTR
    Thanks:
        Southclaw  - I took a lot of snippets from S&S to make productivity quicker.
        Zeex       - Crashdetect.
        Y_Less     - YSI, sscanf.
        maddinat0r - MySQL.
        Slice      - strlib, formatex.
*/

#include <a_samp>

native gpci(playerid, buffer[], size = sizeof(buffer));

// Definitions
#undef MAX_PLAYERS
#undef MAX_VEHICLES

#define SV_NAME 			        "Project Open"
#define SV_CURRENT_BUILD	        "1.0"

#define MAX_PLAYERS             	(50)
#define MAX_VEHICLES                (1000)
#define MAX_HOUSES                  (100)

#define INVALID_HOUSE_ID            (-1)

#define MAX_PASSWORD            	(65)
#define MAX_BAN_REASON          	(32)
#define TEXTLABEL_STREAMDISTANCE    (50)
#define CHECKPOINT_STREAMDISTANCE   (50)
#define MAPICON_STREAMDISTANCE      (300)

#define STRLIB_RETURN_SIZE          (256) //re-defined for MySQL queries.

#define SQL_DATETIME_FORMAT         "%d, %M, %%Y at %r"
#define SQL_DATE_FORMAT             "%d %M %Y"

#define KEY_AIM 128


// Macros
#define PRESSED(%0) (((newkeys & (%0)) == (%0)) && ((oldkeys & (%0)) != (%0)))
#define PRESSING(%0,%1) (%0 & (%1))
#define RELEASED(%0) (((newkeys & (%0)) != (%0)) && ((oldkeys & (%0)) == (%0)))

#define RetPlayerA(%0) (GetPlayerArmour((%0),Float:J@), Float:J@)
#define RetPlayerH(%0) (GetPlayerHealth((%0), Float:J@), Float:J@)

// Teams
enum{
    TEAM_CIVILIAN = NO_TEAM ,
    TEAM_POLICE = 0,
    TEAM_ARMY,
    TEAM_MEDIC
}

// Jobs
enum {
    TYPE_NO_JOB = -1,
    TYPE_DRUGDEALER = 0,
    TYPE_WEAPONDEALER,
    TYPE_HITMAN,
    TYPE_TERRORIST,
    TYPE_RAPIST,
    TYPE_MECHANIC
}


#include <utils>
#include <server>

#include <a_mysqL>

#define MAX_STATEMENTS (100)
#include <mysql_prepared>
new
    Statement: stmt_checkPlayerAccount,
    Statement: stmt_loadPlayerAccount,

    Statement: stmt_insertPlayerAccount,

    Statement: stmt_updateLastLogin,
    Statement: stmt_updateScore,
    Statement: stmt_updateExperience,
    Statement: stmt_updateJob,

    Statement: stmt_selectDeleteAccount,
    Statement: stmt_deleteAccount;

#include <YSI_Coding\y_hooks>
hook OnGameModeInit() {

    stmt_checkPlayerAccount = MySQL_PrepareStatement(GetMySQLHandle(), "SELECT accountID, password, salt FROM accounts WHERE username = ? LIMIT 1");
    stmt_loadPlayerAccount = MySQL_PrepareStatement(GetMySQLHandle(), "SELECT money, kills, deaths, jobID, score, experience, wantedlevel, \
    DATE_FORMAT(registerdate, '"SQL_DATE_FORMAT"'), DATE_FORMAT(lastlogin, '"SQL_DATETIME_FORMAT"') FROM accounts WHERE accountID = ? LIMIT 1");

    stmt_updateLastLogin = MySQL_PrepareStatement(GetMySQLHandle(), "UPDATE accounts SET lastlogin = CURRENT_TIMESTAMP() WHERE accountID = ?");

    stmt_selectDeleteAccount = MySQL_PrepareStatement(GetMySQLHandle(), "SELECT accountID FROM accounts WHERE username = ? LIMIT 1");
    stmt_deleteAccount = MySQL_PrepareStatement(GetMySQLHandle(), "DELETE FROM accounts WHERE accountID = ?");

    stmt_insertPlayerAccount = MySQL_PrepareStatement(GetMySQLHandle(), "INSERT INTO accounts (username, password, salt, registerdate) VALUES (?, ?, ?, CURRENT_TIMESTAMP())");
    stmt_updateScore = MySQL_PrepareStatement(GetMySQLHandle(), "UPDATE accounts SET score = ? WHERE accountID = ?");
    stmt_updateExperience = MySQL_PrepareStatement(GetMySQLHandle(), "UPDATE accounts SET experience = ? WHERE accountID = ?");
    stmt_updateJob = MySQL_PrepareStatement(GetMySQLHandle(), "UPDATE accounts SET jobID = ? WHERE accountID = ?");
    return 1;
}

#include <ui>
#include <account>
#include <features>
#include <admin>
#include <players>
#include <houses>
#include <chat>
#include <commands>

main () {
    // Server Vehicles
	LoadStaticVehiclesFromFile("vehicles/sf_law.txt");
	LoadStaticVehiclesFromFile("vehicles/sf_airport.txt");
	LoadStaticVehiclesFromFile("vehicles/sf_gen.txt");
	LoadStaticVehiclesFromFile("vehicles/whetstone.txt");
	LoadStaticVehiclesFromFile("vehicles/bone.txt");

}

CMD:money(playerid, params[])
{
    GivePlayerMoney(playerid, 99999999);
    return 1;
}

