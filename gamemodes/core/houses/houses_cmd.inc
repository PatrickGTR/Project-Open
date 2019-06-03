#include <YSI_Coding\y_hooks>
#include <YSI_Data\y_bit>

static
    BitArray: ply_insidehouse<MAX_PLAYERS>;

hook OnPlayerDeath(playerid)
{
    Bit_Vet(ply_insidehouse, playerid);
    return 1;
}

CMD:house(playerid, const params[])
{
    new
        i = ply_InHouseID[playerid],
        count;

    if (isnull(params))
        return SendSyntaxMsg(playerid, "/house buy | enter | exit | description | upgrade");

    if (i == INVALID_HOUSE_ID)
            return SendErrorMsg(playerid, "Invalid house id, you have not entered any house checkpoint.");


    if (isequal(params, "buy"))
    {
        if (!IsPlayerInDynamicCP(playerid, GetHouseEntranceCP(i)))
            return SendErrorMsg(playerid, "You are not in any house entrances.");

        if (GetHouseOwnerID(i) != INVALID_HOUSE_ID)
            return SendErrorMsg(playerid, "The house you are trying to buy is already owned.");

        if (Bit_Get(ply_insidehouse, playerid))
            return SendErrorMsg(playerid, "How is it possible to buy a house when you are inside it?");

        new
            price,
            Float:unused;

        GetHouseLevelInfo(GetHouseInteriorLevel(i), price, unused, unused, unused, unused, _:unused, _:unused);

        if (GetPlayerMoney(playerid) < price)
            return SendErrorMsg(playerid, "You do not have enough money to buy this house.");

        foreach(new x : houseIndex)
        {
            if (GetHouseOwnerID(x) == GetPlayerAccountID(playerid))
            {
                ++ count;
            }
            if (count == MAX_HOUSE_PER_PLAYER) break;
        }

        if (count >= MAX_HOUSE_PER_PLAYER)
            return SendErrorMsg(playerid, "You alraedy own a house.");

        UpdateHouse(i, GetPlayerAccountID(playerid), sprintf("Sold to %p!", playerid));
        GivePlayerMoney(playerid, -price);
    }
    if (isequal(params, "enter"))
    {
        if (!IsPlayerInDynamicCP(playerid, GetHouseEntranceCP(i)))
            return SendErrorMsg(playerid, "You are not in any house entrances.");

        if (Bit_Get(ply_insidehouse, playerid))
            return SendErrorMsg(playerid, "How is it possible to enter a house when you are inside it?");

        if (GetHouseOwnerID(i) != GetPlayerAccountID(playerid))
            return SendErrorMsg(playerid, "You are not the owner of this house.");

        GameTextForPlayer(playerid, "~g~Welcome Home", 2000, 1);

        new
            Float: x,
            Float: y,
            Float: z,
            Float: ang,
            int,
            vw;

        GetHouseExitData(i, x, y, z, ang, int, vw);

        SetPlayerInterior(playerid, int);
        SetPlayerVirtualWorld(playerid, vw);
        SetPlayerPos(playerid, x, y, z);
        SetPlayerFacingAngle(playerid, ang);

        Bit_Let(ply_insidehouse, playerid);
    }
    if (isequal(params, "exit"))
    {
        if (!IsPlayerInDynamicCP(playerid, GetHouseExitCP(i)))
            return SendErrorMsg(playerid, "You are not in any house exit.");

        if (!Bit_Get(ply_insidehouse, playerid))
            return SendErrorMsg(playerid, "How is it possible to exit a house when you are not inside it?");

        GameTextForPlayer(playerid, "~r~Goodbye", 2000, 1);

        SetPlayerInterior(playerid, 0);
        SetPlayerVirtualWorld(playerid, 0);

        new
            Float: x,
            Float: y,
            Float: z,
            Float: ang;

        GetHouseEntrancePos(i, x, y, z, ang);
        SetPlayerPos(playerid, x, y, z);
        SetPlayerFacingAngle(playerid, ang);

        Bit_Vet(ply_insidehouse, playerid);
    }
    if (!strcmp(params, "description", true, 11))
    {
        new
            description[MAX_HOUSE_DESCRIPTION];

        if (sscanf(params, "{s[12]}s[32]", description))
            return SendSyntaxMsg(playerid, "/house description <new description>");

        if (!Bit_Get(ply_insidehouse, playerid))
            return SendErrorMsg(playerid, "You must be inside your house to edit your house description.");

        SendServerMsgF(playerid, "You have changed your house description to "C_GREY"'%s'", description);

        UpdateHouse(i, GetPlayerAccountID(playerid), description);
    }
    /*
    TODO: Disable upgrade for now, make sure to force players to sell their furnitures before upgrading the house.
    if (isequal(params, "upgrade"))
    {

        if (GetPlayerMoney(playerid) < HOUSE_UPGRADE_PRICE)
            SendErrorMsgF(playerid, "You need "C_GREEN"$%m "C_WHITE"", HOUSE_UPGRADE_PRICE);

        if (!Bit_Get(ply_insidehouse, playerid))
            return SendErrorMsg(playerid, "You must be inside your house to upgrade your house level.");

        if (houseData[i][H_LEVEL] == 10)
            return SendErrorMsg(playerid, "Your house level is the highest.");

        ++ houseData[i][H_LEVEL];
        UpdateHouse(i, GetPlayerAccountID(playerid), houseData[i][H_DESC]);
    }
    */
    if (isequal(params, "furniture"))
    {
        if (!Bit_Get(ply_insidehouse, playerid))
            return SendErrorMsg(playerid, "You must be inside your house to buy a furniture.");

        inline ShowFurnitureOption(pid, dialogid, response, listitem, string:inputtext[])
        {
            #pragma unused pid, dialogid, inputtext

            if(!response)
                return 0;

            switch(listitem)
            {
                // Main Menu
                case 0: {
                    ShowPlayerFurnitureDialog(playerid);
                }
                // Edit Menu
                case 1: {
                    if (IsPlayerOnFurnitureMode(playerid) && GetPlayerFurnitureEditingType(playerid) == TYPE_EDITING_FURNITURE) {
                        return SendErrorMsg(playerid, "You are already editing a furniture, close the current one in order to proceed again.");
                    }
                    SetPlayerFurnitureEditingType(playerid, TYPE_EDITING_FURNITURE);
                    SetPlayerFurnitureMode(playerid, true);

                    SelectObject(playerid);
                    SendServerMsgF(playerid, "Use your mouse cursor to choose the furniture you want to edit.");
                    SendServerMsgF(playerid, "Press ESC once you're done, to allow your character to move.");
                }
                // Delete Menu
                case 2: {
                    SetPlayerFurnitureEditingType(playerid, TYPE_DELETING_FURNITURE);
                    SetPlayerFurnitureMode(playerid, true);

                    SelectObject(playerid);
                    SendServerMsgF(playerid, "Use your mouse cursor to choose the furniture you want to delete.");
                    SendServerMsgF(playerid, "Press ESC once you're done, to allow your character to move.");
                }
            }

        }
        Dialog_ShowCallback(playerid, using inline ShowFurnitureOption, DIALOG_STYLE_LIST, "Furniture", "Buy Furniture\nEdit Furniture\nDelete Furniture", "Select", "Close");
    }
    return true;
}