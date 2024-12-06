
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

// =========rock�� ������ ��ġ�� ������ ���� ���̱� ������ ����� ����=============//
const POSITION rock_positions[10] = {  // 2x2 ������ 4���� ��ǥ�� �����ϹǷ� 4�� ũ��
    {10, 30}, {10, 31}, {11, 30}, {11, 31}, // ���� 1 (2x2)
    {5, 15}, {5, 16}, {6, 15}, {6, 16},   // ���� 2 (2x2)
    {8, 42}, // ���� 3 (1x1)
    {15, 13} // ���� 4 (1x1)
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

        // �÷��̾��� Faction�� �´� �ǹ����� Ȯ��
        if (attr->faction != FACTION_COMMON && attr->faction != FACTION_PLAYER) {
            continue;
        }
        if (attr->type == BASE) {
            continue;
        }

        // �ڿ��� ������� Ȯ��
        if (resource.spice < attr->cost) {
            continue;
        }
        count++;
        
    }
    return count;

}


BuildingType* listCanCreateBuilding(RESOURCE resource, bool firstcall) {
    int count = 0;

    // �������� �޸� �Ҵ� (�ִ� �ǹ� ����)
    BuildingType* canCreateBuildingList = (BuildingType*)malloc(NUM_BUILDING_TYPES * sizeof(BuildingType));
    if (!canCreateBuildingList) {
        // �޸� �Ҵ� ���� �� ó��
        insert_command_message("Error: Memory allocation failed.");
        return NULL;
    }

    for (int i = 0; i < NUM_BUILDING_TYPES; i++) {
        const BuildingAttributes* attr = &BUILDINGATTRIBUTES[i];

        // �÷��̾��� Faction�� �´� �ǹ����� Ȯ��
        if (attr->faction != FACTION_COMMON && attr->faction != FACTION_PLAYER) {
            continue;
        }
        if (attr->type == BASE) {
            continue;
        }

        // �ڿ��� ������� Ȯ��
        if (resource.spice < attr->cost) {
            continue;
        }

        if (firstcall) // ù��° �Լ� ȣ���϶��� ���â�� �Է�. 
            // �ǹ��� �Ǽ� �������� ǥ��
        {
            insert_command_message("%c: %s SPICE: %d ",
                attr->symbol,
                buildingTypeToString(attr->type),
                attr->cost
            );
        }

        // �ǹ� ���� �߰�
        canCreateBuildingList[count] = attr->type;
        count++;
    }

    // �ʿ��� ��ŭ�� �޸� ũ�⸦ ����
    canCreateBuildingList = (BuildingType*)realloc(canCreateBuildingList, count * sizeof(BuildingType));
    if (!canCreateBuildingList && count > 0) {
        // realloc ���� �� ó��
        insert_command_message("Error: Memory reallocation failed.");
        return NULL;
    }

    return canCreateBuildingList; // ���� �迭 ��ȯ
}

int getCreateBuildingCmd(int user_input, BuildingType* canCreateList, int count) { 
    // �Ǽ� ������ ���� ����� ������ -1 ��ȯ
    if (canCreateList == NULL || count <= 0) {
        return -1;
    };
    for (int i = 0; i < NUM_BUILDING_TYPES; i++) { // BASE�� ����(attr[0])
        const BuildingAttributes* attr = &BUILDINGATTRIBUTES[i];

        // �Էµ� ��ɾ BUILDINGATTRIBUTES�� �ִ��� Ȯ��
        if (attr->symbol == (char)user_input || tolower(attr->symbol) == (char)user_input) {
            // �� ������ �Ǽ� ������ ��Ͽ� ���ԵǾ� �ִ��� Ȯ��
            for (int j = 0; j < count; j++) {
                if (canCreateList[j] == attr->type) {
                    return (int)attr->type; // ��ȿ�� ���� Ÿ�� ��ȯ
                }
            }
            //break; // �Է� ��ɾ�� ���������� �Ǽ� �Ұ�
        }
    }

    return -1; // ��ȿ���� ���� �Է�
}


void buildStateAct(int userInput, POSITION cursor, RESOURCE resource, int* buildingEnum,bool firstCall) {
    BuildingType* canBuildList = listCanCreateBuilding(resource, firstCall);
    int count = countCanCreateBuilding(resource);
    *buildingEnum = getCreateBuildingCmd(userInput, canBuildList, count);
}


