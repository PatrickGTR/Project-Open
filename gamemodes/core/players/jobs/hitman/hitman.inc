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

hook OnPlayerDeath(playerid, killerid, reason) {
    if (killerid != INVALID_PLAYER_ID) {      
        if (IsHitPlayer(playerid) && GetPlayerJob(killerid) == TYPE_HITMAN) {
            SendMsgAllF(COLOR_ORANGE, "** HIT **: "C_GREY"%p (%i) "C_WHITE"has completed the hit on "C_GREY"%p (%i) "C_WHITE"and recieved "C_GREEN"$%i.", killerid, killerid, playerid, playerid, GetPlayerHitAmount(playerid));
            
            GivePlayerMoney(killerid, GetPlayerHitAmount(playerid));

			ResetPlayerHitStatus(playerid);

        }
        if (IsHitPlayer(playerid) && GetPlayerJob(killerid) != TYPE_HITMAN) {
            SendMsgAllF(COLOR_ORANGE, "** HIT **: "C_WHITE"Contract on "C_GREY"%p (%i) "C_WHITE"has been cancelled, player died.", playerid, playerid);

			ResetPlayerHitStatus(playerid);
        }
    }
	return 1;
}

hook OnPlayerDisconnect(playerid, reason) {
    if (IsHitPlayer(playerid)) {
        SendMsgAllF(COLOR_ORANGE, "** HIT **: "C_WHITE"Contract on "C_GREY"%p (%i) "C_WHITE"has been cancelled, player left.", playerid, playerid);
    }
	return 1;
}

ResetPlayerHitStatus(playerid) {

	if(!IsValidPlayerID(playerid)) {
		return 0;
	}

	SetHitPlayer(playerid, false);
	hitAmount[playerid] = -1;
	hitPlacedBy[playerid][0] =
	hitReason[playerid][0] = EOS;
	return 1;
}

GetPlayerHitData(playerid, &hitamount, placedby[], reason[]) {
	hitamount = hitAmount[playerid];
	strcpy(reason, hitReason[playerid], 32);
	strcpy(placedby, hitPlacedBy[playerid], MAX_PLAYER_NAME);
}

SetHitPlayer(playerid, bool: yes_no) {
	Bit_Set(HasHit, playerid, yes_no);
}

IsHitPlayer(playerid) {
	return Bit_Get(HasHit, playerid);
}

SetPlayerHitReason(playerid, const reason[], const placedby[]) {
	if(!IsValidPlayerID(playerid)) {
		return 0;
	}
	if(strlen(reason) > 32) {
		return 0;
	}
	
	strcpy(hitReason[playerid], reason, 32);
	strcpy(hitPlacedBy[playerid], placedby, MAX_PLAYER_NAME);
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


