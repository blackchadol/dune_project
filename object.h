#include <stdio.h>
#include "common.h"
#define COLOR_FRIENDLY 9  // �Ʊ� ��� ���� (�Ķ���)
#define COLOR_ENEMY   4   // ���� ��� ���� (������)
#define COLOR_SANDWORM  6    // ����� ���� (Ȳ���)
#define COLOR_TERRAIN   8    // ��Ÿ ���� ���� (ȸ��)
#define COLOR_SPICE  14   // �����̽� ���� (��Ȳ��)
#define COLOR_PLATE   0    // ���� ���� (������)
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
	//int stamina;
	int vision;//�þ�
	char command[2]; //��ɾ� (�ִ� 2��)
	char symbol; // ȭ�鿡 ǥ���� ����
}UnitAttributes; // ������ �����Ӽ��� �����ϴ� 

/* ���� �Ӽ� �� ���� = -1 ���� ǥ����*/
const UnitAttributes UNIT_ATTRIBUTES[NUM_UNIT_TYPES] = {
	{HARVESTER,5,5,2000, -1, -1, 0, {'H','M'}, 'H'},
	{FREMEN,5,2,400,15,200,8,{'M','P'}, 'F'},
	{SOILDIER,1,1,1000,5,800,1,{'M','P'}, 'S'},
	{PROJECTION,1,1,1200,6,600,1,{'M','P'}, 'P'},
	{TANK,12,5,3000,40,4000,4,{'M','P'}, 'T'},
	{SANDWORD,-1,-1,2500,-100,10000,100,{-1},'W'}
};
// ���� ������ ���� ���� ����� ���� ���������� ����//
typedef enum {
	UNIT_TYPE_FRIENDLY, // �Ʊ� ����
	UNIT_TYPE_ENEMY,    // ���� ����
	UNIT_TYPE_SANDWORM, // �����
	UNIT_TYPE_OTHER,    // ��Ÿ ����
	TERRAIN_TYPE_SPICE, // �����̽� ����
	TERRAIN_TYPE_LAND,  // ���� ����
	// �߰� �׸� �ʿ� �� ����
}UnitColor;

// ������ ���Ḯ��Ʈ�� �����ϱ� ���� ���� ��ġ��, ���� ����ü�� ����Ű�� �����͸� ���� ����ü ����///
typedef struct Unit {
	UnitColor color;
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
	char command; //��ɾ�
	char symbol; // ȭ�鿡 ǥ���� ����
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

//=== �ǹ��� ���Ḯ��Ʈ�� �����ϱ� ���� ����ü ����========?/////////
typedef struct {
	UnitColor color;
	int type; // �ǹ����� �ε���
	int durability; // ������
	struct BUILDING* next;
}BUILDING;


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
