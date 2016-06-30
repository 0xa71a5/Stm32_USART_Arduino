#ifndef _USART
#define _USART
#include "stm32f10x.h"
#include <stdio.h>
#define ElemType       uint8_t
#define QueueSize      200
#define QueueFull      0  
#define QueueEmpty     1  
#define QueueOperateOk 2  

struct FifoQueue
{
    uint16_t front;     
    uint16_t rear;        
    uint16_t count;       
    ElemType dat[QueueSize];
};
void QueueInit(struct FifoQueue *Queue);
uint8_t QueueIn(struct FifoQueue *Queue,ElemType sdat);
uint8_t QueueOut(struct FifoQueue *Queue,ElemType *sdat);
void  Serial_begin();
uint8_t Serial_available();
uint8_t Serial_read();
int fputc(int ch, FILE *f);
#endif