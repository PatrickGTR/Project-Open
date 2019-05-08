#include <YSI_Coding\y_hooks>
#include <YSI_Data\y_bit>
#include <sscanf2>

static 
    weaponPrice[MAX_PLAYERS],
    weaponID[MAX_PLAYERS],
    weaponAmmo[MAX_PLAYERS],
    weaponSeller[MAX_PLAYERS],
    BitArray:HasBeenOffered<MAX_PLAYERS>,
    sellWeaponCooldown[MAX_PLAYERS],
    sellArmourcooldown[MAX_PLAYERS]
;
	

hook OnPlayerConnect(playerid, reason) {
	sellWeaponCooldown[playerid] = 
	sellArmourcooldown[playerid] = 
    weaponID[playerid] = 
    weaponPrice[playerid] = 
    weaponAmmo[playerid] = -1;
    weaponSeller[playerid] = INVALID_PLAYER_ID;
	Bit_Set(HasBeenOffered, playerid, false);
}

/*

	[WEAPON DEALER COMMANDS]

*/

CMD:sellgun(playerid, params[])
{
	
	new 
		weaponid, targetid,weaponprice, weaponammo;

	if (GetPlayerJob(playerid) != TYPE_WEAPONDEALER || GetPlayerClass(playerid) != TEAM_CIVILIAN)
		return SendErrorMsg(playerid, "You are not allowed to use this command if you are not a weapon dealer.");

	if (gettime() - sellWeaponCooldown[playerid] < 40)
		return SendErrorMsgF(playerid, "You just sold a weapon, please wait %i seconds ", gettime() - sellWeaponCooldown[playerid]);

	if (sscanf(params, "riii", targetid, weaponid, weaponprice, weaponammo))
		return SendSyntaxMsg(playerid, "/sellweapon <playerid> <weaponid> <price> <ammo (max 1,000)>");

	if (!IsPlayerConnected(targetid))
		return SendErrorMsg(playerid, "The player you are trying to sell weapon is not connected.");

	if (!IsPlayerSpawned(targetid))
		return SendErrorMsg(playerid, "The player you are trying to sell weapon is not spawned.");

	if (!IsPlayerNearPlayer(playerid, targetid, 4.0))
		return SendErrorMsg(playerid, "The player you are trying to sell weapon is not near you.");

	if (targetid == playerid)
		return SendErrorMsg(playerid, "You can not sell a gun to your self.");

	if ( 1000 <= weaponprice < 8000)
		return SendErrorMsg(playerid, "Weapon price could not go lower than 1,000 and higher than 8,000");

	if (!(22 <= weaponid <= 34))
		return SendErrorMsg(playerid, "Invalid weapon id. Weapon id can only go up to 22 - 34");

	if (!(0 <= weaponammo <= 1000))
		return SendErrorMsg(playerid, "Invalid weapon ammo. Weapon ammo can only go up to 0 - 1000");

	Bit_Set(HasBeenOffered, targetid, true);
	weaponID[targetid] = weaponid;
	weaponPrice[targetid] = weaponprice;
	weaponAmmo[targetid] = weaponammo;
 	weaponSeller[targetid] = playerid;

	SendMsgF(playerid, COLOR_ORANGE, "** WEAPON **: "C_WHITE" You have offered %p (%i) a %W for "C_GREEN"$%i"C_WHITE", wish you luck that he'll accept it!", targetid, targetid, weaponid, weaponprice);
	SendMsgF(targetid, COLOR_ORANGE, "** WEAPON **: "C_WHITE" Weapon dealer "C_GREY"%p (%i) "C_WHITE"offered you a %W for "C_GREEN"$%i"C_WHITE", /acceptgun to accept his offer.", playerid, playerid, weaponid, weaponprice);
	
	sellWeaponCooldown[playerid] = gettime();
	return true;
}

CMD:acceptgun(playerid, params[])
{
	if (HasBeenOffered[playerid])
		return SendErrorMsg(playerid, "No one offered you a gun.");

	if (GetPlayerMoney( playerid ) < weaponPrice[playerid])
		return SendErrorMsg(playerid, "You do not have enough money to buy the weapon.");

	GivePlayerMoney(playerid, -weaponPrice[playerid]);
	GivePlayerMoney(weaponSeller[playerid], weaponPrice[playerid]);

	GivePlayerWeapon(playerid, weaponID[playerid], weaponAmmo[playerid]);

	SendMsgF(weaponSeller[playerid], COLOR_ORANGE, "** WEAPON **: "C_WHITE"%p (%i) accepted your offer, today you earned "C_GREEN"$%i"C_WHITE" for selling guns.", playerid, playerid, weaponPrice[playerid]);
	SendMsgF(playerid, COLOR_ORANGE, "** WEAPON **: "C_WHITE"You "C_GREEN"successfully "C_WHITE"bought a %W for "C_GREEN"$%i"C_WHITE" from "C_GREY"%p(%i)", weaponID[playerid], weaponPrice[playerid], 
	weaponSeller[playerid], weaponSeller[playerid]);

	Bit_Set(HasBeenOffered, playerid, false), 
	weaponID[playerid] = -1, 
	weaponPrice[playerid] = -1, 
	weaponAmmo[playerid] = -1, 
	weaponSeller[playerid] = INVALID_PLAYER_ID;
	return true;
}

