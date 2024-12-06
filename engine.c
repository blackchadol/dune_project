/*
1-1. ASCII ART로 intro 함수 작성
1-2. common.h의 OBJECT_SAMPLE 구조체에 speed 멤버 추가
1-3. 헤더파일에 object.h 파일 추가 및 유닛들 속성을 정의한 구조체 및 배열 선언
1-4. 유닛의 고정수치 구조체 및 가변수치 구조체를 선언(연결리스트로 관리하기 위한 포인터 포함)
1-5. 화면에 표시할 색상 경우의수를 object.h에 열거형으로 선언하고 상수로 해당 경우의 수에대한 색상을 상수로 선언, 
	 추후에 switch/case 문으로 해당 유닛 및 건물의 색상을 검출하는 함수 작성을 위함.
1-6 건물 고정수치 구조체 및 가변수치 구조체 선언(연결리스트 포인터 포함)
1-7 object.h의 건물 및 유닛 고정수치 구조체에 아군건설 가능 or 적군 건설 가능 or 공통건설가능 여부 정의 및 
샌드웜은 중립유닛인 관계로 따로 처리하기로 하여 지움.
1-8 object.h에 스파이스 및 샌드웜 구조체 추가. 
1-9 object.h에 맵에 객체를 표시하기 위한 색상 정의 16진수로(앞의 4비트는 배경색, 뒤 4비트는 글씨색)
글씨색은 하얀색으로 고정
1-10 engine.c에 create_player_unit--> 아군유닛 구조체를 연결리스트로 생성하는 함수구현 
1-11 display.c에 map color을 전역배열로 선언후 color_default로 초기화 후 displayUnit 함수로 char, 색상을 map에 표현하기 위한 정보를 배열에 저장
및 display_map 함수를 colorMap배열을 참조해 색상을 표시하도록 바꿈
1-12 display.c에 displatunit 함수에서 2x2 사이즈의 블럭을 그릴때 빈칸을 그리지 않는 문제가 발생해 . 으로 대체 
1-13 createUnit 함수를 아군 적군 구분안하고 한번에 관리하도록 설계. 연결리스트에서는 faction 멤버로 구분. 
1-14 createBuilding 함수제작 -> layer 0에 건물 설계 
1-15 create sandworm, spice 함수제작, startObject ㅅ함수를 만들어서 게임 초기상태 표시
1-16 상태창 생성 및 display_status 함수 생성 및 
insert_status_message() 함수 제작 -> 문자열 상수를 입력하면 상태창에 입력
1-17. 똑같은 방식으로 시스템메시지, 명령창 제작 및 문자열 입력 함수 제작
1-18. 시스템메시지, 상태창, 명령창을 출력시 테두리 char '#'이 삭제되는 문제해결

2-1 커서가 지형위를 지나갈 때 커서색으로 바뀌고 커서가 지나가면 다시 지형색으로 돌아오도록 코드수정. 
2-2 방향키를 연속으로 두번 눌렀을 때 3칸 씩 움직이도록 cursur_move 및 main 함수 수정
2-3. 스페이스바를 눌렀을 때 모든 오브젝트 연결리스트를 검사하여 해당위치에 있는 오브젝트 타입을 enum으로 선언하여 반환하는 함수작성. 
2-4. 스페이스바를 눌렀을 때 상태창 및 명령어 출력 구현
2-5. esc 키 입력시 상태창 및 명령창 초기화 구현
3. SANDWORM 행동구현. readme.md에 추가설명
4-1 스페이스바를 눌렀을 때 건물의 명령어를 표시하고 명령어를 입력받아 유닛을 생성하는 함수 작성
5 시스템 메시지는 1에서 구현
6-1. main에서 키입력을 상황별로 받기 위해 GAMESTATE enum선언 및 switch case 문으로 관리
6-2. 명령창에 B키를 default로 출력하고 B키를 입력하면 건설 가능한 건물 목록 출력 및 단축기로 선택 및 ESC시 취소 구현
6-3. 명령창 크기 및 시스템 메시지 창 크기 늘리기(높이 9로) 및 첫함수호출 bool 변수를 만들어서 첫 호출시에만 명령창에 출력하는걸로 수정. 
6-4. 메인루프 전 bool is_cursor_2x2 = false;를 선언함으로써 cursur_move, display_cursor함수를 2x2로 출력할 수 있게 구현 및 STATE_BUILD 상태에서 B 명령어가 base 명령어랑 겹쳐서 barracks를 설치 못하는 버그수정.
6-5. GameState가 빌드인 상태에서  명령어입력 -> 건물건설가능 검사-> 가능하면 즉시건설 및 건설 불가시 상태메시지 출력하게 구현
7. 연결리스트는 1에서 구현, 유닛 및 빌딩 연결리스트 구조체에 isally(아군여부)를 추가. 및 스페이스바를 눌렀을 때 아군타입 오브젝트 명령어 출력 구현. 
2-6. 디폴트 상태에서 스페이스바를 눌렀을 때 사용 가능한 명령어를 출력하고 만약 아군 건물에 스페이스바를 누르고 명령어를 누르면 해당 유닛 생성 기능 구현
생성 위치는 해당 건물 주위에 아무 오브젝트도 없고, 맵의 범위도 벗어나지 않는 선
2-7. 명령어가 없는 건물에 space바를 누르면 no command 메시지 출력
2-8. 빌딩 선택후 유닛 생성 과정에 파라미터로 이중포인터를 사용함으로써 연결리스트로 관리하도록 수정. 
+++ 건물 건설시 이중포인터 없이 해서 이게 연결리스트에 넣지 못하는 오류가 있었는데 이중포인터를 이용하여 해결. 
2-9. main 루프 마지막에 유닛개수를 검사해서 업데이트하는 함수 추가. 
2-10. 유닛을 생성할 때 population 값 이하까지만 생성가능하게 하는 코드 추가
8-1. 하베스터 및 다른 유닛의 행동을 처리하기 위해 선택된 유닛 포인터 변수를 만들어 유닛을 선택했을때 해당변수에 저장함, 스페이스바를 눌렀을 때 위치를 기억해서 명령어 받기. 
8-2. 하베스터 전용 연결리스트 구조체 제작(하베스터의 목적지, 명령어 받음여부, 옮기는 스파이스양을 저장하기 위함)
8-3. 코드 유지보수 용이성을 위해 하베스터 구조체를 따로 만들지 않고 하베스터용 구조체 멤버를 유닛 구조체에 생성 
8-4. 하베스터 선택 상태에 스파이스를 M 명령어로 선택할 수 있게 함. 
8-5. m명령어로 선택한 스파이스를 하베스터 구조체에 저장하여 추출 및 본진 왕복 구현.
8-6. 하베스터 움직임 함수에 목적지 변수가 초기화 되지않는 오류 수정.
9-1. 다른 유닛들 m,p 커맨드를 받고 인접하면 전투하는 코드 구현\
-> 각 유닛의 위치 및 기억된 구조체 정보를 가지고 인접한 칸에 만나면 공격력 만큼 체력이 낮아지도록 구현
10-1. 적군 유닛과 만나면 전투를 하고 유닛의 체력이 0이하가 되면 삭제 코드 추가
10-2. 유닛이 적군 건물과 인접하면 유닛의 공격력 만큼 건물 공격 및 내구도 0이되면 삭제
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

#define DOUBLE_PRESS_INTERVAL 300  // 두 번 입력 간의 최대 시간 간격 (밀리초)
#define MOVE_STEP_SINGLE 1         // 기본 이동 칸 수
#define MOVE_STEP_DOUBLE 3         // 두 번 입력 시 이동 칸 수

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
clock_t last_key_time = 0;          // 마지막 키 입력 시간
KEY last_key = k_none;              // 마지막으로 눌린 키 값

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
	Unit* units = NULL; // 객체 연결리스트 초기화
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
	bool spaceStatus = true; // 현재 상태가 스페이스를 눌렀으면 true 바뀌어, 초기에 출력해야 하는값을 중복 출력하지 않게하는 변수
	GameState gameState = STATE_DEFAULT;
	Unit* selectedUnit = NULL;
	int user_input = 0;
	int buildingEnum = -1;
	POSITION spacePos = { 0,0 };
	char unitCommand = '\0'; // 사용자가 유닛에게 내린 명령어 기억변수
	while (1) {
		//SANDWORM* current = sandworm;
		//while (current != NULL) {
		//	updateSandwormBehavior(current, &units, &spice, buildings);
		//	current = current->next;  // 리스트에서 다음 샌드웜으로 이동
		//}
		harvesterMove(&units, map, &resource, sys_clock, &spice);
		// loop 돌 때마다(즉, TICK==10ms마다) 키 입력 확인
		user_input = getInputKey();
		KEY key = get_key(user_input);
		
		int steps = 1;
		is_cursor_2x2 = false; // 루프때마다 2x2를 false로 하고 2x2 커서가 필요한 상황에서만 true로 
		was_cursor_2x2 = false;
		// 키 입력이 있으면 처리
		if (is_arrow_key(key)) {
			clock_t current_time = clock();
			// 같은 키가 짧은 시간 간격 내에 두 번 입력된 경우
			if (key == last_key && (current_time - last_key_time) * 1000 / CLOCKS_PER_SEC <= DOUBLE_PRESS_INTERVAL) {
				steps = 3;  // 두 번 빠르게 입력 시 3칸 이동
			}
			cursor_move(ktod(key), steps, is_cursor_2x2);
			// 현재 키와 시간을 기록하여 다음 입력과 비교
			last_key = key;
			last_key_time = current_time;
		}
		else {
			// 방향키 외의 입력
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
				spacePos.column = 0, spacePos.row = 0; // 스페이스 상태로 가기전 초기화 
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
				// 기본은 true이되, 마지막에 false를 넣어줌으로써 무한루프 방지. 
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
	// layer 0(map[0])에 지형 생성
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

	// layer 1(map[1])은 비워 두기(-1로 채움)
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			map[1][i][j] = -1;
		}
	}

	
}

// (가능하다면) 지정한 방향으로 커서 이동
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
	// 경계 확인 및 제한
	if (is_cursor_2x2) {
		// 2x2 커서의 경우: 오른쪽/아래로 1칸씩 여유 공간 필요
		if (1 <= new_pos.row && new_pos.row <= MAP_HEIGHT - 3 &&
			1 <= new_pos.column && new_pos.column <= MAP_WIDTH - 3) {
			cursor.previous = cursor.current;
			cursor.current = new_pos;
		}
	}
	else {
		// 1x1 커서의 경우: 경계 그대로 확인
		if (1 <= new_pos.row && new_pos.row <= MAP_HEIGHT - 2 &&
			1 <= new_pos.column && new_pos.column <= MAP_WIDTH - 2) {
			cursor.previous = cursor.current;
			cursor.current = new_pos;
		}
	}
}

/* ================= sample object movement =================== */
POSITION sample_obj_next_position(void) {
	// 현재 위치와 목적지를 비교해서 이동 방향 결정	
	POSITION diff = psub(obj.dest, obj.pos);
	DIRECTION dir;

	// 목적지 도착. 지금은 단순히 원래 자리로 왕복
	if (diff.row == 0 && diff.column == 0) {
		if (obj.dest.row == 1 && obj.dest.column == 1) {
			// topleft --> bottomright로 목적지 설정
			POSITION new_dest = { MAP_HEIGHT - 2, MAP_WIDTH - 2 };
			obj.dest = new_dest;
		}
		else {
			// bottomright --> topleft로 목적지 설정
			POSITION new_dest = { 1, 1 };
			obj.dest = new_dest;
		}
		return obj.pos;
	}

	// 가로축, 세로축 거리를 비교해서 더 먼 쪽 축으로 이동
	if (abs(diff.row) >= abs(diff.column)) {
		dir = (diff.row >= 0) ? d_down : d_up;
	}
	else {
		dir = (diff.column >= 0) ? d_right : d_left;
	}

	// va
	// lidation check
	// next_pos가 맵을 벗어나지 않고, (지금은 없지만)장애물에 부딪히지 않으면 다음 위치로 이동
	// 지금은 충돌 시 아무것도 안 하는데, 나중에는 장애물을 피해가거나 적과 전투를 하거나... 등등
	POSITION next_pos = pmove(obj.pos, dir);
	if (1 <= next_pos.row && next_pos.row <= MAP_HEIGHT - 2 && \
		1 <= next_pos.column && next_pos.column <= MAP_WIDTH - 2 && \
		map[1][next_pos.row][next_pos.column] < 0) {

		return next_pos;
	}
	else {
		return obj.pos;  // 제자리
	}
}

