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
1-18. �ý��۸޽���, ����â, ���â�� ��½� �׵θ� char '#'�� �����Ǵ� �����ذ�

2-1 Ŀ���� �������� ������ �� Ŀ�������� �ٲ�� Ŀ���� �������� �ٽ� ���������� ���ƿ����� �ڵ����. 
2-2 ����Ű�� �������� �ι� ������ �� 3ĭ �� �����̵��� cursur_move �� main �Լ� ����
2-3. �����̽��ٸ� ������ �� ��� ������Ʈ ���Ḯ��Ʈ�� �˻��Ͽ� �ش���ġ�� �ִ� ������Ʈ Ÿ���� enum���� �����Ͽ� ��ȯ�ϴ� �Լ��ۼ�. 
2-4. �����̽��ٸ� ������ �� ����â �� ��ɾ� ��� ����
2-5. esc Ű �Է½� ����â �� ���â �ʱ�ȭ ����
3. SANDWORM �ൿ����. readme.md�� �߰�����
4-1 �����̽��ٸ� ������ �� �ǹ��� ��ɾ ǥ���ϰ� ��ɾ �Է¹޾� ������ �����ϴ� �Լ� �ۼ�
5 �ý��� �޽����� 1���� ����
6-1. main���� Ű�Է��� ��Ȳ���� �ޱ� ���� GAMESTATE enum���� �� switch case ������ ����
6-2. ���â�� BŰ�� default�� ����ϰ� BŰ�� �Է��ϸ� �Ǽ� ������ �ǹ� ��� ��� �� ������ ���� �� ESC�� ��� ����
6-3. ���â ũ�� �� �ý��� �޽��� â ũ�� �ø���(���� 9��) �� ù�Լ�ȣ�� bool ������ ���� ù ȣ��ÿ��� ���â�� ����ϴ°ɷ� ����. 
6-4. ���η��� �� bool is_cursor_2x2 = false;�� ���������ν� cursur_move, display_cursor�Լ��� 2x2�� ����� �� �ְ� ���� �� STATE_BUILD ���¿��� B ��ɾ base ��ɾ�� ���ļ� barracks�� ��ġ ���ϴ� ���׼���.
6-5. GameState�� ������ ���¿���  ��ɾ��Է� -> �ǹ��Ǽ����� �˻�-> �����ϸ� ��ðǼ� �� �Ǽ� �Ұ��� ���¸޽��� ����ϰ� ����
7. ���Ḯ��Ʈ�� 1���� ����, ���� �� ���� ���Ḯ��Ʈ ����ü�� isally(�Ʊ�����)�� �߰�. �� �����̽��ٸ� ������ �� �Ʊ�Ÿ�� ������Ʈ ��ɾ� ��� ����. 
2-6. ����Ʈ ���¿��� �����̽��ٸ� ������ �� ��� ������ ��ɾ ����ϰ� ���� �Ʊ� �ǹ��� �����̽��ٸ� ������ ��ɾ ������ �ش� ���� ���� ��� ����
���� ��ġ�� �ش� �ǹ� ������ �ƹ� ������Ʈ�� ����, ���� ������ ����� �ʴ� ��
2-7. ��ɾ ���� �ǹ��� space�ٸ� ������ no command �޽��� ���
2-8. ���� ������ ���� ���� ������ �Ķ���ͷ� ���������͸� ��������ν� ���Ḯ��Ʈ�� �����ϵ��� ����. 
+++ �ǹ� �Ǽ��� ���������� ���� �ؼ� �̰� ���Ḯ��Ʈ�� ���� ���ϴ� ������ �־��µ� ���������͸� �̿��Ͽ� �ذ�. 
2-9. main ���� �������� ���ְ����� �˻��ؼ� ������Ʈ�ϴ� �Լ� �߰�. 
2-10. ������ ������ �� population �� ���ϱ����� ���������ϰ� �ϴ� �ڵ� �߰�
8-1. �Ϻ����� �� �ٸ� ������ �ൿ�� ó���ϱ� ���� ���õ� ���� ������ ������ ����� ������ ���������� �ش纯���� ������, �����̽��ٸ� ������ �� ��ġ�� ����ؼ� ��ɾ� �ޱ�. 
8-2. �Ϻ����� ���� ���Ḯ��Ʈ ����ü ����(�Ϻ������� ������, ��ɾ� ��������, �ű�� �����̽����� �����ϱ� ����)
8-3. �ڵ� �������� ���̼��� ���� �Ϻ����� ����ü�� ���� ������ �ʰ� �Ϻ����Ϳ� ����ü ����� ���� ����ü�� ���� 
8-4. �Ϻ����� ���� ���¿� �����̽��� M ��ɾ�� ������ �� �ְ� ��. 
8-5. m��ɾ�� ������ �����̽��� �Ϻ����� ����ü�� �����Ͽ� ���� �� ���� �պ� ����.
8-6. �Ϻ����� ������ �Լ��� ������ ������ �ʱ�ȭ �����ʴ� ���� ����.
9-1. �ٸ� ���ֵ� m,p Ŀ�ǵ带 �ް� �����ϸ� �����ϴ� �ڵ� ����\
-> �� ������ ��ġ �� ���� ����ü ������ ������ ������ ĭ�� ������ ���ݷ� ��ŭ ü���� ���������� ����
10-1. ���� ���ְ� ������ ������ �ϰ� ������ ü���� 0���ϰ� �Ǹ� ���� �ڵ� �߰�
10-2. ������ ���� �ǹ��� �����ϸ� ������ ���ݷ� ��ŭ �ǹ� ���� �� ������ 0�̵Ǹ� ����
*/



