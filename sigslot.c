#include "sigslot.h"

struct 
{
    signal_node_t node[MAX_SIGNAL_NUMBER];
    uint8_t writer;
    uint8_t reader;
    uint8_t counter;
}signal;

struct 
{
    slot_node_t node[MAX_SLOT_NUMBER];
}slot;

/**************************
 * @brief:signal slot fram initilize
 * 
 * ***********************/
void sigslot_init()
{
    memset(&signal,0,sizeof(signal));
    memset(&slot,0,sizeof(slot));
}
/**************************
 * @brief:signal emit
 * @param sig_name signal name,definition in sigslot_user.h
 * @param params parameters
 * @param params_len parameters length
 * @retval  true:emit success
 *          false:emit fail
 * ***********************/

bool sigslot_emit(signal_name_t sig_name,void *params,uint16_t params_len)
{
    uint8_t index=0;
    if(signal.counter==MAX_SIGNAL_NUMBER)return false;
    SIG_DIS_IRQ();
    index=signal.writer;
    signal.node[index].params=params;
    signal.node[index].params_len=params_len;
    signal.node[index].sig_name=sig_name;
    signal.counter++;
    signal.writer++;
    if(signal.writer==MAX_SIGNAL_NUMBER)signal.writer=0;
    SIG_EN_IRQ();
    return true;    
}
/**************************
 * @brief:connect signal and slot 
 * @param sig_name signal name,definition in sigslot_user.h
 * @param handler  slot function
 * @retval  -1:connect error
 *          other: slot id
 * ***********************/
int32_t sigslot_connect(signal_name_t sig_name,signal_handle handler)
{
    SIG_DIS_IRQ();
    for(uint8_t i=0;i<MAX_SLOT_NUMBER;i++)
    {
        if(slot.node[i].sig_name==SIG_NAME_NULL)
        {
            slot.node[i].sig_name=sig_name;
            slot.node[i].handler=handler;
            SIG_EN_IRQ();
            return i;
            
        }
    }
    SIG_EN_IRQ();
    return -1;    
}
/**************************
 * @brief:sidconnect signal and slot 
 * @param slot_id slot id
 * @retval  true :success
 *          flase:fail   
 * ***********************/
bool sigslot_disconnect(uint32_t slot_id)
{
    SIG_DIS_IRQ();
    if(slot_id>=MAX_SLOT_NUMBER)return false;
    slot.node[slot_id].sig_name=SIG_NAME_NULL;
    slot.node[slot_id].handler=NULL;
    SIG_EN_IRQ();
	return true;
}
/**************************
 * @brief:get signal and execute all connected slot 
 * 
 * ***********************/
void sigslot_exec()
{
    signal_node_t *pnode;
    while (signal.counter)
    {
        pnode=&(signal.node[signal.reader]);
        for(uint8_t i=0;i<MAX_SLOT_NUMBER;i++)
        {
            if(slot.node[i].sig_name==pnode->sig_name)
            {
                slot.node[i].handler(pnode->params,pnode->params_len);
            }
        }
        signal.reader++;
        if(signal.reader==MAX_SIGNAL_NUMBER)signal.reader=0;
        signal.counter--;
    }
}
