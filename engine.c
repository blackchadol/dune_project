/*
1-1. ASCII ART�� intro �Լ� �ۼ�
1-2. common.h�� OBJECT_SAMPLE ����ü�� speed ��� �߰�
1-3. ������Ͽ� object.h ���� �߰� �� ���ֵ� �Ӽ��� ������ ����ü �� �迭 ����
1-4. ������ ������ġ ����ü �� ������ġ ����ü�� ����(���Ḯ��Ʈ�� �����ϱ� ���� ������ ����)
1-5. ȭ�鿡 ǥ���� ���� ����Ǽ��� object.h�� ���������� �����ϰ� ����� �ش� ����� �������� ������ ����� ����, 
	 ���Ŀ� switch/case ������ �ش� ���� �� �ǹ��� ������ �����ϴ� �Լ� �ۼ��� ����.
1-6 �ǹ� ������ġ ����ü �� ������ġ ����ü ����(���Ḯ��Ʈ ������ ����)
1-7 object.h�� �ǹ� �� ���� ������ġ ����ü�� �Ʊ��Ǽ� ���� or ���� �Ǽ� ���� or ����Ǽ����� ���� ���� �� 
������� �߸������� ����� ���� ó���ϱ�� �Ͽ� ����.
1-8 object.h�� �����̽� �� ����� ����ü �߰�. 
1-9 object.h�� �ʿ� ��ü�� ǥ���ϱ� ���� ���� ���� 16������(���� 4��Ʈ�� ����, �� 4��Ʈ�� �۾���)
�۾����� �Ͼ������ ����
1-10 engine.c�� create_player_unit--> �Ʊ����� ����ü�� ���Ḯ��Ʈ�� �����ϴ� �Լ����� 
1-11 display.c�� map color�� �����迭�� ������ color_default�� �ʱ�ȭ �� displayUnit �Լ��� char, ������ map�� ǥ���ϱ� ���� ������ �迭�� ����
�� display_map �Լ��� colorMap�迭�� ������ ������ ǥ���ϵ��� �ٲ�
1-12 display.c�� displatunit �Լ����� 2x2 �������� ���� �׸��� ��ĭ�� �׸��� �ʴ� ������ �߻��� . ���� ��ü 
1-13 createUnit �Լ��� �Ʊ� ���� ���о��ϰ� �ѹ��� �����ϵ��� ����. ���Ḯ��Ʈ������ faction ����� ����. 
1-14 createBuilding �Լ����� -> layer 0�� �ǹ� ���� 
1-15 create sandworm, spice �Լ�����, startObject ���Լ��� ���� ���� �ʱ���� ǥ��
1-16 ����â ���� �� display_status �Լ� ���� �� 
insert_status_message() �Լ� ���� -> ���ڿ� ����� �Է��ϸ� ����â�� �Է�
1-17. �Ȱ��� ������� �ý��۸޽���, ���â ���� �� ���ڿ� �Է� �Լ� ����

2-1 Ŀ���� �������� ������ �� Ŀ�������� �ٲ�� Ŀ���� �������� �ٽ� ���������� ���ƿ����� �ڵ����. 
2-2 ����Ű�� �������� �ι� ������ �� 3ĭ �� �����̵��� cursur_move �� main �Լ� ����
2-3. �����̽��ٸ� ������ �� ��� ������Ʈ ���Ḯ��Ʈ�� �˻��Ͽ� �ش���ġ�� �ִ� ������Ʈ Ÿ���� enum���� �����Ͽ� ��ȯ�ϴ� �Լ��ۼ�. 
2-4. �����̽��ٸ� ������ �� ����â �� ��ɾ� ��� ����
*/



#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "common.h"
#include "io.h"
#include "display.h"
#include "object.h"


#define DOUBLE_PRESS_INTERVAL 300  // �� �� �Է� ���� �ִ� �ð� ���� (�и���)
#define MOVE_STEP_SINGLE 1         // �⺻ �̵� ĭ ��
#define MOVE_STEP_DOUBLE 3         // �� �� �Է� �� �̵� ĭ ��

void init(void);
void intro(void);
void outro(void);
void cursor_move(DIRECTION dir, int steps);
void sample_obj_move(void);
void startObject(Unit** units, BUILDING** buildings, SPICE** spice, SANDWORM** sandworm);
Unit* createUnit(UnitType type, POSITION pos, Unit* head, FactionType faction);
BUILDING* createBuilding(BuildingType type, POSITION pos, BUILDING* head, FactionType faction);
POSITION sample_obj_next_position(void);
SANDWORM* createSandworm(POSITION pos, SANDWORM* head);
SPICE* createSpice(int amount, POSITION pos, SPICE* head);
ObjectInfo checkObjectAtPosition(POSITION pos, Unit* units, BUILDING* buildings, SPICE* spices, SANDWORM* sandworms);
void displayObjectInfoAtPosition(POSITION pos, Unit* units, BUILDING* buildings, SPICE* spices, SANDWORM* sandworms);
void handleSpacebarPress(POSITION cursorPosition, Unit* units, BUILDING* buildings, SPICE* spices, SANDWORM* sandworms);
/* ================= control =================== */
int sys_clock = 0;		// system-wide clock(ms)
CURSOR cursor = { { 1, 1 }, {1, 1} };
clock_t last_key_time = 0;          // ������ Ű �Է� �ð�
KEY last_key = k_none;              // ���������� ���� Ű ��

/* ================= game data =================== */
char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH] = { 0 };
RESOURCE resource = {
	.spice = 0,
	.spice_max = 0,
	.population = 0,
	.population_max = 0
};

OBJECT_SAMPLE obj = {
	.pos = {1, 1},
	.dest = {MAP_HEIGHT - 2, MAP_WIDTH - 2},
	.repr = 'o',
	.speed = 300,
	.next_move_time = 300
};

/* ================= main() =================== */
int main(void) {
	srand((unsigned int)time(NULL));

	init();
	init_status();
	init_system_message();
	init_command();
	intro();
    init_colorMap();
	Unit* units = NULL; // ��ü ���Ḯ��Ʈ �ʱ�ȭ
	BUILDING* buildings = NULL;
	SPICE* spice = NULL;
	SANDWORM* sandworm = NULL;
	startObject(&units,&buildings,&spice,&sandworm);
	display(resource, map, cursor);
	insert_status_message("%d",&units->pos.row);
	while (1) {
		// loop �� ������(��, TICK==10ms����) Ű �Է� Ȯ��
		KEY key = get_key();
		int steps = 1;
		
		// Ű �Է��� ������ ó��
		if (is_arrow_key(key)) {
			clock_t current_time = clock();
			// ���� Ű�� ª�� �ð� ���� ���� �� �� �Էµ� ���
			if (key == last_key && (current_time - last_key_time) * 1000 / CLOCKS_PER_SEC <= DOUBLE_PRESS_INTERVAL) {
				steps = 3;  // �� �� ������ �Է� �� 3ĭ �̵�
			}
			cursor_move(ktod(key),steps);
			// ���� Ű�� �ð��� ����Ͽ� ���� �Է°� ��
			last_key = key;
			last_key_time = current_time;
		}
		else {
			// ����Ű ���� �Է�
			switch (key) {
			case k_quit: outro();
			case k_none:
				break;
			case k_undef:
				break;
			case k_space: {
				handleSpacebarPress(cursor.current, units, buildings, spice, sandworm );
				break;
			}

			default: break;
			}
		}

		
		display(resource, map, cursor);
		Sleep(TICK);
		sys_clock += 10;
	}
}