#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "common.h"
#include "io.h"
#include "display.h"
#include "object.h"
#include <ctype.h>
#include <stdbool.h>

#define DOUBLE_PRESS_INTERVAL 300  // �� �� �Է� ���� �ִ� �ð� ���� (�и���)
#define MOVE_STEP_SINGLE 1         // �⺻ �̵� ĭ ��
#define MOVE_STEP_DOUBLE 3         // �� �� �Է� �� �̵� ĭ ��

void init(void);
void intro(void);
void outro(void);
void cursor_move(DIRECTION dir, int steps, bool is_cursor_2x2);
void sample_obj_move(void);
void startObject(Unit** units, BUILDING** buildings, SPICE** spice, SANDWORM** sandworm);
Unit* createUnit(UnitType type, POSITION pos, Unit* head, FactionType faction);
//BUILDING* createBuilding(BuildingType type, POSITION pos, BUILDING* head, FactionType faction);
POSITION sample_obj_next_position(void);
SANDWORM* createSandworm(POSITION pos, SANDWORM* head);
SPICE* createSpice(int amount, POSITION pos, SPICE* head);
ObjectInfo checkObjectAtPosition(POSITION pos, Unit* units, BUILDING* buildings, SPICE* spices, SANDWORM* sandworms);
void displayObjectInfoAtPosition(POSITION pos, Unit* units, BUILDING* buildings, SPICE* spices, SANDWORM* sandworms,bool spaceStatus);
void handleSpacebarPress(POSITION cursorPosition, Unit* units, BUILDING* buildings, SPICE* spices, SANDWORM* sandworms, bool spaceStatus);
Unit* findClosestUnit(POSITION current_pos, Unit* units);
void updateSandwormBehavior(SANDWORM* sandworm, Unit** units, SPICE** spices, BUILDING* buildings);
void removeUnit(Unit** units, Unit* targetUnit);
void getCommand(int user_input, POSITION pos, GameState* gamestate, Unit** units, BUILDING** buildings, SPICE* spices, SANDWORM* sandworms,Unit** selectedUnit, char* unitCommand);
bool handleBuildingCommand(BUILDING* building, Unit** units, int user_input, POSITION pos, RESOURCE* resource, char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]);
void updatePopulation(Unit* head, RESOURCE* resource);
bool checkPopulationCreateUnit(RESOURCE resource);
void getHarvestCommand(Unit* selectedUnit, int user_input, POSITION cursor, SPICE* spice, GameState* gamestate);
void harvesterMove(Unit** units, char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH], RESOURCE* resource, int sys_clock, SPICE** spiceHead);
void getOtherUnitCommand(int user_input, POSITION cursor, Unit* selectedUnit, char* userCommand, GameState* gamestate);
void updateOtherUnit(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH], int sys_clock, Unit** units, BUILDING** buildings);
void checkBuildingDurability(BUILDING** head, char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]);

