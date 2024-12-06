
#include "common.h"
#include "object.h"
#include "display.h"
#include "io.h"
#include "time.h"
#include <stdlib.h> 
bool handleBuildingCommand(BUILDING* building, Unit** units, int user_input, POSITION pos, RESOURCE* resource, char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]);
Unit* createUnit(UnitType type, POSITION pos, Unit* head, FactionType faction);
POSITION checkCanCreatePos(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH], BUILDING* building);
bool isValidPosition(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH], POSITION pos);
UnitType inputToUnitType(int user_input);
bool isWithinBounds(POSITION pos);
bool checkPopulationCreateUnit(RESOURCE resource);
void getHarvestCommand(Unit* selectedUnit, int user_input, POSITION cursor, SPICE* spice, GameState* gamestate );
POSITION getGoBackHome(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]);
bool isNearBase(Unit* harvester);
void extractSpice(Unit* currentUnit, SPICE* targetSpice);
void harvesterMove(Unit** units, char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH], RESOURCE* resource, int sys_clock, SPICE** spiceHead);
void removeSpice(SPICE** head, SPICE* target);
void removeUnit(Unit** units, Unit* targetUnit);
ObjectInfo checkObjectAtPosition(POSITION pos, Unit* units, BUILDING* buildings, SPICE* spices, SANDWORM* sandworms);
BUILDING* createBuilding(BuildingType type, POSITION pos, BUILDING* head, FactionType faction);
BuildingType* listCanCreateBuilding(RESOURCE resource, bool firstcall);
int countCanCreateBuilding(RESOURCE resource);
int getCreateBuildingCmd(int user_input, BuildingType* canCreateList, int count);
void patrolTarget(Unit* unit);
void findEnemyByVision(Unit* head, Unit* currentUnit);
void executeBattle(Unit* currentUnit, Unit** head, char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]);
void getOtherUnitCommand(int user_input, POSITION cursor, Unit* selectedUnit, char* userCommand, GameState *gamestate);
void updateOtherUnit(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH], int sys_clock, Unit** units);

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