/* ================= subfunctions =================== */
void intro(void) {
	printf("                              ~~~~~~~~~~~~\n");
	printf("                         ~~~~~~~~~~~  ~~~  ~~~~~~~~~~\n");
	printf("                      ~~~    ~~~~~~~~~~~~~~~   ~~~~~~~\n");
	printf("                   ~~~~~    ~~~~    ~~~~~~~~~~~~    ~~~~\n");
	printf("                ~~~~~~   ~~~       ~~~~~    ~~~~~~     ~~~\n");
	printf("             ~~~~~~~   ~~         ~~   ~~      ~~~~~    ~~~\n");
	printf("           ~~~~~~~   ~~                      ~~~~~~~      ~~~\n");
	printf("        ~~~~~~~    ~~                        ~~~~~~~~     ~~~~\n");
	printf("     ~~~~~~~~~~     ~                        ~~~~~~~~      ~~~~~\n");
	printf("   ~~~~~~~~~~~~~               __             ~~~~~~      ~~~~~~~~\n");
	printf(" ~~~~~~~~~~~~~~~~~          __/o \\__            ~~~~    ~~~~~~~~~~~~\n");
	printf("~~~~~~~~~~~~~~~~~~~        /o \\   /o\\              ~~~~~~~~~~~~~~~~~~\n");
	printf("~~~~~~~~~~~~~~~~~~~~~~~   ~~~~~~~~~~~~~~~~~    ~~~~~~~~~~~~~~~~~~~~~~\n");
	printf("~~~~~~~~~~~~~~~~~~~~~~~   ~~~~~~~~~~~~~~~~~    ~~~~~~~~~~~~~~~~~~~~~~\n");
	printf("\n");
	printf("                           DUNE: THE SANDS OF TIME\n");
	Sleep(2000);
	system("cls");
}

void outro(void) {
	printf("exiting...\n");
	exit(0);
}

void init(void) {
	// layer 0(map[0])�� ���� ����
	for (int j = 0; j < MAP_WIDTH; j++) {
		map[0][0][j] = '#';
		map[0][MAP_HEIGHT - 1][j] = '#';
	}

	for (int i = 1; i < MAP_HEIGHT - 1; i++) {
		map[0][i][0] = '#';
		map[0][i][MAP_WIDTH - 1] = '#';
		for (int j = 1; j < MAP_WIDTH - 1; j++) {
			map[0][i][j] = ' ';
		}
	}

	// layer 1(map[1])�� ��� �α�(-1�� ä��)
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			map[1][i][j] = -1;
		}
	}

	// object sample
	//map[1][obj.pos.row][obj.pos.column] = 'o';
}

// (�����ϴٸ�) ������ �������� Ŀ�� �̵�
void cursor_move(DIRECTION dir, int steps) {

	POSITION curr = cursor.current;
	POSITION new_pos = curr;
	if (steps == 3)
	{
		for (int i = 0; i < 3; i++) {
			new_pos = pmove(new_pos, dir);
		}
	}
	else {
		new_pos = pmove(curr, dir);
	}

	// validation check

	if (1 <= new_pos.row && new_pos.row <= MAP_HEIGHT - 2 && \
		1 <= new_pos.column && new_pos.column <= MAP_WIDTH - 2) {

		cursor.previous = cursor.current;
		cursor.current = new_pos;

	}
}

/* ================= sample object movement =================== */
POSITION sample_obj_next_position(void) {
	// ���� ��ġ�� �������� ���ؼ� �̵� ���� ����	
	POSITION diff = psub(obj.dest, obj.pos);
	DIRECTION dir;

	// ������ ����. ������ �ܼ��� ���� �ڸ��� �պ�
	if (diff.row == 0 && diff.column == 0) {
		if (obj.dest.row == 1 && obj.dest.column == 1) {
			// topleft --> bottomright�� ������ ����
			POSITION new_dest = { MAP_HEIGHT - 2, MAP_WIDTH - 2 };
			obj.dest = new_dest;
		}
		else {
			// bottomright --> topleft�� ������ ����
			POSITION new_dest = { 1, 1 };
			obj.dest = new_dest;
		}
		return obj.pos;
	}

	// ������, ������ �Ÿ��� ���ؼ� �� �� �� ������ �̵�
	if (abs(diff.row) >= abs(diff.column)) {
		dir = (diff.row >= 0) ? d_down : d_up;
	}
	else {
		dir = (diff.column >= 0) ? d_right : d_left;
	}

	// va
	// lidation check
	// next_pos�� ���� ����� �ʰ�, (������ ������)��ֹ��� �ε����� ������ ���� ��ġ�� �̵�
	// ������ �浹 �� �ƹ��͵� �� �ϴµ�, ���߿��� ��ֹ��� ���ذ��ų� ���� ������ �ϰų�... ���
	POSITION next_pos = pmove(obj.pos, dir);
	if (1 <= next_pos.row && next_pos.row <= MAP_HEIGHT - 2 && \
		1 <= next_pos.column && next_pos.column <= MAP_WIDTH - 2 && \
		map[1][next_pos.row][next_pos.column] < 0) {

		return next_pos;
	}
	else {
		return obj.pos;  // ���ڸ�
	}
}