/* ================= control =================== */
int sys_clock = 0;		// system-wide clock(ms)
CURSOR cursor = { { 1, 1 }, {1, 1} };
clock_t last_key_time = 0;          // ������ Ű �Է� �ð�
KEY last_key = k_none;              // ���������� ���� Ű ��

/* ================= game data =================== */
char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH] = { 0 };
RESOURCE resource = {
	.spice = 10,
	.spice_max = 20,
	.population = 0,
	.population_max = 20
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
	Unit* units = NULL; // ��ü ���Ḯ��Ʈ �ʱ�ȭ
	BUILDING* buildings = NULL;
	SPICE* spice = NULL;
	SANDWORM* sandworm = NULL;
	init();
	init_status();
	init_system_message();
	init_command();
	intro();
	init_colorMap();
	
	startObject(&units, &buildings, &spice, &sandworm);
	bool is_cursor_2x2 = false;
	bool was_cursor_2x2 = false;
	display(resource, map, cursor, is_cursor_2x2, was_cursor_2x2);
	//insert_status_message("%d",&units->pos.row);
	bool isBcommand = true;
	bool isSpaceTrigger = false;
	bool firstCall = true;
	bool spaceStatus = true; // ���� ���°� �����̽��� �������� true �ٲ��, �ʱ⿡ ����ؾ� �ϴ°��� �ߺ� ������� �ʰ��ϴ� ����
	GameState gameState = STATE_DEFAULT;
	Unit* selectedUnit = NULL;
	int user_input = 0;
	int buildingEnum = -1;
	POSITION spacePos = { 0,0 };
	char unitCommand = '\0'; // ����ڰ� ���ֿ��� ���� ��ɾ� ��ﺯ��
	while (1) {
		//SANDWORM* current = sandworm;
		//while (current != NULL) {
		//	updateSandwormBehavior(current, &units, &spice, buildings);
		//	current = current->next;  // ����Ʈ���� ���� ��������� �̵�
		//}
		harvesterMove(&units, map, &resource, sys_clock, &spice);
		// loop �� ������(��, TICK==10ms����) Ű �Է� Ȯ��
		user_input = getInputKey();
		KEY key = get_key(user_input);
		
		int steps = 1;
		is_cursor_2x2 = false; // ���������� 2x2�� false�� �ϰ� 2x2 Ŀ���� �ʿ��� ��Ȳ������ true�� 
		was_cursor_2x2 = false;
		// Ű �Է��� ������ ó��
		if (is_arrow_key(key)) {
			clock_t current_time = clock();
			// ���� Ű�� ª�� �ð� ���� ���� �� �� �Էµ� ���
			if (key == last_key && (current_time - last_key_time) * 1000 / CLOCKS_PER_SEC <= DOUBLE_PRESS_INTERVAL) {
				steps = 3;  // �� �� ������ �Է� �� 3ĭ �̵�
			}
			cursor_move(ktod(key), steps, is_cursor_2x2);
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
			default: break;
			}
		

			switch (gameState) {
			case STATE_DEFAULT:
			{
				spacePos.column = 0, spacePos.row = 0; // �����̽� ���·� ������ �ʱ�ȭ 
				if (isBcommand) {
					insert_command_message("B:Build");
					isBcommand = false;
				}
				if ((char)user_input == 'B' || (char)user_input == 'b') {
					init_command();
					gameState = STATE_BUILD;
					isBcommand = true;
				}
				if (user_input == SPACEBYTE) {
					gameState = STATE_SPACE;
					spacePos = cursor.current;
					isBcommand = true;
					spaceStatus = true;
					init_command();
				}
				


				break;
			}
			case STATE_BUILD: {

				if (user_input == ESCBYTE) {
					init_command();
					gameState = STATE_DEFAULT;
					firstCall = true;
				}
				else {
					buildStateAct(user_input, cursor.current, resource, &buildingEnum, firstCall);
					firstCall = false;
				}

				if (buildingEnum > 0) {
					gameState = STATE_BUILD_SPACE;
					init_command();
					insert_status_message("build %s", buildingTypeToString(buildingEnum));
					firstCall = true;
				}

				break;
			}
						
			case STATE_SPACE: {
				// �⺻�� true�̵�, �������� false�� �־������ν� ���ѷ��� ����. 
				if (user_input == SPACEBYTE) {
					spaceStatus = true;
					spacePos = cursor.current;

				}

				if (user_input == ESCBYTE) {
					spaceStatus = true;
					init_command();
					init_status();
					gameState = STATE_DEFAULT;
				}
				handleSpacebarPress(spacePos, units, buildings, spice, sandworm, spaceStatus);
				getCommand(user_input, spacePos, &gameState, &units, &buildings, spice, sandworm,&selectedUnit, &unitCommand); 

				spaceStatus = false;
				break;
			}
				
			case STATE_BUILD_SPACE: {
				is_cursor_2x2 = true;
				if (user_input == SPACEBYTE) {

					actBuildSpace(cursor, buildingEnum, &resource, units, &buildings, spice, sandworm);
					is_cursor_2x2 = false;
					was_cursor_2x2 = true;
					gameState = STATE_DEFAULT;
					buildingEnum = -1;
				}
				if (user_input == ESCBYTE) {
					init_status();
					is_cursor_2x2 = false;
					was_cursor_2x2 = true;
					gameState = STATE_DEFAULT;
					buildingEnum = -1;
				}
				break;
			}


			case STATE_HARVESTER_MOVE: {
				if (user_input == SPACEBYTE) {
					gameState = STATE_SPACE;
					spacePos = cursor.current;
					isBcommand = true;
					spaceStatus = true;
					init_command();
				}
				getHarvestCommand(selectedUnit,user_input,cursor.current,spice,&gameState);
				break;
			}

			case STATE_OTHER_UNIT: {
				getOtherUnitCommand(user_input, cursor.current, selectedUnit, &unitCommand, &gameState);
				break;
			}
			
			default: break;


			}

			if (user_input == ESCBYTE) {
				init_status();
				init_command();
				isBcommand = true;
				gameState = STATE_DEFAULT;
			}
			
		}
		updateOtherUnit(map, sys_clock, &units, &buildings);
		updatePopulation(units, &resource);
		checkBuildingDurability(&buildings, map);
		display(resource, map, cursor, is_cursor_2x2, was_cursor_2x2);
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

	
}

