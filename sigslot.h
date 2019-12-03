#ifndef __SIGSLOT_H
#define __SIGSLOT_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "sigslot_user.h"

#define MAX_SIGNAL_NUMBER   20
#define MAX_SLOT_NUMBER     20
#define MAX_SIGNAL_NAME_LEN     10

#define SIG_DIS_IRQ()    __disable_irq()
#define SIG_EN_IRQ()     __enable_irq()



typedef void (*signal_handle)(void *params,uint16_t param_len);

typedef struct
{
    signal_name_t sig_name;
    void *params;
    uint16_t params_len;
}signal_node_t;

typedef struct
{
    signal_name_t sig_name;
    signal_handle handler;
}slot_node_t;


void sigslot_init(void);
bool sigslot_emit(signal_name_t sig_name,void *params,uint16_t params_len);
int32_t sigslot_connect(signal_name_t sig_name,signal_handle handler);
bool sigslot_disconnect(uint32_t id);
void sigslot_exec(void);



#endif
