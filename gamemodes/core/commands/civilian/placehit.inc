CMD:placehit(playerid, params[])
{
	new 
		targetid, hitamount, reason[32];

	if (GetPlayerJob(playerid) == TYPE_HITMAN && GetPlayerClass(playerid) == TEAM_CIVILIAN )
		return SendErrorMsg(playerid, "You are not allowed to place hit on others if you are a hitman.");

	if (sscanf(params, "ris[32]", targetid, hitamount, reason))
		return SendSyntaxMsg(playerid, "/placehit <playerid> <Amount> <Reason>");

	if (!IsPlayerConnected(targetid))
		return SendErrorMsg(playerid, "The player you are placing hit on is not connected.");

	if (!IsPlayerSpawned(targetid))
		return SendErrorMsg(playerid, "The player you are placing hit on is not spawned.");

	if (GetPlayerMoney(playerid) < hitamount)
		return SendErrorMsg(playerid, "You do not have enough money to pay the hit, earn more money!");

	if (hitamount < 10000)
		return SendErrorMsg(playerid, "The hitman will not accept your offer, increase the amount of the hit!");

	if (IsHitPlayer(targetid))
		return SendErrorMsg(playerid, "The player you are placing hit on already have hit.");


	new 
		name[MAX_PLAYER_NAME];
	GetPlayerName(playerid, name, MAX_PLAYER_NAME);

	SetPlayerHitReason(targetid, reason, name);
	SetPlayerHitAmount(targetid, hitamount);
	SetHitPlayer(targetid, true);

	GivePlayerMoney(playerid, -hitamount);

	SendMsgF(playerid, COLOR_ORANGE, "Hit: "C_WHITE"You successfully placed a hit on "C_GREY"%p (%i) "C_WHITE"for "C_GREEN"$%i"C_WHITE", reason: "C_GREY"%s", targetid, targetid, hitamount, reason);
	SendMsgAllF(COLOR_ORANGE, "Hit: "C_GREY"%p (%i) "C_WHITE"placed a hit on "C_GREY"%p (%i) "C_WHITE"for "C_GREEN"$%i"C_WHITE", reason: "C_GREY"%s", playerid, playerid, targetid, targetid, hitamount, reason);
	return true;
}