#include <YSI_Visual\y_dialog>
#include <streamer>

#define MAX_ITEMS                   (20)

static const bombArray[][2] =
{
    {1, 2500},
    {5, 2500 * 5},
    {10, 2500 * 10},
    {20, 2500 * 20}
};

enum E_ITEMS_SHOP
{
    ITEM_NAME[24],
    ITEM_QTY,
    ITEM_PRICE
}

static const itemsTwentyFourSeven[][E_ITEMS_SHOP] = 
    {
        //Piclock
        {"Picklock",            2,      1000 * 2},
        {"Picklock",            10,     1000 * 10},
        {"Picklock",            15,     1000 * 15},
        //Condom
        {"Condom",              2,      1000 * 2},
        {"Condom",              10,     1000 * 10},
        {"Condom",              15,     1000 * 15},
        //Wallet
        {"Wallet",              2,      1000 * 2},
        {"Wallet",              10,     1000 * 10},
        {"Wallet",              15,     1000 * 15},
        //Rope
        {"Rope",                2,      1000 * 2},
        {"Rope",                10,     1000 * 10},
        {"Rope",                15,     1000 * 15},
        //Scissors
        {"Scissors",            2,      1000 * 2},
        {"Scissors",            10,     1000 * 10},
        {"Scissors",            15,     1000 * 15}
    };

new
	bombShopCP_SF,
	bombShopCP_BC,
	twentyFourSevenCP
;


#include <YSI_Coding\y_hooks>

hook OnGameModeInit()
{
	bombShopCP_SF = CreateDynamicCP(-1786.9229, 1207.6658, 25.1250, 3.0);
	bombShopCP_BC = CreateDynamicCP(673.7756, 1706.4987, 7.1875, 1.50);

    CreateDynamicMapIcon(1786.9229, 1207.6658, 25.1250, 8, -1, .streamdistance = MAPICON_STREAMDISTANCE);
    CreateDynamicMapIcon(673.7756, 1706.4987, 7.1875, 8, -1, .streamdistance = MAPICON_STREAMDISTANCE);

	twentyFourSevenCP = CreateDynamicCP(-23.1558, -55.3146, 1003.5469, 1.50);
}

hook OnPlayerEnterDynamicCP(playerid, checkpointid)
{
	if(checkpointid == bombShopCP_SF || checkpointid == bombShopCP_BC)
    {
    	new 
        	string[68/*68 characters per line*/* sizeof(bombArray)/*number of lines*/];

        inline DialogBombShopResponse(ply_id, dialogid, response, listitem, string:inputtext[])
        {
            #pragma unused inputtext, dialogid, ply_id
            if(!response)
                return false;

            if(GetPlayerMoney(playerid) < bombArray[listitem][1])
    	 	{
    	 		SendErrorMsgF(playerid, "You don't have enough money to buy "C_GREY"%i C4", bombArray[listitem][0]),
               	Dialog_ShowCallback(playerid, using inline DialogBombShopResponse, DIALOG_STYLE_LIST, ""C_ORANGE"Bomb Shop", string, "Purchase", "Close");
               	return 1;
           	}

            SendMsgF(playerid, COLOR_ORANGE, "** BOMB SHOP ** "C_WHITE"You successfully bought "C_GREY"%i C4 "C_WHITE"for "C_GREEN"%m", bombArray[listitem][0], bombArray[listitem][1]);
            GivePlayerMoney(playerid, -bombArray[listitem][1]);
            GivePlayerC4(playerid, bombArray[listitem][0]);       
        }


        for(new i = 0; i != sizeof(bombArray); ++i)
        {
            format(string, sizeof(string), "%s"C_WHITE"C4 - Quantity: "C_GREY"%i "C_GREY"- Price: "C_GREEN"%m\n", string, bombArray[i][0], bombArray[i][1]);
        }
        Dialog_ShowCallback(playerid, using inline DialogBombShopResponse, DIALOG_STYLE_LIST, ""C_ORANGE"Bomb Shop", string, "Purchase", "Close");
    }
    if(checkpointid == twentyFourSevenCP)
    {        
        new 
            tmp_string[32],
            string[sizeof(tmp_string) * sizeof(itemsTwentyFourSeven)/*number of lines*/]; 
        inline TwentyFourSevenResponse(plyid, dialogid, response, listitem, string:inputtext[])
        {
            #pragma unused plyid, dialogid, inputtext

            if(!response)
                return false;

            if(GetPlayerMoney(playerid) < itemsTwentyFourSeven[listitem][ITEM_PRICE])
            {
                SendErrorMsgF(playerid, "You don't have enough money to buy %s", itemsTwentyFourSeven[listitem][ITEM_NAME]);
                Dialog_ShowCallback(playerid, using inline TwentyFourSevenResponse, DIALOG_STYLE_TABLIST_HEADERS, "24/7 Items", string, "Select", "Close");
                return 1;
            }
            if(GetPlayerPickLock(playerid) || GetPlayerCondom(playerid) || GetPlayerWallet(playerid) || GetPlayerRope(playerid) || GetPlayerScissors(playerid) >= MAX_ITEMS) 
            {
                SendErrorMsg(playerid, "You are currently holding the maximum amount of this time.");
                Dialog_ShowCallback(playerid, using inline TwentyFourSevenResponse, DIALOG_STYLE_TABLIST_HEADERS, "24/7 Items", string, "Select", "Close");
                return 1;
            }
            switch(listitem)
            {
                case 0: GivePlayerPickLock  (playerid, 2);
                case 1: GivePlayerPickLock  (playerid, 10);
                case 2: GivePlayerPickLock  (playerid, 15);
    
                case 3: GivePlayerCondom    (playerid, 2);
                case 4: GivePlayerCondom    (playerid, 10);
                case 5: GivePlayerCondom    (playerid, 15);
        
                case 6: GivePlayerWallet    (playerid, 2);
                case 7: GivePlayerWallet    (playerid, 10);
                case 8: GivePlayerWallet    (playerid, 15);

                case 9: GivePlayerRope      (playerid, 2);
                case 10: GivePlayerRope     (playerid, 10);
                case 11: GivePlayerRope     (playerid, 15);

                case 12: GivePlayerScissors (playerid, 2);
                case 13: GivePlayerScissors (playerid, 10);
                case 14: GivePlayerScissors (playerid, 15);
                
            }
            SendServerMsgF(playerid, "You bought a "C_GREY"%s "C_WHITE"for "C_GREEN"%m "C_WHITE"from 24/7", itemsTwentyFourSeven[listitem][ITEM_NAME], itemsTwentyFourSeven[listitem][ITEM_PRICE]);
            GivePlayerMoney(playerid, -itemsTwentyFourSeven[listitem][ITEM_PRICE]);
        }

        strcat(string, "Item\tQuantity\tPrice\n");
        for(new i = 0; i < sizeof(itemsTwentyFourSeven); ++i)
        {
            format(tmp_string, sizeof(tmp_string), "%s\t%i\t"C_GREEN"%m\n", 
                 itemsTwentyFourSeven[i][ITEM_NAME], itemsTwentyFourSeven[i][ITEM_QTY], itemsTwentyFourSeven[i][ITEM_PRICE]);

            strcat(string, tmp_string, sizeof(string));
        }
        Dialog_ShowCallback(playerid, using inline TwentyFourSevenResponse, DIALOG_STYLE_TABLIST_HEADERS, "24/7 Items", string, "Select", "Close");
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}
