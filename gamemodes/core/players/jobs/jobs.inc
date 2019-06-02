#include <YSI_Visual\y_dialog>
#include <formatex>
#include <strlib>

static
    playerJob[MAX_PLAYERS];

SetPlayerJob(playerid, job_id, bool:save_job = false) {
    if(save_job == true) {
        mysql_tquery(GetMySQLHandle(), sprintf("UPDATE "TABLE_ACCOUNT" SET "FIELD_JOB" = %i WHERE "FIELD_ACCOUNTID" = %i", job_id, GetPlayerAccountID(playerid)));
    }

    playerJob[playerid] = job_id;
    return playerJob[playerid];
}

GetPlayerJob(playerid) {
    return playerJob[playerid];
}

ShowPlayerJobMenu(playerid) {
    if (GetPlayerJob(playerid) != TYPE_NO_JOB)
        return 0;

    inline jobmenu_response(ply_id, dialogid, response, listitem, string:inputtext[]) {
        #pragma unused ply_id, dialogid, inputtext
        if (!response) {
            SendErrorMsg(playerid, "You have to select a job!");
            return ShowPlayerJobMenu(playerid);
        }

        switch(listitem) {
            case TYPE_DRUGDEALER:  {
                MessageBox(playerid, TYPE_INFORMATION, "Drug Dealer", "Your objective is to create drugs and sell them to earn profit!", 1000 * 10);
                SetPlayerJob(playerid, TYPE_DRUGDEALER, true);
            }
            case TYPE_WEAPONDEALER: {
                MessageBox(playerid, TYPE_INFORMATION, "Weapon Dealer", "Your objective is to sell any kind of weapons and sell them to earn profit!", 1000 * 10);
                SetPlayerJob(playerid, TYPE_WEAPONDEALER, true);
            }
            case TYPE_HITMAN: {
                MessageBox(playerid, TYPE_INFORMATION, "Hitman", "Your objective is to kill players with contract and earn a big amount of money!", 1000 * 10);
                SetPlayerJob(playerid, TYPE_HITMAN, true);
            }
            case TYPE_TERRORIST: {
                MessageBox(playerid, TYPE_INFORMATION, "Terrorist", "Your objective is to plant bombs in cars, explode bank and prison entrances!", 1000 * 10);
                SetPlayerJob(playerid, TYPE_TERRORIST, true);
            }
            case TYPE_RAPIST: {
                MessageBox(playerid, TYPE_INFORMATION, "Rapist", "Your objective is to infect players with different sexually transmitted disseases!", 1000 * 10);
                SetPlayerJob(playerid, TYPE_RAPIST, true);
            }
            case TYPE_MECHANIC: {
                MessageBox(playerid, TYPE_INFORMATION, "Mechanic", "Being mechanic allows you to repair your own vehicle and other player's vehicle!", 1000 * 10);
                SetPlayerJob(playerid, TYPE_MECHANIC, true);
            }
        }
        GivePlayerJobWeapons(playerid);
    }
    Dialog_ShowCallback(playerid, using inline jobmenu_response, DIALOG_STYLE_LIST, "Job Menu", "\
    Drug Dealer\n\
    Weapon Dealer\n\
    Hitman\n\
    Terrorist\n\
    Rapist" , "Choose", "Cancel");
    return 1;
}

GetPlayerJobName(playerid) {
    new
        jobName[24];

    switch(GetPlayerJob(playerid)) {
        case TYPE_DRUGDEALER: {
            strcpy(jobName, "Drug Dealer");
        }
        case TYPE_WEAPONDEALER: {
            strcpy(jobName, "Weapon Dealer");
        }
        case TYPE_HITMAN: {
            strcpy(jobName, "Hitman");
        }
        case TYPE_TERRORIST: {
            strcpy(jobName, "Terrorist");
        }
        case TYPE_RAPIST: {
            strcpy(jobName, "Rapist");
        }
        case TYPE_MECHANIC: {
            strcpy(jobName, "Mechanic");
        }
    }
    return jobName;
}


GivePlayerJobWeapons(playerid) {
    switch(GetPlayerJob(playerid)) {
        case TYPE_DRUGDEALER: {
            GivePlayerWeapon(playerid, WEAPON_COLT45,   200);
            GivePlayerWeapon(playerid, WEAPON_SHOTGUN,  200);
        }
        case TYPE_WEAPONDEALER: {
            GivePlayerWeapon(playerid, WEAPON_M4,       200);
            GivePlayerWeapon(playerid, WEAPON_SHOTGSPA, 200);
        }
        case TYPE_HITMAN: {
            GivePlayerWeapon(playerid, WEAPON_KNIFE,    1);
            GivePlayerWeapon(playerid, WEAPON_SILENCED, 200);
            GivePlayerWeapon(playerid, WEAPON_SNIPER,   200);
        }
        case TYPE_TERRORIST: {
            GivePlayerWeapon(playerid, WEAPON_RIFLE,    200);
            GivePlayerWeapon(playerid, WEAPON_AK47,     200);
        }
        case TYPE_RAPIST: {
            GivePlayerWeapon(playerid, WEAPON_DILDO,    1);
            GivePlayerWeapon(playerid, WEAPON_COLT45,   200);
            GivePlayerWeapon(playerid, WEAPON_SNIPER,   200);
        }
        case TYPE_MECHANIC: {
            GivePlayerWeapon(playerid, WEAPON_AK47, 200);
            GivePlayerWeapon(playerid, WEAPON_SHOTGSPA, 200);
            GivePlayerWeapon(playerid, WEAPON_COLT45, 200);
        }
    }
    return 1;
}


// Drug Dealer
// Weed
#include <sellweed-cmd>
#include <weed-plantation>
// Crack
#include <sellcrack-cmd>

// Hitman
#include <hitman>
#include <track-cmd>
#include <hitlist-cmd>

// Weapon Dealer
#include <sell-cmd>

// Terrorist
#include <plantc4-cmd>

#include <jobs-mechanic>
#include <jobs-rapist>
