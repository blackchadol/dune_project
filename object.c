
#include "common.h"
#include "object.h"
#include "display.h"
bool handleBuildingCommand(BUILDING* building, Unit* units, int user_input, POSITION pos, RESOURCE* resource, char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]);
Unit* createUnit(UnitType type, POSITION pos, Unit* head, FactionType faction);
POSITION checkCanCreatePos(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH], BUILDING* building);
bool isValidPosition(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH], POSITION pos);
UnitType inputToUnitType(int user_input);
bool isWithinBounds(POSITION pos);
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
            insert_command_message("%c: %s SPICE: %d ",
                attr->symbol,
                buildingTypeToString(attr->type),
                attr->cost
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
        if (attr->symbol == (char)user_input || tolower(attr->symbol) == (char)user_input) {
            // 이 빌딩이 건설 가능한 목록에 포함되어 있는지 확인
            for (int j = 0; j < count; j++) {
                if (canCreateList[j] == attr->type) {
                    return (int)attr->type; // 유효한 빌딩 타입 반환
                }
            }
            //break; // 입력 명령어는 존재하지만 건설 불가
        }
    }

    return -1; // 유효하지 않은 입력
}


void buildStateAct(int userInput, POSITION cursor, RESOURCE resource, int* buildingEnum,bool firstCall) {
    BuildingType* canBuildList = listCanCreateBuilding(resource, firstCall);
    int count = countCanCreateBuilding(resource);
    *buildingEnum = getCreateBuildingCmd(userInput, canBuildList, count);
}


bool attemp_building(CURSOR cursor, BuildingType building, Unit* units, BUILDING* buildings, SPICE* spices, SANDWORM* sandworms) {
    // 2x2 영역 검사
    ObjectInfo result[4];
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            // 각 위치에 객체가 있는지 확인
            result[i * 2 + j] = checkObjectAtPosition((POSITION) { cursor.current.row + i, cursor.current.column + j }, units, buildings, spices, sandworms);
        }
    }

    int flag = 0;
    // 해당 자리에 다른 객체가 있으면 건설 불가
    for (int i = 0; i < 4; i++) {
        if (result[i].type != OBJECT_NONE) { // OBJECT_NONE이면 비어 있는 자리
            // 건설해야 되는 자리에 다른 객체가 있으면 건설 불가
            if (result[0].type == OBJECT_BUILDING) {
                BUILDING* thisBuilding = (BUILDING*)result[0].object; {
                    if (thisBuilding->type == PLATE) {
                        return true;
                    }
                }
            }
            flag = 1; // 겹치는 오브젝트가 있다. 
        }

        
        
    }

    if (!flag && building == PLATE) { // 겹치는 오브젝트가 없고 빌딩이 장판이면 건설가능
        return true;
    }
    return false;

}

void actBuildSpace(CURSOR cursor, BuildingType building, RESOURCE* resource, Unit* units, BUILDING**buildings, SPICE* spices, SANDWORM* sandworms) {
    bool canBuild = attemp_building(cursor, building, units, *buildings, spices, sandworms);
    if (canBuild) {
        *buildings = createBuilding(building, cursor.current, *buildings, FACTION_PLAYER);
        const BuildingAttributes* attr = &BUILDINGATTRIBUTES[building];
        insert_status_message("build %s sucessfully", buildingTypeToString(building));
        insert_status_message("Spice(%d -> %d)", resource->spice, resource->spice - attr->cost);
        resource->spice -= attr->cost;
        return;
    }
    else {
        //display_cursor(cursor, false);
        insert_status_message("can't build on this place");
    }

}
// 맵 경계 확인 함수
bool isWithinBounds(POSITION pos) {
    return pos.row >= 0 && pos.row < MAP_HEIGHT &&
        pos.column >= 0 && pos.column < MAP_WIDTH;
}

// 특정 좌표가 생성 가능한지 확인 (map[0]과 map[1] 모두 비어 있어야 함)
bool isValidPosition(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH], POSITION pos) {
    return isWithinBounds(pos) &&
        map[0][pos.row][pos.column] == ' ' &&  // 지형 레이어 비어 있음
        map[1][pos.row][pos.column] == -1;    // 오브젝트 레이어 비어 있음
}

UnitType inputToUnitType(int user_input) {
    if (user_input == 'H' || user_input == 'h') return HARVESTER;
    else if (user_input == 'S' || user_input == 's') return SOILDIER;
    else if (user_input == 'F' || user_input == 'f') return FREMEN;
    else return;
}

POSITION checkCanCreatePos(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH], BUILDING* building) {
    int baseRow = building->position.row;
    int baseCol = building->position.column;

    // 2x2 건물의 인접 12칸 좌표 (시계 방향)
    int directions[12][2] = {
        {-1, -1}, {-1,  0}, {-1,  1}, {-1,  2}, // 위쪽 4칸
        { 0, -1},                   { 0,  2},  // 좌우 2칸
        { 1, -1},                   { 1,  2},  // 좌우 2칸
        { 2, -1}, { 2,  0}, { 2,  1}, { 2,  2}  // 아래쪽 4칸
    };

    for (int i = 0; i < 12; i++) {
        POSITION newPos = { baseRow + directions[i][0], baseCol + directions[i][1] };

        // Layer 0과 Layer 1 모두 유효한 위치인지 확인
        if (isValidPosition(map, newPos)) {
            return newPos;
        }
    }

    // 유효한 위치가 없으면 (-1, -1) 반환
    POSITION invalidPos = { -1, -1 };
    return invalidPos;
}

bool handleBuildingCommand(BUILDING* building, Unit* units, int user_input, POSITION pos, RESOURCE* resource, char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]) {

    if ((char)user_input == BUILDINGATTRIBUTES[building->type].command) {
        POSITION newPos = checkCanCreatePos(map, building); // 건물 2x2 사방에 생성가능한 비어있는 유닛 지형이 있으면 건설. 아니면 오류메시지 출력
        if (newPos.row != -1 && newPos.column != -1) {
            if (UNIT_ATTRIBUTES[inputToUnitType(user_input)].production_cost <= resource->spice) { //  해당 유닛을 만들만큼 스파이스 양이 충분한지 확인
                resource->spice -= UNIT_ATTRIBUTES[inputToUnitType(user_input)].production_cost; // 스파이스 감소
                createUnit(inputToUnitType(user_input), newPos, units, FACTION_PLAYER); // 플레이어 유닛 
                insert_status_message("complete create %s", unitTypeToString(inputToUnitType(user_input)));
                return true;
            }
            else {
                insert_system_message("no enough spice. this unit costs: %d", UNIT_ATTRIBUTES[inputToUnitType(user_input)].production_cost); // 스파이스가 없으면 시스템 창 출력
            }
        }
        else {
            insert_system_message("no place to create unit");
        }
    }
    return false;
}