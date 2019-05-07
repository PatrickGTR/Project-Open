static
    chat_Cooldown[MAX_PLAYERS],
    chat_Count[MAX_PLAYERS],
    chat_LastMessage[MAX_PLAYERS][144];

public OnPlayerText(playerid, text[])
{
    /*
        If the player is muted, don't allow
        him/her to send messsage to the server
    */
    if (IsPlayerMuted(playerid)) {
        SendErrorMsgF(playerid, "You can not speak in global chat, you have been muted for ~r~%i ~w~seconds!", GetPlayerMuteDuration(playerid));
        return 0;
    }

    if(gettime() < chat_Cooldown[playerid]) {
        chat_Count[playerid] ++;

        if(3 <= chat_Count[playerid] < 10) {
            SendServerMsg(playerid, "Do not spam the chat, or you'll be muted!");
            return 0;
        }
        if(chat_Count[playerid] >= 10) {
            chat_Count[playerid] = 0;
            TogglePlayerMute(playerid, true, 60);
            SendServerMsgF(playerid, "You have been muted for "C_GREY"%i "C_WHITE"seconds for spamming the chat", GetPlayerMuteDuration(playerid));
            return 0;
        }
    }
    else {
        if(chat_Count[playerid] > 0) {
            chat_Count[playerid] --;
        }
    }


    new
        processedMessage[144];

    ExpandMentions(text, processedMessage, sizeof(processedMessage), -1);

    /*
        If player sends the exact same message twice.
    */
    if (!isnull(chat_LastMessage[playerid]) && !strcmp(chat_LastMessage[playerid], processedMessage, true, 144)) {
        SendErrorMsg(playerid, "Do not repeat the same message.");
        return 0;
    }
    strcpy(chat_LastMessage[playerid], processedMessage, 144);

    // Normal Global Message
    SendMsgAllF(GetPlayerColor(playerid), "[%i] %p "C_WHITE": %s", playerid, playerid, processedMessage);

    chat_Cooldown[playerid] = gettime() + 2;
    return 0;
}