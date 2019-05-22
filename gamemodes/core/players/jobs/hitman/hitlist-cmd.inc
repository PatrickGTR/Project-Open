CMD:hitlist(playerid, params[])
{
	new 
		count = 0;

	if (GetPlayerJob(playerid) != TYPE_HITMAN || GetPlayerClass(playerid) != TEAM_CIVILIAN)
		return SendErrorMsg(playerid, "You are not allowed to use this command if you are not a hitman.");

	SendClientMessage(playerid, COLOR_GREEN, "______________________ "C_ORANGE"[ BEGIN ] "C_GREEN"______________________");

	new 
		_hitAmount, 
		_hitReason[32],
		_hitPlacedBy[MAX_PLAYER_NAME];		

	foreach(new i : Player) {
		if (IsHitPlayer(i)) {
			GetPlayerHitData(playerid, _hitAmount, _hitPlacedBy, _hitReason);
			SendMsgF(playerid, COLOR_WHITE, "Username: %p (%i) - Placed By: %s - Reason: %s- Amount: "C_GREEN"$%i", i, i, _hitPlacedBy,  _hitReason, _hitAmount);
			++ count;
		}
	}
	if (count != 1) SendClientMessage(playerid, COLOR_WHITE, "There are no hits available");
	SendClientMessage(playerid, COLOR_GREEN, "______________________ "C_ORANGE"[ END ] "C_GREEN"______________________");
	return true;
}