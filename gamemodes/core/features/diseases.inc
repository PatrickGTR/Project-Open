enum {
    DISEASE_NONE = 0,
    DISEASE_GONORRHEA,
    DISEASE_HIV,
    DISEASE_AIDS,
    DISEASE_CHLAMYDIA,
    DISEASE_HERPES
}


static 
    Timer:ply_diseaseTime[MAX_PLAYERS],
    diseaseType[MAX_PLAYERS]
;

IsPlayerInfected(playerid) 
{
    return (diseaseType[playerid] > 0) ? 1 : 0;
}

GivePlayerDisease(playerid, diseasetype)
{
    switch(diseasetype)
    {
        case DISEASE_NONE: {
            diseaseType[playerid] = DISEASE_NONE;
            stop ply_diseaseTime[playerid];
        }
        case DISEASE_GONORRHEA: diseaseType[playerid] = DISEASE_GONORRHEA;
        case DISEASE_HIV: diseaseType[playerid] = DISEASE_HIV;
        case DISEASE_AIDS: diseaseType[playerid] = DISEASE_AIDS;
        case DISEASE_CHLAMYDIA: diseaseType[playerid] = DISEASE_CHLAMYDIA;
        case DISEASE_HERPES: diseaseType[playerid] = DISEASE_HERPES;
        default: diseaseType[playerid] = DISEASE_NONE;
    }
    ply_diseaseTime[playerid] = repeat UpdatePlayerDiseaseDamage(playerid);
    return true;
}

GetDiseaseName(playerid)
{
    new 
        diseaseName[ 10 ];
    switch(diseaseType[playerid])
    {
        case DISEASE_NONE: strcpy(diseaseName, "None");
        case DISEASE_GONORRHEA: strcpy(diseaseName, "Gonorrhea");
        case DISEASE_HIV: strcpy(diseaseName, "HIV");
        case DISEASE_AIDS: strcpy(diseaseName, "AIDS");
        case DISEASE_CHLAMYDIA: strcpy(diseaseName, "Chlamydia");
        case DISEASE_HERPES: strcpy(diseaseName, "Herpes");
    }
    return diseaseName;
}

Float:GetDiseaseDamage(diseasetype)
{
    switch(diseasetype)
    {
        case DISEASE_NONE: return 0.0;
        case DISEASE_GONORRHEA: return 0.5;
        case DISEASE_HIV: return 0.8;
        case DISEASE_AIDS: return 1.0;
        case DISEASE_CHLAMYDIA: return 0.7;
        case DISEASE_HERPES: return 0.1;
    }
    return 0.0;
}

timer UpdatePlayerDiseaseDamage[1000 * 5](playerid)
{
    if (IsPlayerInfected(playerid))
    {
        new 
            Float:ply_health;

        GetPlayerHealth(playerid, ply_health);
        SetPlayerHealth(playerid, ply_health - GetDiseaseDamage(diseaseType[playerid]));
    }
}

#include <YSI_Coding\y_hooks>
hook OnPlayerDeath(playerid, killerid, reason)
{
    if (IsPlayerInfected(playerid) && killerid == INVALID_PLAYER_ID)
    {
        SendMsgAllF(COLOR_LIGHTGREEN, "** RAPE DEATH **: "C_GREY"%p (%i) "C_WHITE"died because of "C_LGREEN"%s.", playerid, playerid, GetDiseaseName(playerid));
        GivePlayerDisease(playerid, DISEASE_NONE);

        stop ply_diseaseTime[playerid];
    }
}


