#include <stdio.h>
#include "common.h"

const char* unitTypeToString(int type);
const char* buildingTypeToString(int type);
int countCanCreateBuilding(RESOURCE resource);


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

// ���� ���� (���� ���Ǵ� object.c������ �̷����)
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