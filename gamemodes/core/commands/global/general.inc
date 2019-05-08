#include <YSI_Visual\y_dialog>

CMD:eject(playerid, params[])
{
    if (isnull(params) || !IsNumeric(params))
        return SendSyntaxMsg(playerid, "/eject <playerid>");

    new 
        targetid = strval(params);

    if (!IsPlayerConnected(targetid))
        return SendErrorMsg(playerid, "The player you are trying to eject from your vehicle is not connected.");

    if (!IsPlayerSpawned(targetid))
        return SendErrorMsg(playerid, "The player you are trying to eject from your vehicle is not spawned.");

    if (!IsPlayerInVehicle(targetid, GetPlayerVehicleID(playerid)))
        return SendErrorMsg(playerid, "The player you are trying to eject from your vehicle is not inside your vehicle.");

    SendMsgF(playerid, COLOR_RED, "** EJECT ** "C_WHITE"You ejected "C_GREY"%p (%i) "C_WHITE"from your vehicle.", targetid, targetid);
    SendMsgF(playerid, COLOR_RED, "** EJECT ** "C_WHITE"You have been ejected by"C_GREY"%p (%i) "C_WHITE"from his vehicle.", playerid, playerid);
    RemovePlayerFromVehicle(targetid);
    return true;
}

CMD:cmds(playerid, params[])
{
    inline DialogCommandsResponse(pid, dialogid, response, listitem, string:inputtext[])
    {
        #pragma unused dialogid, pid, inputtext
        if (!response)
            return false;

        switch(listitem)
        {
            case 0: ShowCommands(playerid, 0);
            case 1: ShowCommands(playerid, 1);
            case 2: ShowCommands(playerid, 2);
            case 3:
            {
                Dialog_Show(playerid, DIALOG_STYLE_MSGBOX, "House Commands", "\
                        "C_GREY"/house buy "C_WHITE"- Purchase a house.\n\
                        "C_GREY"/house enter "C_WHITE"- Enter a house of yours.\n\
                        "C_GREY"/house exit "C_WHITE"- Exit a house of yours.\n\
                        "C_GREY"/house description "C_WHITE"- Change house description text.\n\
                        "C_GREY"/house upgrade "C_WHITE"- Upgrade your house to a better interior.\n\
                        "C_GREY"/house furniture "C_WHITE"- Choices to buy, edit & delete a furniture.", "Close", "");
            }
            case 4:
            {
                    Dialog_Show(playerid, DIALOG_STYLE_MSGBOX, "Attachment Commands", "\
                        "C_GREY"/attachment buy "C_WHITE"- Purchase a attachment.\n\
                        "C_GREY"/attachment delete "C_WHITE"- Delete an attachment.\n\
                        "C_GREY"/attachment edit "C_WHITE"- Edit an attachment.\n\
                        "C_GREY"/attachment view "C_WHITE"- List all the attachment you own.\n\
                        "C_GREY"/attachment attach "C_WHITE"- Attach an attachment you own.\n\
                        "C_GREY"/attachment detach "C_WHITE"- Detach an attachment you own.\n", "Close", "");
            }
        }
    }
    Dialog_ShowCallback(playerid, using inline DialogCommandsResponse, DIALOG_STYLE_LIST, "Server Commands", "General Commands\nTeam Commands\nClass Commands\nHouse Commands\nAttachment Commands", "Select", "Close");
    return 1;
}

