#include <YSI_Coding\y_hooks>
#include <YSI_Data\y_bit>

new 
    playerClassID[MAX_PLAYERS],
    BitArray: first_time<MAX_PLAYERS>;

hook OnGameModeInit()
{
    // Civilian Skin
    AddPlayerClass(56,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	AddPlayerClass(119, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	AddPlayerClass(55,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	AddPlayerClass(60,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	AddPlayerClass(11,  0, 0, 0, 0, 0, 0, 0, 0, 0 , 0);
	AddPlayerClass(2,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	AddPlayerClass(299, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	AddPlayerClass(296, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	AddPlayerClass(297, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	AddPlayerClass(294, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	AddPlayerClass(293, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	AddPlayerClass(289, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	AddPlayerClass(272, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	AddPlayerClass(264, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	AddPlayerClass(268, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	AddPlayerClass(259, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	AddPlayerClass(250, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	AddPlayerClass(240, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	AddPlayerClass(221, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	AddPlayerClass(134, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	AddPlayerClass(29,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	AddPlayerClass(22,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	AddPlayerClass(19,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

	// Police Officer Skins
	AddPlayerClass(280, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	AddPlayerClass(281, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	AddPlayerClass(282, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	AddPlayerClass(283, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	AddPlayerClass(284, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	AddPlayerClass(285, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

	// Army Skins
	AddPlayerClass(121, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	AddPlayerClass(191, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	AddPlayerClass(287, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

	//Medic Skins
	AddPlayerClass( 274, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	AddPlayerClass( 275, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	AddPlayerClass( 276, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
}

public OnPlayerRequestClass(playerid, classid)
{

	SetPlayerPos(playerid, -1971.1536, 129.4870, 27.6875 );
	SetPlayerFacingAngle(playerid, 90.0);
	SetPlayerCameraPos(playerid, -1974.1431, 133.3700, 29.7107 );
	SetPlayerCameraLookAt(playerid, -1970.6431, 129.3700, 28.2107 );

    if(!Bit_Get(first_time, playerid)){
        InterpolateCameraPos( playerid, -1976.4252, 119.9899, 30.0, -1974.0302, 133.0427, 27.6940, 10000, CAMERA_MOVE );
	    InterpolateCameraLookAt( playerid, -1974.0302,133.0427,27.6940, -1971.1536,129.4870,27.6875, 10000, CAMERA_MOVE );
        Bit_Let(first_time, playerid);
    }

	switch(classid) {
        case 0 .. 22: {
            ShowClassTextdraw(playerid, COLOR_WHITE, "Civilian", "- Rob stores and kill players to earn XP.~n~- Show players you run the streets.");
            SetPlayerColor(playerid, COLOR_WHITE);
            SetPlayerClass(playerid, TEAM_CIVILIAN);
        }
        case 23 .. 28: {
            ShowClassTextdraw(playerid, COLOR_BLUE, "Police Officer", "- Hunt down and arrest criminals for cash and XP.~n~- Bring criminals to justice.");
            SetPlayerColor(playerid, COLOR_BLUE);
            SetPlayerClass(playerid, TEAM_POLICE);
        }
        case 29 .. 31: {
            ShowClassTextdraw(playerid, COLOR_PURPLE, "San Fierro Army", "- Hunt down and arrest criminals for cash and XP.~n~- Access powerful weaponry and machinery to kill criminals.");
            SetPlayerColor(playerid, COLOR_PURPLE);
            SetPlayerClass(playerid, TEAM_ARMY);
        }
        case 32 .. 34: {
            ShowClassTextdraw(playerid, COLOR_GREEN, "Medic", "- Heal people from diseases, keeping the city clean.~n~- ");
            SetPlayerColor(playerid, COLOR_GREEN);
            SetPlayerClass(playerid, TEAM_MEDIC);
        }
    }
    return 1;
}

hook OnPlayerDisconnect(playerid, reason) {
    Bit_Vet(first_time, playerid); 
    return 1;
}

hook OnPlayerConnect(playerid) {
    SetPlayerJob(playerid, TYPE_NO_JOB);
    return 1;
}

hook OnPlayerSpawn(playerid) {
    SetPlayerInterior(playerid, 0); //Set default interior to 0 when player spawns.
    SetPlayerVirtualWorld(playerid, 0); //Set virtualworld interior to 0 when player spawns.

    switch(GetPlayerClass(playerid)) {
        case TEAM_POLICE: {
            SetPlayerPos(playerid, -1606.4692, 674.9128, -5.2422);
            SetPlayerFacingAngle(playerid, 1.5953);
            SetCameraBehindPlayer(playerid);

            GivePlayerWeapon(playerid, WEAPON_NITESTICK,    1);
            GivePlayerWeapon(playerid, WEAPON_DEAGLE,       200);
            GivePlayerWeapon(playerid, WEAPON_SHOTGUN,      200);
        }
        case TEAM_ARMY: {
            SetPlayerPos(playerid, -1347.8856, 499.8250, 18.2344);
            SetPlayerFacingAngle(playerid, 358.7014);
            SetCameraBehindPlayer(playerid);

            GivePlayerWeapon(playerid, WEAPON_GRENADE,      1);
            GivePlayerWeapon(playerid, WEAPON_DEAGLE,       150);
            GivePlayerWeapon(playerid, WEAPON_M4,           450);
            GivePlayerWeapon(playerid, WEAPON_SHOTGSPA,     150);
        }
        case TEAM_MEDIC: {
            SetPlayerPos(playerid, -2651.3867, 631.9839, 14.4545);
            SetPlayerFacingAngle(playerid, 178.1564);

            GivePlayerWeapon(playerid, WEAPON_SPRAYCAN, 500);
            GivePlayerWeapon(playerid, WEAPON_SHOTGUN, 50);
            GivePlayerWeapon(playerid, WEAPON_COLT45, 100);
        }
        case TEAM_CIVILIAN: {
            if (!ShowPlayerJobMenu(playerid)) { 
                GivePlayerJobWeapons(playerid);
            }
            SpawnPlayerToSpawnPoint(playerid);
            //UpdatePlayerColour(playerid);
        }
    }
    return 1;
}

SetPlayerClass(playerid, classid) {
    return playerClassID[playerid] = classid;
}

GetPlayerClass(playerid) {
    return playerClassID[playerid];
}