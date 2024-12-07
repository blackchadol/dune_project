# dune_project

20211153 오준수

# 구현한 부분

1) ~ 10) 까지 11) 적군 행동을 제외한 부분 구현

# 길찾기 및 시야 구현을 통한 유닛 움직임 구현방법. 

아래는 unit 길찾기 및 시야에 들어온 적군을 기억해 타겟을 바꾸어 유닛 움직임을 구현하는 함수이다. 
```c
void updateOtherUnit(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH], int sys_clock, Unit** units, BUILDING** buildings ) {
    Unit* currentUnit = *units;
    

    while (currentUnit != NULL) {
        if (currentUnit->type != HARVESTER) { // 하베스터 행동은 다른곳에서 처리하기에. 
            const UnitAttributes* attributes = &UNIT_ATTRIBUTES[currentUnit->type];
            findEnemyByVision(*units, currentUnit); // 이 함수에서는 현재유닛 기준에서 시야에 들어온 적군이 있으면 적군 구조체 저장 및 타겟 업데이트.

            if (currentUnit->target.row > 0 && currentUnit->target.column > 0 && currentUnit->command != '\0') {  // 타겟이 유효한지 확인. ,, --> 타겟으로 주기적으로 이동하는 함수.
                if (sys_clock % attributes->move_period != 0) {      // 이동주기 검사.
                    return;  // 아직 이동 시간이 되지 않았음
                }
                // 타겟이랑 유닛이랑 1칸 차이일 때 if 커맨드가 P이면 타겟을 patrol_pos로
                // 만약 patrolPos == pos가 같다면 command null 밑 패트롤 pos랑 타겟 둘다 0으로 
                patrolTarget(currentUnit);


```
- 위 코드에서는 현재 게임상 존재하는 유닛 연결리스트를 순회한다. 유닛이 명령어를 받을 때 구조체 멤버 target에 해당 위치를 저장하고 만약 명령을 받아 target으로 움직여야 되는 유닛일때 아래내용을 수헹한다. 또한 현재유닛 기준에서 findEnemyByVision 함수를 이용해 만약 유닛의 시야만큼 떨어진 적군 유닛이 있으면 해당 유닛의 구조체의 findEnemy멤버에 UNIT* 형태로 저장

- 또한 만약 유닛에 대한 사용자의 명령어가 p(왕복)라면 patrol을 위해 돌아와야되는 위치를 unit->patrolPos에 저장 후 target에 도착한 후에 unit->target = unit->patrolPos로 바꾸는 형태로 구현

- target 위치를 찾아 Dir 을 정해 움직이는 것은 아래 샌드웜 움직임 방식과 같음. 
```c
void findEnemyByVision(Unit* head, Unit* currentUnit) {
    const UnitAttributes* attributes = &UNIT_ATTRIBUTES[currentUnit->type];
    while (head != NULL) {
        // 현재 유닛과의 거리 계산
        int row_diff = abs(head->pos.row - currentUnit->pos.row);
        int col_diff = abs(head->pos.column - currentUnit->pos.column);

        // 조건: vision 거리 이내 & 소속이 다름
        if (row_diff <= attributes->vision && col_diff <= attributes->vision &&  // --> 현재 유닛기준에서 적군 유닛이 시야에 들어왔는지 확인, 커맨드가 입력된 상태였으면 수행하기에 
            head->isally != currentUnit->isally) {

            currentUnit->findEnemy = head; //  적군을 찾았으니 적군 정보 저장
            currentUnit->target = head->pos;

        }
        head = head->next;
    }
}
        
```
- 시야를 이용해 적군 유닛을 찾는 방법은 위 코드처럼 Unit 연결리스트를 순회하며 좌표평면상 거리가 vision보다 작고, 서로 적군여부(isally)가 다르면 unit->findEnemy에 적군 Unit포인터 저장
- unit->target도 적군 위치로 수정. 



위와 같이 유닛의 움직임을 끝내고 나면 적군이 인접해있을때, 적군의 건물이 인접해있을때 전투 행동을 실시한다. 

