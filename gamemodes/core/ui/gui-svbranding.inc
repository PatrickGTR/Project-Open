


#include <YSI_Coding\y_hooks>


new 
	Text:gTD_Main_UPPER, 
	Text:gTD_Main_LOWER, 
	Text:gTD_Main_BUILD;

hook OnGameModeInit()
{
	gTD_Main_UPPER = TextDrawCreate 	(639.0, 411.0, "San Fierro");
	TextDrawLetterSize 					(gTD_Main_UPPER, 0.800000, 2.500000);
	TextDrawAlignment 					(gTD_Main_UPPER, 3);
	TextDrawColor 						(gTD_Main_UPPER, -1);
	TextDrawSetShadow 					(gTD_Main_UPPER, 0);
	TextDrawSetOutline 					(gTD_Main_UPPER, 1);
	TextDrawBackgroundColor 			(gTD_Main_UPPER, TD_OUTLINE_COLOUR);
	TextDrawFont 						(gTD_Main_UPPER, 3);
	TextDrawSetProportional 			(gTD_Main_UPPER, 1);
	TextDrawSetShadow 					(gTD_Main_UPPER, 0);
	

	gTD_Main_LOWER = TextDrawCreate 	(632.0, 423.5, "~b~Cops ~w~and ~r~Robbers");
	TextDrawLetterSize 					(gTD_Main_LOWER, 0.600000, 2.500000);
	TextDrawAlignment 					(gTD_Main_LOWER, 3);
	TextDrawColor 						(gTD_Main_LOWER, -1);
	TextDrawSetShadow 					(gTD_Main_LOWER, 1);
	TextDrawSetOutline 					(gTD_Main_LOWER, 1);
	TextDrawBackgroundColor 			(gTD_Main_LOWER, TD_OUTLINE_COLOUR);
	TextDrawFont 						(gTD_Main_LOWER, 0);
	TextDrawSetProportional 			(gTD_Main_LOWER, 1);
	TextDrawSetShadow 					(gTD_Main_LOWER, 1);
	
	gTD_Main_BUILD = TextDrawCreate 	(564.0, 402.0, "build_version_"SV_CURRENT_BUILD"");
	TextDrawLetterSize 					(gTD_Main_BUILD, 0.217777, 1.522666);
	TextDrawAlignment 					(gTD_Main_BUILD, 2);
	TextDrawColor 						(gTD_Main_BUILD, -1);
	TextDrawSetShadow 					(gTD_Main_BUILD, 0);
	TextDrawSetOutline 					(gTD_Main_BUILD, 1);
	TextDrawBackgroundColor 			(gTD_Main_BUILD, TD_OUTLINE_COLOUR);
	TextDrawFont 						(gTD_Main_BUILD, 2);
	TextDrawSetProportional 			(gTD_Main_BUILD, 1);
	TextDrawSetShadow 					(gTD_Main_BUILD, 0);
	return 1;
}

hook OnGameModeExit() {
	TextDrawDestroy(gTD_Main_UPPER);
	TextDrawDestroy(gTD_Main_LOWER);
	TextDrawDestroy(gTD_Main_BUILD);
	return 1;
}

hook OnPlayerSpawn(playerid) {
	TextDrawShowForPlayer(playerid, gTD_Main_UPPER);
	TextDrawShowForPlayer(playerid, gTD_Main_LOWER);
	TextDrawShowForPlayer(playerid, gTD_Main_BUILD);
	return 1;
}

hook OnPlayerDeath(playerid, killerid, reason) {
	TextDrawHideForPlayer(playerid, gTD_Main_UPPER);
	TextDrawHideForPlayer(playerid, gTD_Main_LOWER);
	TextDrawHideForPlayer(playerid, gTD_Main_BUILD);
	return 1;
}

hook OnPlayerDisconnect(playerid, reason)
{
	TextDrawHideForPlayer(playerid, gTD_Main_UPPER);
	TextDrawHideForPlayer(playerid, gTD_Main_LOWER);
	TextDrawHideForPlayer(playerid, gTD_Main_BUILD);
	return 1;
}
