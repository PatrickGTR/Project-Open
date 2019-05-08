#include <YSI_Coding\y_hooks>
#include <YSI\y_dialog>

#define MAX_AMMUNATION_AMMO 250

static const WeaponPistol[][] = {
    {22, 1000, MAX_AMMUNATION_AMMO},
    {23, 1500, MAX_AMMUNATION_AMMO},
    {24, 4500, MAX_AMMUNATION_AMMO}
};

static const WeaponSub[][] =  {
    {28, 2000, MAX_AMMUNATION_AMMO},
    {32, 2000, MAX_AMMUNATION_AMMO},
    {29, 2500, MAX_AMMUNATION_AMMO}
};

static const WeaponShotgun[][] = {
    {27, 3500, MAX_AMMUNATION_AMMO},
    {26, 4500, MAX_AMMUNATION_AMMO},
    {25, 2500, MAX_AMMUNATION_AMMO}
};

static const WeaponAssRifle[][] = {
    {30, 4500, MAX_AMMUNATION_AMMO},
    {31, 4500, MAX_AMMUNATION_AMMO}
};

static const WeaponRifles[][] =  {
    {33, 3500, MAX_AMMUNATION_AMMO},
    {34, 5000, MAX_AMMUNATION_AMMO}
};

static CPAmmunation;

hook OnGameModeInit() {
     CPAmmunation = CreateDynamicCP(313.6852,-132.4308,999.60166, 1.50);
     Create3DTextLabel("Ammunation\n"C_WHITE"Buy your weapons here!", COLOR_YELLOW, 313.6852,-132.4308,999.60166, TEXTLABEL_STREAMDISTANCE, -1, 1);
}

hook OnPlayerEnterDynamicCP(playerid, checkpointid) {
    if(checkpointid == CPAmmunation) {
        ShowPlayerAmmunationOption(playerid);
    }
}

