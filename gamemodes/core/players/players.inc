
#include <classes>
#include <cuffs>
#include <jail>
#include <teams>
#include <jobs>

#include <YSI_Coding\y_hooks>

hook OnPlayerConnect(playerid) {
    // Preload all the animations //
    PreloadAnimLib(playerid, "BOMBER");
    PreloadAnimLib(playerid, "PED");
    PreloadAnimLib(playerid, "SHOP");
    PreloadAnimLib(playerid, "ROB_BANK");
    PreloadAnimLib(playerid, "BD_FIRE");
}