void sample_obj_move(void) {
	if (sys_clock <= obj.next_move_time) {
		// ���� �ð��� �� ����
		return;
	}

	// ������Ʈ(�ǹ�, ���� ��)�� layer1(map[1])�� ����
	map[1][obj.pos.row][obj.pos.column] = -1;
	obj.pos = sample_obj_next_position();
	map[1][obj.pos.row][obj.pos.column] = obj.repr;

	obj.next_move_time = sys_clock + obj.speed;
}





// �Ʊ� ���� ���� �Լ�
Unit* createUnit(UnitType type, POSITION pos, Unit* head, FactionType faction) {
	// ���� �Ӽ� ��������
	const UnitAttributes* attributes = &UNIT_ATTRIBUTES[type];
	
 
	// ���� ���� Ȯ��
	if (attributes->faction != faction && attributes->faction != FACTION_COMMON) {
		printf("�� ������ ������ �� �����ϴ�.\n");
		return head; // ���� ���� ���� �� ���� ����Ʈ ��ȯ
	}
	if (map[1][pos.row][pos.column] != -1) { // �����Ϸ��� ��ġ�� �ٸ� ������Ʈ�� ���� �� 
		printf("�� ������ ������ �� �����ϴ�.\n");
		return head; // ���� ���� ���� �� ���� ����Ʈ ��ȯ
	}

	// ���ο� ���� ���� �Ҵ�
	Unit* new_unit = (Unit*)malloc(sizeof(Unit));
	if (new_unit == NULL) {
		fprintf(stderr, "Memory allocation failed\n");
		return NULL;
	}

	
	// ���� �ʱ�ȭ
	new_unit->type = type; // ��������
	new_unit->health = attributes->stamina; // ü��
	new_unit->pos = pos; // ������ġ
	new_unit->next = head; // ���� ����Ʈ�� �� �տ� �߰�

	int color;
	if (faction == FACTION_PLAYER) {
		color = COLOR_FRIENDLY;
	}
	else if (faction == FACTION_ENEMY) {
		color = COLOR_ENEMY;
	}
	else {
		// �߸��� FACTION �Է�
		return head;
	}
	

	displayUnit(map,pos, color, 1, 1, attributes->symbol);

	return new_unit;
}


BUILDING* createBuilding(BuildingType type, POSITION pos, BUILDING* head, FactionType faction) {
	const BuildingAttributes* attributes = &BUILDINGATTRIBUTES[type];
	if (attributes->faction != faction && attributes->faction != FACTION_COMMON) {
		printf("�� �ǹ��� ������ �� �����ϴ�.\n");
		return head; // ���� ���� ���� �� ���� ����Ʈ ��ȯ
	}
	if (type != PLATE) {
		if (map[0][pos.row][pos.column] != 'P') { // �����Ϸ��� ��ġ�� �ٸ� ������Ʈ�� ���� �� �Ǵ� ������ �ƴҶ�. 
			printf("�� �ǹ��� ������ �� �����ϴ�.\n");
			return head; // ���� ���� ���� �� ���� ����Ʈ ��ȯ
		}
	}

	BUILDING* new_building = (BUILDING*)malloc(sizeof(BUILDING));
	if (new_building == NULL) {
		fprintf(stderr, "Memory allocation failed\n");
		return NULL;
	}

	int color;
	if (type == PLATE) {
		color = COLOR_PLATE;
	}
	else if (faction == FACTION_PLAYER) {
		color = COLOR_FRIENDLY;
	}
	else if (faction == FACTION_ENEMY) {
		color = COLOR_ENEMY;
	}
	else {
		// �߸��� FACTION �Է�
		return head;
	}

	new_building->position = pos;
	new_building->type = type; // ���� ����
	new_building->durability = attributes->durability; // ������ �ʱ�ȭ (������� ����)
	new_building->next = head; // ���� ����Ʈ�� �� �տ� �߰�

	displayUnit(map, pos, color, 2, 0, attributes->symbol);
	return new_building;

}

