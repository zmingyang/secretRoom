#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

//#include <vector>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#include <time.h>
#include <sys/time.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include "mqtt.h"
#include <mosquitto.h>
#include "cJSON.h"
//#include "lib_print.h"
#include "msgQueue.hpp"
#include "fsm.h"

#include <thread>

static struct mosquitto *mosq = NULL;

bool session = true;
#define 	FILE_TEm		"Config.json"


//#define HOST "192.168.0.247"

#define HOST "127.0.0.1"
//#define HOST "localhost"
#define PORT  1883
#define KEEP_ALIVE 6
#define MSG_MAX_SIZE  512

#define HOST_LOG 1



void Mosq_Send(char *topic,char *buf, int len)
{
	int retval = -1;
	int sendcount =0 ;

	retval = mosquitto_publish(mosq,NULL, (const char*) topic,len,buf,2,0);
	
	if(retval == MOSQ_ERR_NO_CONN)
	{

#if 1

		while(1)
		{
			sleep(1);
			if(mosquitto_connect(mosq, HOST, PORT, KEEP_ALIVE))
			{
				printf("JD Unable to connect.\r\n");
			}

			sendcount++;
			if(sendcount>=3)
				break;
		}
#endif
	}
}


void my_message_callback_GetData(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message)
{


    if(message->payloadlen)
	{
        if (strncmp((const char*)message->topic, "/Private", 7) == 0)
        {

            FromMQTTQueue.putMsg((char*)message->payload);
        }
        else if(strcmp((const char*)message->topic, TOPIC_FROM_IO_BOARD_MSG) == 0)
		{
            printf("get %s \n", TOPIC_FROM_IO_BOARD_MSG);
			printf("get %s \n", (char*)message->payload);


		}

		else
		{
             printf("unknow  topic! \n");
		}

	}
}

static std::thread thr;

static void sendThread()
{
    while (1)
    {
        std::string mqttMsg;
        ToMQTTQueue.getMsg(mqttMsg);

        cJSON* root;
        cJSON* element;
        root = cJSON_Parse(mqttMsg.c_str());

        std::string topic = cJSON_GetStringValue(cJSON_GetObjectItem(root, "PrivateTopic"));
        Mosq_Send( (char*)topic.c_str(), (char *)(mqttMsg.c_str()), mqttMsg.length());
        cJSON_Delete(root);

        printf("send success!!\n");
    }
}


void my_connect_callback(struct mosquitto *mosq, void *userdata, int result)
{
    if(!result)
	{


        for(int i = 0; i< fsm.Node_TD_IP.size(); i++)
        {
            std::string ip = fsm.Node_TD_IP[i]->IPAddr;
            std::string port = std::to_string(fsm.Node_TD_IP[i]->Port);
            std::string privateTopic = "/Private/" + ip + ":" + port + "/FromDev";
            mosquitto_subscribe(mosq, NULL, privateTopic.c_str(), 2);//private topic

        }

        mosquitto_subscribe(mosq, NULL, TOPIC_FROM_IO_BOARD_MSG, 2);
		//mosquitto_subscribe(mosq, NULL, TOPIC_FROM_IO_BOARD_MSG, 2);

        thr = std::thread(sendThread);
        thr.detach();
	}else{
        fprintf(stderr, "Connect failed\n");
    }
}

void my_subscribe_callback(struct mosquitto *mosq, void *userdata, int mid, int qos_count, const int *granted_qos)
{

}

void my_log_callback(struct mosquitto *mosq, void *userdata, int level, const char *str)
{
}




int connectMQTT()
{


    mosquitto_lib_init();
    mosq = mosquitto_new(NULL,session,NULL);
    if(!mosq){
        printf("create client failed..\n");
        mosquitto_lib_cleanup();
        
    }

	mosquitto_log_callback_set(mosq, my_log_callback);
	mosquitto_connect_callback_set(mosq, my_connect_callback);
	mosquitto_message_callback_set(mosq, my_message_callback_GetData);

	if(mosquitto_connect(mosq, HOST, PORT, KEEP_ALIVE)){
        fprintf(stderr, "Unable to connect mqtt server.\n");
		exit(1);

	}




	printf("mqtt connected .\n");

    int loop = mosquitto_loop_start(mosq);
	//mosquitto_loop_forever(mosq, -1, 1);

	// while(1)
	// {
	// 	char opt = getchar();
		
	// 	if (opt == '1')
	// 	{
			
	// 		printf( "send %s \n", Topic_DataCenter_Reg );

	// 		cJSON * root = cJSON_CreateObject();
	// 		cJSON_AddItemToObject(root, "token", cJSON_CreateString("123"));
	// 		char JsonCurrentTime[32];
	// 		GetCurrentJsonTime(JsonCurrentTime);
	// 		cJSON_AddItemToObject(root, "timestamp", cJSON_CreateString(JsonCurrentTime));

	// 		//const char* types[] = {"MultiMeter"};
	// 		const char* types[] = {""};
	// 		cJSON * jPageText = cJSON_CreateStringArray(types,0);

	// 		cJSON_AddItemToObject(root, "body", jPageText);
	// 		printf (" %s \n ", cJSON_Print(root));
	// 		Mosq_Send(Topic_DataCenter_Reg, cJSON_Print(root), strlen (cJSON_Print(root))) ;
			
	// 	}
	// 	else if (opt == '2')
	// 	{
	// 		cJSON * root = cJSON_CreateObject();
	// 		cJSON_AddItemToObject(root, "token", cJSON_CreateString("123"));
	// 		char JsonCurrentTime[32];
	// 		GetCurrentJsonTime(JsonCurrentTime);
	// 		cJSON_AddItemToObject(root, "timestamp", cJSON_CreateString(JsonCurrentTime));

	// 		//const char* types[] = {"MultiMeter"};
	// 		cJSON * body = cJSON_CreateArray();
	// 		cJSON * bodyObj = cJSON_CreateObject();

	// 		//a9da384c5e672fca
	// 		//67cb49c333866c01

	// 		cJSON_AddItemToObject(bodyObj	, "dev", cJSON_CreateString("MultiMeter_a9da384c5e672fca"));
	// 		//cJSON_AddItemToObject(bodyObj	, "dev", cJSON_CreateString("Meter_frozen_ba16817bd84c99da"));
	// 		cJSON_AddItemToObject(bodyObj	, "totalcall", cJSON_CreateString("1"));
	// 		cJSON * NullArray = cJSON_CreateArray();
	// 		cJSON_AddItemToObject(bodyObj	,"body", NullArray);

	// 		cJSON_AddItemToArray(body, bodyObj);
	// 		cJSON_AddItemToObject(root, "body", body);
	// 		printf (" %s \n ", cJSON_Print(root));
			
	// 			printf (" %s \n ", Topic_META_Request);
	// 		Mosq_Send(Topic_META_Request, cJSON_Print(root), strlen (cJSON_Print(root))) ;
	// 		//Mosq_Send(Topic_DataCenter_Reg ,var, strlen (cJSON_Print(root)） );
			
	// 	}
	// }


    return 0;
	
}


int disconnectMQTT()
{
    mosquitto_lib_cleanup();
}
