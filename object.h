#include <stdio.h>
#include "common.h"
#define COLOR_FRIENDLY 9  // 아군 배경 색상 (파란색)
#define COLOR_ENEMY   4   // 적군 배경 색상 (빨간색)
#define COLOR_SANDWORM  6    // 샌드웜 색상 (황토색)
#define COLOR_TERRAIN   8    // 기타 지형 색상 (회색)
#define COLOR_SPICE  14   // 스파이스 색상 (주황색)
#define COLOR_PLATE   0    // 장판 색상 (검은색)
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
	SANDWORD, // 샌드웜
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
	//int stamina;
	int vision;//시야
	char command[2]; //명령어 (최대 2개)
	char symbol; // 화면에 표시할 문자
}UnitAttributes; // 유닛의 고정속성을 정의하는 

/* 유닛 속성 중 없음 = -1 으로 표현함*/
const UnitAttributes UNIT_ATTRIBUTES[NUM_UNIT_TYPES] = {
	{HARVESTER,5,5,2000, -1, -1, 0, {'H','M'}, 'H'},
	{FREMEN,5,2,400,15,200,8,{'M','P'}, 'F'},
	{SOILDIER,1,1,1000,5,800,1,{'M','P'}, 'S'},
	{PROJECTION,1,1,1200,6,600,1,{'M','P'}, 'P'},
	{TANK,12,5,3000,40,4000,4,{'M','P'}, 'T'},
	{SANDWORD,-1,-1,2500,-100,10000,100,{-1},'W'}
};
// 색상 구분을 위해 색상 경우의 수를 열거형으로 선언//
typedef enum {
	UNIT_TYPE_FRIENDLY, // 아군 유닛
	UNIT_TYPE_ENEMY,    // 적군 유닛
	UNIT_TYPE_SANDWORM, // 샌드웜
	UNIT_TYPE_OTHER,    // 기타 유닛
	TERRAIN_TYPE_SPICE, // 스파이스 지형
	TERRAIN_TYPE_LAND,  // 장판 지형
	// 추가 항목 필요 시 정의
}UnitColor;

// 유닛을 연결리스트로 관리하기 위해 가변 수치랑, 다음 구조체를 가리키는 포인터를 넣은 구조체 선언///
typedef struct Unit {
	UnitColor color;
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
	char command; //명령어
	char symbol; // 화면에 표시할 문자
}BuildingAttributes;

const BuildingAttributes BUILDINGATTRIBUTES[NUM_BUILDING_TYPES]{
	{BASE, 0, 'H','B'},
	{PLATE, 1, -1,'P'},
	{DORMITORY,2,-1,'D'},
	{GARAGE,4,-1,'G'},
	{BARRACKS,4,'S','B'},
	{SHELTER,5,'F','S'},
	{ARENA,3,'F','A'},
	{FACTORY,5,'T','F'}
};

//=== 건물도 연결리스트로 관리하기 위한 구조체 선언========?/////////
typedef struct {
	UnitColor color;
	int type; // 건물유형 인덱스
	int durability; // 내구도
	struct BUILDING* next;
}BUILDING;


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