SANDWORM* createSandworm(POSITION pos, SANDWORM* head) {
	SANDWORM* new_sandworm = (SANDWORM*)malloc(sizeof(SANDWORM));
	if (new_sandworm == NULL) {
		fprintf(stderr, "Memory allocation failed\n");
		return NULL;
	}
	SANDWORM sandworm = { pos, 2500, 10000 };
	new_sandworm->move_period = 2500;
	new_sandworm->attack_period = 10000;
	new_sandworm->position = pos;
	new_sandworm->next = head;
	displayUnit(map, pos, COLOR_SANDWORM, 1, 1, 'W');
	return new_sandworm;
}

SPICE* createSpice(int howMuch, POSITION pos, SPICE* head) {
	SPICE* new_spice = (SPICE*)malloc(sizeof(SPICE));
	if (new_spice == NULL) {
		fprintf(stderr, "Memory allocation failed\n");
		return NULL;
	}
	new_spice->amount = howMuch;
	new_spice->position = pos;
	new_spice->next = head;
	char symbol = new_spice->amount + '0';
	displayUnit(map, pos, COLOR_SPICE, 1, 0, symbol);
	return new_spice;

}


void startObject(Unit** units, BUILDING** buildings, SPICE** spice, SANDWORM** sandworm ) {

	*units = createUnit(0, (POSITION) { 14, 1 }, *units, FACTION_PLAYER); // �÷��̾� ���� ����
	*units = createUnit(0, (POSITION) { 3, 58 }, *units, FACTION_ENEMY); // �÷��̾� ���� ����
	
	*buildings = createBuilding(1, (POSITION) { 15, 1 }, *buildings, FACTION_PLAYER);
	*buildings = createBuilding(0, (POSITION) { 15, 1 }, *buildings, FACTION_PLAYER);
	*buildings = createBuilding(1, (POSITION) { 15, 3 }, *buildings, FACTION_PLAYER);

	*buildings = createBuilding(1, (POSITION) { 1, 57 }, *buildings, FACTION_ENEMY);
	*buildings = createBuilding(0, (POSITION) { 1, 57 }, *buildings, FACTION_ENEMY);
	*buildings = createBuilding(1, (POSITION) { 1, 55 }, * buildings, FACTION_ENEMY);
	
	*spice = createSpice(5, (POSITION) { 12, 1 }, *spice);
	*spice = createSpice(5, (POSITION) { 5, 58 }, *spice);
	
	*sandworm = createSandworm((POSITION) { 5, 10 },*sandworm);
	*sandworm = createSandworm((POSITION) { 11, 40 },*sandworm);


	////====== �� �׸��� ============///

	displayUnit(map, (POSITION) { 10, 30 }, COLOR_ROCK, 2, 0, 'R');
	displayUnit(map, (POSITION) { 5, 15 }, COLOR_ROCK, 2, 0, 'R');
	displayUnit(map, (POSITION) { 8, 42}, COLOR_ROCK, 1, 0, 'R');
	displayUnit(map, (POSITION) { 15, 13 }, COLOR_ROCK, 1, 0, 'R');

}



