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

2-1 커서가 지형위를 지나갈 때 커서색으로 바뀌고 커서가 지나가면 다시 지형색으로 돌아오도록 코드수정. 
2-2 방향키를 연속으로 두번 눌렀을 때 3칸 씩 움직이도록 cursur_move 및 main 함수 수정
*/



#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "common.h"
#include "io.h"
#include "display.h"
#include "object.h"


#define DOUBLE_PRESS_INTERVAL 300  // 두 번 입력 간의 최대 시간 간격 (밀리초)
#define MOVE_STEP_SINGLE 1         // 기본 이동 칸 수
#define MOVE_STEP_DOUBLE 3         // 두 번 입력 시 이동 칸 수

void init(void);
void intro(void);
void outro(void);
void cursor_move(DIRECTION dir, int steps);
void sample_obj_move(void);
void startObject();
Unit* createUnit(UnitType type, POSITION pos, Unit* head, FactionType faction);
BUILDING* createBuilding(BuildingType type, POSITION pos, BUILDING* head, FactionType faction);
POSITION sample_obj_next_position(void);
SANDWORM createSandworm(POSITION pos);
SPICE* createSpice(int amount, POSITION pos, SPICE* head);


/* ================= control =================== */
int sys_clock = 0;		// system-wide clock(ms)
CURSOR cursor = { { 1, 1 }, {1, 1} };
clock_t last_key_time = 0;          // 마지막 키 입력 시간
KEY last_key = k_none;              // 마지막으로 눌린 키 값

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
	startObject();
	display(resource, map, cursor);
	
	while (1) {
		// loop 돌 때마다(즉, TICK==10ms마다) 키 입력 확인
		KEY key = get_key();
		int steps = 1;
		
		// 키 입력이 있으면 처리
		if (is_arrow_key(key)) {
			clock_t current_time = clock();
			// 같은 키가 짧은 시간 간격 내에 두 번 입력된 경우
			if (key == last_key && (current_time - last_key_time) * 1000 / CLOCKS_PER_SEC <= DOUBLE_PRESS_INTERVAL) {
				steps = 3;  // 두 번 빠르게 입력 시 3칸 이동
			}
			cursor_move(ktod(key),steps);
			// 현재 키와 시간을 기록하여 다음 입력과 비교
			last_key = key;
			last_key_time = current_time;
		}
		else {
			// 방향키 외의 입력
			switch (key) {
			case k_quit: outro();
			case k_none:
			case k_undef:
			default: break;
			}
		}

		// 샘플 오브젝트 동작
		//sample_obj_move();
		//Unit* units = NULL; // 유닛 리스트 초기화
		//units = createUnit(0, (POSITION) { 10, 10 }, units, FACTION_PLAYER); // 플레이어 유닛 생성

		//BUILDING* buildings = NULL;
		//buildings = createBuilding(0, (POSITION) { 3, 5 }, buildings, FACTION_PLAYER);

		// 화면 출력
		
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

	// object sample
	//map[1][obj.pos.row][obj.pos.column] = 'o';
}

// (가능하다면) 지정한 방향으로 커서 이동
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
			printf("이 건물은 생성할 수 없습니다.\n");
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

	new_building->type = type; // 빌딩 유형
	new_building->durability = attributes->durability; // 내구도 초기화 (비용으로 설정)
	new_building->next = head; // 현재 리스트의 맨 앞에 추가

	displayUnit(map, pos, color, 2, 0, attributes->symbol);

}

SANDWORM createSandworm(POSITION pos) {
	SANDWORM sandworm = { pos, 2500, 10000 };
	displayUnit(map, pos, COLOR_SANDWORM, 1, 1, 'W');
	return sandworm;
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

}


void startObject() {
	Unit* units = NULL; // 유닛 리스트 초기화
	units = createUnit(0, (POSITION) { 14, 1 }, units, FACTION_PLAYER); // 플레이어 유닛 생성
	units = createUnit(0, (POSITION) { 3, 58 }, units, FACTION_ENEMY); // 플레이어 유닛 생성
	BUILDING* buildings = NULL;
	buildings = createBuilding(1, (POSITION) { 15, 1 }, buildings, FACTION_PLAYER);
	buildings = createBuilding(0, (POSITION) { 15, 1 }, buildings, FACTION_PLAYER);
	buildings = createBuilding(1, (POSITION) { 15, 3 }, buildings, FACTION_PLAYER);

	buildings = createBuilding(1, (POSITION) { 1, 57 }, buildings, FACTION_ENEMY);
	buildings = createBuilding(0, (POSITION) { 1, 57 }, buildings, FACTION_ENEMY);

	SPICE* spice = NULL;
	spice = createSpice(5, (POSITION) { 12, 1 }, spice);
	spice = createSpice(5, (POSITION) { 5, 58 }, spice);

	SANDWORM sandworm1 = createSandworm((POSITION) { 5, 10 });
	SANDWORM sandworm2 = createSandworm((POSITION) { 11, 40 });


	////====== 돌 그리기 ============///

	displayUnit(map, (POSITION) { 10, 30 }, COLOR_ROCK, 2, 0, 'R');
	displayUnit(map, (POSITION) { 5, 15 }, COLOR_ROCK, 2, 0, 'R');
	displayUnit(map, (POSITION) { 8, 42}, COLOR_ROCK, 1, 0, 'R');
	displayUnit(map, (POSITION) { 15, 13 }, COLOR_ROCK, 1, 0, 'R');

}