CMD:sellarmour(playerid, params[])
{
	new 
		targetid, armourprice, armouramount; 

	if (GetPlayerJob(playerid) != TYPE_WEAPONDEALER || GetPlayerClass(playerid) != TEAM_CIVILIAN)
		return SendErrorMsg(playerid, "You are not allowed to use this command if you are not a weapon dealer.");

	if (gettime() - sellArmourcooldown[playerid] < 40)
		return SendErrorMsgF(playerid, "You just sold a weapon, please wait %i seconds ", gettime() - sellArmourcooldown[playerid]);

	if (sscanf(params, "rii", targetid, armourprice, armouramount))
		return SendSyntaxMsg(playerid, "/sellarmour <playerid> <Price> <Amount>");

	if (!IsPlayerConnected(targetid))
		return SendErrorMsg(playerid, "The player you are trying to sell weapon is not connected.");

	if (!IsPlayerSpawned(targetid))
		return SendErrorMsg(playerid, "The player you are trying to sell weapon is not spawned.");

	if (!IsPlayerNearPlayer(playerid, targetid, 4.0))
		return SendErrorMsg(playerid, "The player you are trying to sell armour is not near you.");

	if (targetid == playerid)
		return SendErrorMsg(playerid, "You can not sell an armour to your self.");

	if (!(0 <= armouramount <= 100))
		return SendErrorMsg(playerid, "Invalid armour amount. armour can only go up to 1 - 100"); 

	if (RetPlayerA(targetid) >= 80.0)
		return SendErrorMsgF(playerid, "The player you are trying to sell armour has got enough (%.1f).", RetPlayerA(targetid));

	Bit_Set(HasBeenOffered, targetid, true);
	weaponPrice[targetid] = armourprice, 
	weaponAmmo[targetid] = armouramount, 
	weaponSeller[targetid] = playerid;

	SendMsgF(playerid, COLOR_ORANGE, "** WEAPON **: "C_WHITE" You have offered "C_GREY"%p (%i) "C_WHITE"an armour(%i) for "C_GREEN"$%i"C_WHITE", wish you luck that he'll accept it!", targetid, targetid, armouramount, armourprice);
	SendMsgF(targetid, COLOR_ORANGE, "** WEAPON **: "C_WHITE" Weapon dealer "C_GREY"%p (%i) "C_WHITE"offered you an armour(%i) for "C_GREEN"$%i"C_WHITE", /acceptarmour to accept his offer.", playerid, playerid, armouramount, armourprice);
	
	sellArmourcooldown[playerid] = gettime();
	return true;
}

CMD:acceptarmour(playerid, params[])
{
	if (HasBeenOffered[playerid])
		return SendErrorMsg(playerid, "No one offered you armour.");

	if (GetPlayerMoney( playerid ) < weaponPrice[playerid])
		return SendErrorMsg(playerid, "You do not have enough money to buy the weapon.");

	GivePlayerMoney(playerid, -weaponPrice[playerid] );
	GivePlayerMoney(weaponSeller[playerid], weaponPrice[playerid]);

	new 
		Float:ply_armour;

	GetPlayerArmour(playerid, ply_armour);

	SetPlayerArmour(playerid, ply_armour + weaponAmmo[playerid]);
	

	SendMsgF(weaponSeller[playerid], COLOR_ORANGE, "** WEAPON **: "C_WHITE"%p (%i) accepted your offer, today you earned "C_GREEN"$%i"C_WHITE" for selling an armour.", playerid, playerid, weaponPrice[playerid]);
	SendMsgF(playerid, COLOR_ORANGE, "** WEAPON **: "C_WHITE"You "C_GREEN"successfully "C_WHITE"bought an armour(%i) for "C_GREEN"$%i"C_WHITE" from "C_GREY"%p(%i)", weaponAmmo[playerid], weaponPrice[playerid], 
	weaponSeller[playerid], weaponSeller[playerid]);

	Bit_Set(HasBeenOffered, playerid, false), 
	weaponPrice[playerid] = -1, 
	weaponAmmo[playerid] = -1, 
	weaponSeller[playerid] = INVALID_PLAYER_ID;
	return true;
}