```c
    // 여기에 현재 유닛포인터와 어쩌구를 넣어서 전투 확인
                executeBattle(currentUnit, units, map);
                attackBuilding(buildings, currentUnit, map); // 빌딩 한칸 옆에 있으면 빌딩 공격. 
```
- 위 두개의 함수는 모두 연결리스트 순회 -> 적군 인접여부 확인 -> 적군 내구도 - 아군의 공격력 이라는 알고리즘으로 구현


# Main 함수 게임루프 키입력 구현
```c
typedef enum {
	STATE_DEFAULT,    // 기본 상태
	STATE_SPACE,      // 스페이스바 상태
	STATE_BUILD,     // 빌드 상태(B를 누른 상태)
	STATE_BUILD_SPACE, // 빌드 상태에서 스페이스를 대기하는 상태
	STATE_HARVESTER_MOVE,
	STATE_OTHER_UNIT
} GameState;
```
- 위와같이 방향키 입력이 아닐 때 키 입력을 분리해서 받기위해 게임상태를 정의하는 enum선언, 

```c
bool isBcommand = true;
bool isSpaceTrigger = false;
bool firstCall = true;
bool spaceStatus = true; /

```
- 위는 특정 STATUS가 되었을 때 출력해야할 사항을 무한 출력하지 않도록 선언한 bool 변수

```c
user_input = getInputKey();
		KEY key = get_key(user_input);
		

```
- 기존 스켈레톤 코드는 유저의 입력을 바로 key에 저장했지만 이러면 방향키를 제외한 키 입력을 사용하기 어려워 int 변수 user_input에 저장 후 키 확인용으로 사용




# 샌드웜 움직임 코드 해석

이 문서에서는 샌드웜의 행동을 구현하는 `findClosestUnit`, `updateSandwormBehavior`, `removeUnit` 함수의 코드를 한 줄씩 해석하여 설명한다.

## `findClosestUnit` 함수 해석

```c
Unit* findClosestUnit(POSITION current_pos, Unit* units) {
```
- `findClosestUnit` 함수는 샌드웜의 위치(`current_pos`)와 주어진 유닛 리스트(`units`)를 기준으로 가장 가까운 유닛을 찾는 함수이다.

```c
    Unit* closestUnit = NULL;
    int minDistance = MAP_HEIGHT * MAP_WIDTH;
```
- `closestUnit` 포인터를 초기화하고, `minDistance`를 맵의 최대 크기로 설정해 초기 거리 값을 크게 설정한다. 이후 이 값보다 더 작은 거리가 발견되면 갱신한다.

```c
    Unit* currentUnit = units;
```
- `currentUnit` 포인터는 유닛 연결리스트의 시작점을 가리킨다.

```c
    while (currentUnit != NULL) {
```
- 유닛 리스트의 끝까지 반복하며, 각 유닛에 대해 현재 위치와의 거리를 계산한다.

```c
        int distance = abs(current_pos.row - currentUnit->pos.row) + abs(current_pos.column - currentUnit->pos.column);
```
- 맨해튼 거리 공식(|x2 - x1| + |y2 - y2|)을 사용해 `current_pos`와 `currentUnit` 위치 사이의 거리를 계산한다.

```c
        if (distance < minDistance) {
            minDistance = distance;
            closestUnit = currentUnit;
        }
```
- 계산한 `distance`가 `minDistance`보다 작으면 `minDistance`를 갱신하고, `closestUnit`을 `currentUnit`으로 설정하여 가장 가까운 유닛을 추적한다.

```c
        currentUnit = currentUnit->next;
    }
```
- `currentUnit`을 다음 유닛으로 이동하여 반복을 계속한다.

```c
    return closestUnit;
}
```
- 가장 가까운 유닛을 가리키는 포인터 `closestUnit`을 반환한다.

## `updateSandwormBehavior` 함수 해석

```c
void updateSandwormBehavior(SANDWORM* sandworm, Unit** units, SPICE** spices, BUILDING* buildings) {
```
- `updateSandwormBehavior` 함수는 샌드웜의 행동을 업데이트하는 함수로, 샌드웜의 이동, 유닛 공격, 스파이스 배설 등의 행동을 처리한다.

```c
    POSITION spicePos = sandworm->position;
    if (sandworm == NULL) return;
```
- 샌드웜의 위치를 `spicePos`에 저장하고, `sandworm` 포인터가 유효하지 않으면 함수에서 빠져나간다.

