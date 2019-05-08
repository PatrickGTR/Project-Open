#include <YSI_Coding\y_hooks>
#include <YSI_Data\y_bit>

static
BitArray: spec_isAdminSpectating<MAX_PLAYERS>,
		  spec_AdminSpectateID[MAX_PLAYERS],

		  //Save player position when going specmode.
   Float: spec_plyPosX[MAX_PLAYERS],
   Float: spec_plyPosY[MAX_PLAYERS],
   Float: spec_plyPosZ[MAX_PLAYERS],
   Float: spec_plyHealth[MAX_PLAYERS],
   Float: spec_plyArmour[MAX_PLAYERS],
		  spec_plyIntID[MAX_PLAYERS],
		  spec_plyVWID[MAX_PLAYERS]
 ;


hook OnPlayerSpawn(playerid) {
	if(IsPlayerSpectating(playerid)) {

		if(!IsAdminOnDuty(playerid)) {

			SetPlayerHealth(playerid, spec_plyHealth[playerid]);

			if(spec_plyArmour[playerid] > 0) {
				SetPlayerArmour(playerid, spec_plyArmour[playerid]);
			}
		}

		SetPlayerPos(playerid, spec_plyPosX[playerid], spec_plyPosY[playerid], spec_plyPosZ[playerid]);
		SetCameraBehindPlayer(playerid);
		SetPlayerInterior(playerid, spec_plyIntID[playerid]);
		SetPlayerInterior(playerid, spec_plyVWID[playerid]);

		Bit_Set(spec_isAdminSpectating, playerid, false);

		SendServerMsg(playerid, "You have been placed back from your previous position and data.");

		return Y_HOOKS_CONTINUE_RETURN_0;
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys) {
	if(IsPlayerSpectating(playerid)) {
		if(PRESSED(KEY_FIRE)) {
			SpectatePlayer(playerid, SpectatePrevPlayer(playerid));
		}
		if(PRESSED(KEY_AIM)) {
			SpectatePlayer(playerid, SpectateNextPlayer(playerid));
		}
	}
}

hook OnPlayerEnterDynamicCP(playerid, checkpointid) {

	if(IsPlayerSpectating(playerid)) {
		static
			_ply_interiorID[MAX_PLAYERS];

		new
			targetid = spec_AdminSpectateID[playerid];

		if(_ply_interiorID[targetid] != GetPlayerInterior(targetid)) {
			SpectatePlayer(playerid, targetid);

			_ply_interiorID[targetid] = GetPlayerInterior(targetid);
		}
	}
}

SpectateSaveData(playerid)
{
	GetPlayerPos(playerid, spec_plyPosX[playerid], spec_plyPosY[playerid], spec_plyPosZ[playerid]);
	GetPlayerHealth(playerid, spec_plyHealth[playerid]);
	GetPlayerArmour(playerid, spec_plyArmour[playerid]);
	spec_plyIntID[playerid] = GetPlayerInterior(playerid);
	spec_plyVWID[playerid] = GetPlayerVirtualWorld(playerid);
}

SpectateNextPlayer(playerid) {
	new
		targetid = spec_AdminSpectateID[playerid];

	if(!IsValidPlayerID(targetid))
		return 0;

	// current targetid + 1 and find the next connected playerid.
	for(new i = targetid + 1; i < MAX_PLAYERS; i++)
	{
		// if ID is same as playerid, skip it.
		if(i == playerid)
			continue;
		// if Player is not connected, skip it.
		if(!IsPlayerConnected(i))
			continue;
		// asign the i variable to targetid.
		targetid = i;
		break;
	}
	return targetid;
}

SpectatePrevPlayer(playerid) {
	new
		targetid = spec_AdminSpectateID[playerid];

	if(!IsValidPlayerID(targetid))
		return 0;

	// current targetid - 1 and find the next connected playerid.
	for(new i = targetid - 1; i < targetid; i--)
	{
		// if ID is same as playerid, skip it.
		if(i == playerid)
			continue;
		// if ID is not valid, skip it.
		if(!IsValidPlayerID(i))
			continue;
		// if Player is not connected, skip it.
		if(!IsPlayerConnected(i))
			continue;
		// asign the i variable to targetid.
		targetid = i;
		break;
	}
	return targetid;
}

SpectatePlayer(playerid, targetid) {

	if(targetid == INVALID_PLAYER_ID)
		return 0;

	if (IsPlayerInAnyVehicle(targetid)) {
		SetPlayerInterior(playerid, GetPlayerInterior(targetid));
		SetPlayerVirtualWorld(playerid, GetPlayerVirtualWorld(targetid));
		PlayerSpectateVehicle(playerid, GetPlayerVehicleID(targetid));

	}
	else {

		SetPlayerInterior(playerid, GetPlayerInterior(targetid));
		SetPlayerVirtualWorld(playerid, GetPlayerVirtualWorld(targetid));
		PlayerSpectatePlayer(playerid, targetid);

	}

	Bit_Set(spec_isAdminSpectating, playerid, true);
	spec_AdminSpectateID[playerid] = targetid;

	SendMsgF(playerid, COLOR_YELLOW, "[SPECTATE] "C_WHITE"You are now spectating %p[%i]", targetid, targetid);
	SendAdminMessageF("You are now spectating %p[%i]", targetid, targetid);
	return 1;
}

IsPlayerSpectating(playerid)
	return Bit_Get(spec_isAdminSpectating, playerid);