ShowCommands(playerid, type)
{
    new 
        string[1024];
    switch(type)
    {
        case 0:
        {
            strcat(string, ""C_ORANGE"Bought items from 24/7\n\
                "C_GREY"/breakcuff "C_WHITE"- Escape from cuffs using a picklock.\n\
                "C_GREY"/breakout "C_WHITE"- Escape from jail using a picklock.\n\
                "C_GREY"/usecondom "C_WHITE" - Protect your self from sex diseases.\n\
                "C_GREY"/usewallet "C_WHITE" - Protect your self from robbers.\n\
                "C_GREY"/tie "C_WHITE"- Tie a player using a rope.\n\
                "C_GREY"/breaktie "C_WHITE"- Escape from being tied.\n\n");

            strcat(string, ""C_ORANGE"General Commands\n\
                "C_GREY"/payticket "C_WHITE"- Pay current ticket.\n\
                "C_GREY"/rob "C_WHITE"- Rob the nearest player.\n\
                "C_GREY"/placehit "C_WHITE"- Place a hit on a player you HATE.\n\
                "C_GREY"/eject "C_WHITE"- Eject a player from your vehicle.\n\
                "C_GREY"/smokeweed "C_WHITE"- Smoke weed that you bought.\n\
                "C_GREY"/smokecrack "C_WHITE"- Smoke crack that you bought.\n\
                "C_GREY"/buyweed "C_WHITE"- Buy weed from a drug dealer that offered you.\n\
                "C_GREY"/buycrack "C_WHITE"- Buy crack from a drug dealer that offered you.\n\
                "); 
            Dialog_Show(playerid, DIALOG_STYLE_MSGBOX, ""C_ORANGE"General Commands", string, "Close", "");
        }
        case 1:
        {   
            strcat(string, ""C_ORANGE"Basic Commands:\n\n\
            "C_GREY"/loc "C_WHITE"-  Locate a player's current location.\n\
            "C_GREY"/r "C_WHITE"- Communicate to our fellow police officers.\n\
            "C_GREY"/ar "C_WHITE"- Arrest a wanted player and send him to jail.\n\
            "C_GREY"/cuff "C_WHITE"- Cuff a player and you can arrest him afterwards.\n\
            "C_GREY"/uncuff "C_WHITE"- Remove a player's cuff and set him free.\n\
            "C_GREY"/backup "C_WHITE"- Send other units to help you out with the situation.\n\
            "C_GREY"/ticket "C_WHITE"- Give a low wanted level player a ticket, and lose his/her stars.\n\
            "C_GREY"/pullover "C_WHITE"- Pull over a low wanted player and proceed giving him/her a ticket.");

            Dialog_Show(playerid, DIALOG_STYLE_MSGBOX, ""C_ORANGE"Law Enforcement Officers Command", string, "Close", "");
        }
        case 2:
        {
            strcat(string, ""C_ORANGE"Hitman\n\
                "C_GREY"/hfind "C_WHITE"- Find a specific player's location.\n\
                "C_GREY"/hitlist "C_WHITE"- Check a list of players with hits on them.\n\n");

            strcat(string, ""C_ORANGE"Weapon Dealer\n\
                "C_GREY"/sellgun "C_WHITE"- Sell weapon to a player.\n\
                "C_GREY"/sellarmour "C_WHITE"- Sell armour to a player.\n\n");

            strcat(string, ""C_ORANGE"Rapist\n\
                "C_GREY"/rape "C_WHITE"- Rape a player and infect him with different kinds of infections.\n\n");

            strcat(string, ""C_ORANGE"Terrorist\n\
                "C_GREY"/plantc4 "C_WHITE"- Plant a C4 to.\n\
                "C_GREY"/detonate "C_WHITE"- To detonate the c4 that has been planted.\n\n");

            strcat(string, ""C_ORANGE"Drug Dealer\n\    
                "C_GREY"/plantweed "C_WHITE"- Plant a weed seed.\n\
                "C_GREY"/harvestweed "C_WHITE"- Harvest the planted weed.\n\
                "C_GREY"/sellweed "C_WHITE"- Sell weed to player.\n\
                "C_GREY"/sellcrack "C_WHITE"- Sell crack to player.");

            strcat(string, ""C_ORANGE"Mechanic\n\
                "C_GREY"/mechfix "C_WHITE"- Fix the vehicle you're in.");

            Dialog_Show(playerid, DIALOG_STYLE_MSGBOX, ""C_ORANGE"Job Commands", string, "Close", "");
        }
    }
    return 1;
}