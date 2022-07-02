#ifndef __MAIN_TTU_JZQ__
#define __MAIN_TTU_JZQ__

#define MAIN_DEBUG_EN 1




#define TOPIC_FROM_IO_BOARD_MSG "/Public/FromDev"



extern int connectMQTT();

extern void Mosq_Send(char *topic,char *buf,int len);
#endif