```c
    if (sys_clock % 2500 != 0) {
        return;
    }
```
- 샌드웜이 일정 시간 간격으로만 이동하도록 하기 위해, `sys_clock`을 이용해 이동 주기를 체크한다.

```c
    Unit* closestUnit = findClosestUnit(sandworm->position, *units);
```
- 현재 샌드웜 위치를 기준으로 가장 가까운 유닛을 찾기 위해 `findClosestUnit` 함수를 호출한다.

```c
    if (closestUnit != NULL) {
```
- 가까운 유닛이 존재하는 경우, 해당 유닛을 향해 이동을 시작한다.

```c
        POSITION target_pos = closestUnit->pos;
        POSITION diff = psub(target_pos, sandworm->position);
        DIRECTION dir;
```
- `target_pos`는 가장 가까운 유닛의 위치이며, `diff`는 현재 샌드웜 위치와 목표 위치 간의 차이이다. `dir`은 이동 방향을 나타낸다.

```c
        if (abs(diff.row) >= abs(diff.column)) {
            dir = (diff.row >= 0) ? d_down : d_up;
        }
        else {
            dir = (diff.column >= 0) ? d_right : d_left;
        }
```
- `diff` 값에 따라 세로 또는 가로 방향으로 샌드웜이 이동하도록 설정한다.

```c
        POSITION next_pos = pmove(sandworm->position, dir);
```
- `next_pos`는 `dir` 방향으로 한 칸 이동한 샌드웜의 새로운 위치이다.

```c
        if (1 <= next_pos.row && next_pos.row <= MAP_HEIGHT - 2 &&
            1 <= next_pos.column && next_pos.column <= MAP_WIDTH - 2 &&
            map[0][next_pos.row][next_pos.column] == ' ') {
```
- `next_pos`가 맵 범위 내에 있고 빈 공간인지 확인한다.

```c
            map[1][sandworm->position.row][sandworm->position.column] = -1;
            setColor(sandworm->position, COLOR_DEFAULT);
            sandworm->position = next_pos;
            displayUnit(map, next_pos, COLOR_SANDWORM, 1, 1, 'W');
```
- 기존 위치에서 샌드웜을 제거하고, 새로운 위치에 샌드웜을 표시한다.

```c
            if (closestUnit->pos.row == sandworm->position.row && closestUnit->pos.column == sandworm->position.column) {
                removeUnit(units, closestUnit);
                insert_status_message("A Sandworm has eaten a unit!\n");
            }
        }
    }
```
- 이동 후 샌드웜의 위치가 유닛과 겹치면 `removeUnit` 함수를 호출해 유닛을 제거하고, 상태창에 메시지를 출력한다.

```c
    if (rand() % 100 < 30) {
        int spiceAmount = rand() % 9 + 1;
        *spices = createSpice(spiceAmount, spicePos, *spices);
        map[0][spicePos.row][spicePos.column] = spiceAmount + '0';
        setColor(spicePos, COLOR_SPICE);
        insert_status_message("A Sandworm has released some Spice!\n");
    }
}
```
- 30% 확률로 스파이스를 배설하며, `spiceAmount`는 1~9 사이의 랜덤 값이다. `createSpice` 함수를 통해 스파이스를 생성하고 맵에 위치와 색상을 표시한다.

## `removeUnit` 함수 해석

```c
void removeUnit(Unit** head, Unit* target) {
    if (head == NULL || *head == NULL || target == NULL) {
        return;
    }
```
- 유효하지 않은 포인터 값이 들어올 경우 함수를 종료한다.

```c
    Unit* current = *head;
    Unit* previous = NULL;
```
- `current`는 리스트의 첫 유닛을, `previous`는 바로 앞 유닛을 가리킨다.

```c
    if (current == target) {
        *head = current->next;
        free(current);
        return;
    }
```
- 삭제하려는 유닛이 리스트의 첫 유닛이면, `head`를 다음 유닛으로 설정하고 첫 유닛을 삭제한다.

```c
    while (current != NULL) {
        if (current == target) {
            previous->next = current->next;
            free(current);
            return;
        }
        previous = current;
        current = current->next;
    }
}
```
- 리스트를 순회하며 `target` 유닛을 찾아 제거하고, 이전 유닛과 연결을 유지한다.
