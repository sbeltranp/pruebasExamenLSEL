#ifndef _TEST_FSM_H
#define _TEST_FSM_H

#include "fsm.h"

bool is_true(fsm_t* f);
bool is_true2(fsm_t* f);
void do_nothing(fsm_t* f);

void* fsm_malloc(size_t s);
void fsm_free(void*);

#endif
