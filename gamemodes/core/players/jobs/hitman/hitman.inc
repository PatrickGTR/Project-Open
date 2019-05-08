#include <YSI_Coding\y_hooks>
#include <YSI_Data\y_iterate>
#include <YSI_Data\y_bit>
#include <sscanf2>

static 
    hitAmount[MAX_PLAYERS],
    hitReason[MAX_PLAYERS][32],
    hitPlacedBy[MAX_PLAYERS][MAX_PLAYER_NAME],
    BitArray:HasHit<MAX_PLAYERS>
;

hook OnPlayerConnect(playerid) {
	ResetPlayerHitStatus(playerid);
	return 1;
}

ResetPlayerHitStatus(playerid) {

	if(!IsValidPlayerID(playerid)) {
		return 0;
	}

	Bit_Set(HasHit, playerid, false);
	hitAmount[playerid] = -1;
	hitPlacedBy[playerid][0] =
	hitReason[playerid][0] = EOS;
	return 1;
}

SetPlayerHitReason(playerid, const reason[]) {
	if(!IsValidPlayerID(playerid)) {
		return 0;
	}
	if(strlen(reason) > 32) {
		return 0;
	}
	
	strcpy(hitReason[playerid], reason, 32);
	return 1;
}

SetPlayerHitAmount(playerid, amount) {

	if(!IsValidPlayerID(playerid)) {
		return 0;
	}

	if(!IsValidMoneyAmount(amount)) {
		return 0;
	}

	hitAmount[playerid] = amount;
	return 1;
}

GetPlayerHitAmount(playerid) {
	return hitAmount[playerid];
}

hook OnPlayerDeath(playerid, killerid, reason)
{
    if (killerid != INVALID_PLAYER_ID)
    {      
        if (Bit_Get(HasHit, playerid) && GetPlayerJob(killerid) == TYPE_HITMAN)
        {
            SendMsgAllF(COLOR_ORANGE, "** HIT **: "C_GREY"%p (%i) "C_WHITE"has completed the hit on "C_GREY"%p (%i) "C_WHITE"and recieved "C_GREEN"$%i.", killerid, killerid, playerid, playerid, GetPlayerHitAmount(playerid));
            
            GivePlayerMoney(killerid, GetPlayerHitAmount(playerid));

			ResetPlayerHitStatus(playerid);

        }
        if (Bit_Get(HasHit, playerid) && GetPlayerJob(killerid) != TYPE_HITMAN)
        {
            SendMsgAllF(COLOR_ORANGE, "** HIT **: "C_WHITE"Contract on "C_GREY"%p (%i) "C_WHITE"has been cancelled, player died.", playerid, playerid);

			ResetPlayerHitStatus(playerid);
        }
    }
}

hook OnPlayerDisconnect(playerid, reason)
{
    if (Bit_Get(HasHit, playerid)) 
    {
        SendMsgAllF(COLOR_ORANGE, "** HIT **: "C_WHITE"Contract on "C_GREY"%p (%i) "C_WHITE"has been cancelled, player left.", playerid, playerid);
    }
	return 1;
}
/*
	[HITMAN COMMANDS]
*/
CMD:hitlist(playerid, params[])
{
	new 
		count = 0;

	if (GetPlayerJob(playerid) != TYPE_HITMAN || GetPlayerClass(playerid) != TEAM_CIVILIAN)
		return SendErrorMsg(playerid, "You are not allowed to use this command if you are not a hitman.");

	SendClientMessage(playerid, COLOR_GREEN, "______________________ "C_ORANGE"[ BEGIN ] "C_GREEN"______________________");
	foreach(new i : Player)
	{
		if (Bit_Get(HasHit, i))
		{
			SendMsgF(playerid, COLOR_WHITE, "Username: %p (%i) - Placed By: %s - Reason: %s- Amount: "C_GREEN"$%i", i, i, hitPlacedBy[i],  hitReason[i], hitAmount[i]);
			++ count;
		}
	}
	if (count != 1) SendClientMessage(playerid, COLOR_WHITE, "There are no hits available");
	SendClientMessage(playerid, COLOR_GREEN, "______________________ "C_ORANGE"[ END ] "C_GREEN"______________________");
	return true;
}

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

	if (Bit_Get(HasHit, targetid))
		return SendErrorMsg(playerid, "The player you are placing hit on already have hit.");

	SetPlayerHitAmount(targetid, hitamount);
	Bit_Set(HasHit, targetid, true);

	new 
		name[MAX_PLAYER_NAME];
	GetPlayerName(playerid, name, MAX_PLAYER_NAME);

	strcpy(hitReason[targetid], reason, 32);
	strcpy(hitPlacedBy[targetid], name, MAX_PLAYER_NAME);

	GivePlayerMoney(playerid, -hitamount);

	SendMsgF(playerid, COLOR_ORANGE, "Hit: "C_WHITE"You successfully placed a hit on "C_GREY"%p (%i) "C_WHITE"for "C_GREEN"$%i"C_WHITE", reason: "C_GREY"%s", targetid, targetid, hitamount, reason);
	SendMsgAllF(COLOR_ORANGE, "Hit: "C_GREY"%p (%i) "C_WHITE"placed a hit on "C_GREY"%p (%i) "C_WHITE"for "C_GREEN"$%i"C_WHITE", reason: "C_GREY"%s", playerid, playerid, targetid, targetid, hitamount, reason);
	return true;
}