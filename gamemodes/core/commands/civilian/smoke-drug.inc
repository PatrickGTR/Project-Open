



static 
	smokeWeedCooldown[MAX_PLAYERS],
	smokeCrackCooldown[MAX_PLAYERS]
;

CMD:smokeweed(playerid, params[])
{
	if(GetPlayerCrack(playerid) < 1)
		return SendErrorMsg(playerid, "You don't have enough weed to smoke");

	if(gettime() < smokeWeedCooldown[playerid])
		return SendErrorMsg(playerid, "You just smoked weed recently, please try again later!");

	new
		Float:hp
	;

	GetPlayerHealth(playerid, hp);
	
	if(hp > 80)
		return SendErrorMsg(playerid, "You can not smoke weed if your hp is above 80");

	smokeWeedCooldown[playerid] = gettime() + 2;

	SetPlayerHealth(playerid, 100);
	SetPlayerWeed(playerid, GetPlayerWeed(playerid) - 1);
	SendMsgF(playerid, COLOR_PURPLE, "** SMOKED ** "C_WHITE"You have smoked 10 grams of weed, you have "C_GREY"%i grams "C_WHITE"left.", GetPlayerWeed(playerid));
	return 1;
}

CMD:smokecrack(playerid, params[])
{
	if(GetPlayerCrack(playerid) < 1)
		return SendErrorMsg(playerid, "You don't have enough crack to smoke");

	if(gettime() < smokeCrackCooldown[playerid])
		return SendErrorMsg(playerid, "You just smoked weed recently, please try again later!");
	
	if(RetPlayerA(playerid) > 60)
		return SendErrorMsg(playerid, "You can not smoke crack if your armour is above 60");

	smokeCrackCooldown[playerid] = gettime() + 2;

	SetPlayerArmour(playerid, 100);
	SetPlayerCrack(playerid, GetPlayerCrack(playerid) - 1);
	SendMsgF(playerid, COLOR_PURPLE, "** SMOKED ** "C_WHITE"You have smoked 10 grams of crack, you have "C_GREY"%i grams "C_WHITE"left.", GetPlayerCrack(playerid));
	return 1;
}