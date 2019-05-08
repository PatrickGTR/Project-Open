#include <YSI_Coding\y_hooks>
#include <YSI_Data\y_bit>
#include <sscanf2>

static
	sellCrackCooldown[MAX_PLAYERS],
	BitArray:HasBeenOfferedCrack<MAX_PLAYERS>,
	crackSeller[MAX_PLAYERS],
	crackPrice[MAX_PLAYERS],
	crackAmount[MAX_PLAYERS]
;


hook OnPlayerConnect(playerid) {
	sellCrackCooldown[playerid] =
	crackPrice[playerid] = 
	crackAmount[playerid] = -1;
	crackSeller[playerid] = INVALID_PLAYER_ID;
	Bit_Set(HasBeenOfferedCrack, playerid, false);
    return 1;
}


CMD:sellcrack(playerid, params[])
{
	if (GetPlayerJob(playerid) != TYPE_DRUGDEALER || GetPlayerClass(playerid) != TEAM_CIVILIAN)
		return SendErrorMsg(playerid, "Only drug dealers can use this command");

	new
		targetid,
		price,
		amount;

	if(sscanf(params, "rii", targetid, price, amount))
		return SendSyntaxMsg(playerid, "/sellweed <playerid> <price> <amount>"), SendSyntaxMsg(playerid, "1 = 10 grams, 2 = 20 grams.. and so on.");

	if(GetPlayerCrack(playerid) > amount)
		return SendErrorMsgF(playerid, "You only have %i crack on you and you're trying to sell %i!", GetPlayerCrack(playerid), amount);

	if(amount > 5)
		return SendErrorMsg(playerid, "You can only sell 50 grams at once");

	if(5000 > price > 20000)
		return SendErrorMsg(playerid, "The minimum price is $5,000 and maximum price is $20,000");

	if(GetPlayerCrack(targetid) >= MAX_CRACK)
		return SendErrorMsg(playerid, "You can not sell crack to this player, (s)he has reached the maximum amount (s)he could hold.");

	if(gettime() < sellCrackCooldown[playerid])
		return SendErrorMsgF(playerid, "sold crack recently, please try in %i seconds!", gettime() - sellCrackCooldown[playerid]);

	sellCrackCooldown[playerid] = gettime() + 60;

	Bit_Set(HasBeenOfferedCrack, targetid, true);
	crackSeller[targetid]	= playerid;
	crackPrice[targetid] 	= price;
	crackAmount[targetid] 	= amount;

	SendMsgF(playerid, COLOR_GREEN, "** CRACK **: "C_WHITE" You have offered "C_GREY"%p (%i) "C_WHITE"with "C_GREY"%i grams "C_WHITE"of crack for "C_GREEN"%m", playerid, playerid, amount * 10);
	SendMsgF(targetid, COLOR_GREEN, "** CRACK **: "C_WHITE" Drug dealer "C_GREY"%p (%i) "C_WHITE"offered you "C_GREY"%i grams "C_WHITE"of crack, use "C_GREY"/buycrack to take his offer.", playerid, playerid, amount * 10);
	return 1;
}

CMD:buycrack(playerid, params[])
{
	if(GetPlayerClass(playerid) != TEAM_CIVILIAN)
		return SendErrorMsg(playerid, "Only Civilians can use this command");

	if(!Bit_Get(HasBeenOfferedCrack, playerid))
		return SendErrorMsg(playerid, "No one has offered you to buy crack.");

	if(GetPlayerCrack(playerid) >= MAX_CRACK)
		return SendErrorMsg(playerid, "You are unable to buy crack, you've reached the maximum amount you can hold");

	if(GetPlayerMoney(playerid) < crackPrice[playerid])
		return SendErrorMsgF(playerid, "You don'tt have enough money to buy crack from "C_GREY"%p", playerid);

	Bit_Set(HasBeenOfferedCrack, playerid, false);
	GivePlayerMoney(playerid, -crackPrice[playerid]);
	GivePlayerMoney(crackSeller[playerid], crackPrice[playerid]);
	GivePlayerCrack(playerid, crackAmount[playerid]);

	SendMsgF(crackSeller[playerid], COLOR_GREEN, "** CRACK **: "C_WHITE"%p (%i) accepted your offer, you earned "C_GREEN"$%m"C_WHITE" for selling "C_GREY"%i grams "C_WHITE"of crack.",
		playerid, playerid, crackPrice[playerid], crackAmount[playerid] * 10);

	SendMsgF(playerid, COLOR_GREEN, "** CRACK **: "C_WHITE"You successfully bought "C_GREY"%i grams "C_WHITE"of crack for "C_GREEN"$%m"C_WHITE" from "C_GREY"%p(%i)",
		crackAmount[playerid] * 10, crackPrice[playerid], crackSeller[playerid], crackSeller[playerid]);
	return 1;
}