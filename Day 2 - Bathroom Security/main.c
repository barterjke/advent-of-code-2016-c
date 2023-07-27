#define _CRT_SECURE_NO_DEPRECATE

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define BUFF_SIZE 100

int main() {
    FILE * file = fopen("input.txt","r");
    char buf[BUFF_SIZE];
    memset(buf, 0, BUFF_SIZE);
    fread(buf, sizeof(char), BUFF_SIZE, file);
    fclose(file);
    int i = 0;
    while (buf[i] != 0){
        printf("%c %c\n", buf[i], buf[i + 1]);
        i += 4;
    }
    return 0;
}