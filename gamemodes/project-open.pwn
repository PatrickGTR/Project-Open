/*
    Contributor:
        PatrickGTR
    Thanks:
        Southclaw  - I took a lot of snippets from S&S to make productivity quicker.
        Zeex       - Crashdetect.
        Y_Less     - YSI, sscanf.
        maddinat0r - MySQL, helped me with an issue in house system.
        Slice      - strlib, formatex.
*/ 

#include <a_samp>

native gpci(playerid, buffer[], size = sizeof(buffer));

// Definitions
#undef MAX_PLAYERS 
#undef MAX_VEHICLES

#define SV_NAME 			"Project Open"
#define SV_CURRENT_BUILD	"1.0"

#define MAX_PLAYERS             	(32)
#define MAX_VEHICLES                (1000)

#define MAX_PASSWORD            	(65)
#define MAX_BAN_REASON          	(32)
#define TEXTLABEL_STREAMDISTANCE    (50)
#define CHECKPOINT_STREAMDISTANCE   (50)
#define MAPICON_STREAMDISTANCE      (300)

#define STRLIB_RETURN_SIZE      (256) //re-defined for MySQL queries.

#define SQL_DATETIME_FORMAT     "%%d, %%M, %%Y at %%r"
#define SQL_DATE_FORMAT         "%%d %%M %%Y"

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
#include <ui>
#include <account-auth>
#include <items>
#include <skills>
#include <vip>
#include <features>
#include <admin>
#include <players>
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
