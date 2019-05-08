static
	cmd_Cooldown[MAX_PLAYERS],
	cmd_Count[MAX_PLAYERS];

public OnPlayerCommandReceived(playerid, cmd[]) {
	// if player is not spawned, do not allow him to use commands.
	if(!IsPlayerSpawned(playerid))
		return 0;

	// anti command spam
	if(gettime() < cmd_Cooldown[playerid]) {
		cmd_Count[playerid] ++;

		if(cmd_Count[playerid] >= 3) {
	 		SendServerMsgF(playerid, "Do not spam the command "C_GREY"'/%s' - count: %i", cmd, cmd_Count[playerid]);
	 		return 0;
		}
	}
	else {
		if(cmd_Count[playerid] > 0) {
			cmd_Count[playerid] --;
		}
	}

	//get time data everytime a command is recieved.
	cmd_Cooldown[playerid] = gettime() + 3;
	return 1;
}

public OnPlayerCommandPerformed(playerid, cmd[], success) {

    if (!success) {
    	return SendErrorMsgF(playerid, "The command ~r~'/%s' ~w~does not exist, try typing the command once again.", cmd);
	}
	
    //get time data everytime a command is performed.
    cmd_Cooldown[playerid] = gettime() + 3;
    return 1;
}