bool attemp_building(CURSOR cursor, BuildingType building, Unit* units, BUILDING* buildings, SPICE* spices, SANDWORM* sandworms) {
    // 2x2 ���� �˻�
    ObjectInfo result[4];
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            // �� ��ġ�� ��ü�� �ִ��� Ȯ��
            result[i * 2 + j] = checkObjectAtPosition((POSITION) { cursor.current.row + i, cursor.current.column + j }, units, buildings, spices, sandworms);
        }
    }

    int flag = 0;
    // �ش� �ڸ��� �ٸ� ��ü�� ������ �Ǽ� �Ұ�
    for (int i = 0; i < 4; i++) {
        if (result[i].type != OBJECT_NONE) { // OBJECT_NONE�̸� ��� �ִ� �ڸ�
            // �Ǽ��ؾ� �Ǵ� �ڸ��� �ٸ� ��ü�� ������ �Ǽ� �Ұ�
            if (result[0].type == OBJECT_BUILDING) {
                BUILDING* thisBuilding = (BUILDING*)result[0].object; {
                    if (thisBuilding->type == PLATE) {
                        return true;
                    }
                }
            }
            flag = 1; // ��ġ�� ������Ʈ�� �ִ�. 
        }

        
        
    }

    if (!flag && building == PLATE) { // ��ġ�� ������Ʈ�� ���� ������ �����̸� �Ǽ�����
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
// �� ��� Ȯ�� �Լ�
bool isWithinBounds(POSITION pos) {
    return pos.row >= 0 && pos.row < MAP_HEIGHT &&
        pos.column >= 0 && pos.column < MAP_WIDTH;
}

// Ư�� ��ǥ�� ���� �������� Ȯ�� (map[0]�� map[1] ��� ��� �־�� ��)
bool isValidPosition(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH], POSITION pos) {
    return isWithinBounds(pos) &&
        map[0][pos.row][pos.column] == ' ' &&  // ���� ���̾� ��� ����
        map[1][pos.row][pos.column] == -1;    // ������Ʈ ���̾� ��� ����
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

    // 2x2 �ǹ��� ���� 12ĭ ��ǥ (�ð� ����)
    int directions[12][2] = {
        {-1, -1}, {-1,  0}, {-1,  1}, {-1,  2}, // ���� 4ĭ
        { 0, -1},                   { 0,  2},  // �¿� 2ĭ
        { 1, -1},                   { 1,  2},  // �¿� 2ĭ
        { 2, -1}, { 2,  0}, { 2,  1}, { 2,  2}  // �Ʒ��� 4ĭ
    };

    for (int i = 0; i < 12; i++) {
        POSITION newPos = { baseRow + directions[i][0], baseCol + directions[i][1] };

        // Layer 0�� Layer 1 ��� ��ȿ�� ��ġ���� Ȯ��
        if (isValidPosition(map, newPos)) {
            return newPos;
        }
    }

    // ��ȿ�� ��ġ�� ������ (-1, -1) ��ȯ
    POSITION invalidPos = { -1, -1 };
    return invalidPos;
}

