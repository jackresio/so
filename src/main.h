#ifndef SISTEMI_OPERATIVI_MAIN_H
#define SISTEMI_OPERATIVI_MAIN_H

typedef enum {
    SUPPLY_END,
    DEMAND_END,
    DAYS
} TermStatus;

void free_memory(void);

void sig_handler(int sig_num);


#endif
