#include <stdio.h>
#include "common.h"
// �ʿ� ��ü�� ǥ���ϱ� ���� ���� ����, �ʿ信 ���� ���� ���� ������//
#define COLOR_FRIENDLY 0x9F   // �Ʊ� ��� ���� (�Ķ���)
#define COLOR_ENEMY    0xCF   // ���� ��� ���� (������)
#define COLOR_SANDWORM 0x6F   // ����� ���� (Ȳ���)
#define COLOR_ROCK     0x7F   // ��Ÿ ���� ���� (ȸ��)
#define COLOR_SPICE    0xdF   // �����̽� ���� (�����)
#define COLOR_PLATE    0x8F   // ���� ���� (������)

// ======================   �ش� ��ġ�� �ִ� ��ü�� ã�� ����, ��ü�� � Ÿ������ �˻��ϱ� ���� ���� ===========//
typedef enum {
	OBJECT_NONE,
	OBJECT_UNIT,
	OBJECT_BUILDING,
	OBJECT_SPICE,
	OBJECT_SANDWORM,
	OBJECT_ROCK,  // �߰��� ���� Ÿ��
} ObjectType;

typedef struct {
	ObjectType type;  // ��ü�� Ÿ�� (����, �ǹ�, �����̽�, ����� ��)
	void* object;     // �ش� ��ü�� ������ (����, �ǹ�, �����̽�, ����� ��)
} ObjectInfo;
// =========rock�� ������ ��ġ�� ������ ���� ���̱� ������ ����� ����=============//
const POSITION rock_positions[10] = {  // 2x2 ������ 4���� ��ǥ�� �����ϹǷ� 4�� ũ��
	{10, 30}, {10, 31}, {11, 30}, {11, 31}, // ���� 1 (2x2)
	{5, 15}, {5, 16}, {6, 15}, {6, 16},   // ���� 2 (2x2)
	{8, 42}, // ���� 3 (1x1)
	{15, 13} // ���� 4 (1x1)
};


//========6���� ������ �Ӽ��� �����ϴ� ����ü �� ��� �迭 �߰�=========//
//== ������ ������ �ʿ��� �� ������ ���� ���� ����=======//
typedef enum {
	HARVESTER,
	FREMEN,
	SOILDIER,
	PROJECTION, // ����
	TANK, // ������
	//SANDWORD, // ������� ���� ó���ϱ�� ����, �߸� �����̱� ����
	NUM_UNIT_TYPES
}UnitType;
// ========���� �� �ǹ��� ���� �Ǽ������� �ִ���, �Ʊ��� �ִ���, ������ �ִ��� �Ǵ��ϱ� ���� ========= //
typedef enum {
	FACTION_PLAYER,   // �Ʊ�
	FACTION_ENEMY,    // ����
	FACTION_COMMON // ����
} FactionType;

typedef struct {
	UnitType type;
	int production_cost; // ������
	int population; //�α���
	int move_period; // �̵��ֱ�
	int attack_power; // ���ݷ�
	int attack_period; // �����ֱ�
	int stamina;
	int vision;//�þ�
	char command[2]; //��ɾ� (�ִ� 2��)
	char symbol; // ȭ�鿡 ǥ���� ����
	FactionType faction; // ��������
}UnitAttributes; // ������ �����Ӽ��� �����ϴ� 

/* ���� �Ӽ� �� ���� = -1 ���� ǥ����*/
const UnitAttributes UNIT_ATTRIBUTES[NUM_UNIT_TYPES] = {
	{HARVESTER,5,5,2000, -1, -1, 70,0, {'H','M'}, 'H',FACTION_COMMON},
	{FREMEN,5,2,400,15,200,25,8,{'M','P'}, 'F',FACTION_PLAYER},
	{SOILDIER,1,1,1000,5,800,15,1,{'M','P'}, 'S',FACTION_PLAYER},
	{PROJECTION,1,1,1200,6,600,10,1,{'M','P'}, 'P',FACTION_ENEMY},
	{TANK,12,5,3000,40,4000,60,4,{'M','P'}, 'T',FACTION_ENEMY}
};
//// ���� ������ ���� ���� ����� ���� ���������� ����//
//typedef enum {
//	UNIT_TYPE_FRIENDLY, // �Ʊ� ����
//	UNIT_TYPE_ENEMY,    // ���� ����
//	UNIT_TYPE_SANDWORM, // �����
//	UNIT_TYPE_OTHER,    // ��Ÿ ����
//	TERRAIN_TYPE_SPICE, // �����̽� ����
//	TERRAIN_TYPE_LAND,  // ���� ����
//	// �߰� �׸� �ʿ� �� ����
//}UnitColor;

// ������ ���Ḯ��Ʈ�� �����ϱ� ���� ���� ��ġ��, ���� ����ü�� ����Ű�� �����͸� ���� ����ü ����///
typedef struct Unit {
	int type;               // ���� ���� �ε���
	int health;             // ���� ü��
	POSITION pos;           // ���� ��ġ
	struct Unit* next;      // ���� ������ ����Ű�� ������ (���� ����Ʈ)
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
//=========== �ǹ�(������ ������)�� ������ġ�� ����ü�� ����==============//

typedef struct {
	BuildingType type;
	int cost; // �Ǽ����
	int durability; // ������
	char command; //��ɾ�
	char symbol; // ȭ�鿡 ǥ���� ����
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

//=== �ǹ��� ���Ḯ��Ʈ�� �����ϱ� ���� ����ü ����========?/////////
typedef struct {
	int type; // �ǹ����� �ε���
	int durability; // ������
	POSITION position;
	struct BUILDING* next;
}BUILDING;

typedef struct {
	POSITION position;
	int move_period;
	int attack_period;
	struct SANDWORM* next;

}SANDWORM;  // ����� ����ü �߰�.

typedef struct {
	int amount; // �����̽� ���差(1 ~ 9)
	POSITION position;
	struct SPICE* next;

}SPICE; // �����̽� ����ü 

//int get_unit_color(Unit* unit) {
//	switch (unit->type) {
//	case UNIT_TYPE_FRIENDLY:
//		return COLOR_FRIENDLY; // �Ʊ� ��� ����
//	case UNIT_TYPE_ENEMY:
//		return COLOR_ENEMY; // ���� ��� ����
//	case UNIT_TYPE_SANDWORM:
//		return COLOR_SANDWORM; // ����� ����
//	case UNIT_TYPE_OTHER:
//		return COLOR_TERRAIN; // ��Ÿ ���� ����
//	default:
//		return COLOR_TERRAIN; // �⺻ ���� (ȸ��)
//	}
//}
