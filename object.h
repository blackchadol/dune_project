#include <stdio.h>
#include "common.h"

const char* unitTypeToString(int type);
const char* buildingTypeToString(int type);
int countCanCreateBuilding(RESOURCE resource);


// 맵에 객체를 표시하기 위한 색상 정의, 필요에 따라 색상 임의 변경함//
#define COLOR_FRIENDLY 0x9F   // 아군 배경 색상 (파란색)
#define COLOR_ENEMY    0xCF   // 적군 배경 색상 (빨간색)
#define COLOR_SANDWORM 0x6F   // 샌드웜 색상 (황토색)
#define COLOR_ROCK     0x7F   // 기타 지형 색상 (회색)
#define COLOR_SPICE    0xdF   // 스파이스 색상 (보라색)
#define COLOR_PLATE    0x8F   // 장판 색상 (검은색)

// ======================   해당 위치에 있는 객체를 찾기 위해, 객체가 어떤 타입인지 검사하기 위한 선언 ===========//
typedef enum {
	OBJECT_NONE,
	OBJECT_UNIT,
	OBJECT_BUILDING,
	OBJECT_SPICE,
	OBJECT_SANDWORM,
	OBJECT_ROCK,  // 추가된 바위 타입
} ObjectType;

typedef struct {
	ObjectType type;  // 객체의 타입 (유닛, 건물, 스파이스, 샌드웜 등)
	void* object;     // 해당 객체의 포인터 (유닛, 건물, 스파이스, 샌드웜 등)
} ObjectInfo;
// =========rock의 개수나 위치는 변하지 않을 것이기 때문에 상수로 선언=============//





//========6개의 유닛의 속성을 정의하는 구조체 및 상수 배열 추가=========//
//== 유닛의 정보가 필요할 때 가져다 쓰기 위한 구조=======//
typedef enum {
	HARVESTER,
	FREMEN,
	SOILDIER,
	PROJECTION, // 투사
	TANK, // 중전차
	//SANDWORD, // 샌드웜은 따로 처리하기로 결정, 중립 유닛이기 때문
	NUM_UNIT_TYPES
}UnitType;
// ========유닛 및 건물이 공통 건설권한이 있는지, 아군만 있는지, 적군만 있는지 판단하기 위함 ========= //
typedef enum {
	FACTION_PLAYER,   // 아군
	FACTION_ENEMY,    // 적군
	FACTION_COMMON // 공통
} FactionType;

typedef struct {
	UnitType type;
	int production_cost; // 생산비용
	int population; //인구수
	int move_period; // 이동주기
	int attack_power; // 공격력
	int attack_period; // 공격주기
	int stamina;
	int vision;//시야
	char command[2]; //명령어 (최대 2개)
	char symbol; // 화면에 표시할 문자
	FactionType faction; // 생성권한
}UnitAttributes; // 유닛의 고정속성을 정의하는 

/* 유닛 속성 중 없음 = -1 으로 표현함*/

//// 색상 구분을 위해 색상 경우의 수를 열거형으로 선언//
//typedef enum {
//	UNIT_TYPE_FRIENDLY, // 아군 유닛
//	UNIT_TYPE_ENEMY,    // 적군 유닛
//	UNIT_TYPE_SANDWORM, // 샌드웜
//	UNIT_TYPE_OTHER,    // 기타 유닛
//	TERRAIN_TYPE_SPICE, // 스파이스 지형
//	TERRAIN_TYPE_LAND,  // 장판 지형
//	// 추가 항목 필요 시 정의
//}UnitColor;

// 유닛을 연결리스트로 관리하기 위해 가변 수치랑, 다음 구조체를 가리키는 포인터를 넣은 구조체 선언///
typedef struct Unit {
	int type;               // 유닛 유형 인덱스
	int health;             // 현재 체력
	POSITION pos;           // 현재 위치
	struct Unit* next;      // 다음 유닛을 가리키는 포인터 (연결 리스트)
} Unit;


typedef enum {
	BASE,
	PLATE,
	DORMITORY,
	GARAGE,
	BARRACKS,
	SHELTER,
	ARENA,
	FACTORY,
	NUM_BUILDING_TYPES
}BuildingType;
//=========== 건물(바위를 제외한)의 고정수치를 구조체로 선언==============//

typedef struct {
	BuildingType type;
	int cost; // 건설비용
	int durability; // 내구도
	char command; //명령어
	char symbol; // 화면에 표시할 문자
	FactionType faction;
}BuildingAttributes;



//=== 건물도 연결리스트로 관리하기 위한 구조체 선언========?/////////
typedef struct {
	int type; // 건물유형 인덱스
	int durability; // 내구도
	POSITION position;
	struct BUILDING* next;
}BUILDING;

typedef struct {
	POSITION position;
	int move_period;
	int attack_period;
	struct SANDWORM* next;

}SANDWORM;  // 샌드웜 구조체 추가.

typedef struct {
	int amount; // 스파이스 매장량(1 ~ 9)
	POSITION position;
	struct SPICE* next;

}SPICE; // 스파이스 구조체 

//int get_unit_color(Unit* unit) {
//	switch (unit->type) {
//	case UNIT_TYPE_FRIENDLY:
//		return COLOR_FRIENDLY; // 아군 배경 색상
//	case UNIT_TYPE_ENEMY:
//		return COLOR_ENEMY; // 적군 배경 색상
//	case UNIT_TYPE_SANDWORM:
//		return COLOR_SANDWORM; // 샌드웜 색상
//	case UNIT_TYPE_OTHER:
//		return COLOR_TERRAIN; // 기타 지형 색상
//	default:
//		return COLOR_TERRAIN; // 기본 색상 (회색)
//	}
//}

// 변수 선언 (실제 정의는 object.c에서만 이루어짐)
extern const POSITION rock_positions[10];
extern const UnitAttributes UNIT_ATTRIBUTES[NUM_UNIT_TYPES];
extern const BuildingAttributes BUILDINGATTRIBUTES[NUM_BUILDING_TYPES];
BuildingType* listCanCreateBuilding(RESOURCE resource, bool firtsCall);
int getCreateBuildingCmd(int user_input, BuildingType* canCreateList, int count);
void buildStateAct(int userInput, POSITION cursor, RESOURCE resource, int* buildingEnum, bool fisrtCall);
ObjectInfo checkObjectAtPosition(POSITION pos, Unit* units, BUILDING* buildings, SPICE* spices, SANDWORM* sandworms);
BUILDING* createBuilding(BuildingType type, POSITION pos, BUILDING* head, FactionType faction);
bool attemp_building(CURSOR cursor, BuildingType building, Unit* units, BUILDING* buildings, SPICE* spices, SANDWORM* sandworms);
void actBuildSpace(CURSOR cursor, BuildingType building, RESOURCE* resource, Unit* units, BUILDING* buildings, SPICE* spices, SANDWORM* sandworms);