ObjectInfo checkObjectAtPosition(POSITION pos, Unit* units, BUILDING* buildings, SPICE* spices, SANDWORM* sandworms) {
	ObjectInfo result = { OBJECT_NONE, NULL };  // �⺻���� �ƹ� �͵� ����

	// 1. ���� �˻�: ������ ����� ������ ��ġ���� �˻�
	for (int i = 0; i < 10; i++) {
		if (rock_positions[i].row == pos.row && rock_positions[i].column == pos.column) {
			result.type = OBJECT_ROCK;
			return result;  // �ش� ��ġ�� ������ ������ �ٷ� ��ȯ
		}
	}

	// 2. ���� �˻�
	Unit* currentUnit = units;
	while (currentUnit != NULL) {
		if (currentUnit->pos.row == pos.row && currentUnit->pos.column == pos.column) {
			result.type = OBJECT_UNIT;
			result.object = currentUnit;  // �ش� ���� ������ ��ȯ
			return result;
		}
		currentUnit = currentUnit->next;
	}

	// 3. �ǹ� �˻�
	BUILDING* currentBuilding = buildings;
	while (currentBuilding != NULL) {
		// �ǹ��� ũ�⿡ ���� ���� ��ǥ�� ������ �� �����Ƿ�, �ǹ��� ��� ��ǥ�� üũ
		for (int i = 0; i < NUM_BUILDING_TYPES; i++) {
			BuildingAttributes buildingAttr = BUILDINGATTRIBUTES[i];

			if (pos.row >= currentBuilding->position.row && pos.row < currentBuilding->position.row + 2 &&
				pos.column >= currentBuilding->position.column && pos.column < currentBuilding->position.column + 2) {
				result.type = OBJECT_BUILDING;
				result.object = currentBuilding;  // �ش� �ǹ� ������ ��ȯ
				return result;
			}
		}
		currentBuilding = currentBuilding->next;
	}

	// 4. �����̽� �˻�
	SPICE* currentSpice = spices;
	while (currentSpice != NULL) {
		if (currentSpice->position.row == pos.row && currentSpice->position.column == pos.column) {
			result.type = OBJECT_SPICE;
			result.object = currentSpice;  // �ش� �����̽� ������ ��ȯ
			return result;
		}
		currentSpice = currentSpice->next;
	}

	// 5. ����� �˻�
	SANDWORM* currentSandworm = sandworms;
	while (currentSandworm != NULL) {
		if (currentSandworm->position.row == pos.row && currentSandworm->position.column == pos.column) {
			result.type = OBJECT_SANDWORM;
			result.object = currentSandworm;  // �ش� ����� ������ ��ȯ
			return result;
		}
		currentSandworm = currentSandworm->next;
	}

	return result;  // ��� �˻翡�� �ش����� ������ OBJECT_NONE ��ȯ
}

// Ŀ���� ��ġ�� ���ڷ� �޾� �ش� ��ġ�� ������Ʈ ���� ����ϴ� �Լ�
void displayObjectInfoAtPosition(POSITION pos, Unit* units, BUILDING* buildings, SPICE* spices, SANDWORM* sandworms) {
	ObjectInfo objInfo = checkObjectAtPosition(pos, units, buildings, spices, sandworms);

	// ������Ʈ Ÿ�Կ� ���� ���� ���
	switch (objInfo.type) {
	case OBJECT_UNIT:
	{
		Unit* unit = (Unit*)objInfo.object;
		insert_status_message("Unit Type: %s\n", unitTypeToString(unit->type));
		insert_status_message("Health: %d\n\n", unit->health);
		insert_command_message("Command : %c, %c\n", UNIT_ATTRIBUTES[unit->type].command[0], UNIT_ATTRIBUTES[unit->type].command[1]);
		break;
	}
	case OBJECT_BUILDING:
	{
		BUILDING* building = (BUILDING*)objInfo.object;
		
		insert_status_message("Building Type: %s\n", buildingTypeToString(building->type));
		if (building->durability < 0) {
			insert_status_message("Durability: nononono\n\n");
		}
		else {
			insert_status_message("Durability: %d\n\n", building->durability);
		}
		insert_command_message("Command : %c\n", BUILDINGATTRIBUTES[building->type].command);
		break;
	}
	case OBJECT_SPICE:
	{
		SPICE* spice = (SPICE*)objInfo.object;
		insert_status_message("Type: SPICE\n");
		insert_status_message("Spice Amount: %d\n\n", spice->amount);
		break;
	}
	case OBJECT_SANDWORM:
	{
		SANDWORM* sandworm = (SANDWORM*)objInfo.object;
		insert_status_message("Type: SANDWORM\n\n");
		break;
	}
	case OBJECT_ROCK:
		insert_status_message("Type: ROCK\n\n");
		break;
	default:
		insert_status_message("desert\n\n", pos.row, pos.column);
		break;
	}
}

// �����̽��ٸ� ������ ��, ���� Ŀ�� ��ǥ�� �ش��ϴ� ������Ʈ ������ ����ϴ� �Լ�
void handleSpacebarPress(POSITION cursorPosition, Unit* units, BUILDING* buildings, SPICE* spices, SANDWORM* sandworms) {
	// Ŀ�� ��ġ���� ������Ʈ ���� ���
	displayObjectInfoAtPosition(cursorPosition, units, buildings, spices, sandworms);
}