void actBuildSpace(CURSOR cursor, BuildingType building, RESOURCE* resource, Unit* units, BUILDING** buildings, SPICE* spices, SANDWORM* sandworms) {
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

bool handleBuildingCommand(BUILDING* building, Unit** units, int user_input, POSITION pos, RESOURCE* resource, char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]) {

    char command = BUILDINGATTRIBUTES[building->type].command;
    if ((char)user_input == command || (char)user_input == tolower(command)) {
        POSITION newPos = checkCanCreatePos(map, building); // 건물 2x2 사방에 생성가능한 비어있는 유닛 지형이 있으면 건설. 아니면 오류메시지 출력
        if (newPos.row != -1 && newPos.column != -1) {
            if (UNIT_ATTRIBUTES[inputToUnitType(user_input)].production_cost <= resource->spice) { //  해당 유닛을 만들만큼 스파이스 양이 충분한지 확인
                bool leftPopulation = checkPopulationCreateUnit(*resource);
                if (leftPopulation) {
                    resource->spice -= UNIT_ATTRIBUTES[inputToUnitType(user_input)].production_cost; // 스파이스 감소
                    *units = createUnit(inputToUnitType(user_input), newPos, *units, FACTION_PLAYER); // 플레이어 유닛 

                    insert_status_message("complete create %s", unitTypeToString(inputToUnitType(user_input)));
                    return true;
                }

                else {
                    insert_system_message("ERROR: population get max");
                }
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


void getHarvestCommand(Unit* selectedUnit, int user_input, POSITION cursor, SPICE* spice, GameState* gamestate) { 

    if ((char)user_input == 'M' || (char)user_input == 'm') {
        // 스파이스 위치 검사. 
        SPICE* currentSpice = spice;
        while (currentSpice != NULL) {
            if (currentSpice->position.row == cursor.row && currentSpice->position.column == cursor.column) {
                init_command();
                init_status();
                insert_system_message("sucessfully select spice");
                insert_status_message("selected spice amount: %d", currentSpice->amount);
                selectedUnit->targetSpice = currentSpice;
                selectedUnit->firstCommand = true; // 명령어 call 됨. 이렇게 되고 건드리면 안됨, 첫 명령을 완수하고 나서는 계속 루프를 돌거임. 
                //selectedUnit->isCommand = true; // 명령어를 받는 존재, 만약 명령어를 끝내고 본진으로 돌아오면 fasle
                *gamestate = STATE_DEFAULT;
                return;
            }
           

            currentSpice = currentSpice->next;
        }
       insert_system_message("you selected wrong position "); // m을 눌렀지만 스파이스 위치가 아니라면 반환. 
    }
    
        
    

    return;
}
POSITION getGoBackHome(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]) {
    POSITION upsideHome[3] = { {14,1}, {14,2}, {14,3} };
    for (int i = 0; i < 3; i++) {
       
        if (map[0][upsideHome[i].row][upsideHome[i].column] == ' ' && // 집 위에 3칸이 비어있으면 그 값을 리턴
            map[1][upsideHome[i].row][upsideHome[i].column] == -1) return upsideHome[i];
        
    }
    return (POSITION){ -1, -1 }; // 본진 위가 꽉 차있으면 잘못된 값 리턴. 
}

bool isNearBase(Unit* harvester) {
    int row = harvester->pos.row;
    int column = harvester->pos.column;
    POSITION upsideHome[3] = { {14,1}, {14,2}, {14,3} };
    for (int i = 0; i < 3; i++) {
        if (row == upsideHome[i].row && column == upsideHome[i].column) return true;
    }
    return false;
}

void extractSpice(Unit* currentUnit, SPICE* targetSpice) {
    int extractAmount = (rand() % 3) + 2;
    if (targetSpice->amount < extractAmount) extractAmount = targetSpice->amount; // 랜덤 추출량이 매장량보다 많으면 매장량을 추출량으로
    currentUnit->targetSpice->amount -= extractAmount; // 타겟스파이스의 양을 줄이기
    currentUnit->carrying_spice += extractAmount;// 추출 후 추출량 구조체에 저장. 
    insert_system_message("harvester just extract spice");
    insert_system_message("amount : %d", extractAmount);
}
void harvesterMove(Unit** units, char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH], RESOURCE* resource, int sys_clock, SPICE** spiceHead) {
    // 샌드웜의 이동 주기마다 움직임 처리
    if (sys_clock % 2000 != 0) {
        return;  // 아직 이동 시간이 되지 않았음
    }
    Unit* currentUnit = *units;

    //===================== 유닛 구조체를 참조해 하베스터일때 설정된 타겟이 있는지, 타겟을 들렀다가 본진으로 타겟을 변경했는지 참조해서 목적지 정하기 =======================
    while (currentUnit != NULL) {
        POSITION target_pos = { -1, -1 };
        if (currentUnit->type == HARVESTER) {
            if (currentUnit->firstCommand && currentUnit->targetSpice != NULL) { // 명령을 입력 받았고 타겟 스파이스가 널 포인터가 아니면. 
                if (!currentUnit->goBase)
                    target_pos = currentUnit->targetSpice->position;
                else {
                    target_pos = getGoBackHome(map); // 여기서 어떻게 처리
                }

            }
            //if (currentUnit->goBase) {
            //    target_pos = getGoBackHome(map); // 여기서 어떻게 처리
            //}

        }



        ////======유닛이 하베스터고 타겟 위치가 유효하다면 해당 방향으로 이동==========//
        if (target_pos.row > 0 && target_pos.column > 0) {
        

            if (currentUnit->goBase && isNearBase(currentUnit)) { // 집가는 길인데 집 가는 길에 도착하면
                currentUnit->goBase = false;
                resource->spice += currentUnit->carrying_spice; // 자원 늘어남 
                currentUnit->carrying_spice = 0;
                if (resource->spice > resource->spice_max) {
                    resource->spice = resource->spice_max; // 만약 보유한도량을 초과할 때 대처
                }
                if (currentUnit->targetSpice->amount <= 0) {
                    currentUnit->firstCommand = false; //  명령어 받는 기록도 초기화용
                    currentUnit->targetSpice = NULL;
                }
            }

            POSITION diff = psub(target_pos, currentUnit->pos);
            DIRECTION dir;

            if (abs(diff.row) >= abs(diff.column)) {
                dir = (diff.row >= 0) ? d_down : d_up;
            }
            else {
                dir = (diff.column >= 0) ? d_right : d_left;
            }

            POSITION next_pos = pmove(currentUnit->pos, dir);
            if (1 <= next_pos.row && next_pos.row <= MAP_HEIGHT - 2 &&
                1 <= next_pos.column && next_pos.column <= MAP_WIDTH - 2 &&
                map[0][next_pos.row][next_pos.column] == ' ' &&
                map[1][next_pos.row][next_pos.column] == -1) {

                map[1][currentUnit->pos.row][currentUnit->pos.column] = -1;  // 하베스터 위치를 비움
                setColor(currentUnit->pos, COLOR_DEFAULT);  // 색상 초기화
                currentUnit->pos = next_pos;
                displayUnit(map, next_pos, COLOR_FRIENDLY, 1, 1, 'H');
            }
            if (currentUnit->targetSpice != NULL) {
                if ((abs(currentUnit->pos.row - currentUnit->targetSpice->position.row) == 1 &&
                    currentUnit->pos.column == currentUnit->targetSpice->position.column) ||
                    (abs(currentUnit->pos.column - currentUnit->targetSpice->position.column) == 1 &&
                        currentUnit->pos.row == currentUnit->targetSpice->position.row)) { // 타겟 스파이스 옆에 도착하면. 
                    // 스파이스 추출 및 집가는 불리언 트루로 바꾸기. // 3초걸리게 어떻게 하지요.
                    currentUnit->goBase = true;
                    extractSpice(currentUnit, currentUnit->targetSpice);
                    char symbol = currentUnit->targetSpice->amount + '0';
                    displayUnit(map, currentUnit->targetSpice->position, COLOR_SPICE, 1, 0, symbol);
                    // extractSpice(currentUnit, currentUnit->targetSpice);

                    if (currentUnit->targetSpice->amount <= 0) {
                        toDefaultColorEmpty(currentUnit->targetSpice->position, map); // 유닛 삭제 display 함수
                        removeSpice(spiceHead, currentUnit->targetSpice);
                        //currentUnit->firstCommand = false; //  명령어 받는 기록도 초기화용
                        //currentUnit->targetSpice = NULL;
                    }
                }
            }

           
        }
        currentUnit = currentUnit->next;
    }
}
        //currentUnit = currentUnit->next;
    





void removeSpice(SPICE** head, SPICE* target) {
    if (head == NULL || *head == NULL || target == NULL) {
        return;
    }

    SPICE* current = *head;
    SPICE* previous = NULL;

    // 첫 번째 노드가 target인 경우
    if (current == target) {
        *head = current->next; // 헤드를 다음 노드로 변경
        free(current);         // 현재 노드 메모리 해제
        return;
    }

    // 연결 리스트 순회
    while (current != NULL) {
        if (current == target) {
            previous->next = current->next; // 이전 노드가 현재 노드의 다음 노드를 가리키도록 설정
            free(current);                  // 현재 노드 메모리 해제
            return;
        }
        previous = current;                 // 이전 노드를 갱신
        current = current->next;            // 다음 노드로 이동
    }

}


void getOtherUnitCommand(int user_input, POSITION cursor, Unit* selectedUnit, char* userCommand, GameState* gamestate) {

    //if (userCommand == 'M' || userCommand == 'm') {

    //}
    init_command();
    insert_command_message("Press space on target place");

    if (user_input == SPACEBYTE) { // 유저가 스페이스바를 누르면 명령어 수행


        if (*userCommand == 'P' || *userCommand == 'p') {
            selectedUnit->command = 'P';
            selectedUnit->target = cursor;
            selectedUnit->patrolPos = selectedUnit->pos; // patrol(순찰)명령을 받을 때 현재위치가 돌아와야할 위치. 
        }
        else if (*userCommand == 'M' || *userCommand == 'm') {
            selectedUnit->command = 'M';
            selectedUnit->target = cursor;
        }
        *gamestate = STATE_DEFAULT;
    }

    else if (user_input == ESCBYTE) {
        userCommand = '\0';
        *gamestate = STATE_DEFAULT;
    }
    return;
}

void updateOtherUnit(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH], int sys_clock, Unit** units) {
    Unit* currentUnit = *units;
    

    while (currentUnit != NULL) {
        if (currentUnit->type != HARVESTER) { // 하베스터 행동은 다른곳에서 처리하기에. 
            const UnitAttributes* attributes = &UNIT_ATTRIBUTES[currentUnit->type];
            findEnemyByVision(*units, currentUnit); // 이 함수에서는 현재유닛 기준에서 시야에 들어온 적군이 있으면 적군 구조체 저장 및 타겟 업데이트.

            if (currentUnit->target.row > 0 && currentUnit->target.column > 0 && currentUnit->command != '\0') {  // 타겟이 유효한지 확인. ,, --> 타겟으로 주기적으로 이동하는 함수.
                if (sys_clock % attributes->move_period != 0) {      // 이동주기 검사.
                    return;  // 아직 이동 시간이 되지 않았음
                }
                // 타겟이랑 유닛이랑 1칸 차이일 때 if 커맨드가 P이면 타겟을 patrol_pos로
                // 만약 patrolPos == pos가 같다면 command null 밑 패트롤 pos랑 타겟 둘다 0으로 
                patrolTarget(currentUnit);



                POSITION diff = psub(currentUnit->target, currentUnit->pos);
                DIRECTION dir;


                // 이동 방향 계산 (행, 열 차이에 따라 결정)
                if (abs(diff.row) >= abs(diff.column)) {
                    dir = (diff.row >= 0) ? d_down : d_up;
                }
                else {
                    dir = (diff.column >= 0) ? d_right : d_left;
                }

                // 다음 위치 계산
                POSITION next_pos = pmove(currentUnit->pos, dir);
                // 이동 가능한지 확인
                if (1 <= next_pos.row && next_pos.row <= MAP_HEIGHT - 2 &&
                    1 <= next_pos.column && next_pos.column <= MAP_WIDTH - 2 &&
                    map[0][next_pos.row][next_pos.column] == ' ' &&
                    map[1][next_pos.row][next_pos.column] == -1) {

                    // 현재 위치 비우기
                    map[1][currentUnit->pos.row][currentUnit->pos.column] = -1;
                    setColor(currentUnit->pos, COLOR_DEFAULT);

                    // 새로운 위치로 이동
                    currentUnit->pos = next_pos;
                    int color;
                    if (currentUnit->isally)color = COLOR_FRIENDLY;
                    else color = COLOR_ENEMY;
                    displayUnit(map, next_pos, color, 1, 1, attributes->symbol);
                }

                // 여기에 현재 유닛포인터와 어쩌구를 넣어서 전투 확인
                executeBattle(currentUnit, units, map);

            }
        }
        currentUnit = currentUnit->next;

    }
}

void patrolTarget(Unit* unit) {
    if ((abs(unit->target.row - unit->pos.row) == 1 && unit->target.column == unit->pos.column) ||
        (abs(unit->target.column - unit->pos.column) == 1 && unit->target.row == unit->pos.row)) {     // 좌표평면에서 유닛의 위치랑 타겟의 위치가 1 차이난다(근접하다면)
        if (unit->command == 'P') unit->target = unit->patrolPos; // 순찰 중이었다면 인접하면 패트롤로 돌아가기. 
    }

    if (unit->pos.row == unit->patrolPos.row && unit->pos.column == unit->patrolPos.column && 
        unit->patrolPos.row == unit->target.row && unit->patrolPos.column == unit->target.column) {   // 순찰 중 && 목적지가 순찰지로 번경됨 && 현재위치가 순찰지라면 순찰갔다가 돌아온거니 해당 처리. 
        unit->command = '\0';
        unit->patrolPos = (POSITION){ -1,-1 };
        unit->target = (POSITION){ -1,-1 };
    }
}

void findEnemyByVision(Unit* head, Unit* currentUnit) {
    const UnitAttributes* attributes = &UNIT_ATTRIBUTES[currentUnit->type];
    while (head != NULL) {
        // 현재 유닛과의 거리 계산
        int row_diff = abs(head->pos.row - currentUnit->pos.row);
        int col_diff = abs(head->pos.column - currentUnit->pos.column);

        // 조건: vision 거리 이내 & 소속이 다름
        if (row_diff <= attributes->vision && col_diff <= attributes->vision &&  // --> 현재 유닛기준에서 적군 유닛이 시야에 들어왔는지 확인, 커맨드가 입력된 상태였으면 수행하기에 
            head->isally != currentUnit->isally) {

            currentUnit->findEnemy = head; //  적군을 찾았으니 적군 정보 저장
            currentUnit->target = head->pos;

        }
        head = head->next;
    }
}



void executeBattle(Unit * currentUnit, Unit** head, char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]) {
    const UnitAttributes* attributes = &UNIT_ATTRIBUTES[currentUnit->type];
    if (currentUnit->findEnemy == NULL) return;

    Unit* enemy = currentUnit->findEnemy;
    if (abs(currentUnit->pos.row - enemy->pos.row) + abs(currentUnit->pos.column - enemy->pos.column) == 1) { //적군과 이거랑 그 한칸 차이라면
        enemy->health -= attributes->attack_power;
        if (!enemy->isally) { // 적군이 공격당했을 때
            insert_system_message("attack enemy %s", unitTypeToString(enemy->type));
            insert_system_message("enemy health %d -> %d", enemy->health + attributes->attack_power, enemy->health);

        }
        else { //  아군이 공격당했을 때
            insert_system_message("%s attackted by enemy", unitTypeToString(enemy->type));
            insert_system_message("health %d -> %d", enemy->health + attributes->attack_power, enemy->health);
        }

     
    }

    if (enemy->health <= 0) { // 적군이 그 0이 되었으면!!
        insert_system_message("delete %s", unitTypeToString(enemy->type));
        toDefaultColorEmpty(enemy->pos, map);
        removeUnit(head, enemy); // 연결 리스트에서 유닛 제거
        
        currentUnit->findEnemy = NULL; // 적군을 제거하면 적군 유닛 포인터 null
    }
        
    
}