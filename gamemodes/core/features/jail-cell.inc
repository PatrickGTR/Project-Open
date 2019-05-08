#include <YSI_Data\y_iterate>
#include <YSI_Coding\y_hooks>

static
	cell_Gate1,
	cell_Gate2,
	cell_Gate3,
	cell_Gate4,
	bool:cell_isDoorBreached;

CMD:gotosfpd(playerid, params[]) {
	GivePlayerC4(playerid, 2);
	SetPlayerPos(playerid, 214.70010, 112.66120, 999.29272);
	return 1;
}

hook OnGameModeInit() {
	//The body with no cell
	CreateDynamicObject(19301, 246.23441, 117.80470, 1005.60718, 0.00000, 0.00000, 180.00000);

	//Explodable gates
	cell_Gate1 = CreateDynamicObject(19302, 214.70010, 112.66120, 999.29272, 0.00000, 0.00000, 0.00000);
    CreateDynamicObject(19303, 216.46831, 112.66720, 999.29272, 0.00000, 0.00000, 0.00000);

	cell_Gate2 = CreateDynamicObject(19302, 218.64430, 112.66720, 999.29272, 0.00000, 0.00000, 0.00000);
	CreateDynamicObject(19303, 220.40430, 112.66720, 999.29272, 0.00000, 0.00000, 0.00000);

	cell_Gate3 = CreateDynamicObject(19302, 222.60410, 112.66720, 999.29272, 0.00000, 0.00000, 0.00000);
    CreateDynamicObject(19303, 224.38879, 112.66720, 999.29272, 0.00000, 0.00000, 0.00000);

	cell_Gate4 = CreateDynamicObject(19302, 226.47610, 112.66720, 999.29272, 0.00000, 0.00000, 0.00000);
    CreateDynamicObject(19303, 228.27631, 112.66720, 999.29272, 0.00000, 0.00000, 0.00000);
}

hook OnPlayerConnect(playerid) {
	RemoveBuildingForPlayer(playerid, 14847, 246.2344, 117.8047, 1005.6094, 0.25);
}

ExplodeCellDoors(playerid) {
	if (IsPlayerInRangeOfPoint(playerid, 50.0, 218.8895, 114.5328, 999.0134)) {
		SFCellOpen(playerid);
		defer SFCellClose();
	}
}

timer SFCellClose[1000 * 15]() {
	MoveDynamicObject(cell_Gate1, 214.70010, 112.66120, 999.29272, 0.5, 0.0, 0.0, 0.0);
	MoveDynamicObject(cell_Gate2, 218.64430, 112.66720, 999.29272, 0.5, 0.0, 0.0, 0.0);
	MoveDynamicObject(cell_Gate3, 222.60410, 112.66720, 999.29272, 0.5, 0.0, 0.0, 0.0);
	MoveDynamicObject(cell_Gate4, 226.47610, 112.66720, 999.29272, 0.5, 0.0, 0.0, 0.0);

	cell_isDoorBreached = false;
}

SFCellOpen(playerid) {
	if(cell_isDoorBreached)
		return 0;

	foreach(new i : Player) {
		if(IsPlayerInJail(i)) {
			SetPlayerJailTime(i, 0);
			SendMsgF(i, COLOR_ORANGE, "** JAIL BREACH ** The cell doors have been breached, the doors are open for you to escape! RUN!");
		}
	}
	SendMsgAllF(COLOR_ORANGE, "** JAIL BREACH ** %p (%i) "C_WHITE"managed to free all the prisoners by planting c4 and detonated it.", playerid, playerid);
	MoveDynamicObject(cell_Gate1, 214.70010 + 1.5, 112.66120, 999.29272, 0.5, 0.0, 0.0, 0.0);
	MoveDynamicObject(cell_Gate2, 218.64430 + 1.5, 112.66720, 999.29272, 0.5, 0.0, 0.0, 0.0);
	MoveDynamicObject(cell_Gate3, 222.60410 + 1.5, 112.66720, 999.29272, 0.5, 0.0, 0.0, 0.0);
	MoveDynamicObject(cell_Gate4, 226.47610 + 1.5, 112.66720, 999.29272, 0.5, 0.0, 0.0, 0.0);

	cell_isDoorBreached = true;
	return 1;
}

