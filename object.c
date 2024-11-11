

#include "object.h"

// =========rock의 개수나 위치는 변하지 않을 것이기 때문에 상수로 선언=============//
const POSITION rock_positions[10] = {  // 2x2 바위는 4개의 좌표를 차지하므로 4배 크기
    {10, 30}, {10, 31}, {11, 30}, {11, 31}, // 바위 1 (2x2)
    {5, 15}, {5, 16}, {6, 15}, {6, 16},   // 바위 2 (2x2)
    {8, 42}, // 바위 3 (1x1)
    {15, 13} // 바위 4 (1x1)
};

const UnitAttributes UNIT_ATTRIBUTES[NUM_UNIT_TYPES] = {
    {HARVESTER,5,5,2000, -1, -1, 70,0, {'H','M'}, 'H',FACTION_COMMON},
    {FREMEN,5,2,400,15,200,25,8,{'M','P'}, 'F',FACTION_PLAYER},
    {SOILDIER,1,1,1000,5,800,15,1,{'M','P'}, 'S',FACTION_PLAYER},
    {PROJECTION,1,1,1200,6,600,10,1,{'M','P'}, 'P',FACTION_ENEMY},
    {TANK,12,5,3000,40,4000,60,4,{'M','P'}, 'T',FACTION_ENEMY}
};

const BuildingAttributes BUILDINGATTRIBUTES[NUM_BUILDING_TYPES] = {
    {BASE, 0,50, 'H','B',FACTION_COMMON},
    {PLATE, 1,-1, -1,'P',FACTION_COMMON},
    {DORMITORY,2,10,-1,'D',FACTION_COMMON},
    {GARAGE,4,10,-1,'G',FACTION_COMMON},
    {BARRACKS,4,20,'S','B',FACTION_PLAYER},
    {SHELTER,5,30,'F','S',FACTION_PLAYER},
    {ARENA,3,15,'F','A',FACTION_ENEMY},
    {FACTORY,5,30,'T','F',FACTION_ENEMY}
}; 


const char* unitTypeToString(int type) {
    switch (type) {
    case 0: return "HARVESTER";
    case 1: return "FREMEN";
    case 2: return "SOILDIER";
    case 3: return "PROJECTION";
    case 4: return "TANK";
    default: return "UNKNOWN_UNIT_TYPE";
    }
}


const char* buildingTypeToString(int type) {
    switch (type) {
    case 0: return "BASE";
    case 1: return "PLATE";
    case 2: return "DORMITORY";
    case 3: return "GARAGE";
    case 4: return "BARRACKS";
    case 5: return "SHELTER";
    case 6: return "ARENA";
    case 7: return "FACTORY";
    default: return "UNKNOWN_BUILDING_TYPE";
    }
}
