#define _CRT_SECURE_NO_DEPRECATE

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFF_SIZE 1000

typedef struct {
  int x;
  int y;
} Vec2;

// strtok
// gcc -std=c11 main.c -o main.exe && ./main.exe

void swap(int *lhs, int *rhs) {
  int tmp = *lhs;
  *lhs = *rhs;
  *rhs = tmp;
}

int distance_walked(char *buff) {
  Vec2 position = {0, 0};
  Vec2 direction = {0, 1};
  char *buff_ptr = buff;
  while (*buff_ptr != 0) {
    char rotation = *buff_ptr;
    int length = strtol(buff_ptr + 1, &buff_ptr, 10);
    assert(buff_ptr - &buff[0] <= BUFF_SIZE);
    if (rotation == 'R')
      direction.x *= -1;
    else
      direction.y *= -1;
    swap(&direction.x, &direction.y);
    position.x += direction.x * length;
    position.y += direction.y * length;
    if (*buff_ptr == 0) {
      return abs(position.x) + abs(position.y);
    }
    buff_ptr += 2;
  }
  return -1;
}

int hash32shift(int key) {
  key = ~key + (key << 15); // key = (key << 15) - key - 1;
  key = key ^ (key >> 12);
  key = key + (key << 2);
  key = key ^ (key >> 4);
  key = key * 2057; // key = (key + (key << 3)) + (key << 11);
  key = key ^ (key >> 16);
  return key;
}

#define HASH_SIZE BUFF_SIZE

typedef struct {
  Vec2 key;
  int value;
} Pair;

typedef struct {
  Pair *array[HASH_SIZE];
  int (*hash_func)(int);
} HashMap;

Pair **contains(HashMap *map, int x, int y) {
  int index = (map->hash_func(x) ^ map->hash_func(x)) % HASH_SIZE;
  while (index < HASH_SIZE) {
    Pair *pair = map->array[index];
    if (pair == NULL || (pair->key.x == x && pair->key.y == y))
      return &map->array[index];
    index++;
  }
  assert(0);
  return 0;
}

int *get(HashMap *map, int x, int y) {
  Pair **pair = contains(map, x, y);
  if (*pair == NULL) {
    *pair = malloc(sizeof(Pair));
    (*pair)->key.x = x;
    (*pair)->key.y = y;
    (*pair)->value = 0;
    return &(*pair)->value;
  }
  assert((*pair)->key.x == x && (*pair)->key.y == y);
  return &(*pair)->value;
}

void set(HashMap *map, int x, int y, int value) { *get(map, x, y) = value; }

int visited_twice(char *buff) {
  Vec2 position = {0, 0};
  Vec2 direction = {0, 1};
  char *buff_ptr = buff;
  HashMap map;
  map.hash_func = &hash32shift;
  memset(map.array, 0, sizeof(map.array));
  set(&map, position.x, position.y, 1);
  while (*buff_ptr != 0) {
    char rotation = *buff_ptr;
    int length = strtol(buff_ptr + 1, &buff_ptr, 10);
    assert(buff_ptr - &buff[0] <= BUFF_SIZE);
    if (rotation == 'R')
      direction.x *= -1;
    else
      direction.y *= -1;
    swap(&direction.x, &direction.y);
    for (int i = 0; i < length; i++) {
      position.x += direction.x;
      position.y += direction.y;
      Pair **pair = contains(&map, position.x, position.y);
      if (*pair != NULL)
        return abs((*pair)->key.x) + abs((*pair)->key.y);
      set(&map, position.x, position.y, 1);
    }
    if (*buff_ptr == 0)
      break;
    buff_ptr += 2;
  }
  return -1;
}

int main() {
  FILE *file = fopen("input.txt", "r");
  char buff[BUFF_SIZE];
  memset(buff, 0, BUFF_SIZE);
  fread(buff, sizeof(char), BUFF_SIZE, file);
  fclose(file);
  assert(distance_walked("R2, L3") == 5);
  assert(distance_walked("R2, R2, R2") == 2);
  assert(distance_walked("R5, L5, R5, R3") == 12);
  printf("Part 1 answer is %d\n", distance_walked(&buff[0]));
  assert(visited_twice("R8, R4, R4, R8") == 4);
  printf("Part 2 answer is %d\n", visited_twice(&buff[0]));
  return 0;
}