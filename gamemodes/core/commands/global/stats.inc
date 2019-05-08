#include <YSI_Visual\y_dialog>
#include <strlib>

CMD:stats(playerid, params[])
{
	new
		string[700];

	strcat(string, ""C_GREEN"Account Statistics\n"C_WHITE"");
	strcat(string, sprintf("Account ID: #%i\n", GetPlayerAccountID(playerid)));
	strcat(string, sprintf("Admin Status: %s\n", GetAdminRankName(playerid)));
	strcat(string, sprintf("Current Job: %s\n", GetPlayerJobName(playerid)));
	
	new 
		playerRegisterDate[40];
	GetPlayerRegisterDate(playerid, playerRegisterDate, sizeof(playerRegisterDate));
	strcat(string, sprintf("Register Date: %s\n", playerRegisterDate));

	strcat(string, "\n"C_GREEN"In-Game Statistics\n"C_WHITE"");
	strcat(string, sprintf("Money: %m\n", GetPlayerMoney(playerid)));
	strcat(string, sprintf("Score: %i\n", GetPlayerScore(playerid)));
	strcat(string, sprintf("Kills: %i - Deaths: %i - K/D Ratio: %0.2f\n", GetPlayerKills(playerid), GetPlayerDeaths(playerid), floatdiv(GetPlayerKills(playerid), GetPlayerDeaths(playerid))));

	strcat(string, "\n"C_GREEN"Skill Statistics\t\t\tAmount\n"C_WHITE"");
	strcat(string, sprintf("Players Raped: \t\t\tx %i\n", GetPlayerTotalRape(playerid)));
	strcat(string, sprintf("Been Raped: \t\t\tx %i\n", GetPlayerTotalBeenRape(playerid)));
	strcat(string, sprintf("Players Robbed: \t\tx %i\n", GetPlayerTotalRob(playerid)));
	strcat(string, sprintf("Been Robbed: \t\t\tx %i\n", GetPlayerTotalBeenRob(playerid)));
	strcat(string, sprintf("Players Arrested: \t\tx %i\n", GetPlayerTotalArrest(playerid)));
	strcat(string, sprintf("Been Arrested: \t\t\tx %i\n", GetPlayerTotalBeenArrest(playerid)));
	strcat(string, sprintf("Store Robbed: \t\t\tx %i\n", GetPlayerTotalStoreRob(playerid)));
	strcat(string, sprintf("Cuff Escapes: \t\t\tx %i\n", GetPlayerTotalCuffEscape(playerid)));

	strcat(string, "\n"C_GREEN"Item Statistics\t\t\tAmount\n"C_WHITE"");
	strcat(string, sprintf("Weed: \t\t\t\t%i Grams\n", GetPlayerWeed(playerid)));
	strcat(string, sprintf("Crack: \t\t\t\t%i Grams\n", GetPlayerCrack(playerid)));
	strcat(string, sprintf("Rope: \t\t\t\tx %i\n", GetPlayerRope(playerid)));
	strcat(string, sprintf("Lockpick: \t\t\tx %i\n", GetPlayerPickLock(playerid)));
	strcat(string, sprintf("Scissors: \t\t\tx %i\n", GetPlayerScissors(playerid)));
	strcat(string, sprintf("Condom: \t\t\tx %i\n", GetPlayerCondom(playerid)));
	strcat(string, sprintf("Wallet: \t\t\t\tx %i\n", GetPlayerWallet(playerid)));
	strcat(string, sprintf("C4: \t\t\t\tx %i\n", GetPlayerC4(playerid)));

	Dialog_Show(playerid, DIALOG_STYLE_MSGBOX, "Your Statistics", string, "Close", "");
	return 1;
}