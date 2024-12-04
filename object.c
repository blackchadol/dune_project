
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

bool handleBuildingCommand(BUILDING* building, Unit* units, int user_input, POSITION pos, RESOURCE* resource, char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]) {

    if ((char)user_input == BUILDINGATTRIBUTES[building->type].command) {
        POSITION newPos = checkCanCreatePos(map, building); // �ǹ� 2x2 ��濡 ���������� ����ִ� ���� ������ ������ �Ǽ�. �ƴϸ� �����޽��� ���
        if (newPos.row != -1 && newPos.column != -1) {
            if (UNIT_ATTRIBUTES[inputToUnitType(user_input)].production_cost <= resource->spice) { //  �ش� ������ ���鸸ŭ �����̽� ���� ������� Ȯ��
                resource->spice -= UNIT_ATTRIBUTES[inputToUnitType(user_input)].production_cost; // �����̽� ����
                createUnit(inputToUnitType(user_input), newPos, units, FACTION_PLAYER); // �÷��̾� ���� 
                insert_status_message("complete create %s", unitTypeToString(inputToUnitType(user_input)));
                return true;
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