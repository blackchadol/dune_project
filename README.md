# dune_project

21학번 오준수


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
