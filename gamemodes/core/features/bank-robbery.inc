#include <YSI_Coding\y_hooks>
#include <formatex>
#include <strlib>

#define ROBBERY_STREAMER_IDENTIFIER (200)

static Float:bankRobberyPoints[8][3] = {
	{1437.8900, -978.1204, 983.5342},
	{1437.8900, -974.1539, 983.5342},
	{1437.8900, -970.2146, 983.5342},
	{1437.8900, -966.2356, 983.5342},
	{1432.8900, -966.1697, 983.5342},
	{1432.8900, -970.1374, 983.5342},
	{1432.8900, -974.0399, 983.5342},
	{1432.8900, -977.9689, 983.5342}
};


static
	bankRobberyPickup[8],
	Text3D:bankRobberyLabel[8],
	bool:safeHasBeenRobbed[8],
	plySafeID[MAX_PLAYERS],
	gBankSafe,
	isBankDoorBreached
;


hook OnGameModeInit() {
    // Bank Safe Door //
    gBankSafe = CreateDynamicObject(2634, 1435.35193, -980.29688, 984.21887, 0.00000, 0.00000, 179.04001);

    new
    	arr[2];

    arr[0] = ROBBERY_STREAMER_IDENTIFIER;
    for(new i = 0; i != sizeof(bankRobberyPickup); i++) {
    	bankRobberyPickup[i] = CreateDynamicPickup(1274, 1, bankRobberyPoints[i][0], bankRobberyPoints[i][1], bankRobberyPoints[i][2], 1, 1, .streamdistance = 25);
    	bankRobberyLabel[i] = CreateDynamic3DTextLabel(sprintf("Safe #%i - Press N to Rob\n"C_GREEN"Available for robbing", i), COLOR_BLUE, bankRobberyPoints[i][0], bankRobberyPoints[i][1], bankRobberyPoints[i][2], 25.0, .testlos = 1);
    	arr[1] = i;

	   	Streamer_SetArrayData(STREAMER_TYPE_PICKUP, bankRobberyPickup[i], E_STREAMER_EXTRA_ID, arr, sizeof(arr));
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerPickUpDynPickup(playerid, pickupid) {
	new
		arr[2]
	;

	Streamer_GetArrayData(STREAMER_TYPE_PICKUP, pickupid, E_STREAMER_EXTRA_ID, arr, sizeof(arr));

	if(arr[0] == ROBBERY_STREAMER_IDENTIFIER && pickupid == bankRobberyPickup[arr[1]]) {
		plySafeID[playerid] = arr[1];
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys) {
	if ((newkeys & KEY_NO) == KEY_NO && GetPlayerState(playerid) == PLAYER_STATE_ONFOOT && IsPlayerInRangeOfPoint(playerid, 50.0, 1435.3818, -981.1404, 983.7663)) {
		new
			safeID;

		safeID =  plySafeID[playerid];

		if(safeHasBeenRobbed[safeID] == true)
			return SendErrorMsgF(playerid, "safe #%i has been robbed recently, try again later!", safeID);

		GameTextForPlayer(playerid, "Robbing Safe.....~n~wait 5 seconds...", 3000, 6);
		ApplyAnimation(playerid, "ROB_BANK", "CAT_SAFE_ROB", 4.1, 1, 1, 1, 5, 1);

		defer UpdateSafeStatus(playerid, safeID);
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

timer UpdateSafeStatus[1000 * 5](playerid, safeID) {
	new
		cashRobbed = random(10000 - 5000) + 5000;


	GivePlayerMoney(playerid, cashRobbed);
	MessageBox(playerid, TYPE_MSGBOX, "Bank", sprintf("You took ~g~%m ~w~from safe id ~y~#%i", cashRobbed, safeID), 3000);

	SendMsgAllF(COLOR_ORANGE, "** BANK ROBBERY ** %p (%i) "C_WHITE"has robbed "C_GREEN"%m "C_WHITE"from bank safe "C_GREY"#%i", playerid, playerid, cashRobbed, safeID);

	UpdateDynamic3DTextLabelText(bankRobberyLabel[safeID], COLOR_BLUE, sprintf("Safe %i - Press N to Rob\n"C_RED"Unavailable for robbing", safeID));
	safeHasBeenRobbed[safeID] = true;

	ClearAnimations(playerid, 1);
}


ExplodeBankDoors(playerid) {
	if(isBankDoorBreached)
		return 0;

	if (IsPlayerInRangeOfPoint(playerid, 50.0, 1435.3818, -981.1404, 983.7663)) {
		MoveDynamicObject(gBankSafe, 1435.3333, -980.2640, 982.5589, 1.50, -90.9400, -0.2200, 179.0000);
		SendMsgAllF(COLOR_ORANGE, "** BANK ROBBERY ** %p (%i) "C_WHITE"managed to break the bank safe by planting c4 at door and detonated it.", playerid, playerid);
		defer CloseBankSafe();

		isBankDoorBreached = true;
	}
	return 1;
}

timer CloseBankSafe[1000 * 60 * 10]() { //Bank can be robbed every 10 minutes.
	for(new i = 0; i != sizeof(bankRobberyPoints); i++) {
		Update3DTextLabelText(bankRobberyLabel[i], COLOR_BLUE, "Safe #1 - Press N to Rob\n"C_GREEN"Available for robbing");
		safeHasBeenRobbed[i] = true;
	}
	SendMsgAllF(COLOR_ORANGE, "** BANK ** "C_WHITE"Bank safe can be now rob again! GO! GO! GO!");
	MoveDynamicObject(gBankSafe, 1435.35193, -980.29688, 984.21887, 1.50, 0.00000, 0.00000, 179.04001);

	isBankDoorBreached = false;
}