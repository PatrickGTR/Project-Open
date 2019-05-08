#include <YSI_Coding\y_hooks>
#include <strlib>
#include <progress2>

static 
	  Text: exp_Ball1[MAX_PLAYERS],
	  Text: exp_Ball2[MAX_PLAYERS],
	  Text: exp_PlayerCurrentLevel[MAX_PLAYERS],
	  Text: exp_PlayerNextLevel[MAX_PLAYERS],
PlayerText: exp_PlayerTotal[MAX_PLAYERS],
 PlayerBar: exp_Bar[MAX_PLAYERS]
;

hook OnGameModeInit()
{
	for(new i = 0; i < MAX_PLAYERS; i++)
	{
		exp_Ball1[i] = TextDrawCreate(189.000000, 105.000000, "LD_POOL:ball");
		TextDrawLetterSize           (exp_Ball1[i], 0.000000, 0.000000);
		TextDrawTextSize             (exp_Ball1[i], 18.000000, 21.000000);
		TextDrawAlignment            (exp_Ball1[i], 1);
		TextDrawColor                (exp_Ball1[i], -1347440641);
		TextDrawSetShadow            (exp_Ball1[i], 0);
		TextDrawSetOutline           (exp_Ball1[i], 0);
		TextDrawBackgroundColor      (exp_Ball1[i], 255);
		TextDrawFont                 (exp_Ball1[i], 4);
		TextDrawSetProportional      (exp_Ball1[i], 0);
		TextDrawSetShadow            (exp_Ball1[i], 0);

		exp_Ball2[i] = TextDrawCreate(420.000000, 105.000000, "LD_POOL:ball");
		TextDrawLetterSize           (exp_Ball2[i], 0.000000, 0.000000);
		TextDrawTextSize             (exp_Ball2[i], 18.000000, 21.000000);
		TextDrawAlignment            (exp_Ball2[i], 1);
		TextDrawColor                (exp_Ball2[i], -1347440641);
		TextDrawSetShadow            (exp_Ball2[i], 0);
		TextDrawSetOutline           (exp_Ball2[i], 0);
		TextDrawBackgroundColor      (exp_Ball2[i], 255);
		TextDrawFont                 (exp_Ball2[i], 4);
		TextDrawSetProportional      (exp_Ball2[i], 0);
		TextDrawSetShadow            (exp_Ball2[i], 0);

		exp_PlayerCurrentLevel[i] = TextDrawCreate	(194.500000, 107.000000, "1");
		TextDrawLetterSize                        	(exp_PlayerCurrentLevel[i], 0.400000, 1.600000);
		TextDrawAlignment                         	(exp_PlayerCurrentLevel[i], 1);
		TextDrawColor                             	(exp_PlayerCurrentLevel[i], -1378294017);
		TextDrawSetShadow                         	(exp_PlayerCurrentLevel[i], 0);
		TextDrawSetOutline                        	(exp_PlayerCurrentLevel[i], 1);
		TextDrawBackgroundColor                   	(exp_PlayerCurrentLevel[i], 255);
		TextDrawFont                              	(exp_PlayerCurrentLevel[i], 3);
		TextDrawSetProportional                   	(exp_PlayerCurrentLevel[i], 1);
		TextDrawSetShadow                         	(exp_PlayerCurrentLevel[i], 0);

		exp_PlayerNextLevel[i] = TextDrawCreate		(425.500000, 107.200012, "2");
		TextDrawLetterSize                     		(exp_PlayerNextLevel[i], 0.400000, 1.600000);
		TextDrawAlignment                      		(exp_PlayerNextLevel[i], 1);
		TextDrawColor                          		(exp_PlayerNextLevel[i], -1378294017);
		TextDrawSetShadow                      		(exp_PlayerNextLevel[i], 0);
		TextDrawSetOutline                     		(exp_PlayerNextLevel[i], 1);
		TextDrawBackgroundColor                		(exp_PlayerNextLevel[i], 255);
		TextDrawFont                           		(exp_PlayerNextLevel[i], 3);
		TextDrawSetProportional                		(exp_PlayerNextLevel[i], 1);
		TextDrawSetShadow                      		(exp_PlayerNextLevel[i], 0);
	}
}

hook OnPlayerConnect(playerid)
{
	exp_PlayerTotal[playerid] = CreatePlayerTextDraw(playerid, 320.000000, 121.000000, "300 / 1000");
	PlayerTextDrawLetterSize                        (playerid, exp_PlayerTotal[playerid], 0.500000, 1.600000);
	PlayerTextDrawAlignment                         (playerid, exp_PlayerTotal[playerid], 2);
	PlayerTextDrawColor                             (playerid, exp_PlayerTotal[playerid], -1378294017);
	PlayerTextDrawSetShadow                         (playerid, exp_PlayerTotal[playerid], 0);
	PlayerTextDrawSetOutline                        (playerid, exp_PlayerTotal[playerid], 1);
	PlayerTextDrawBackgroundColor                   (playerid, exp_PlayerTotal[playerid], 255);
	PlayerTextDrawFont                              (playerid, exp_PlayerTotal[playerid], 3);
	PlayerTextDrawSetProportional                   (playerid, exp_PlayerTotal[playerid], 1);
	PlayerTextDrawSetShadow                         (playerid, exp_PlayerTotal[playerid], 0);

	exp_Bar[playerid] = CreatePlayerProgressBar(playerid, 212.000000, 111.000000, 208.500000, 9.699998, -1378294017, 1000.0000, 0);
}

ShowPlayerAllXPTD(playerid)
{
	TextDrawShowForPlayer(playerid, exp_Ball1[playerid]);
	TextDrawShowForPlayer(playerid, exp_Ball2[playerid]);
	TextDrawShowForPlayer(playerid, exp_PlayerCurrentLevel[playerid]);
	TextDrawShowForPlayer(playerid, exp_PlayerNextLevel[playerid]);

	PlayerTextDrawShow(playerid, exp_PlayerTotal[playerid]);

	ShowPlayerProgressBar(playerid, exp_Bar[playerid]);
	return 1;
}

HidePlayerAllXPTD(playerid)
{
	TextDrawHideForPlayer(playerid, exp_Ball1[playerid]);
	TextDrawHideForPlayer(playerid, exp_Ball2[playerid]);
	TextDrawHideForPlayer(playerid, exp_PlayerCurrentLevel[playerid]);
	TextDrawHideForPlayer(playerid, exp_PlayerNextLevel[playerid]);

	PlayerTextDrawHide(playerid, exp_PlayerTotal[playerid]);

	HidePlayerProgressBar(playerid, exp_Bar[playerid]);
	return 1;
}

UpdatePlayerXPBar(playerid, amount) {
	return SetPlayerProgressBarValue(playerid, exp_Bar[playerid], GetPlayerProgressBarValue(playerid, exp_Bar[playerid]) + amount);
}

SetPlayerTotalXPTD(playerid, current, total) {
	return PlayerTextDrawSetString(playerid, exp_PlayerTotal[playerid], sprintf("%i / %i", current, total));
}

SetPlayerCurrentLevelTD(playerid, current) {
	return TextDrawSetString(exp_PlayerCurrentLevel[playerid], sprintf("%i", current));
}

SetPlayerNextLevelTD(playerid, next_level) {
	return TextDrawSetString(exp_PlayerCurrentLevel[playerid], sprintf("%i", next_level));
}