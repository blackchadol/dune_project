#include <stdio.h>
#include "common.h"
// 맵에 객체를 표시하기 위한 색상 정의, 필요에 따라 색상 임의 변경함//
#define COLOR_FRIENDLY 0x9F   // 아군 배경 색상 (파란색)
#define COLOR_ENEMY    0xCF   // 적군 배경 색상 (빨간색)
#define COLOR_SANDWORM 0x6F   // 샌드웜 색상 (황토색)
#define COLOR_ROCK     0x7F   // 기타 지형 색상 (회색)
#define COLOR_SPICE    0xdF   // 스파이스 색상 (보라색)
#define COLOR_PLATE    0x8F   // 장판 색상 (검은색)
//typedef struct {
//	int size; // 표시할 객체가 2x2인지, 1x1인지
//	int color; // 표시할 캐릭터의 색상.
//	POSITION coordinate; // 표시할 캐릭터의 좌표
//	char ch; // 어떤 캐릭터를 표시할것인지. 
//}OBJECT_LOCATION;
//
//OBJECT_LOCATION sample_object_create = { 1, 0x1F,{10,10},'B' };

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
const UnitAttributes UNIT_ATTRIBUTES[NUM_UNIT_TYPES] = {
	{HARVESTER,5,5,2000, -1, -1, 70,0, {'H','M'}, 'H',FACTION_COMMON},
	{FREMEN,5,2,400,15,200,25,8,{'M','P'}, 'F',FACTION_PLAYER},
	{SOILDIER,1,1,1000,5,800,15,1,{'M','P'}, 'S',FACTION_PLAYER},
	{PROJECTION,1,1,1200,6,600,10,1,{'M','P'}, 'P',FACTION_ENEMY},
	{TANK,12,5,3000,40,4000,60,4,{'M','P'}, 'T',FACTION_ENEMY}
};
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

//=== 건물도 연결리스트로 관리하기 위한 구조체 선언========?/////////
typedef struct {
	int type; // 건물유형 인덱스
	int durability; // 내구도
	struct BUILDING* next;
}BUILDING;

typedef struct {
	POSITION position;
	int move_period;
	int attack_period;

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
