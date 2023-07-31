#include "../util.h"
#include <assert.h>

typedef struct {
    bool is_trash;
    int bot_id;
} ChipInfo;

typedef struct {
    int value_1;
    int value_2;
    ChipInfo high;
    ChipInfo low;
} Bot;

void add_value(Bot *bot, int value) {
    if (bot->value_1 == 0)
        bot->value_1 = value;
    else {
        assert(bot->value_2 == 0);
        bot->value_2 = value;
    }
}

#define MAX_BOTS 300

void give_away(Bot *bot, Bot *bots, int outputs[3]) {
    int high     = bot->value_1 > bot->value_2 ? bot->value_1 : bot->value_2;
    int low      = bot->value_1 > bot->value_2 ? bot->value_2 : bot->value_1;
    bot->value_1 = 0;
    bot->value_2 = 0;
    if (bot->low.is_trash && bot->low.bot_id < 3)
        outputs[bot->low.bot_id] = low;
    if (bot->high.is_trash && bot->high.bot_id < 3)
        outputs[bot->high.bot_id] = high;
    if (!bot->low.is_trash) add_value(&bots[bot->low.bot_id], low);
    if (!bot->high.is_trash) add_value(&bots[bot->high.bot_id], high);
}

int find_bot_id_responsible_for(CharArray input, const int desired_high,
                                const int desired_low, int outputs[3]) {
    Bot bots[MAX_BOTS] = {0};
    char *line;
    char *lines = input.buf;
    while ((line = strtok_r(lines, "\n", &lines))) {
        // ex str: bot 2 gives low to bot 1 and high to output 0
        if (strstr(line, "value") == NULL) {
            int bot_id = atoi(strstr(line, "bot ") + strlen("bot "));
            assert(bot_id < MAX_BOTS);
            char *to          = strstr(line, "to ") + strlen("to ");
            bool low_is_trash = (to[0] == 'o');
            to += low_is_trash ? strlen("output ") : strlen("bot ");
            int low_bot        = atoi(to);
            to                 = strstr(to, "to ") + strlen("to ");
            bool high_is_trash = (to[0] == 'o');
            to += high_is_trash ? strlen("output ") : strlen("bot ");
            int high_bot      = atoi(to - 1);
            bots[bot_id].low  = (ChipInfo){low_is_trash, low_bot};
            bots[bot_id].high = (ChipInfo){high_is_trash, high_bot};
        } else {
            // ex str: value 5 goes to bot 2
            int value  = atoi(line + strlen("value "));
            int bot_id = atoi(strstr(line, "bot ") + strlen("bot "));
            add_value(&bots[bot_id], value);
        }
    }
    int responsible_id = -1;
    while (outputs[0] * outputs[1] * outputs[2] == 0 || responsible_id == -1) {
        for (int id = 0; id < MAX_BOTS; id++) {
            Bot *bot = &bots[id];
            if ((bot->value_1 == desired_high && bot->value_2 == desired_low) ||
                (bot->value_2 == desired_high && bot->value_1 == desired_low))
                responsible_id = id;
            if (bot->value_1 != 0 && bot->value_2 != 0) {
                give_away(&bots[id], &bots[0], outputs);
            }
        }
    }
    return responsible_id;
}

void test() {
    CharArray input = read_file("test_input.txt");
    int outputs[3]  = {0};
    ASSERT_EQ(find_bot_id_responsible_for(input, 5, 2, outputs), 2, "%d");
    ASSERT_EQ(outputs[0] * outputs[1] * outputs[2], 30, "%d");
    free(input.buf);
}

int main() {
    test();
    CharArray input = read_file("input.txt");
    int outputs[3]  = {0};
    printf("Part 1 answer is %d\n",
           find_bot_id_responsible_for(input, 61, 17, outputs));
    printf("Part 2 answer is %d\n", outputs[0] * outputs[1] * outputs[2]);
    free(input.buf);
    return 0;
}