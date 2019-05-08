#include <YSI_Coding\y_hooks>
#include <strlib>

static
	ply_currentXP[MAX_PLAYERS];

SetPlayerXP(playerid, amount, bool:first_spawn = false)
{
	ply_currentXP[playerid] = amount;

	if(!first_spawn)
	{
		ShowPlayerAllXPTD(playerid);

		SetPlayerTotalXPTD(playerid, GetPlayerXP(playerid), 1000);
		SetPlayerCurrentLevelTD(playerid, GetPlayerScore(playerid));
		SetPlayerNextLevelTD(playerid, GetPlayerScore(playerid) + 1);
		UpdatePlayerXPBar(playerid, GetPlayerXP(playerid) + amount);

		if(GetPlayerXP(playerid) >= 1000)
		{
			ply_currentXP[playerid] = 0;
			GivePlayerScore(playerid, 1);
		}

		defer HidePlayerXPTD(playerid);
	}

	mysql_tquery(GetSQLHandle(), sprintf("UPDATE "T_ACCOUNT" SET "F_EXP" = %i WHERE "F_ACCOUNTID" = %i", amount, GetPlayerAccountID(playerid)));
	return 1;
}

timer HidePlayerXPTD[3000](playerid)
{
	HidePlayerAllXPTD(playerid);
}

GivePlayerXP(playerid, amount)
	return SetPlayerXP(playerid, GetPlayerXP(playerid) + amount);

GetPlayerXP(playerid)
	return ply_currentXP[playerid];