
#include "common.h"
#include "object.h"
#include "display.h"

//Unit* removeUnit(Unit* units, Unit* targetUnit);

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



int countCanCreateBuilding(RESOURCE resource) {

    int count = 0;
    for (int i = 0; i < NUM_BUILDING_TYPES; i++) {
        const BuildingAttributes* attr = &BUILDINGATTRIBUTES[i];

        // 플레이어의 Faction에 맞는 건물인지 확인
        if (attr->faction != FACTION_COMMON && attr->faction != FACTION_PLAYER) {
            continue;
        }
        if (attr->type == BASE) {
            continue;
        }

        // 자원이 충분한지 확인
        if (resource.spice < attr->cost) {
            continue;
        }
        count++;
        
    }
    return count;

}


BuildingType* listCanCreateBuilding(RESOURCE resource, bool firstcall) {
    int count = 0;

    // 동적으로 메모리 할당 (최대 건물 개수)
    BuildingType* canCreateBuildingList = (BuildingType*)malloc(NUM_BUILDING_TYPES * sizeof(BuildingType));
    if (!canCreateBuildingList) {
        // 메모리 할당 실패 시 처리
        insert_command_message("Error: Memory allocation failed.");
        return NULL;
    }

    for (int i = 0; i < NUM_BUILDING_TYPES; i++) {
        const BuildingAttributes* attr = &BUILDINGATTRIBUTES[i];

        // 플레이어의 Faction에 맞는 건물인지 확인
        if (attr->faction != FACTION_COMMON && attr->faction != FACTION_PLAYER) {
            continue;
        }
        if (attr->type == BASE) {
            continue;
        }

        // 자원이 충분한지 확인
        if (resource.spice < attr->cost) {
            continue;
        }

        if (firstcall) // 첫번째 함수 호출일때만 명령창에 입력. 
            // 건물이 건설 가능함을 표시
        {
            insert_command_message("%c: %s ",
                attr->symbol,
                buildingTypeToString(attr->type)
            );
        }

        // 건물 유형 추가
        canCreateBuildingList[count] = attr->type;
        count++;
    }

    // 필요한 만큼만 메모리 크기를 조정
    canCreateBuildingList = (BuildingType*)realloc(canCreateBuildingList, count * sizeof(BuildingType));
    if (!canCreateBuildingList && count > 0) {
        // realloc 실패 시 처리
        insert_command_message("Error: Memory reallocation failed.");
        return NULL;
    }

    return canCreateBuildingList; // 동적 배열 반환
}

int getCreateBuildingCmd(int user_input, BuildingType* canCreateList, int count) { 
    // 건설 가능한 빌딩 목록이 없으면 -1 반환
    if (canCreateList == NULL || count <= 0) {
        return -1;
    };
    for (int i = 0; i < NUM_BUILDING_TYPES; i++) { // BASE는 제외(attr[0])
        const BuildingAttributes* attr = &BUILDINGATTRIBUTES[i];

        // 입력된 명령어가 BUILDINGATTRIBUTES에 있는지 확인
        if (attr->symbol == (char)user_input) {
            // 이 빌딩이 건설 가능한 목록에 포함되어 있는지 확인
            for (int j = 0; j < count; j++) {
                if (canCreateList[j] == attr->type) {
                    return (int)attr->type; // 유효한 빌딩 타입 반환
                }
            }
            break; // 입력 명령어는 존재하지만 건설 불가
        }
    }

    return -1; // 유효하지 않은 입력
}


void buildStateAct(int userInput, POSITION cursor, RESOURCE resource, int* buildingEnum,bool firstCall) {
    BuildingType* canBuildList = listCanCreateBuilding(resource, firstCall);
    int count = countCanCreateBuilding(resource);
    *buildingEnum = getCreateBuildingCmd(userInput, canBuildList, count);
}
