#include <stdio.h>
#include "common.h"

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

typedef struct {
	UnitType type;
	int cost;
	int people;
	int moveCycle;
	int attack;
	int attackCycle;
	int stamina;
	int vision;
	char command[2];
}UnitInfo;

const UnitInfo UNIT_INFO[NUM_UNIT_TYPES] = {
	{HARVESTER,5,5,2000, -1, -1, 70 , 0, {'H','M'}},
	{FREMEN,5,2,400,15,200,25,8,{'M','P'}},
	{SOILDIER,1,1,1000,5,800,15,1,{'M','P'}},
	{PROJECTION,1,1,1200,6,600,10,1,{'M','P'}},
	{TANK,12,5,3000,40,4000,60,4,{'M','P'}},
	{SANDWORD,-1,-1,2500,-100,10000,-100,100,{-1}}
};