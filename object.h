#include <stdio.h>
#include "common.h"

//typedef struct {
//	int size; // ǥ���� ��ü�� 2x2����, 1x1����
//	int color; // ǥ���� ĳ������ ����.
//	POSITION coordinate; // ǥ���� ĳ������ ��ǥ
//	char ch; // � ĳ���͸� ǥ���Ұ�����. 
//}OBJECT_LOCATION;
//
//OBJECT_LOCATION sample_object_create = { 1, 0x1F,{10,10},'B' };

//========6���� ������ �Ӽ��� �����ϴ� ����ü �� ��� �迭 �߰�=========//
//== ������ ������ �ʿ��� �� ������ ���� ���� ����=======//
typedef enum {
	HARVESTER,
	FREMEN,
	SOILDIER,
	PROJECTION, // ����
	TANK, // ������
	SANDWORD, // �����
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