// (�����ϴٸ�) ������ �������� Ŀ�� �̵�
void cursor_move(DIRECTION dir, int steps, bool is_cursor_2x2) {

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
	// ��� Ȯ�� �� ����
	if (is_cursor_2x2) {
		// 2x2 Ŀ���� ���: ������/�Ʒ��� 1ĭ�� ���� ���� �ʿ�
		if (1 <= new_pos.row && new_pos.row <= MAP_HEIGHT - 3 &&
			1 <= new_pos.column && new_pos.column <= MAP_WIDTH - 3) {
			cursor.previous = cursor.current;
			cursor.current = new_pos;
		}
	}
	else {
		// 1x1 Ŀ���� ���: ��� �״�� Ȯ��
		if (1 <= new_pos.row && new_pos.row <= MAP_HEIGHT - 2 &&
			1 <= new_pos.column && new_pos.column <= MAP_WIDTH - 2) {
			cursor.previous = cursor.current;
			cursor.current = new_pos;
		}
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
	new_unit->target = (POSITION){ -1, -1 }; // ���� ������ ������ ���� 
	new_unit->command = '\0'; // �Է¹��� ��ɾ ����ϱ� ����. 
	new_unit->patrolPos = pos; // patrol ����� ���� �� �պ��� ��ġ�� ����ϱ� ���� ���
	new_unit->findEnemy = NULL; // ���� �þ߿� ���� ���Դٸ�!!
	// �Ϻ����� ���� ������ �ʱ�ȭ
	if (type == HARVESTER) {
		new_unit->goBase = false;
		new_unit->firstCommand = false;
		new_unit->carrying_spice = 0;
		new_unit->targetSpice = NULL;  // �ʱ� ��ǥ ����
	}
	
	if (faction == FACTION_PLAYER) new_unit->isally = true;
	else new_unit->isally = false;
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
			insert_system_message("�� �ǹ��� ������ �� �����ϴ�.\n");
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
	
	if (faction == FACTION_PLAYER) new_building->isally = true;
	else new_building->isally = false;
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
	*units = createUnit(1, (POSITION) { 10, 10 }, * units, FACTION_PLAYER); // ����� �ൿ�� �������� �������� ����
	
	*units = createUnit(3, (POSITION) { 5, 58 }, * units, FACTION_ENEMY);
	//*units = createUnit(4, (POSITION) { 15, 8 }, * units, FACTION_ENEMY);
	

	*buildings = createBuilding(1, (POSITION) { 15, 1 }, *buildings, FACTION_PLAYER);
	*buildings = createBuilding(0, (POSITION) { 15, 1 }, *buildings, FACTION_PLAYER);
	*buildings = createBuilding(1, (POSITION) { 15, 3 }, *buildings, FACTION_PLAYER);

	*buildings = createBuilding(1, (POSITION) { 1, 57 }, *buildings, FACTION_ENEMY);
	*buildings = createBuilding(0, (POSITION) { 1, 57 }, *buildings, FACTION_ENEMY);
	*buildings = createBuilding(1, (POSITION) { 1, 55 }, * buildings, FACTION_ENEMY);

	*buildings = createBuilding(1, (POSITION) { 15, 6 }, * buildings, FACTION_ENEMY);
	*buildings = createBuilding(0, (POSITION) { 15, 6 }, * buildings, FACTION_ENEMY);
	
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
		//for (int i = 0; i < NUM_BUILDING_TYPES; i++) {
			//BuildingAttributes buildingAttr = BUILDINGATTRIBUTES[i];
		for (int i = 0; i < 2; ++i) {
			for (int j = 0; j < 2; ++j) {
				// currentBuilding�� 2x2 ���� ��ǥ�� ���
				int checkRow = currentBuilding->position.row + i;
				int checkColumn = currentBuilding->position.column + j;

				// pos�� �ش� ��ǥ�� ��ġ�ϴ��� Ȯ��
				if (pos.row == checkRow && pos.column == checkColumn) {
					result.type = OBJECT_BUILDING;     // ���� ���� ���Ե� ���
					result.object = currentBuilding;  // �ش� �ǹ� ������ ��ȯ
					return result;                    // ��� ��ȯ
				}
			}
		}
		currentBuilding = currentBuilding->next; // ���� �ǹ��� �̵�
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
void displayObjectInfoAtPosition(POSITION pos, Unit* units, BUILDING* buildings, SPICE* spices, SANDWORM* sandworms, bool spaceStatus) {
	ObjectInfo objInfo = checkObjectAtPosition(pos, units, buildings, spices, sandworms);

	// ������Ʈ Ÿ�Կ� ���� ���� ���
	switch (objInfo.type) {
	case OBJECT_UNIT:
	{
		Unit* unit = (Unit*)objInfo.object;
		if (spaceStatus) { // �� ������Ʈ�� ó�� ȣ������ ���� ���. 
			insert_status_message("Unit Type: %s\n", unitTypeToString(unit->type));
			insert_status_message("Health: %d\n\n", unit->health);
			if (unit->isally) {// ������ �Ʊ�Ÿ���̸� ��ɾ� ��밡��. 
				if (unit->type == HARVESTER) {
					insert_command_message("Press H ");
				}
				else {
					insert_command_message("Command : %c, %c\n", UNIT_ATTRIBUTES[unit->type].command[0], UNIT_ATTRIBUTES[unit->type].command[1]);
				}
			}
			//insert_command_message("Command : %c, %c\n", UNIT_ATTRIBUTES[unit->type].command[0], UNIT_ATTRIBUTES[unit->type].command[1]);
		}
		break;
	}
	case OBJECT_BUILDING:
	{
		BUILDING* building = (BUILDING*)objInfo.object;
		
		if (spaceStatus) {
			insert_status_message("Building Type: %s\n", buildingTypeToString(building->type));
			if (building->durability < 0) {
				insert_status_message("Durability: nononono\n\n");
			}
			else {
				insert_status_message("Durability: %d\n\n", building->durability);
			}

			if (building->isally) {
				if (BUILDINGATTRIBUTES[building->type].command > 0) {
					insert_command_message("Command : %c \n", BUILDINGATTRIBUTES[building->type].command);
				}
				else {
					insert_command_message("No command can use");
				}
			}
		}
		
		break;


	}
	case OBJECT_SPICE:
	{
		SPICE* spice = (SPICE*)objInfo.object;
		if (spaceStatus) {
			insert_status_message("Type: SPICE\n");
			insert_status_message("Spice Amount: %d\n\n", spice->amount);
		}
		break;
	}
	case OBJECT_SANDWORM:
	{
		SANDWORM* sandworm = (SANDWORM*)objInfo.object;
		if (spaceStatus) {
			insert_status_message("Type: SANDWORM\n\n");
		}
		break;
	}
	case OBJECT_ROCK:
		if (spaceStatus) {
			insert_status_message("Type: ROCK\n\n");
		}
		break;
	default:
		if (spaceStatus) {
			insert_status_message("desert\n\n", pos.row, pos.column);
		}
		break;
	}
}

// �����̽��ٸ� ������ ��, ���� Ŀ�� ��ǥ�� �ش��ϴ� ������Ʈ ������ ����ϴ� �Լ�
void handleSpacebarPress(POSITION cursorPosition, Unit* units, BUILDING* buildings, SPICE* spices, SANDWORM* sandworms, bool spaceStatus) {
	// Ŀ�� ��ġ���� ������Ʈ ���� ���
	displayObjectInfoAtPosition(cursorPosition, units, buildings, spices, sandworms,spaceStatus);



}



// ������� �̵��� �� ���� ����� ������ ã�� �Լ�
Unit* findClosestUnit(POSITION current_pos, Unit* units) {
	Unit* closestUnit = NULL;
	int minDistance = MAP_HEIGHT * MAP_WIDTH;  // �ִ� �Ÿ�

	Unit* currentUnit = units;
	while (currentUnit != NULL) {
		int distance = abs(current_pos.row - currentUnit->pos.row) + abs(current_pos.column - currentUnit->pos.column);
		if (distance < minDistance) {
			minDistance = distance;
			closestUnit = currentUnit;
		}
		currentUnit = currentUnit->next;
	}

	return closestUnit;
}

void updateSandwormBehavior(SANDWORM* sandworm, Unit** units, SPICE** spices, BUILDING* buildings) {
	POSITION spicePos = sandworm->position;
	if (sandworm == NULL) return;

	// ������� �̵� �ֱ⸶�� ������ ó��
	if (sys_clock % 2500 != 0) {
		return;  // ���� �̵� �ð��� ���� �ʾ���
	}

	// ���� ����� ��ġ���� ���� ����� ������ ã��
	Unit* closestUnit = findClosestUnit(sandworm->position, *units);

	// ���� ����� ������ ���� ��� �������� �̵�
	if (closestUnit != NULL) {
		POSITION target_pos = closestUnit->pos;
		POSITION diff = psub(target_pos, sandworm->position);
		DIRECTION dir;

		// ������� ���� ����� ������ ���� �̵��ϵ��� ������ ����
		if (abs(diff.row) >= abs(diff.column)) {
			dir = (diff.row >= 0) ? d_down : d_up;
		}
		else {
			dir = (diff.column >= 0) ? d_right : d_left;
		}

		// ������� �� �������� �̵���Ŵ
		POSITION next_pos = pmove(sandworm->position, dir);

		// �̵��� �����ϴٸ� ����� ��ġ ����
		if (1 <= next_pos.row && next_pos.row <= MAP_HEIGHT - 2 &&
			1 <= next_pos.column && next_pos.column <= MAP_WIDTH - 2 &&
			map[0][next_pos.row][next_pos.column] == ' ') {

			// ���� ��ġ���� ������� �����ϰ�, ���ο� ��ġ�� ��ġ
			map[1][sandworm->position.row][sandworm->position.column] = -1;  // ����� ��ġ�� ���
			setColor(sandworm->position, COLOR_DEFAULT);  // ���� �ʱ�ȭ
			sandworm->position = next_pos;

			// �̵��� ��ġ�� �����̽��� �̹� �輳�� ���̶��, ���� ǥ��
			//if (map[0][sandworm->position.row][sandworm->position.column] != ' ') {
			//	setColor(sandworm->position, COLOR_SPICE);  // �����̽��� �ִ� ���̶�� ���� ����
			//}
			//else {
			//	setColor(sandworm->position, COLOR_SANDWORM);  // ����� ��ġ ����
			//}
			displayUnit(map, next_pos, COLOR_SANDWORM, 1, 1, 'W');

			// ������ �������� ��Ƹ���
			if (closestUnit->pos.row == sandworm->position.row && closestUnit->pos.column == sandworm->position.column) {
				// ������ ����
				removeUnit(units, closestUnit);
				insert_system_message("A Sandworm has eaten a unit!\n");
			}
		}
	}

	// ���� �ð����� �����̽� �輳 ���θ� üũ (�輳 �ֱ�� ����)
	if (rand() % 100 < 20) {
		int spiceAmount = rand() % 9 + 1;  // ���� �����̽� �� (1~9)
		//POSITION spicePos = sandworm->position;  // ���� ��ġ�� �����̽� �輳

		// �����̽� ����
		*spices = createSpice(spiceAmount, spicePos, *spices);

		// �����̽� ǥ��
		map[0][spicePos.row][spicePos.column] = spiceAmount + '0';  // ���ڷ� ǥ��
		setColor(spicePos, COLOR_SPICE);  // ���� ����
		insert_system_message("A Sandworm has released some Spice!\n");
	}
}



void removeUnit(Unit** head, Unit* target) {
	if (head == NULL || *head == NULL || target == NULL) {
		return;
	}

	Unit* current = *head;
	Unit* previous = NULL;

	if (current == target) {
		*head = current->next;
		free(current);
		return;
	}

	while (current != NULL) {
		if (current == target) {
			previous->next = current->next;
			free(current);
			return;
		}
		previous = current;
		current = current->next;
	}
	return;
}



///     ============   ��ɾ �޴� �Լ� ========== /// 
// 1. �ǹ��� ��ɾ�

void getCommand(int user_input,POSITION pos, GameState* gamestate, Unit** unitHead, BUILDING** buildingHead, SPICE* spices, SANDWORM* sandworms,Unit** selectedUnit,char* unitCommand) {
	// ���⿡ �ǹ� ���� ������ �� �ִ� ���� ���� �Լ�
	// ���������� ���� ������ ������ position ���
	
	ObjectInfo objInfo = checkObjectAtPosition(pos, *unitHead, *buildingHead, spices, sandworms);

	if (objInfo.type == OBJECT_BUILDING) {
		BUILDING* building = (BUILDING*)objInfo.object;
		if (building->isally) { // �Ʊ������̸�
			bool isBuild = handleBuildingCommand(building, unitHead, user_input, pos, &resource, map);
			if (isBuild) {
				init_command();
				*gamestate = STATE_DEFAULT;
			}
		}
	}// ���� Ŀ���� ��ġ�� �ǹ��� ��


	else if (objInfo.type == OBJECT_UNIT) { // ���� Ŀ���� ��ġ�� ������ ��
		Unit* unit = (Unit*)objInfo.object;
		if (unit->isally) {
			if (unit->type == HARVESTER) { // �Ϻ����͸� 
				if (user_input == 'H' || user_input == 'h') { // ����� ��ǲ�� H�� h�̸� �Ϻ����� ��� �Է� ���·� ��ȯ
					*gamestate = STATE_HARVESTER_MOVE;
					*selectedUnit = unit;    // ���� ������ ���������� ���������� ������ ������ ������ ����. 
					init_command();
					insert_command_message("press 'M' on spice");
					return;
				}
			}
			else { // �Ϻ����Ͱ� �ƴϐ�
				if (user_input == 'M' || user_input == 'm' || user_input == 'P' || user_input == 'p') {
					*unitCommand = (char)user_input;
					*gamestate = STATE_OTHER_UNIT;
					*selectedUnit = unit;
					
					return;
				}
			}
		}

	}


	else return;   /// �ǹ��� ���ֵ� �ƴϸ� ��ɾ ���� �ʿ䰡 ���� ������ �׳� ����. 


}

void updatePopulation(Unit* head, RESOURCE* resource) {
	int playerUnitCount = 0; // FACTION_PLAYER ���� ī��Ʈ

	// ���� ����Ʈ ��ȸ
	Unit* current = head;
	while (current != NULL) {
		if (current->isally) { // FACTION_PLAYER�� ���
			playerUnitCount++;
		}
		current = current->next;
	}

	// RESOURCE ����ü�� population ������Ʈ
	resource->population = playerUnitCount;

	// population�� population_max�� �ʰ����� �ʵ��� ����
	if (resource->population > resource->population_max) {
		resource->population = resource->population_max;
	}
}


bool checkPopulationCreateUnit(RESOURCE resource) {
	if (resource.population >= resource.population_max) {
		return false;
	}
	else return true;
}