void sample_obj_move(void) {
	if (sys_clock <= obj.next_move_time) {
		// 아직 시간이 안 됐음
		return;
	}

	// 오브젝트(건물, 유닛 등)은 layer1(map[1])에 저장
	map[1][obj.pos.row][obj.pos.column] = -1;
	obj.pos = sample_obj_next_position();
	map[1][obj.pos.row][obj.pos.column] = obj.repr;

	obj.next_move_time = sys_clock + obj.speed;
}





// 아군 유닛 생성 함수
Unit* createUnit(UnitType type, POSITION pos, Unit* head, FactionType faction) {
	// 유닛 속성 가져오기
	const UnitAttributes* attributes = &UNIT_ATTRIBUTES[type];
	
 
	// 생성 권한 확인
	if (attributes->faction != faction && attributes->faction != FACTION_COMMON) {
		printf("이 유닛은 생성할 수 없습니다.\n");
		return head; // 유닛 생성 실패 시 현재 리스트 반환
	}
	if (map[1][pos.row][pos.column] != -1) { // 생성하려는 위치에 다른 오브젝트가 있을 때 
		printf("이 유닛은 생성할 수 없습니다.\n");
		return head; // 유닛 생성 실패 시 현재 리스트 반환
	}

	// 새로운 유닛 동적 할당
	Unit* new_unit = (Unit*)malloc(sizeof(Unit));
	if (new_unit == NULL) {
		fprintf(stderr, "Memory allocation failed\n");
		return NULL;
	}

	
	// 유닛 초기화
	new_unit->type = type; // 유닛유형
	new_unit->health = attributes->stamina; // 체력
	new_unit->pos = pos; // 현재위치
	new_unit->next = head; // 현재 리스트의 맨 앞에 추가
	new_unit->target = (POSITION){ -1, -1 }; // 유닛 움직임 구현을 위한 
	new_unit->command = '\0'; // 입력받은 명령어를 기억하기 위함. 
	new_unit->patrolPos = pos; // patrol 명령을 받을 때 왕복할 위치를 기억하기 위한 멤버
	new_unit->findEnemy = NULL; // 만약 시야에 적이 들어왔다면!!
	// 하베스터 전용 데이터 초기화
	if (type == HARVESTER) {
		new_unit->goBase = false;
		new_unit->firstCommand = false;
		new_unit->carrying_spice = 0;
		new_unit->targetSpice = NULL;  // 초기 목표 없음
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
		// 잘못된 FACTION 입력
		return head;
	}
	

	displayUnit(map,pos, color, 1, 1, attributes->symbol);

	return new_unit;
}


