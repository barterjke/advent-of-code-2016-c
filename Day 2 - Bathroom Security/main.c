#define _CRT_SECURE_NO_DEPRECATE

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct A {
    int a;
};

#define ASSERT_EQ(got, expected, format)                                       \
    if (got != expected) {                                                     \
        printf("Expected " #format ", got " #format "\n", expected, got);      \
        exit(0);                                                               \
    }

typedef struct {
    char *buf;
    size_t size;
} CharArray;

CharArray read_file(const char *file_path) {
    FILE *file = fopen(file_path, "r");
    assert(file != NULL);
    fseek(file, 0, SEEK_END);   // seek to end of file
    size_t file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    CharArray array = {.buf  = malloc(file_size * sizeof(char)),
                       .size = file_size};
    fread(array.buf, sizeof(char), file_size, file);
    fclose(file);
    return array;
}

int find_bathrom_code(CharArray input) {
    int x    = 1;
    int y    = 1;
    int code = 0;
    for (int i = 0; i < input.size; i++) {
        switch (input.buf[i]) {
        case 'U':
            if (y > 0) y--;
            break;
        case 'D':
            if (y < 2) y++;
            break;
        case 'L':
            if (x > 0) x--;
            break;
        case 'R':
            if (x < 2) x++;
            break;
        case '\n':
            code = code * 10 + 1 + y * 3 + x;
            break;
        default:
            printf("Error: '%c'\n", input.buf[i]);
            exit(0);
        }
    }
    return code;
}

#define CODE_SIZE 10

const char *find_unusual_bathrom_code(CharArray input) {
    char *keypad[5];
    keypad[0] = "  1 ";
    keypad[1] = " 234 ";
    keypad[2] = "56789";
    keypad[3] = " ABC ";
    keypad[4] = "  D  ";
    int x     = 0;
    int y     = 2;
    static char code[CODE_SIZE];
    memset(&code, 0, sizeof(code));
    size_t code_index = 0;
    for (int i = 0; i < input.size; i++) {
        switch (input.buf[i]) {
        case 'U':
            if (y > 0 && keypad[y - 1][x] != ' ') y--;
            break;
        case 'D':
            if (y + 1 < 5 && keypad[y + 1][x] != ' ') y++;
            break;
        case 'L':
            if (x > 0 && keypad[y][x - 1] != ' ') x--;
            break;
        case 'R':
            if (x + 1 < 5 && keypad[y][x + 1] != ' ') x++;
            break;
        case '\n':
            code[code_index++] = keypad[y][x];
            assert(code_index < 10);
            break;
        default:
            printf("Error: '%c'\n", input.buf[i]);
            exit(0);
        }
    }
    return code;
}

void test() {
    CharArray test_input = read_file("test_input.txt");
    ASSERT_EQ(find_bathrom_code(test_input), 1985, "%d");
    const char *test_code = find_unusual_bathrom_code(test_input);
    free(test_input.buf);
    if (strcmp(test_code, "5DB3") != 0) {
        printf("exptected %s, got %s\n", "5DB3", test_code);
        exit(0);
    }
}

int main() {
    test();
    CharArray input = read_file("input.txt");
    printf("Part 1 answer is %d\n", find_bathrom_code(input));
    printf("Part 2 answer is %s\n", find_unusual_bathrom_code(input));
    free(input.buf);
    return 0;
}