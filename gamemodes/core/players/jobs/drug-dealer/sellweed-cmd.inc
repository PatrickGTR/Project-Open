#include <YSI_Coding\y_hooks>
#include <YSI_Data\y_bit>
#include <sscanf2>

static
	sellWeedCooldown[MAX_PLAYERS],
	BitArray:HasBeenOfferedWeed<MAX_PLAYERS>,
	weedSeller[MAX_PLAYERS],
	weedPrice[MAX_PLAYERS],
	weedAmount[MAX_PLAYERS]
;

hook OnPlayerConnect(playerid) {
	sellWeedCooldown[playerid] =
	weedPrice[playerid] = 
	weedAmount[playerid] = -1;
	weedSeller[playerid] = INVALID_PLAYER_ID;
	Bit_Set(HasBeenOfferedWeed, playerid, false);
    return 1;
}


CMD:sellweed(playerid, params[])
{
	if (GetPlayerJob(playerid) != TYPE_DRUGDEALER || GetPlayerClass(playerid) != TEAM_CIVILIAN)
		return SendErrorMsg(playerid, "Only drug dealers can use this command");

	new
		targetid,
		price,
		amount
	;

	if(sscanf(params, "rii", targetid, price, amount))
		return SendSyntaxMsg(playerid, "/sellweed <playerid> <price> <amount>"), SendSyntaxMsg(playerid, "1 = 10 grams, 2 = 20 grams.. and so on.");

	if(GetPlayerWeed(playerid) > amount)
		return SendErrorMsgF(playerid, "You only have %i weed on you and you're trying to sell %i!", GetPlayerWeed(playerid), amount);

	if(amount > 5)
		return SendErrorMsg(playerid, "You can only sell 50 grams at once");

	if(5000 > price > 20000)
		return SendErrorMsg(playerid, "The minimum price is $5,000 and maximum price is $20,000");

	if(GetPlayerWeed(targetid) >= MAX_WEED)
		return SendErrorMsg(playerid, "You can not sell weed to this player, (s)he has reached the maximum amount (s)he could hold.");

	if(gettime() < sellWeedCooldown[playerid])
		return SendErrorMsgF(playerid, "You've sold weed recently, please try in %i seconds!", gettime() - sellWeedCooldown[playerid]);

	sellWeedCooldown[playerid] = gettime() + 60;

	Bit_Set(HasBeenOfferedWeed, targetid, true);
	weedSeller[targetid]	= playerid;
	weedPrice[targetid] 	= price;
	weedAmount[targetid] 	= amount;

	SendMsgF(playerid, COLOR_GREEN, "** WEED **: "C_WHITE" You have offered "C_GREY"%p (%i) "C_WHITE"with "C_GREY"%i grams "C_WHITE"of weed for "C_GREEN"%m", playerid, playerid, amount * 10);
	SendMsgF(targetid, COLOR_GREEN, "** WEED **: "C_WHITE" Drug dealer "C_GREY"%p (%i) "C_WHITE"offered you "C_GREY"%i grams "C_WHITE"of weed, use "C_GREY"/buyweed to take his offer.", playerid, playerid, amount * 10);
	return 1;
}

CMD:buyweed(playerid, params[])
{
	if(GetPlayerClass(playerid) != TEAM_CIVILIAN)
		return SendErrorMsg(playerid, "Only Civilians can use this command");

	if(!Bit_Get(HasBeenOfferedWeed, playerid))
		return SendErrorMsg(playerid, "No one has offered you to buy weed.");

	if(GetPlayerWeed(playerid) >= MAX_WEED)
		return SendErrorMsg(playerid, "You are unable to buy weed, you've reached the maximum amount you can hold");

	if(GetPlayerMoney(playerid) < weedPrice[playerid])
		return SendErrorMsgF(playerid, "You don't have enough money to buy weed from "C_GREY"%p", playerid);

	Bit_Set(HasBeenOfferedWeed, playerid, false);
	GivePlayerMoney(playerid, -weedPrice[playerid]);
	GivePlayerMoney(weedSeller[playerid], weedPrice[playerid]);
	GivePlayerWeed(playerid, weedAmount[playerid]);

	SendMsgF(weedSeller[playerid], COLOR_GREEN, "** WEED **: "C_WHITE"%p (%i) accepted your offer, you earned "C_GREEN"$%m"C_WHITE" for selling "C_GREY"%i grams "C_WHITE"of weed.",
		playerid, playerid, weedPrice[playerid], weedAmount[playerid] * 10);

	SendMsgF(playerid, COLOR_GREEN, "** WEED **: "C_WHITE"You successfully bought "C_GREY"%i grams "C_WHITE"of weed for "C_GREEN"$%m"C_WHITE" from "C_GREY"%p(%i)",
		weedAmount[playerid] * 10, weedPrice[playerid], weedSeller[playerid], weedSeller[playerid]);
	return 1;
}