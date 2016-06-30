#include "stm32f10x.h"
#include "usart.h"
struct FifoQueue MyQueue;
uint8_t data;
void QueueInit(struct FifoQueue *Queue)
{
		Queue->rear=0;
    Queue->front = Queue->rear;
    Queue->count = 0;   
}

uint8_t QueueIn(struct FifoQueue *Queue,ElemType sdat)
{
    if((Queue->front == Queue->rear) && (Queue->count == QueueSize))
    {    
        return QueueFull;    
    }else
    {  
        Queue->dat[Queue->rear] = sdat;
        Queue->rear = (Queue->rear + 1) % QueueSize;
        Queue->count = Queue->count + 1;
        return QueueOperateOk;
    }
}
// Queue Out
uint8_t QueueOut(struct FifoQueue *Queue,ElemType *sdat)
{
    if((Queue->front == Queue->rear) && (Queue->count == 0))
    {                   
        return QueueEmpty;
    }else
    {                 
        *sdat = Queue->dat[Queue->front];
        Queue->front = (Queue->front + 1) % QueueSize;
        Queue->count = Queue->count - 1;
        return QueueOperateOk;
    }
}
void  Serial_begin()
{
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef nvic;
		/* config USART1 clock */
		QueueInit(&MyQueue);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
		
		/* USART1 GPIO config */
		/* Configure USART1 Tx (PA.09) as alternate function push-pull */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	
		/* Configure USART1 Rx (PA.10) as input floating */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
			
		/* USART1 mode config */
		USART_InitStructure.USART_BaudRate = 115200;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No ;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		USART_Init(USART1, &USART_InitStructure); 
		USART_Cmd(USART1, ENABLE);
		USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
		
		nvic.NVIC_IRQChannel=USART1_IRQn;
		nvic.NVIC_IRQChannelCmd=ENABLE;
		nvic.NVIC_IRQChannelPreemptionPriority=0;
		nvic.NVIC_IRQChannelSubPriority=0;
		NVIC_Init(&nvic);
		
}
int fputc(int ch, FILE *f)
{
		USART_SendData(USART1, (uint8_t) ch);
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);		
		return (ch);
}

uint8_t Serial_available()
{
	if(MyQueue.count==0&&(MyQueue.front==MyQueue.rear))
		return 0;
	else
		return 1;
}

uint8_t Serial_read()
{
	QueueOut(&MyQueue,&data);
	return data;
}
