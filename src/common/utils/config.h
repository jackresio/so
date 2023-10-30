#ifndef SISTEMI_OPERATIVI_CONFIG_H
#define SISTEMI_OPERATIVI_CONFIG_H

extern int SO_NAVI;
extern int SO_PORTI;
extern int SO_MERCI;
extern int SO_SIZE;
extern int SO_MIN_VITA;
extern int SO_MAX_VITA;
extern int SO_LATO;
extern int SO_SPEED;
extern int SO_CAPACITY;
extern int SO_BANCHINE;
extern int SO_FILL;
extern int SO_LOADSPEED;
extern int SO_DAYS;
extern int SO_SWELL_DURATION;
extern int SO_STORM_DURATION;
extern int SO_MAELSTROM;

/**
 * Read specific configuration field
 * @param path path to configuration file
 * @param name configuration field name
 * @return return the config value of the field
 */
int read_config(const char *path, const char *name);


/**
 * Initialize all variable read from config file specified by path
 * @param path configurations file path
 */
void init_config(const char *path);

#endif //SISTEMI_OPERATIVI_CONFIG_H