bool handleBuildingCommand(BUILDING* building, Unit** units, int user_input, POSITION pos, RESOURCE* resource, char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]) {

    char command = BUILDINGATTRIBUTES[building->type].command;
    if ((char)user_input == command || (char)user_input == tolower(command)) {
        POSITION newPos = checkCanCreatePos(map, building); // �ǹ� 2x2 ��濡 ���������� ����ִ� ���� ������ ������ �Ǽ�. �ƴϸ� �����޽��� ���
        if (newPos.row != -1 && newPos.column != -1) {
            if (UNIT_ATTRIBUTES[inputToUnitType(user_input)].production_cost <= resource->spice) { //  �ش� ������ ���鸸ŭ �����̽� ���� ������� Ȯ��
                bool leftPopulation = checkPopulationCreateUnit(*resource);
                if (leftPopulation) {
                    resource->spice -= UNIT_ATTRIBUTES[inputToUnitType(user_input)].production_cost; // �����̽� ����
                    *units = createUnit(inputToUnitType(user_input), newPos, *units, FACTION_PLAYER); // �÷��̾� ���� 

                    insert_status_message("complete create %s", unitTypeToString(inputToUnitType(user_input)));
                    return true;
                }

                else {
                    insert_system_message("ERROR: population get max");
                }
            }
            else {
                insert_system_message("no enough spice. this unit costs: %d", UNIT_ATTRIBUTES[inputToUnitType(user_input)].production_cost); // �����̽��� ������ �ý��� â ���
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
        // �����̽� ��ġ �˻�. 
        SPICE* currentSpice = spice;
        while (currentSpice != NULL) {
            if (currentSpice->position.row == cursor.row && currentSpice->position.column == cursor.column) {
                init_command();
                init_status();
                insert_system_message("sucessfully select spice");
                insert_status_message("selected spice amount: %d", currentSpice->amount);
                selectedUnit->targetSpice = currentSpice;
                selectedUnit->firstCommand = true; // ��ɾ� call ��. �̷��� �ǰ� �ǵ帮�� �ȵ�, ù ����� �ϼ��ϰ� ������ ��� ������ ������. 
                //selectedUnit->isCommand = true; // ��ɾ �޴� ����, ���� ��ɾ ������ �������� ���ƿ��� fasle
                *gamestate = STATE_DEFAULT;
                return;
            }
           

            currentSpice = currentSpice->next;
        }
       insert_system_message("you selected wrong position "); // m�� �������� �����̽� ��ġ�� �ƴ϶�� ��ȯ. 
    }
    
        
    

    return;
}
POSITION getGoBackHome(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]) {
    POSITION upsideHome[3] = { {14,1}, {14,2}, {14,3} };
    for (int i = 0; i < 3; i++) {
       
        if (map[0][upsideHome[i].row][upsideHome[i].column] == ' ' && // �� ���� 3ĭ�� ��������� �� ���� ����
            map[1][upsideHome[i].row][upsideHome[i].column] == -1) return upsideHome[i];
        
    }
    return (POSITION){ -1, -1 }; // ���� ���� �� �������� �߸��� �� ����. 
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
    if (targetSpice->amount < extractAmount) extractAmount = targetSpice->amount; // ���� ���ⷮ�� ���差���� ������ ���差�� ���ⷮ����
    currentUnit->targetSpice->amount -= extractAmount; // Ÿ�ٽ����̽��� ���� ���̱�
    currentUnit->carrying_spice += extractAmount;// ���� �� ���ⷮ ����ü�� ����. 
    insert_system_message("harvester just extract spice");
    insert_system_message("amount : %d", extractAmount);
}
void harvesterMove(Unit** units, char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH], RESOURCE* resource, int sys_clock, SPICE** spiceHead) {
    // ������� �̵� �ֱ⸶�� ������ ó��
    if (sys_clock % 2000 != 0) {
        return;  // ���� �̵� �ð��� ���� �ʾ���
    }
    Unit* currentUnit = *units;

    //===================== ���� ����ü�� ������ �Ϻ������϶� ������ Ÿ���� �ִ���, Ÿ���� �鷶�ٰ� �������� Ÿ���� �����ߴ��� �����ؼ� ������ ���ϱ� =======================
    while (currentUnit != NULL) {
        POSITION target_pos = { -1, -1 };
        if (currentUnit->type == HARVESTER) {
            if (currentUnit->firstCommand && currentUnit->targetSpice != NULL) { // ����� �Է� �޾Ұ� Ÿ�� �����̽��� �� �����Ͱ� �ƴϸ�. 
                if (!currentUnit->goBase)
                    target_pos = currentUnit->targetSpice->position;
                else {
                    target_pos = getGoBackHome(map); // ���⼭ ��� ó��
                }

            }
            //if (currentUnit->goBase) {
            //    target_pos = getGoBackHome(map); // ���⼭ ��� ó��
            //}

        }



        ////======������ �Ϻ����Ͱ� Ÿ�� ��ġ�� ��ȿ�ϴٸ� �ش� �������� �̵�==========//
        if (target_pos.row > 0 && target_pos.column > 0) {
        

            if (currentUnit->goBase && isNearBase(currentUnit)) { // ������ ���ε� �� ���� �濡 �����ϸ�
                currentUnit->goBase = false;
                resource->spice += currentUnit->carrying_spice; // �ڿ� �þ 
                currentUnit->carrying_spice = 0;
                if (resource->spice > resource->spice_max) {
                    resource->spice = resource->spice_max; // ���� �����ѵ����� �ʰ��� �� ��ó
                }
                if (currentUnit->targetSpice->amount <= 0) {
                    currentUnit->firstCommand = false; //  ��ɾ� �޴� ��ϵ� �ʱ�ȭ��
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

                map[1][currentUnit->pos.row][currentUnit->pos.column] = -1;  // �Ϻ����� ��ġ�� ���
                setColor(currentUnit->pos, COLOR_DEFAULT);  // ���� �ʱ�ȭ
                currentUnit->pos = next_pos;
                displayUnit(map, next_pos, COLOR_FRIENDLY, 1, 1, 'H');
            }
            if (currentUnit->targetSpice != NULL) {
                if ((abs(currentUnit->pos.row - currentUnit->targetSpice->position.row) == 1 &&
                    currentUnit->pos.column == currentUnit->targetSpice->position.column) ||
                    (abs(currentUnit->pos.column - currentUnit->targetSpice->position.column) == 1 &&
                        currentUnit->pos.row == currentUnit->targetSpice->position.row)) { // Ÿ�� �����̽� ���� �����ϸ�. 
                    // �����̽� ���� �� ������ �Ҹ��� Ʈ��� �ٲٱ�. // 3�ʰɸ��� ��� ������.
                    currentUnit->goBase = true;
                    extractSpice(currentUnit, currentUnit->targetSpice);
                    char symbol = currentUnit->targetSpice->amount + '0';
                    displayUnit(map, currentUnit->targetSpice->position, COLOR_SPICE, 1, 0, symbol);
                    // extractSpice(currentUnit, currentUnit->targetSpice);

                    if (currentUnit->targetSpice->amount <= 0) {
                        toDefaultColorEmpty(currentUnit->targetSpice->position, map); // ���� ���� display �Լ�
                        removeSpice(spiceHead, currentUnit->targetSpice);
                        //currentUnit->firstCommand = false; //  ��ɾ� �޴� ��ϵ� �ʱ�ȭ��
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

    // ù ��° ��尡 target�� ���
    if (current == target) {
        *head = current->next; // ��带 ���� ���� ����
        free(current);         // ���� ��� �޸� ����
        return;
    }

    // ���� ����Ʈ ��ȸ
    while (current != NULL) {
        if (current == target) {
            previous->next = current->next; // ���� ��尡 ���� ����� ���� ��带 ����Ű���� ����
            free(current);                  // ���� ��� �޸� ����
            return;
        }
        previous = current;                 // ���� ��带 ����
        current = current->next;            // ���� ���� �̵�
    }

}


void getOtherUnitCommand(int user_input, POSITION cursor, Unit* selectedUnit, char* userCommand, GameState* gamestate) {

    //if (userCommand == 'M' || userCommand == 'm') {

    //}
    init_command();
    insert_command_message("Press space on target place");

    if (user_input == SPACEBYTE) { // ������ �����̽��ٸ� ������ ��ɾ� ����


        if (*userCommand == 'P' || *userCommand == 'p') {
            selectedUnit->command = 'P';
            selectedUnit->target = cursor;
            selectedUnit->patrolPos = selectedUnit->pos; // patrol(����)����� ���� �� ������ġ�� ���ƿ;��� ��ġ. 
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
        if (currentUnit->type != HARVESTER) { // �Ϻ����� �ൿ�� �ٸ������� ó���ϱ⿡. 
            const UnitAttributes* attributes = &UNIT_ATTRIBUTES[currentUnit->type];
            findEnemyByVision(*units, currentUnit); // �� �Լ������� �������� ���ؿ��� �þ߿� ���� ������ ������ ���� ����ü ���� �� Ÿ�� ������Ʈ.

            if (currentUnit->target.row > 0 && currentUnit->target.column > 0 && currentUnit->command != '\0') {  // Ÿ���� ��ȿ���� Ȯ��. ,, --> Ÿ������ �ֱ������� �̵��ϴ� �Լ�.
                if (sys_clock % attributes->move_period != 0) {      // �̵��ֱ� �˻�.
                    return;  // ���� �̵� �ð��� ���� �ʾ���
                }
                // Ÿ���̶� �����̶� 1ĭ ������ �� if Ŀ�ǵ尡 P�̸� Ÿ���� patrol_pos��
                // ���� patrolPos == pos�� ���ٸ� command null �� ��Ʈ�� pos�� Ÿ�� �Ѵ� 0���� 
                patrolTarget(currentUnit);



                POSITION diff = psub(currentUnit->target, currentUnit->pos);
                DIRECTION dir;


                // �̵� ���� ��� (��, �� ���̿� ���� ����)
                if (abs(diff.row) >= abs(diff.column)) {
                    dir = (diff.row >= 0) ? d_down : d_up;
                }
                else {
                    dir = (diff.column >= 0) ? d_right : d_left;
                }

                // ���� ��ġ ���
                POSITION next_pos = pmove(currentUnit->pos, dir);
                // �̵� �������� Ȯ��
                if (1 <= next_pos.row && next_pos.row <= MAP_HEIGHT - 2 &&
                    1 <= next_pos.column && next_pos.column <= MAP_WIDTH - 2 &&
                    map[0][next_pos.row][next_pos.column] == ' ' &&
                    map[1][next_pos.row][next_pos.column] == -1) {

                    // ���� ��ġ ����
                    map[1][currentUnit->pos.row][currentUnit->pos.column] = -1;
                    setColor(currentUnit->pos, COLOR_DEFAULT);

                    // ���ο� ��ġ�� �̵�
                    currentUnit->pos = next_pos;
                    int color;
                    if (currentUnit->isally)color = COLOR_FRIENDLY;
                    else color = COLOR_ENEMY;
                    displayUnit(map, next_pos, color, 1, 1, attributes->symbol);
                }

                // ���⿡ ���� ���������Ϳ� ��¼���� �־ ���� Ȯ��
                executeBattle(currentUnit, units, map);

            }
        }
        currentUnit = currentUnit->next;

    }
}

void patrolTarget(Unit* unit) {
    if ((abs(unit->target.row - unit->pos.row) == 1 && unit->target.column == unit->pos.column) ||
        (abs(unit->target.column - unit->pos.column) == 1 && unit->target.row == unit->pos.row)) {     // ��ǥ��鿡�� ������ ��ġ�� Ÿ���� ��ġ�� 1 ���̳���(�����ϴٸ�)
        if (unit->command == 'P') unit->target = unit->patrolPos; // ���� ���̾��ٸ� �����ϸ� ��Ʈ�ѷ� ���ư���. 
    }

    if (unit->pos.row == unit->patrolPos.row && unit->pos.column == unit->patrolPos.column && 
        unit->patrolPos.row == unit->target.row && unit->patrolPos.column == unit->target.column) {   // ���� �� && �������� �������� ����� && ������ġ�� ��������� �������ٰ� ���ƿ°Ŵ� �ش� ó��. 
        unit->command = '\0';
        unit->patrolPos = (POSITION){ -1,-1 };
        unit->target = (POSITION){ -1,-1 };
    }
}

void findEnemyByVision(Unit* head, Unit* currentUnit) {
    const UnitAttributes* attributes = &UNIT_ATTRIBUTES[currentUnit->type];
    while (head != NULL) {
        // ���� ���ְ��� �Ÿ� ���
        int row_diff = abs(head->pos.row - currentUnit->pos.row);
        int col_diff = abs(head->pos.column - currentUnit->pos.column);

        // ����: vision �Ÿ� �̳� & �Ҽ��� �ٸ�
        if (row_diff <= attributes->vision && col_diff <= attributes->vision &&  // --> ���� ���ֱ��ؿ��� ���� ������ �þ߿� ���Դ��� Ȯ��, Ŀ�ǵ尡 �Էµ� ���¿����� �����ϱ⿡ 
            head->isally != currentUnit->isally) {

            currentUnit->findEnemy = head; //  ������ ã������ ���� ���� ����
            currentUnit->target = head->pos;

        }
        head = head->next;
    }
}



void executeBattle(Unit * currentUnit, Unit** head, char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]) {
    const UnitAttributes* attributes = &UNIT_ATTRIBUTES[currentUnit->type];
    if (currentUnit->findEnemy == NULL) return;

    Unit* enemy = currentUnit->findEnemy;
    if (abs(currentUnit->pos.row - enemy->pos.row) + abs(currentUnit->pos.column - enemy->pos.column) == 1) { //������ �̰Ŷ� �� ��ĭ ���̶��
        enemy->health -= attributes->attack_power;
        if (!enemy->isally) { // ������ ���ݴ����� ��
            insert_system_message("attack enemy %s", unitTypeToString(enemy->type));
            insert_system_message("enemy health %d -> %d", enemy->health + attributes->attack_power, enemy->health);

        }
        else { //  �Ʊ��� ���ݴ����� ��
            insert_system_message("%s attackted by enemy", unitTypeToString(enemy->type));
            insert_system_message("health %d -> %d", enemy->health + attributes->attack_power, enemy->health);
        }

     
    }

    if (enemy->health <= 0) { // ������ �� 0�� �Ǿ�����!!
        insert_system_message("delete %s", unitTypeToString(enemy->type));
        toDefaultColorEmpty(enemy->pos, map);
        removeUnit(head, enemy); // ���� ����Ʈ���� ���� ����
        
        currentUnit->findEnemy = NULL; // ������ �����ϸ� ���� ���� ������ null
    }
        
    
}