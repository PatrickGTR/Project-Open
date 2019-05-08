#include <teams-medic>
#include <teams-police>

IsPlayerLEO(playerid) 
{   
    switch(GetPlayerClass(playerid))
    {
        case TEAM_ARMY: return 1;
        case TEAM_POLICE: return 1;
        default: return 0;
    }
    return 0;
}

IsPlayerMedic(playerid)
    return (GetPlayerClass(playerid) == TEAM_MEDIC);