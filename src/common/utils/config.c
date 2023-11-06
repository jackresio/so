#include <stdlib.h>
#include <signal.h>
#include "utils.h"
#include <stdio.h>
#include <string.h>

int SO_NAVI;
int SO_PORTI;
int SO_MERCI;
int SO_SIZE;
int SO_MIN_VITA;
int SO_MAX_VITA;
int SO_LATO;
int SO_SPEED;
int SO_CAPACITY;
int SO_BANCHINE;
int SO_FILL;
int SO_LOADSPEED;
int SO_DAYS;
int SO_STORM_DURATION;
int SO_SWELL_DURATION;
int SO_MAELSTROM;

static int read_config(const char *path, const char *name) {
    FILE *f;
    char format[50];
    char line[50];
    char const *flag = " = %d";
    int value = -1;

    strcpy(format, name);
    strcat(format, flag);

    f = fopen(path, "r");
    if (f == NULL) {
        CHECK_ERROR
        exit(1);
    }

    while (fgets(line, 50, f) != NULL) {
        sscanf(line, format, &value);
    }
    fclose(f);

    return value;
}

void init_config(const char *path) {
    SO_NAVI = read_config(path, "SO_NAVI");
    SO_PORTI = read_config(path, "SO_PORTI");
    if (SO_PORTI < 4){
        SO_PORTI = 4;
    }
    SO_MERCI = read_config(path, "SO_MERCI");
    SO_SIZE = read_config(path, "SO_SIZE");
    SO_MIN_VITA = read_config(path, "SO_MIN_VITA");
    SO_MAX_VITA = read_config(path, "SO_MAX_VITA");
    SO_LATO = read_config(path, "SO_LATO");
    SO_SPEED = read_config(path, "SO_SPEED");
    SO_CAPACITY = read_config(path, "SO_CAPACITY");
    SO_BANCHINE = read_config(path, "SO_BANCHINE");
    SO_FILL = read_config(path, "SO_FILL");
    SO_LOADSPEED = read_config(path, "SO_LOADSPEED");
    SO_DAYS = read_config(path, "SO_DAYS");
    SO_STORM_DURATION = read_config(path, "SO_STORM_DURATION");
    SO_SWELL_DURATION = read_config(path, "SO_SWELL_DURATION");
    SO_MAELSTROM = read_config(path, "SO_MAELSTROM");

}