BUILDING* createBuilding(BuildingType type, POSITION pos, BUILDING* head, FactionType faction) {
	const BuildingAttributes* attributes = &BUILDINGATTRIBUTES[type];
	if (attributes->faction != faction && attributes->faction != FACTION_COMMON) {
		printf("이 건물은 생성할 수 없습니다.\n");
		return head; // 유닛 생성 실패 시 현재 리스트 반환
	}
	if (type != PLATE) {
		if (map[0][pos.row][pos.column] != 'P') { // 생성하려는 위치에 다른 오브젝트가 있을 때 또는 장판이 아닐때. 
			insert_system_message("이 건물은 생성할 수 없습니다.\n");
			return head; // 유닛 생성 실패 시 현재 리스트 반환
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
		// 잘못된 FACTION 입력
		return head;
	}

	new_building->position = pos;
	new_building->type = type; // 빌딩 유형
	new_building->durability = attributes->durability; // 내구도 초기화 (비용으로 설정)
	new_building->next = head; // 현재 리스트의 맨 앞에 추가
	
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

	*units = createUnit(0, (POSITION) { 14, 1 }, *units, FACTION_PLAYER); // 플레이어 유닛 생성
	*units = createUnit(0, (POSITION) { 3, 58 }, *units, FACTION_ENEMY); // 플레이어 유닛 생성
	*units = createUnit(1, (POSITION) { 10, 10 }, * units, FACTION_PLAYER); // 샌드웜 행동을 보기위한 샘플유닛 생성
	
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


	////====== 돌 그리기 ============///

	displayUnit(map, (POSITION) { 10, 30 }, COLOR_ROCK, 2, 0, 'R');
	displayUnit(map, (POSITION) { 5, 15 }, COLOR_ROCK, 2, 0, 'R');
	displayUnit(map, (POSITION) { 8, 42}, COLOR_ROCK, 1, 0, 'R');
	displayUnit(map, (POSITION) { 15, 13 }, COLOR_ROCK, 1, 0, 'R');

}



ObjectInfo checkObjectAtPosition(POSITION pos, Unit* units, BUILDING* buildings, SPICE* spices, SANDWORM* sandworms) {
	ObjectInfo result = { OBJECT_NONE, NULL };  // 기본값은 아무 것도 없음

	// 1. 바위 검사: 바위는 상수로 지정된 위치에서 검사
	for (int i = 0; i < 10; i++) {
		if (rock_positions[i].row == pos.row && rock_positions[i].column == pos.column) {
			result.type = OBJECT_ROCK;
			return result;  // 해당 위치에 바위가 있으면 바로 반환
		}
	}

	// 2. 유닛 검사
	Unit* currentUnit = units;
	while (currentUnit != NULL) {
		if (currentUnit->pos.row == pos.row && currentUnit->pos.column == pos.column) {
			result.type = OBJECT_UNIT;
			result.object = currentUnit;  // 해당 유닛 포인터 반환
			return result;
		}
		currentUnit = currentUnit->next;
	}

	// 3. 건물 검사
	BUILDING* currentBuilding = buildings;
	while (currentBuilding != NULL) {
		// 건물은 크기에 따라 여러 좌표를 차지할 수 있으므로, 건물의 모든 좌표를 체크
		//for (int i = 0; i < NUM_BUILDING_TYPES; i++) {
			//BuildingAttributes buildingAttr = BUILDINGATTRIBUTES[i];
		for (int i = 0; i < 2; ++i) {
			for (int j = 0; j < 2; ++j) {
				// currentBuilding의 2x2 영역 좌표를 계산
				int checkRow = currentBuilding->position.row + i;
				int checkColumn = currentBuilding->position.column + j;

				// pos가 해당 좌표와 일치하는지 확인
				if (pos.row == checkRow && pos.column == checkColumn) {
					result.type = OBJECT_BUILDING;     // 영역 내에 포함된 경우
					result.object = currentBuilding;  // 해당 건물 포인터 반환
					return result;                    // 결과 반환
				}
			}
		}
		currentBuilding = currentBuilding->next; // 다음 건물로 이동
	}

	// 4. 스파이스 검사
	SPICE* currentSpice = spices;
	while (currentSpice != NULL) {
		if (currentSpice->position.row == pos.row && currentSpice->position.column == pos.column) {
			result.type = OBJECT_SPICE;
			result.object = currentSpice;  // 해당 스파이스 포인터 반환
			return result;
		}
		currentSpice = currentSpice->next;
	}

	// 5. 샌드웜 검사
	SANDWORM* currentSandworm = sandworms;
	while (currentSandworm != NULL) {
		if (currentSandworm->position.row == pos.row && currentSandworm->position.column == pos.column) {
			result.type = OBJECT_SANDWORM;
			result.object = currentSandworm;  // 해당 샌드웜 포인터 반환
			return result;
		}
		currentSandworm = currentSandworm->next;
	}

	return result;  // 모든 검사에서 해당하지 않으면 OBJECT_NONE 반환
}

// 커서의 위치를 인자로 받아 해당 위치의 오브젝트 정보 출력하는 함수
void displayObjectInfoAtPosition(POSITION pos, Unit* units, BUILDING* buildings, SPICE* spices, SANDWORM* sandworms, bool spaceStatus) {
	ObjectInfo objInfo = checkObjectAtPosition(pos, units, buildings, spices, sandworms);

	// 오브젝트 타입에 따라 정보 출력
	switch (objInfo.type) {
	case OBJECT_UNIT:
	{
		Unit* unit = (Unit*)objInfo.object;
		if (spaceStatus) { // 한 오브젝트를 처음 호출했을 때만 출력. 
			insert_status_message("Unit Type: %s\n", unitTypeToString(unit->type));
			insert_status_message("Health: %d\n\n", unit->health);
			if (unit->isally) {// 유닛이 아군타입이면 명령어 사용가능. 
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

// 스페이스바를 눌렀을 때, 현재 커서 좌표에 해당하는 오브젝트 정보를 출력하는 함수
void handleSpacebarPress(POSITION cursorPosition, Unit* units, BUILDING* buildings, SPICE* spices, SANDWORM* sandworms, bool spaceStatus) {
	// 커서 위치에서 오브젝트 정보 출력
	displayObjectInfoAtPosition(cursorPosition, units, buildings, spices, sandworms,spaceStatus);



}



// 샌드웜이 이동할 때 가장 가까운 유닛을 찾는 함수
Unit* findClosestUnit(POSITION current_pos, Unit* units) {
	Unit* closestUnit = NULL;
	int minDistance = MAP_HEIGHT * MAP_WIDTH;  // 최대 거리

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

	// 샌드웜의 이동 주기마다 움직임 처리
	if (sys_clock % 2500 != 0) {
		return;  // 아직 이동 시간이 되지 않았음
	}

	// 현재 샌드웜 위치에서 가장 가까운 유닛을 찾음
	Unit* closestUnit = findClosestUnit(sandworm->position, *units);

	// 가장 가까운 유닛이 있을 경우 그쪽으로 이동
	if (closestUnit != NULL) {
		POSITION target_pos = closestUnit->pos;
		POSITION diff = psub(target_pos, sandworm->position);
		DIRECTION dir;

		// 샌드웜이 가장 가까운 유닛을 향해 이동하도록 방향을 설정
		if (abs(diff.row) >= abs(diff.column)) {
			dir = (diff.row >= 0) ? d_down : d_up;
		}
		else {
			dir = (diff.column >= 0) ? d_right : d_left;
		}

		// 샌드웜을 그 방향으로 이동시킴
		POSITION next_pos = pmove(sandworm->position, dir);

		// 이동이 가능하다면 샌드웜 위치 갱신
		if (1 <= next_pos.row && next_pos.row <= MAP_HEIGHT - 2 &&
			1 <= next_pos.column && next_pos.column <= MAP_WIDTH - 2 &&
			map[0][next_pos.row][next_pos.column] == ' ') {

			// 현재 위치에서 샌드웜을 제거하고, 새로운 위치에 배치
			map[1][sandworm->position.row][sandworm->position.column] = -1;  // 샌드웜 위치를 비움
			setColor(sandworm->position, COLOR_DEFAULT);  // 색상 초기화
			sandworm->position = next_pos;

			// 이동한 위치가 스파이스가 이미 배설된 곳이라면, 색상 표시
			//if (map[0][sandworm->position.row][sandworm->position.column] != ' ') {
			//	setColor(sandworm->position, COLOR_SPICE);  // 스파이스가 있는 곳이라면 색상 유지
			//}
			//else {
			//	setColor(sandworm->position, COLOR_SANDWORM);  // 샌드웜 위치 색상
			//}
			displayUnit(map, next_pos, COLOR_SANDWORM, 1, 1, 'W');

			// 유닛을 만났으면 잡아먹음
			if (closestUnit->pos.row == sandworm->position.row && closestUnit->pos.column == sandworm->position.column) {
				// 유닛을 제거
				removeUnit(units, closestUnit);
				insert_system_message("A Sandworm has eaten a unit!\n");
			}
		}
	}

	// 일정 시간마다 스파이스 배설 여부를 체크 (배설 주기는 랜덤)
	if (rand() % 100 < 20) {
		int spiceAmount = rand() % 9 + 1;  // 랜덤 스파이스 양 (1~9)
		//POSITION spicePos = sandworm->position;  // 현재 위치에 스파이스 배설

		// 스파이스 생성
		*spices = createSpice(spiceAmount, spicePos, *spices);

		// 스파이스 표시
		map[0][spicePos.row][spicePos.column] = spiceAmount + '0';  // 숫자로 표시
		setColor(spicePos, COLOR_SPICE);  // 색상 설정
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



///     ============   명령어를 받는 함수 ========== /// 
// 1. 건물의 명령어

void getCommand(int user_input,POSITION pos, GameState* gamestate, Unit** unitHead, BUILDING** buildingHead, SPICE* spices, SANDWORM* sandworms,Unit** selectedUnit,char* unitCommand) {
	// 여기에 건물 옆에 생성할 수 있는 공간 검증 함수
	// 공간검증을 통한 생성할 유닛의 position 계산
	
	ObjectInfo objInfo = checkObjectAtPosition(pos, *unitHead, *buildingHead, spices, sandworms);

	if (objInfo.type == OBJECT_BUILDING) {
		BUILDING* building = (BUILDING*)objInfo.object;
		if (building->isally) { // 아군빌딩이면
			bool isBuild = handleBuildingCommand(building, unitHead, user_input, pos, &resource, map);
			if (isBuild) {
				init_command();
				*gamestate = STATE_DEFAULT;
			}
		}
	}// 현재 커서의 위치가 건물일 때


	else if (objInfo.type == OBJECT_UNIT) { // 현재 커서의 위치가 유닛일 때
		Unit* unit = (Unit*)objInfo.object;
		if (unit->isally) {
			if (unit->type == HARVESTER) { // 하베스터면 
				if (user_input == 'H' || user_input == 'h') { // 사용자 인풋이 H나 h이면 하베스터 명령 입력 상태로 변환
					*gamestate = STATE_HARVESTER_MOVE;
					*selectedUnit = unit;    // 만약 유닛을 선택했으면 선택유닛의 정보를 포인터 변수에 저장. 
					init_command();
					insert_command_message("press 'M' on spice");
					return;
				}
			}
			else { // 하베스터가 아니먄
				if (user_input == 'M' || user_input == 'm' || user_input == 'P' || user_input == 'p') {
					*unitCommand = (char)user_input;
					*gamestate = STATE_OTHER_UNIT;
					*selectedUnit = unit;
					
					return;
				}
			}
		}

	}


	else return;   /// 건물도 유닛도 아니면 명령어를 받을 필요가 없기 때문에 그냥 리턴. 


}

void updatePopulation(Unit* head, RESOURCE* resource) {
	int playerUnitCount = 0; // FACTION_PLAYER 유닛 카운트

	// 연결 리스트 순회
	Unit* current = head;
	while (current != NULL) {
		if (current->isally) { // FACTION_PLAYER인 경우
			playerUnitCount++;
		}
		current = current->next;
	}

	// RESOURCE 구조체의 population 업데이트
	resource->population = playerUnitCount;

	// population이 population_max를 초과하지 않도록 조정
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