ShowPlayerAmmunationOption(playerid) {
    inline DialogWeaponResponse(plyid, dialogid, response, listitem, string:inputtext[]) {
        #pragma unused plyid, dialogid, inputtext

        if(!response)
            return false;

        new
            weapname[32],
            string[512];

        switch(listitem) {
            case 0: {
                strcat(string, "Weapon\tPrice\tAmmo\n");
                for(new i = 0; i < sizeof(WeaponPistol); ++i) {
                    GetWeaponName(WeaponPistol[i][0], weapname, sizeof(weapname));
                    format(string, sizeof(string), "%s%s\t"C_GREEN"%m\t"C_WHITE"%i bullets\n", string, weapname, WeaponPistol[i][1], WeaponPistol[i][2]);
                }
                inline DialogPistolResponse(plyid2, dialogid2, response2, listitem2, string:inputtext2[])
                {
                    #pragma unused plyid2, dialogid2, inputtext2

                    if(!response2)
                        return ShowPlayerAmmunationOption(playerid);

                    GetWeaponName(WeaponPistol[listitem2][0], weapname, sizeof(weapname));

                    if(GetPlayerMoney(playerid) < WeaponPistol[listitem2][1])
                        return SendErrorMsgF(playerid, "You don't have enough money to buy %s", weapname);

                    GivePlayerWeapon(playerid, WeaponPistol[listitem2][0], WeaponPistol[listitem2][2]);
                    GivePlayerMoney(playerid, -WeaponPistol[listitem2][1]);

                    SendServerMsgF(playerid, "You bought a "C_GREY"%s "C_WHITE"for "C_GREEN"%m", weapname, WeaponPistol[listitem2][1]);
                }
                Dialog_ShowCallback(playerid, using inline DialogPistolResponse, DIALOG_STYLE_TABLIST_HEADERS, "Pistols", string, "Select", "Back");
                //Pistol
            }
            case 1: {
                strcat(string, "Weapon\tPrice\tAmmo\n");
                for(new i = 0; i < sizeof(WeaponSub); ++i) {
                    GetWeaponName(WeaponSub[i][0], weapname, sizeof(weapname));
                    format(string, sizeof(string), "%s%s\t"C_GREEN"%m\t"C_WHITE"%i bullets\n", string, weapname, WeaponSub[i][1], WeaponSub[i][2]);
                }
                inline DialogSubmachineResponse(plyid2, dialogid2, response2, listitem2, string:inputtext2[]) {
                    #pragma unused plyid2, dialogid2, inputtext2

                    if(!response2)
                        return ShowPlayerAmmunationOption(playerid);

                    GetWeaponName(WeaponSub[listitem2][0], weapname, sizeof(weapname));
                    if(GetPlayerMoney(playerid) < WeaponSub[listitem2][1])
                        return SendErrorMsgF(playerid, "You don't have enough money to buy %s", weapname);

                    GivePlayerWeapon(playerid, WeaponSub[listitem2][0], WeaponSub[listitem2][2]);
                    GivePlayerMoney(playerid, -WeaponSub[listitem2][1]);
                    SendServerMsgF(playerid, "You bought a "C_GREY"%s "C_WHITE"for "C_GREEN"%m", weapname, WeaponSub[listitem2][1]);
                }
                Dialog_ShowCallback(playerid, using inline DialogSubmachineResponse, DIALOG_STYLE_TABLIST_HEADERS, "Sub-Machine Guns", string, "Select", "Back");
                //Sub Machine Gun
            }
            case 2: {
                strcat(string, "Weapon\tPrice\tAmmo\n");
                for(new i = 0; i < sizeof(WeaponShotgun); ++i) {
                    GetWeaponName(WeaponShotgun[i][0], weapname, sizeof(weapname));
                    format(string, sizeof(string), "%s%s\t"C_GREEN"%m\t"C_WHITE"%i bullets\n", string, weapname, WeaponShotgun[i][1], WeaponShotgun[i][2]);
                }
                inline DialogShotgunResponse(plyid2, dialogid2, response2, listitem2, string:inputtext2[]) {
                    #pragma unused plyid2, dialogid2, inputtext2

                    if(!response2)
                        return ShowPlayerAmmunationOption(playerid);

                    GetWeaponName(WeaponShotgun[listitem2][0], weapname, sizeof(weapname));
                    if(GetPlayerMoney(playerid) < WeaponShotgun[listitem2][1])
                        return SendErrorMsgF(playerid, "You don't have enough money to buy %s", weapname);

                    GivePlayerWeapon(playerid, WeaponShotgun[listitem2][0], WeaponShotgun[listitem2][2]);
                    GivePlayerMoney(playerid, -WeaponShotgun[listitem2][1]);
                    SendServerMsgF(playerid, "You bought a "C_GREY"%s "C_WHITE"for "C_GREEN"%m", weapname, WeaponShotgun[listitem2][1]);
                }
                Dialog_ShowCallback(playerid, using inline DialogShotgunResponse, DIALOG_STYLE_TABLIST_HEADERS, "Pistols", string, "Select", "Back");
                //Shotguns
            }
            case 3: {
                strcat(string, "Weapon\tPrice\tAmmo\n");
                for(new i = 0; i < sizeof(WeaponAssRifle); ++i) {
                    GetWeaponName(WeaponAssRifle[i][0], weapname, sizeof(weapname));
                    format(string, sizeof(string), "%s%s\t"C_GREEN"%m\t"C_WHITE"%i bullets\n", string, weapname, WeaponAssRifle[i][1], WeaponAssRifle[i][2]);
                }
                inline DialogAssRifleResponse(plyid2, dialogid2, response2, listitem2, string:inputtext2[]) {
                    #pragma unused plyid2, dialogid2, inputtext2

                    if(!response2)
                        return ShowPlayerAmmunationOption(playerid);

                    GetWeaponName(WeaponAssRifle[listitem2][0], weapname, sizeof(weapname));
                    if(GetPlayerMoney(playerid) < WeaponAssRifle[listitem2][1])
                        return SendErrorMsgF(playerid, "You don't have enough money to buy %s", weapname);

                    GivePlayerWeapon(playerid, WeaponAssRifle[listitem2][0], WeaponAssRifle[listitem2][2]);
                    GivePlayerMoney(playerid, -WeaponAssRifle[listitem2][1]);
                    SendServerMsgF(playerid, "You bought a "C_GREY"%s "C_WHITE"for "C_GREEN"%m", weapname, WeaponAssRifle[listitem2][1]);
                }
                Dialog_ShowCallback(playerid, using inline DialogAssRifleResponse, DIALOG_STYLE_TABLIST_HEADERS, "Pistols", string, "Select", "Back");
                //Assault Rifle
            }
            case 4: {
                strcat(string, "Weapon\tPrice\tAmmo\n");
                for(new i = 0; i < sizeof(WeaponRifles); ++i) {
                    GetWeaponName(WeaponRifles[i][0], weapname, sizeof(weapname));
                    format(string, sizeof(string), "%s%s\t"C_GREEN"%m\t"C_WHITE"%i bullets\n", string, weapname, WeaponRifles[i][1], WeaponRifles[i][2]);
                }
                inline DialogRiflesResponse(plyid2, dialogid2, response2, listitem2, string:inputtext2[]) {
                    #pragma unused plyid2, dialogid2, inputtext2

                    if(!response2)
                        return ShowPlayerAmmunationOption(playerid);

                    GetWeaponName(WeaponRifles[listitem2][0], weapname, sizeof(weapname));
                    if(GetPlayerMoney(playerid) < WeaponRifles[listitem2][1])
                        return SendErrorMsgF(playerid, "You don't have enough money to buy %s", weapname);

                    GivePlayerWeapon(playerid, WeaponRifles[listitem2][0], WeaponRifles[listitem2][2]);
                    GivePlayerMoney(playerid, -WeaponRifles[listitem2][1]);
                    SendServerMsgF(playerid, "You bought a "C_GREY"%s "C_WHITE"for "C_GREEN"%m", weapname, WeaponRifles[listitem2][1]);
                }
                Dialog_ShowCallback(playerid, using inline DialogRiflesResponse, DIALOG_STYLE_TABLIST_HEADERS, "Pistols", string, "Select", "Back");
                //Rifles
            }
        }
    }
    Dialog_ShowCallback(playerid, using inline DialogWeaponResponse, DIALOG_STYLE_LIST, "Ammunation", "Pistols\nSub-machine guns\nShotguns\nAssault rifles\nRifles", "Select", "Close");
    return 1;
}