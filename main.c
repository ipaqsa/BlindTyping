#include <stdio.h>
#include <termios.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define RED "\x1B[31m"
#define RST "\x1B[0m"
typedef struct {
    int x;
    int y;
} point_t;

typedef struct {
    char keyboard[3][13];
} vkeyboard_t;

void show_floor(int flag, int y_o, point_t point) {
    printf("\n");
    if (flag == 1) {
        char floor_up[12][6] = {"_____ ", "_____ ", "_____ ", "_____ ", "_____ ", "_____ ",
                                "_____ ", "_____ ", "_____ ", "_____ ", "_____ ", "_____ "};
        for (int y = 0; y < 12; y++) {
            if (y_o == point.y && y == point.x) printf(RED);
            for (int x = 0; x < 6; x++)
                printf("%c", floor_up[y][x]);
            printf(RST);
        }
        printf("\n");
    }
    else {
        char floor_down[12][6] = {"----- ", "----- ", "----- ", "----- ", "----- ", "----- ",
                                  "----- ", "----- ", "----- ", "----- ", "----- ", "----- "};
        for (int y = 0; y < 12; y++) {
            if (y_o == point.y && y == point.x) printf(RED);
            for (int x = 0; x < 6; x++)
                printf("%c", floor_down[y][x]);
            printf(RST);
        }
    }
}

void keyboard_show(vkeyboard_t* vkeyboard, char actual_char) {
    point_t actual_point;
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 12; x++) {
            if (vkeyboard->keyboard[y][x] == actual_char) {
                actual_point.x = x;
                actual_point.y = y;
            }
        }
    }
    for (int y = 0; y < 3; y++) {
        show_floor(1,y,actual_point);
        for (int x = 0; x < 12; x++) {
            if (vkeyboard->keyboard[y][x] == actual_char) printf(RED);
            printf("| %c | ", vkeyboard->keyboard[y][x]);
            printf(RST);
        }
        show_floor(0,y, actual_point);
    }
}

void words_init(char **words) {
    FILE *file = fopen("../words.txt", "r");
    char buffer[128];
    memset(buffer, 0, 128);
    int c_w = 0;
    while (!feof(file)) {
       fgets(buffer, 128, file);
       int s_e = 0;
       while (buffer[s_e] != 0) {
           s_e++;
       }
       words[c_w] = malloc(sizeof(char)*s_e);
       strncpy(words[c_w], buffer, s_e-1);
       c_w++;
    }
}

int generate(int max) {
    return rand() & max;
}


int main() {
    srand(time(NULL));
    char **words = malloc(sizeof(char*)*50);
    words_init(words);
    vkeyboard_t vkeyboard;
    strcpy(vkeyboard.keyboard[0], "qwertyuiop[]");
    strcpy(vkeyboard.keyboard[1], "asdfghjkl;'\\");
    strcpy(vkeyboard.keyboard[2], "zxcvbnm,./");

    struct termios term_o, term_n;
    tcgetattr(fileno(stdin), &term_o);
    term_n = term_o;
    term_n.c_lflag &= ~ECHO;
    term_n.c_lflag &= ~ICANON;
    term_n.c_cc[VMIN] = 1;
    term_n.c_cc[VTIME] = 0;
    term_n.c_iflag &= ~ISIG;
    tcsetattr(fileno(stdin), TCSANOW, &term_n);

    int buffer_p = 0;
    char buffer[128];
    memset(buffer, 0, 128);
    int point = 0;

    int random_idx = generate(15);
    char *word = words[random_idx];
    char actual_char = -1;
    while (actual_char != '`') {
        scanf("%c", &actual_char);
        if (actual_char == 127) {
            buffer[buffer_p] = 0;
            buffer_p--;
            scanf("%c", &actual_char);
        }
        buffer[buffer_p] = actual_char;
        buffer_p++;
        if (buffer_p >= strlen(word)+1) {
            if (strcmp(buffer, word) != -1) point++;
            random_idx = generate(15);
            word = words[random_idx];
            memset(buffer, 0, 128);
            buffer_p = 0;
        }
        system("clear");
        printf("%s\t", word);
        printf("Points %d", point);
        printf("\n-----------------------------------------------------------------------------\n");
        printf("%s", buffer);
        printf("\n-----------------------------------------------------------------------------\n");
        keyboard_show(&vkeyboard, actual_char);
    }
    tcsetattr(fileno(stdin), TCSANOW, &term_o);
    return 0;
}
