#include <YSI_Coding\y_hooks>
#include <formatex>
#include <strlib>

static 
	PlayerText: playerZone[MAX_PLAYERS],
	PlayerText: vehicleSpeedometer[MAX_PLAYERS]
;

hook OnPlayerConnect(playerid) {
	playerZone[playerid] = CreatePlayerTextDraw(playerid, 5.0, 428.0, "Location: ~g~Downton");
	PlayerTextDrawLetterSize(playerid,playerZone[playerid], 0.2, 0.999);
	PlayerTextDrawAlignment(playerid, playerZone[playerid], 1);
	PlayerTextDrawFont(playerid, playerZone[playerid], 1);
	PlayerTextDrawColor(playerid, playerZone[playerid], -1);
	PlayerTextDrawSetOutline(playerid, playerZone[playerid], 1);
	PlayerTextDrawBackgroundColor(playerid, playerZone[playerid], TD_OUTLINE_COLOUR);
	PlayerTextDrawSetProportional(playerid, playerZone[playerid], 1);
	
	vehicleSpeedometer[playerid] = CreatePlayerTextDraw(playerid, 5.0, 438.0, "Vehicle: ~g~ Speed: ~y~0 ~g~KM/H Vehicle Health: ~g~0"); 
	PlayerTextDrawLetterSize(playerid, vehicleSpeedometer[playerid], 0.2, 0.999);
	PlayerTextDrawAlignment(playerid, vehicleSpeedometer[playerid], 1);
	PlayerTextDrawFont(playerid, vehicleSpeedometer[playerid], 1);
	PlayerTextDrawColor(playerid, vehicleSpeedometer[playerid], -1);
	PlayerTextDrawSetOutline(playerid, vehicleSpeedometer[playerid], 1);
	PlayerTextDrawBackgroundColor(playerid, playerZone[playerid], TD_OUTLINE_COLOUR);
	PlayerTextDrawSetProportional(playerid, vehicleSpeedometer[playerid], 1);
	return 1;
}

hook OnPlayerSpawn(playerid) {
	PlayerTextDrawShow(playerid, playerZone[playerid]);
	return 1;
}

hook OnPlayerDeath(playerid, killerid, reason) {
	PlayerTextDrawHide(playerid, playerZone[playerid]);

	if(IsPlayerInAnyVehicle(playerid)) {
		PlayerTextDrawHide(playerid, vehicleSpeedometer[playerid]);
	}
	return 1;
}

hook OnPlayerDisconnect(playerid, reason) {
    PlayerTextDrawDestroy(playerid, playerZone[playerid]);
	PlayerTextDrawDestroy(playerid, vehicleSpeedometer[playerid]);
	return 1;
}

hook OnPlayerStateChange(playerid, newstate, oldstate) {
    if (newstate == PLAYER_STATE_DRIVER || newstate == PLAYER_STATE_PASSENGER)  {
    	PlayerTextDrawShow(playerid, vehicleSpeedometer[playerid]);
	}
	else {
		PlayerTextDrawHide(playerid, vehicleSpeedometer[playerid]);
	}
	return 1;
}

#define tabulate_td "		"

ptask OnSpeedoMeterUpdate[500](playerid) {
	if (IsPlayerInAnyVehicle(playerid)) {
		static 
			Float:vehicleHealth
		;
		GetVehicleHealth(GetPlayerVehicleID(playerid), vehicleHealth);
		PlayerTextDrawSetString(playerid, vehicleSpeedometer[playerid], sprintf("\
					~w~Vehicle: ~g~%v"tabulate_td"\
					~w~Vehicle Health: ~g~%.2f"tabulate_td"\
					~w~Speed: ~y~%d ~g~km/h\
				", GetVehicleModel(GetPlayerVehicleID(playerid)), (((vehicleHealth - 250.0) / (1000.0 - 250.0)) * 100.0), GetVehicleSpeedKMH(playerid, TYPE_KMH))
			);
	}
	UpdatePlayerTDLocation(playerid);
}

#undef tabulate_td

static 
	playercurrentZone[MAX_PLAYERS][MAX_ZONE_NAME]
;

UpdatePlayerTDLocation(playerid) {
	if (!isequal(GetPlayerLocation(playerid), playercurrentZone[playerid])) {
		PlayerTextDrawSetString(playerid, playerZone[playerid], sprintf("~w~Location: ~g~%s", GetPlayerLocation(playerid)));

		strcpy(playercurrentZone[playerid], GetPlayerLocation(playerid), MAX_ZONE_NAME);
	}
	return 1;
}