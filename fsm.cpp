
#include <stdio.h>
#include <stdlib.h>

#include <string>
#include <math.h>
#include <time.h>
#include <QTime>
#include <QDir>
#include <QDate>

#include "fsm.h"
#include "msgQueue.hpp"
#include "cJSON.h"
#include "inputBoard.h"
#include "outputBoard.h"



static void Stringsplit(const std::string& str, const char split, std::vector<std::string>& res)
{
    if (str == "")		return;
    //在字符串末尾也加入分隔符，方便截取最后一段
    std::string strs = str + split;
    size_t pos = strs.find(split);

    // 若找不到内容则字符串搜索函数返回 npos
    while (pos != strs.npos)
    {
        std::string temp = strs.substr(0, pos);
        res.push_back(temp);
        //去掉已分割的字符串,在剩下的字符串中进行分割
        strs = strs.substr(pos + 1, strs.size());
        pos = strs.find(split);
    }
}

stateMachine fsm;
#define TD_Task_D_FILEPATH "db2json//TD_Task_D.json"
#define TD_Task_D_Do_FILEPATH "db2json//TD_Task_D_Do.json"
#define TD_Task_D_IF_FILEPATH "db2json//TD_Task_D_IF.json"
#define TD_DB_TO_SCM_FILEPATH "db2json//TD_DB_TO_SCM.json"
#define TD_SCM_TO_DB_FILEPATH "db2json//TD_SCM_TO_DB.json"
#define TD_IP_FILEPATH "db2json//TD_IP.json"


int stateMachine::load_TD_IP()
{
    FILE * pFile;
   long lSize;
   char * buffer;
   size_t result;

   pFile = fopen ( TD_IP_FILEPATH , "rb" );
   if (pFile==NULL) {fputs ("File error",stderr); exit (1);}

   // obtain file size:
   fseek (pFile , 0 , SEEK_END);
   lSize = ftell (pFile);
   rewind (pFile);

   // allocate memory to contain the whole file:
   buffer = (char*) malloc (sizeof(char)*lSize);
   if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}

   // copy the file into the buffer:
   result = fread (buffer,1,lSize,pFile);
   if (result != lSize) {fputs ("Reading error",stderr); exit (3);}

      /* the whole file is now loaded in the memory buffer. */

   // terminate
   fclose (pFile);

   cJSON* root;
   cJSON* element;

    root = cJSON_Parse(buffer);
   if (!root)
    {
        printf("DevList Invalid Json Error before: [%s]\n",cJSON_GetErrorPtr());
        return 1;
    }

   cJSON *array = cJSON_GetObjectItem(root, "RECORDS");
   int size = cJSON_GetArraySize(array);
   for( int i = 0; i< size; i++)
   {
      //printf("+++++++++++++++++++++++++++++++++++++++++++++++\n");
      printf ("%s \n",  cJSON_GetStringValue(cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "Dscr")));

      TD_IP *newSNode = new (TD_IP);

      newSNode->id =  cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "id" )->valueint;
      newSNode->IPAddr = cJSON_GetStringValue(cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "IPAddr"));
      newSNode->iNum = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "iNum" )->valueint;
      newSNode->Dispose = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "Dispose" )->valueint;
      if( cJSON_GetStringValue(cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "dscr")) != NULL)
      {
        newSNode->dscr = cJSON_GetStringValue(cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "dscr"));
      }
      else
      {
        newSNode->dscr = "";
      }
      newSNode->isConn = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "isConn" )->valueint;
      newSNode->Port = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "Port" )->valueint;

      Node_TD_IP.push_back(newSNode);
   }


   free (buffer);


   return 0;
}

int stateMachine::save_TD_IP( std::string filename )
{

   cJSON * root = cJSON_CreateObject();


    cJSON * body = cJSON_CreateArray();

   for(int i = 0; i < Node_TD_IP.size(); i++)
   {
        cJSON * bodyObj = cJSON_CreateObject();
       cJSON_AddItemToObject(bodyObj, "id", cJSON_CreateNumber(Node_TD_IP[i]->id));
       cJSON_AddItemToObject(bodyObj, "IPAddr", cJSON_CreateString(Node_TD_IP[i]->IPAddr.c_str()));

       cJSON_AddItemToObject(bodyObj, "iNum", cJSON_CreateNumber( Node_TD_IP[i]->iNum  ));
       cJSON_AddItemToObject(bodyObj, "Dispose", cJSON_CreateNumber( Node_TD_IP[i]->Dispose ));
       cJSON_AddItemToObject(bodyObj, "dscr", cJSON_CreateString(Node_TD_IP[i]->dscr.c_str()));
       cJSON_AddItemToObject(bodyObj, "isConn", cJSON_CreateNumber(Node_TD_IP[i]->isConn));
        cJSON_AddItemToObject(bodyObj, "Port", cJSON_CreateNumber(Node_TD_IP[i]->Port));
       cJSON_AddItemToObject(body, "RECORDS", bodyObj);
   }
    cJSON_AddItemToObject(root, "RECORDS", body);

   char *printBuf = cJSON_Print(root);


  FILE * pFile;
  long lSize;
  size_t result;

  pFile = fopen ( filename.c_str() , "wb" );
  if (pFile==NULL) {fputs ("File error",stderr); exit (1);}


  // write the buffer  into the file:
  lSize = strlen(printBuf);
  result = fwrite( printBuf, 1, lSize,pFile);
  if (result != lSize) {fputs ("Reading error",stderr); exit (3);}

     /* the whole file is now loaded in the memory buffer. */

  // terminate
  fclose (pFile);




   return 0;
}


int stateMachine::load_TD_SCM_TO_DB()
{
    FILE * pFile;
   long lSize;
   char * buffer;
   size_t result;

   pFile = fopen ( TD_SCM_TO_DB_FILEPATH , "rb" );
   if (pFile==NULL) {fputs ("File error",stderr); exit (1);}

   // obtain file size:
   fseek (pFile , 0 , SEEK_END);
   lSize = ftell (pFile);
   rewind (pFile);

   // allocate memory to contain the whole file:
   buffer = (char*) malloc (sizeof(char)*lSize);
   if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}

   // copy the file into the buffer:
   result = fread (buffer,1,lSize,pFile);
   if (result != lSize) {fputs ("Reading error",stderr); exit (3);}

      /* the whole file is now loaded in the memory buffer. */

   // terminate
   fclose (pFile);

   cJSON* root;
   cJSON* element;

    root = cJSON_Parse(buffer);
   if (!root)
    {
        printf("DevList Invalid Json Error before: [%s]\n",cJSON_GetErrorPtr());
        return 1;
    }

   cJSON *array = cJSON_GetObjectItem(root, "RECORDS");
   int size = cJSON_GetArraySize(array);
   for( int i = 0; i< size; i++)
   {
      //printf("+++++++++++++++++++++++++++++++++++++++++++++++\n");
      printf ("%s \n",  cJSON_GetStringValue(cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "Dscr")));

      //printf ("%s \n", key.c_str()  );

      SCM_DB *newSNode = new (SCM_DB);
      char * pEnd;
      newSNode->id = strtol(cJSON_GetStringValue(cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "id")), &pEnd, 10);
      newSNode->IP_addr = cJSON_GetStringValue(cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "IP_addr"));
      newSNode->SCM_addr = cJSON_GetStringValue(cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "SCM_addr"));
      newSNode->type = cJSON_GetStringValue(cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "type"));
      newSNode->dscr = cJSON_GetStringValue(cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "dscr"));
      newSNode->pins["P00"] = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "P00" )->valueint;
      newSNode->pins["P01"] = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "P01" )->valueint;
      newSNode->pins["P02"] = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "P02" )->valueint;
      newSNode->pins["P03"] = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "P03" )->valueint;
      newSNode->pins["P04"] = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "P04" )->valueint;
      newSNode->pins["P05"] = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "P05" )->valueint;
      newSNode->pins["P06"] = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "P06" )->valueint;
      newSNode->pins["P07"] = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "P07" )->valueint;

      newSNode->pins["P10"] = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "P10" )->valueint;
      newSNode->pins["P11"] = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "P11" )->valueint;
      newSNode->pins["P12"] = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "P12" )->valueint;
      newSNode->pins["P13"] = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "P13" )->valueint;
      newSNode->pins["P14"] = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "P14" )->valueint;
      newSNode->pins["P15"] = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "P15" )->valueint;
      newSNode->pins["P16"] = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "P16" )->valueint;
      newSNode->pins["P17"] = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "P17" )->valueint;

      newSNode->pins["P20"] = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "P20" )->valueint;
      newSNode->pins["P21"] = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "P21" )->valueint;
      newSNode->pins["P22"] = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "P22" )->valueint;
      newSNode->pins["P23"] = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "P23" )->valueint;
      newSNode->pins["P24"] = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "P24" )->valueint;
      newSNode->pins["P25"] = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "P25" )->valueint;
      newSNode->pins["P26"] = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "P26" )->valueint;
      newSNode->pins["P27"] = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "P27" )->valueint;

      newSNode->pins["P30"] = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "P30" )->valueint;
      newSNode->pins["P31"] = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "P31" )->valueint;
      newSNode->pins["P32"] = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "P32" )->valueint;
      newSNode->pins["P33"] = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "P33" )->valueint;
      newSNode->pins["P34"] = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "P34" )->valueint;
      newSNode->pins["P35"] = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "P35" )->valueint;
      newSNode->pins["P36"] = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "P36" )->valueint;
      newSNode->pins["P37"] = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "P37" )->valueint;



       newSNode->titles =  cJSON_GetStringValue(cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "titles"));
       newSNode->camp = cJSON_GetStringValue(cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "camp"));
       newSNode->Ipid = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "Ipid" )->valueint;
       newSNode->num = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "num" )->valueint;

       inputIOBoard *newBoard = new inputIOBoard(IOBoardGraphicsView, QString::fromLocal8Bit("IO输入板-") + QString::number(newSNode->id,10), IOBoardmenu);
       newBoard->node =newSNode;
       newBoard->setPos(QPointF(320, i*60));
       IOBoardScene->addItem(newBoard);
       Node_SCM_DB.push_back(newBoard);

   }


   free (buffer);


   return 0;
}


int stateMachine::save_TD_SCM_TO_DB(std::string filename)
{

    cJSON * root = cJSON_CreateObject();
    cJSON * body = cJSON_CreateArray();

    for(int i = 0; i < Node_SCM_DB.size(); i++)
    {

       cJSON * bodyObj = cJSON_CreateObject();
       cJSON_AddItemToObject(bodyObj, "id", cJSON_CreateString(std::to_string(Node_SCM_DB[i]->node->id).c_str()));
       cJSON_AddItemToObject(bodyObj, "IP_addr", cJSON_CreateString(Node_SCM_DB[i]->node->IP_addr.c_str()));

       cJSON_AddItemToObject(bodyObj, "SCM_addr", cJSON_CreateString( Node_SCM_DB[i]->node->SCM_addr.c_str() ));
       cJSON_AddItemToObject(bodyObj, "type", cJSON_CreateString( Node_SCM_DB[i]->node->type.c_str()));
       cJSON_AddItemToObject(bodyObj, "dscr", cJSON_CreateString(Node_SCM_DB[i]->node->dscr.c_str() ));
        for (auto x:Node_SCM_DB[i]->node->pins)
        {
            cJSON_AddItemToObject(bodyObj, x.first.c_str(), cJSON_CreateNumber(x.second ));
        }

       cJSON_AddItemToObject(bodyObj, "OldSendStr", cJSON_CreateString(Node_SCM_DB[i]->node->OldSendStr.c_str() ));
       cJSON_AddItemToObject(bodyObj, "titles", cJSON_CreateString(Node_SCM_DB[i]->node->titles.c_str() ));
       cJSON_AddItemToObject(bodyObj, "camp", cJSON_CreateString( Node_SCM_DB[i]->node->camp.c_str() ));
       cJSON_AddItemToObject(bodyObj, "Ipid", cJSON_CreateNumber(Node_SCM_DB[i]->node->Ipid ));
       cJSON_AddItemToObject(bodyObj, "num", cJSON_CreateNumber(Node_SCM_DB[i]->node->num  ));


       cJSON_AddItemToObject(body, "RECORDS", bodyObj);


    }
     cJSON_AddItemToObject(root, "RECORDS", body);

    char *printBuf = cJSON_Print(root);


   FILE * pFile;
   long lSize;
   size_t result;

   pFile = fopen ( filename.c_str() , "wb" );
   if (pFile==NULL) {fputs ("File error",stderr); exit (1);}


   // write the buffer  into the file:
   lSize = strlen(printBuf);
   result = fwrite( printBuf, 1, lSize,pFile);
   if (result != lSize) {fputs ("Reading error",stderr); exit (3);}

   /* the whole file is now loaded in the memory buffer. */
   // terminate
   fclose (pFile);


}
int stateMachine::load_TD_DB_TO_SCM()
{
    FILE * pFile;
   long lSize;
   char * buffer;
   size_t result;

   pFile = fopen ( TD_DB_TO_SCM_FILEPATH , "rb" );
   if (pFile==NULL) {fputs ("File error",stderr); exit (1);}

   // obtain file size:
   fseek (pFile , 0 , SEEK_END);
   lSize = ftell (pFile);
   rewind (pFile);

   // allocate memory to contain the whole file:
   buffer = (char*) malloc (sizeof(char)*lSize);
   if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}

   // copy the file into the buffer:
   result = fread (buffer,1,lSize,pFile);
   if (result != lSize) {fputs ("Reading error",stderr); exit (3);}

      /* the whole file is now loaded in the memory buffer. */

   // terminate
   fclose (pFile);



   cJSON* root;
   cJSON* element;

    root = cJSON_Parse(buffer);
   if (!root)
    {
        printf("DevList Invalid Json Error before: [%s]\n",cJSON_GetErrorPtr());
        return 1;
    }

   cJSON *array = cJSON_GetObjectItem(root, "RECORDS");
   int size = cJSON_GetArraySize(array);
   for( int i = 0; i< size; i++)
   {
      //printf("+++++++++++++++++++++++++++++++++++++++++++++++\n");
      printf ("%s \n",  cJSON_GetStringValue(cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "Dscr")));

      //printf ("%s \n", key.c_str()  );

      DB_SCM *newSNode = new (DB_SCM);
      char * pEnd;
      newSNode->id = strtol(cJSON_GetStringValue(cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "id")), &pEnd, 10);
      newSNode->IP_addr = cJSON_GetStringValue(cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "IP_addr"));
      newSNode->SCM_addr = cJSON_GetStringValue(cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "SCM_addr"));
      newSNode->type = cJSON_GetStringValue(cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "type"));
      newSNode->dscr = cJSON_GetStringValue(cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "dscr"));
      newSNode->pins["P00"] = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "P00" )->valueint;
      newSNode->pins["P01"] = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "P01" )->valueint;
      newSNode->pins["P02"] = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "P02" )->valueint;
      newSNode->pins["P03"] = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "P03" )->valueint;
      newSNode->pins["P04"] = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "P04" )->valueint;
      newSNode->pins["P05"] = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "P05" )->valueint;
      newSNode->pins["P06"] = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "P06" )->valueint;
      newSNode->pins["P07"] = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "P07" )->valueint;

      newSNode->pins["P10"] = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "P10" )->valueint;
      newSNode->pins["P11"] = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "P11" )->valueint;
      newSNode->pins["P12"] = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "P12" )->valueint;
      newSNode->pins["P13"] = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "P13" )->valueint;
      newSNode->pins["P14"] = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "P14" )->valueint;
      newSNode->pins["P15"] = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "P15" )->valueint;
      newSNode->pins["P16"] = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "P16" )->valueint;
      newSNode->pins["P17"] = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "P17" )->valueint;

      newSNode->pins["P20"] = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "P20" )->valueint;
      newSNode->pins["P21"] = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "P21" )->valueint;
      newSNode->pins["P22"] = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "P22" )->valueint;
      newSNode->pins["P23"] = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "P23" )->valueint;
      newSNode->pins["P24"] = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "P24" )->valueint;
      newSNode->pins["P25"] = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "P25" )->valueint;
      newSNode->pins["P26"] = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "P26" )->valueint;
      newSNode->pins["P27"] = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "P27" )->valueint;

      newSNode->pins["P30"] = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "P30" )->valueint;
      newSNode->pins["P31"] = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "P31" )->valueint;
      newSNode->pins["P32"] = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "P32" )->valueint;
      newSNode->pins["P33"] = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "P33" )->valueint;
      newSNode->pins["P34"] = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "P34" )->valueint;
      newSNode->pins["P35"] = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "P35" )->valueint;
      newSNode->pins["P36"] = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "P36" )->valueint;
      newSNode->pins["P37"] = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "P37" )->valueint;



       newSNode->titles =  cJSON_GetStringValue(cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "titles"));
       newSNode->camp = cJSON_GetStringValue(cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "camp"));
       newSNode->Ipid = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "Ipid" )->valueint;
       newSNode->num = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "num" )->valueint;


       outputIOBoard *newBoard = new outputIOBoard(IOBoardGraphicsView, QString::fromLocal8Bit("IO输出板-") + QString::number(newSNode->id,10), IOBoardmenu);
       IOBoardScene->addItem(newBoard);
       newBoard->setPos(QPointF(0, i*60));
       newBoard->node = newSNode;
       Node_DB_SCM.push_back(newBoard);



   }



   free (buffer);


   return 0;
}

int stateMachine::save_TD_DB_TO_SCM(std::string filename)
{

    cJSON * root = cJSON_CreateObject();
    cJSON * body = cJSON_CreateArray();

    for(int i = 0; i < Node_DB_SCM.size(); i++)
    {

       cJSON * bodyObj = cJSON_CreateObject();
       cJSON_AddItemToObject(bodyObj, "id", cJSON_CreateString(std::to_string(Node_DB_SCM[i]->node->id).c_str()));
       cJSON_AddItemToObject(bodyObj, "IP_addr", cJSON_CreateString(Node_DB_SCM[i]->node->IP_addr.c_str()));

       cJSON_AddItemToObject(bodyObj, "SCM_addr", cJSON_CreateString( Node_DB_SCM[i]->node->SCM_addr.c_str() ));
       cJSON_AddItemToObject(bodyObj, "type", cJSON_CreateString( Node_DB_SCM[i]->node->type.c_str()));
       cJSON_AddItemToObject(bodyObj, "dscr", cJSON_CreateString(Node_DB_SCM[i]->node->dscr.c_str() ));
        for (auto x:Node_DB_SCM[i]->node->pins)
        {
            cJSON_AddItemToObject(bodyObj, x.first.c_str(), cJSON_CreateNumber(x.second ));
        }

       cJSON_AddItemToObject(bodyObj, "OldSendStr", cJSON_CreateString(Node_DB_SCM[i]->node->OldSendStr.c_str() ));
       cJSON_AddItemToObject(bodyObj, "titles", cJSON_CreateString(Node_DB_SCM[i]->node->titles.c_str() ));
       cJSON_AddItemToObject(bodyObj, "camp", cJSON_CreateString( Node_DB_SCM[i]->node->camp.c_str() ));
       cJSON_AddItemToObject(bodyObj, "Ipid", cJSON_CreateNumber(Node_DB_SCM[i]->node->Ipid ));
       cJSON_AddItemToObject(bodyObj, "num", cJSON_CreateNumber(Node_DB_SCM[i]->node->num  ));


       cJSON_AddItemToObject(body, "RECORDS", bodyObj);


    }
     cJSON_AddItemToObject(root, "RECORDS", body);

    char *printBuf = cJSON_Print(root);


   FILE * pFile;
   long lSize;
   size_t result;

   pFile = fopen ( filename.c_str() , "wb" );
   if (pFile==NULL) {fputs ("File error",stderr); exit (1);}


   // write the buffer  into the file:
   lSize = strlen(printBuf);
   result = fwrite( printBuf, 1, lSize,pFile);
   if (result != lSize) {fputs ("Reading error",stderr); exit (3);}

   /* the whole file is now loaded in the memory buffer. */
   // terminate
   fclose (pFile);



}

int stateMachine::load_TD_Task_D_IF()
{
    FILE * pFile;
   long lSize;
   char * buffer;
   size_t result;

   pFile = fopen ( TD_Task_D_IF_FILEPATH , "rb" );
   if (pFile==NULL) {fputs ("File error",stderr); exit (1);}

   // obtain file size:
   fseek (pFile , 0 , SEEK_END);
   lSize = ftell (pFile);
   rewind (pFile);

   // allocate memory to contain the whole file:
   buffer = (char*) malloc (sizeof(char)*lSize);
   if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}

   // copy the file into the buffer:
   result = fread (buffer,1,lSize,pFile);
   if (result != lSize) {fputs ("Reading error",stderr); exit (3);}

      /* the whole file is now loaded in the memory buffer. */

   // terminate
   fclose (pFile);



   cJSON* root;
   cJSON* element;

	root = cJSON_Parse(buffer);
   if (!root)
	{
    	printf("DevList Invalid Json Error before: [%s]\n",cJSON_GetErrorPtr());
		return 1;
	}

   cJSON *array = cJSON_GetObjectItem(root, "RECORDS");
   int size = cJSON_GetArraySize(array);
   for( int i = 0; i< size; i++)
   {
      //printf("+++++++++++++++++++++++++++++++++++++++++++++++\n");
      printf ("%s \n",  cJSON_GetStringValue(cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "Dscr")));

      //printf ("%s \n", key.c_str()  );

      Edge::stateNode_IF newSNode;
      newSNode.id = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "id" )->valueint;
      newSNode.From_Task_D_ID = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "From_Task_D_ID" )->valueint;
      newSNode.To_Task_D_ID = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "To_Task_D_ID" )->valueint;
      if(cJSON_GetStringValue(cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "Dscr"))  != NULL)
      {
        newSNode.Dscr =  cJSON_GetStringValue(cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "Dscr"));
      }
      else
      {
          newSNode.Dscr = "";
      }


      newSNode.waitss = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "waitss" )->valueint;


      //for (int i = 0; i < Node_D.size(); i++)


          //fsm.Node_IF[i].To_Task_D_ID;
          //scene->addItem(fsm.Node_IF[i].n);
      Node *from;
      Node *to;
      int ret = 0;
      ret = getNode_D_by_ID( newSNode.From_Task_D_ID, &from );
      if(ret != 0)
      {
          printf("something error \n");
          continue;
      }
      ret = getNode_D_by_ID( newSNode.To_Task_D_ID, &to );
      if(ret != 0)
      {
          printf("something error \n");
          continue;
      }

      Edge *newEdge = new Edge( from, to, edgeMenu );
      newEdge->IF = newSNode;
      Node_IF.push_back(newEdge);
      scene->addItem(newEdge);


   }


   free (buffer);


   return 0;
}

int stateMachine::save_TD_Task_D_IF( std::string filename )
{

    cJSON * root = cJSON_CreateObject();
    cJSON * body = cJSON_CreateArray();

    for(int i = 0; i < Node_IF.size(); i++)
    {

           cJSON * bodyObj = cJSON_CreateObject();
           cJSON_AddItemToObject(bodyObj, "id", cJSON_CreateNumber(Node_IF[i]->IF.id));
           cJSON_AddItemToObject(bodyObj, "From_Task_D_ID", cJSON_CreateNumber(Node_IF[i]->IF.From_Task_D_ID));

           cJSON_AddItemToObject(bodyObj, "To_Task_D_ID", cJSON_CreateNumber( Node_IF[i]->IF.To_Task_D_ID ));
           cJSON_AddItemToObject(bodyObj, "Dscr", cJSON_CreateString( Node_IF[i]->IF.Dscr.c_str()  ));
           cJSON_AddItemToObject(bodyObj, "waitss", cJSON_CreateNumber( Node_IF[i]->IF.waitss ));


           cJSON_AddItemToObject(body, "RECORDS", bodyObj);


    }
     cJSON_AddItemToObject(root, "RECORDS", body);

    char *printBuf = cJSON_Print(root);


   FILE * pFile;
   long lSize;
   size_t result;

   pFile = fopen ( filename.c_str() , "wb" );
   if (pFile==NULL) {fputs ("File error",stderr); exit (1);}


   // write the buffer  into the file:
   lSize = strlen(printBuf);
   result = fwrite( printBuf, 1, lSize,pFile);
   if (result != lSize) {fputs ("Reading error",stderr); exit (3);}

   /* the whole file is now loaded in the memory buffer. */
   // terminate
   fclose (pFile);




    return 0;
}
int stateMachine::getNode_D_by_ID(int id, Node **n)
{
    for(int i = 0; i < Node_D.size(); i++)
    {

        for (int ii = 0; ii < Node_D[i].size(); ii++)
        {
            if( Node_D[i][ii]->node.id == id )
            {
                *n = Node_D[i][ii];
                return 0;
            }
        }
    }
    return 1;
}

int stateMachine::getNode_D_IF_by_ID(int id, Edge **n)
{
    for(int i = 0; i < Node_IF.size(); i++)
    {

        if( Node_IF[i]->IF.id == id )
        {
            *n = Node_IF[i];
            return 0;
        }
    }
    return 1;
}
int stateMachine::load_TD_Task_D_Do( )
{
   FILE * pFile;
   long lSize;
   char * buffer;
   size_t result;

   pFile = fopen ( TD_Task_D_Do_FILEPATH , "rb" );
   if (pFile==NULL) {fputs ("File error",stderr); exit (1);}

   // obtain file size:
   fseek (pFile , 0 , SEEK_END);
   lSize = ftell (pFile);
   rewind (pFile);

   // allocate memory to contain the whole file:
   buffer = (char*) malloc (sizeof(char)*lSize);
   if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}

   // copy the file into the buffer:
   result = fread (buffer,1,lSize,pFile);
   if (result != lSize) {fputs ("Reading error",stderr); exit (3);}

      /* the whole file is now loaded in the memory buffer. */

   // terminate
   fclose (pFile);



   cJSON* root;
   cJSON* element;

	root = cJSON_Parse(buffer);
   if (!root)
	{
    	printf("DevList Invalid Json Error before: [%s]\n",cJSON_GetErrorPtr());
		return 1;
	}

   cJSON *array = cJSON_GetObjectItem(root, "RECORDS");
   int size = cJSON_GetArraySize(array);
   for( int i = 0; i< size; i++)
   {
        //printf("+++++++++++++++++++++++++++++++++++++++++++++++\n");
        printf ("%s \n",  cJSON_GetStringValue(cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "Dscr")));


        //printf ("%s \n", key.c_str()  );

        stateNode_Do *newSNode = new(stateNode_Do);
        newSNode->id = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "id" )->valueint;
        newSNode->Task_D_ID = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "Task_D_ID" )->valueint;
        newSNode->DB_TO_SCM_Addr = cJSON_GetStringValue(cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "DB_TO_SCM_Addr"));
        newSNode->P = cJSON_GetStringValue(cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "P"));
        newSNode->V = cJSON_GetStringValue(cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "V"));
        newSNode->Dscr = cJSON_GetStringValue(cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "Dscr"));

        newSNode->waitss = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "waitss" )->valueint;
        newSNode->isDo = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "isDo" )->valueint;
        newSNode->SCM_ID = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "SCM_ID" )->valueint;

        Node_Do.push_back( newSNode );

   }


   free (buffer);


   return 0;
}

int stateMachine::save_TD_Task_D_Do(std::string filename)
{


    cJSON * root = cJSON_CreateObject();
    cJSON * body = cJSON_CreateArray();

    for(int i = 0; i < Node_Do.size(); i++)
    {

           cJSON * bodyObj = cJSON_CreateObject();
           cJSON_AddItemToObject(bodyObj, "id", cJSON_CreateNumber(Node_Do[i]->id));
           cJSON_AddItemToObject(bodyObj, "Task_D_ID", cJSON_CreateNumber(Node_Do[i]->Task_D_ID));

           cJSON_AddItemToObject(bodyObj, "DB_TO_SCM_Addr", cJSON_CreateString( Node_Do[i]->DB_TO_SCM_Addr.c_str() ));
           cJSON_AddItemToObject(bodyObj, "P", cJSON_CreateString( Node_Do[i]->P.c_str() ));
           cJSON_AddItemToObject(bodyObj, "V", cJSON_CreateString( Node_Do[i]->V.c_str() ));
           cJSON_AddItemToObject(bodyObj, "Dscr", cJSON_CreateString( Node_Do[i]->Dscr.c_str() ));
            cJSON_AddItemToObject(bodyObj, "waitss", cJSON_CreateNumber( Node_Do[i]->waitss ));
            cJSON_AddItemToObject(bodyObj, "isDo", cJSON_CreateNumber( Node_Do[i]->isDo ));
            cJSON_AddItemToObject(bodyObj, "SCM_ID", cJSON_CreateNumber( Node_Do[i]->SCM_ID ));

           cJSON_AddItemToObject(body, "RECORDS", bodyObj);


    }
     cJSON_AddItemToObject(root, "RECORDS", body);

    char *printBuf = cJSON_Print(root);


   FILE * pFile;
   long lSize;
   size_t result;

   pFile = fopen ( filename.c_str() , "wb" );
   if (pFile==NULL) {fputs ("File error",stderr); exit (1);}


   // write the buffer  into the file:
   lSize = strlen(printBuf);
   result = fwrite( printBuf, 1, lSize,pFile);
   if (result != lSize) {fputs ("Reading error",stderr); exit (3);}

   /* the whole file is now loaded in the memory buffer. */
   // terminate
   fclose (pFile);




    return 0;
}

int stateMachine::load_TD_Task_D( )
{
   FILE * pFile;
   long lSize;
   char * buffer;
   size_t result;

   pFile = fopen ( TD_Task_D_FILEPATH , "rb" );
   if (pFile==NULL) {fputs ("File error",stderr); exit (1);}

   // obtain file size:
   fseek (pFile , 0 , SEEK_END);
   lSize = ftell (pFile);
   rewind (pFile);

   // allocate memory to contain the whole file:
   buffer = (char*) malloc (sizeof(char)*lSize);
   if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}

   // copy the file into the buffer:
   result = fread (buffer,1,lSize,pFile);
   if (result != lSize) {fputs ("Reading error",stderr); exit (3);}

      /* the whole file is now loaded in the memory buffer. */

   // terminate
   fclose (pFile);



   cJSON* root;
   cJSON* element;


   std::vector<Node*> tmp;
   Node_D.assign(1024,tmp);

	root = cJSON_Parse(buffer);
   if (!root)
	{
    	printf("DevList Invalid Json Error before: [%s]\n",cJSON_GetErrorPtr());
		return 1;
	}

   cJSON *array = cJSON_GetObjectItem(root, "RECORDS");
   int size = cJSON_GetArraySize(array);
   for( int i = 0; i< size; i++)
   {

      printf ("%s \n",  cJSON_GetStringValue(cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "Dscr")));

      std::string key = std::to_string(cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "M_id" )->valueint) + 
                        "_" +
                         std::to_string(cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "Step" )->valueint);


      Node::stateNode newSNode;
      newSNode.id = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "id" )->valueint;
      newSNode.M_id = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "M_id" )->valueint;
      newSNode.Step = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "Step" )->valueint;
      newSNode.Dscr = cJSON_GetStringValue(cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "Dscr"));
      newSNode.isPlaying = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "isPlaying" )->valueint;
      newSNode.isEnd = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "isEnd" )->valueint;
      newSNode.isPlayed = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "isPlayed" )->valueint;
      newSNode.point = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "point" )->valueint;
      newSNode.isBegin = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "isBegin" )->valueint;
      newSNode.isManualEnd = cJSON_GetObjectItem(cJSON_GetArrayItem(array, i), "isManualEnd" )->valueint;

      Node *newN = new Node(graphicsView,
                             QString::number(newSNode.id,10) + QString("_") + QString::number(newSNode.M_id,10) + QString("_") + QString::fromLocal8Bit(newSNode.Dscr.c_str()),
                             mymenu);
      //Node_D.insert( std::pair<std::string, stateNode>(key, *newSNode));

      newN->node = newSNode;
      Node_D[newSNode.M_id].push_back( newN);



   }


   free (buffer);



   for (int i = 0; i < Node_D.size(); i++)

   {

       for( int ii = 0; ii < Node_D[i].size(); ii++)

       {

           qreal centerX = 0;
           qreal centerY = Node_D[i][ii]->node.M_id * 500;
           qreal radius = 200;
           qreal size = qreal(Node_D[i].size() + 1);
           qreal step = qreal(360.0)/size;

           qreal x = centerX + radius * cosf(90 + ( ii * step));
           qreal y = centerY + radius * sinf(90 + (ii * step));
           Node_D[i][ii]->setPos(x, y);
           Node_D[i][ii]->setColor(Qt::red);

           scene->addItem( Node_D[i][ii]);
       }
   }

   return 0;
}


int stateMachine::save_TD_Task_D(std::string filename)
{


    cJSON * root = cJSON_CreateObject();
    cJSON * body = cJSON_CreateArray();

    for(int i = 0; i < Node_D.size(); i++)
    {
        for(int ii = 0; ii < Node_D[i].size(); ii++)
        {
           cJSON * bodyObj = cJSON_CreateObject();
           cJSON_AddItemToObject(bodyObj, "id", cJSON_CreateNumber(Node_D[i][ii]->node.id));
           cJSON_AddItemToObject(bodyObj, "M_id", cJSON_CreateNumber(Node_D[i][ii]->node.id));

           cJSON_AddItemToObject(bodyObj, "Step", cJSON_CreateNumber( Node_D[i][ii]->node.Step  ));
           cJSON_AddItemToObject(bodyObj, "Dscr", cJSON_CreateString( Node_D[i][ii]->node.Dscr.c_str()  ));
           cJSON_AddItemToObject(bodyObj, "isPlaying", cJSON_CreateNumber(Node_D[i][ii]->node.isPlaying ));
           cJSON_AddItemToObject(bodyObj, "isEnd", cJSON_CreateNumber(Node_D[i][ii]->node.isEnd ));
           //cJSON_AddItemToObject(bodyObj, "dt", cJSON_CreateNumber(Node_D[i][ii]->node ));
           cJSON_AddItemToObject(bodyObj, "isPlayed", cJSON_CreateNumber(Node_D[i][ii]->node.isPlayed ));
           cJSON_AddItemToObject(bodyObj, "point", cJSON_CreateNumber(Node_D[i][ii]->node.point ));
           cJSON_AddItemToObject(bodyObj, "isBegin", cJSON_CreateNumber(Node_D[i][ii]->node.isBegin ));
           cJSON_AddItemToObject(bodyObj, "isManualEnd", cJSON_CreateNumber(Node_D[i][ii]->node.isManualEnd ));

           cJSON_AddItemToObject(body, "RECORDS", bodyObj);
        }

    }
     cJSON_AddItemToObject(root, "RECORDS", body);

    char *printBuf = cJSON_Print(root);


   FILE * pFile;
   long lSize;
   size_t result;

   pFile = fopen ( filename.c_str() , "wb" );
   if (pFile==NULL) {fputs ("File error",stderr); exit (1);}


   // write the buffer  into the file:
   lSize = strlen(printBuf);
   result = fwrite( printBuf, 1, lSize,pFile);
   if (result != lSize) {fputs ("Reading error",stderr); exit (3);}

   /* the whole file is now loaded in the memory buffer. */
   // terminate
   fclose (pFile);






    return 0;
}

static int flag = 0;
void tickIOBoard()
{

                unsigned char C0 = 0x82;
                unsigned char C1 = 0x41;


                  unsigned char C2 = 0;
                if(flag == 0)
                {
                    C2 = 0x01;
                    flag = 1;
                }
                else
                {
                    C2 = 0x02;
                    flag = 0;
                }
                unsigned char C3 = 0x01;
                unsigned char C4 = 0x01;
                unsigned char C5 = 0x01;
                unsigned char C6 = 0x01;


                ///test
                //84 41 01 03 02 01 00 00 cc ff

                /// test


                unsigned int C7C8 = C0 + C1 + C2 + C3 + C4 + C5 + C6;

                unsigned char C7= ((C7C8 & 0xFF00) >> 8);
                unsigned char C8= C7C8 & 0xFF;
                unsigned char C9 = 0xFF;


                char msgBuf[64];
                memset(msgBuf, 0x0, sizeof(msgBuf));
                sprintf(msgBuf,"%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x", C0,C1,C2,C3,C4,C5,C6,C7,C8,C9);

                cJSON * root = cJSON_CreateObject();
                cJSON_AddItemToObject(root, "IP", cJSON_CreateString("192.168.0.112"));
                cJSON_AddItemToObject(root, "port", cJSON_CreateNumber(20108));
                //std::string Topic = "/Private/" + Node_TD_IP[i]->IPAddr + ":" + std::to_string(Node_TD_IP[i]->Port) + "/ToDev";
                std::string Topic = "/Private/192.168.0.112:20108/ToDev";

                cJSON_AddItemToObject(root, "PrivateTopic", cJSON_CreateString(Topic.c_str()));
                cJSON_AddItemToObject(root, "devType", cJSON_CreateString("ioBoard1.0"));
                cJSON_AddItemToObject(root, "MsgType", cJSON_CreateString("1"));//1: control IO 2: cmd
                cJSON_AddItemToObject(root, "devProperty", cJSON_CreateString(msgBuf));


                char *printBuf = cJSON_Print(root);

                ToMQTTQueue.putMsg(printBuf);
                free(printBuf);
                cJSON_Delete(root);


}

//int stateMachine::sentIOBoardMsg_by_ID(int id)
//{
//    std::thread startProcess =std::thread(tickIOBoard);
//    startProcess.detach();
//}


int stateMachine::update_DB_SCM_IOBoard(stateNode_Do * doNode)
{

    std::vector<std::string> Pvector,Vvector;


    Stringsplit(doNode->P, ',', Pvector);
    Stringsplit(doNode->V, ',', Vvector);

    for(int i = 0; i < Node_DB_SCM.size(); i++)
    {
        if(Node_DB_SCM[i]->node->SCM_addr  == doNode->DB_TO_SCM_Addr )
        {

            for(int ii = 0; ii < Pvector.size();ii++)
            {
                char * pEnd;
                std::string key = Pvector[ii];
                std::string val = Vvector[ii];
                Node_DB_SCM[i]->node->pins[Pvector[ii]] = strtol(Vvector[ii].c_str(), &pEnd, 10);

            }
            Node_DB_SCM[i]->update();

        }
    }


}

int stateMachine::update_DB_SCM_IOBoard(std::string  scmID, std::vector<int> pins)
{
    DB_SCM* dbScm = search_DB_SCM_by_ID(scmID);
    if(dbScm == NULL)
    {
        return -1;
    }

    dbScm->pins["P00"] = pins[0];
    dbScm->pins["P01"] = pins[1];
    dbScm->pins["P02"] = pins[2];
    dbScm->pins["P03"] = pins[3];
    dbScm->pins["P04"] = pins[4];
    dbScm->pins["P05"] = pins[5];

    dbScm->pins["P06"] = pins[6];
    dbScm->pins["P07"] = pins[7];
    dbScm->pins["P10"] = pins[8];
    dbScm->pins["P11"] = pins[9];
    dbScm->pins["P12"] = pins[10];
    dbScm->pins["P13"] = pins[11];

    dbScm->pins["P14"] = pins[12];
    dbScm->pins["P15"] = pins[13];
    dbScm->pins["P16"] = pins[14];
    dbScm->pins["P17"] = pins[15];
    dbScm->pins["P20"] = pins[16];
    dbScm->pins["P21"] = pins[17];

    dbScm->pins["P22"] = pins[18];
    dbScm->pins["P23"] = pins[19];
    dbScm->pins["P24"] = pins[20];
    dbScm->pins["P25"] = pins[21];
    dbScm->pins["P26"] = pins[22];
    dbScm->pins["P27"] = pins[23];

    dbScm->pins["P32"] = pins[24];
    dbScm->pins["P33"] = pins[25];
    dbScm->pins["P34"] = pins[26];
    dbScm->pins["P35"] = pins[27];
    dbScm->pins["P36"] = pins[28];
    dbScm->pins["P37"] = pins[29];


}

int stateMachine::sentIOBoardMsg_by_BoardID(std::string SCM_ID )
{
    DB_SCM* dbScm = search_DB_SCM_by_ID( SCM_ID);
    if(dbScm == NULL)
    {
        return -1;
    }

    std::string scm_addr = dbScm->SCM_addr;
    if(scm_addr.c_str()[0] == 'P')
    {
        return -1;
    }

    char * pEnd;
    unsigned char scmAddr =  strtol(dbScm->SCM_addr.c_str(),&pEnd, 2);
    unsigned char C0 = unsigned (scmAddr);
    unsigned char C1 = strtol(dbScm->type.c_str(),&pEnd, 2);
    unsigned char C2 = unsigned (dbScm->pins["P05"]<<5)|(dbScm->pins["P04"]<<4)|(dbScm->pins["P03"]<<3)|(dbScm->pins["P02"]<<2)|(dbScm->pins["P01"]<<1)|(dbScm->pins["P00"]<<0);
    unsigned char C3 = unsigned (dbScm->pins["P13"]<<5)|(dbScm->pins["P12"]<<4)|(dbScm->pins["P11"]<<3)|(dbScm->pins["P10"]<<2)|(dbScm->pins["P07"]<<1)|(dbScm->pins["P06"]<<0);
    unsigned char C4 = unsigned (dbScm->pins["P21"]<<5)|(dbScm->pins["P20"]<<4)|(dbScm->pins["P17"]<<3)|(dbScm->pins["P16"]<<2)|(dbScm->pins["P15"]<<1)|(dbScm->pins["P14"]<<0);
    unsigned char C5 = unsigned (dbScm->pins["P27"]<<5)|(dbScm->pins["P26"]<<4)|(dbScm->pins["P25"]<<3)|(dbScm->pins["P24"]<<2)|(dbScm->pins["P23"]<<1)|(dbScm->pins["P22"]<<0);
    unsigned char C6 = unsigned (dbScm->pins["P37"]<<5)|(dbScm->pins["P36"]<<4)|(dbScm->pins["P35"]<<3)|(dbScm->pins["P34"]<<2)|(dbScm->pins["P33"]<<1)|(dbScm->pins["P32"]<<0);

    ///test
    //84 41 01 03 02 01 00 00 cc ff
    /// test

    unsigned int C7C8 = C0 + C1 + C2 +C3 + C4 + C5 + C6;

    unsigned char C7= ((C7C8 & 0xFF00) >> 8);
    unsigned char C8= C7C8 & 0xFF;
    unsigned char C9 = 0xFF;


    char msgBuf[64];
    memset(msgBuf, 0x0, sizeof(msgBuf));
    sprintf(msgBuf,"%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x", C0,C1,C2,C3,C4,C5,C6,C7,C8,C9);

    generateMQTTMsg(dbScm->Ipid, msgBuf);
}

int stateMachine::sentIOBoardMsg_by_ID(int id)
{
    for(int i = 0; i < Node_Do.size(); i++)
    {
        if(Node_Do[i]->Task_D_ID == id)
        {

            DB_SCM* dbScm = search_DB_SCM_by_ID(Node_Do[i]->DB_TO_SCM_Addr);
            if(dbScm == NULL)
            {
                continue;
            }

            std::string scm_addr = dbScm->SCM_addr;
            if(scm_addr.c_str()[0] == 'P')
            {
                continue;
            }

            char * pEnd;
            unsigned char scmAddr =  strtol(dbScm->SCM_addr.c_str(),&pEnd, 2);

            unsigned char C0 = unsigned (scmAddr);
            unsigned char C1 = strtol(dbScm->type.c_str(),&pEnd, 2);
            unsigned char C2 = unsigned (dbScm->pins["P05"]<<5)|(dbScm->pins["P04"]<<4)|(dbScm->pins["P03"]<<3)|(dbScm->pins["P02"]<<2)|(dbScm->pins["P01"]<<1)|(dbScm->pins["P00"]<<0);
            unsigned char C3 = unsigned (dbScm->pins["P13"]<<5)|(dbScm->pins["P12"]<<4)|(dbScm->pins["P11"]<<3)|(dbScm->pins["P10"]<<2)|(dbScm->pins["P07"]<<1)|(dbScm->pins["P06"]<<0);
            unsigned char C4 = unsigned (dbScm->pins["P21"]<<5)|(dbScm->pins["P20"]<<4)|(dbScm->pins["P17"]<<3)|(dbScm->pins["P16"]<<2)|(dbScm->pins["P15"]<<1)|(dbScm->pins["P14"]<<0);
            unsigned char C5 = unsigned (dbScm->pins["P27"]<<5)|(dbScm->pins["P26"]<<4)|(dbScm->pins["P25"]<<3)|(dbScm->pins["P24"]<<2)|(dbScm->pins["P23"]<<1)|(dbScm->pins["P22"]<<0);
            unsigned char C6 = unsigned (dbScm->pins["P37"]<<5)|(dbScm->pins["P36"]<<4)|(dbScm->pins["P35"]<<3)|(dbScm->pins["P34"]<<2)|(dbScm->pins["P33"]<<1)|(dbScm->pins["P32"]<<0);


            ///test
            //84 41 01 03 02 01 00 00 cc ff

            /// test


            unsigned int C7C8 = C0 + C1 + C2 + C3 + C4 + C5 + C6;

            unsigned char C7= ((C7C8 & 0xFF00) >> 8);
            unsigned char C8= C7C8 & 0xFF;
            unsigned char C9 = 0xFF;


            char msgBuf[64];
            memset(msgBuf, 0x0, sizeof(msgBuf));
            sprintf(msgBuf,"%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x", C0,C1,C2,C3,C4,C5,C6,C7,C8,C9);

            generateMQTTMsg(dbScm->Ipid, msgBuf);


        }
    }
}



static std::vector<int> Transform(int n)
{
    std::vector<int>m;
    for(int i = 7; i>=0; i--)
    {
        m.push_back( ( (n>>i) & 1) );//与1做位操作，前面位数均为0
        //cout<<( (n>>i) & 1);//输出二进制
    }
    //cout<<endl;
    return m;
}

int stateMachine::updateIOBoardMsg_by_ID( std::string msg)
{

    cJSON* root;
    cJSON* element;
    root = cJSON_Parse(msg.c_str());

    std::string devProperty = cJSON_GetStringValue(cJSON_GetObjectItem(root, "devProperty"));
    std::string IP = cJSON_GetStringValue(cJSON_GetObjectItem(root, "IP"));
    std::string port = cJSON_GetStringValue(cJSON_GetObjectItem(root, "port"));
    std::vector<std::string> strGroup;

    Stringsplit(devProperty, ',', strGroup);


    unsigned char buf[10] = {0};
    printf("%d \n", strGroup.size());

    for (int i = 0; i < strGroup.size(); i++)
    {
     int nValude = 0;
     sscanf(strGroup[i].c_str(), "%x", &nValude);

     buf[i] = nValude;
    }

    for(int i = 0; i < 10; i++)
    {
     printf( "#16  %x \n", buf[i] );
    }

    std::vector<int> scmAddrVec = Transform(buf[0]);

    std::string scmAddrStr = std::to_string(scmAddrVec[0])+std::to_string(scmAddrVec[1])+std::to_string(scmAddrVec[2])+std::to_string(scmAddrVec[3])+
            std::to_string(scmAddrVec[4])+std::to_string(scmAddrVec[5])+std::to_string(scmAddrVec[6])+std::to_string(scmAddrVec[7]);


    for(int i = 0; i < fsm.Node_SCM_DB.size(); i++)
    {
        if(fsm.Node_SCM_DB[i]->node->IP_addr == IP && fsm.Node_SCM_DB[i]->node->SCM_addr ==  scmAddrStr)
        {


//            unsigned char C2 = unsigned (dbScm->P05<<5)|(dbScm->P04<<4)|(dbScm->P03<<3)|(dbScm->P02<<2)|(dbScm->P01<<1)|(dbScm->P00<<0);
//            unsigned char C3 = unsigned (dbScm->P13<<5)|(dbScm->P12<<4)|(dbScm->P11<<3)|(dbScm->P10<<2)|(dbScm->P07<<1)|(dbScm->P06<<0);
//            unsigned char C4 = unsigned (dbScm->P21<<5)|(dbScm->P20<<4)|(dbScm->P17<<3)|(dbScm->P16<<2)|(dbScm->P15<<1)|(dbScm->P14<<0);
//            unsigned char C5 = unsigned (dbScm->P27<<5)|(dbScm->P26<<4)|(dbScm->P25<<3)|(dbScm->P24<<2)|(dbScm->P23<<1)|(dbScm->P22<<0);
//            unsigned char C6 = unsigned (dbScm->P37<<5)|(dbScm->P36<<4)|(dbScm->P35<<3)|(dbScm->P34<<2)|(dbScm->P33<<1)|(dbScm->P32<<0);

            fsm.Node_SCM_DB[i]->node->pins["P00"] = (buf[2] >> 0)& 0x01;
            fsm.Node_SCM_DB[i]->node->pins["P01"] = (buf[2] >> 1)& 0x01;
            fsm.Node_SCM_DB[i]->node->pins["P02"] = (buf[2] >> 2)& 0x01;
            fsm.Node_SCM_DB[i]->node->pins["P03"] = (buf[2] >> 3)& 0x01;
            fsm.Node_SCM_DB[i]->node->pins["P04"] = (buf[2] >> 4)& 0x01;
            fsm.Node_SCM_DB[i]->node->pins["P05"] = (buf[2] >> 5)& 0x01;

            fsm.Node_SCM_DB[i]->node->pins["P06"] = (buf[3] >> 0)& 0x01;
            fsm.Node_SCM_DB[i]->node->pins["P07"] = (buf[3] >> 1)& 0x01;
            fsm.Node_SCM_DB[i]->node->pins["P10"] = (buf[3] >> 2)& 0x01;
            fsm.Node_SCM_DB[i]->node->pins["P11"] = (buf[3] >> 3)& 0x01;
            fsm.Node_SCM_DB[i]->node->pins["P12"] = (buf[3] >> 4)& 0x01;
            fsm.Node_SCM_DB[i]->node->pins["P13"] = (buf[3] >> 5)& 0x01;

            fsm.Node_SCM_DB[i]->node->pins["P14"] = (buf[4] >> 0)& 0x01;
            fsm.Node_SCM_DB[i]->node->pins["P15"] = (buf[4] >> 1)& 0x01;
            fsm.Node_SCM_DB[i]->node->pins["P16"] = (buf[4] >> 2)& 0x01;
            fsm.Node_SCM_DB[i]->node->pins["P17"] = (buf[4] >> 3)& 0x01;
            fsm.Node_SCM_DB[i]->node->pins["P20"] = (buf[4] >> 4)& 0x01;
            fsm.Node_SCM_DB[i]->node->pins["P21"] = (buf[4] >> 5)& 0x01;

            fsm.Node_SCM_DB[i]->node->pins["P22"] = (buf[5] >> 0)& 0x01;
            fsm.Node_SCM_DB[i]->node->pins["P23"] = (buf[5] >> 1)& 0x01;
            fsm.Node_SCM_DB[i]->node->pins["P24"] = (buf[5] >> 2)& 0x01;
            fsm.Node_SCM_DB[i]->node->pins["P25"] = (buf[5] >> 3)& 0x01;
            fsm.Node_SCM_DB[i]->node->pins["P26"] = (buf[5] >> 4)& 0x01;
            fsm.Node_SCM_DB[i]->node->pins["P27"] = (buf[5] >> 5)& 0x01;

            fsm.Node_SCM_DB[i]->node->pins["P32"] = (buf[6] >> 0)& 0x01;
            fsm.Node_SCM_DB[i]->node->pins["P33"] = (buf[6] >> 1)& 0x01;
            fsm.Node_SCM_DB[i]->node->pins["P34"] = (buf[6] >> 2)& 0x01;
            fsm.Node_SCM_DB[i]->node->pins["P35"] = (buf[6] >> 3)& 0x01;
            fsm.Node_SCM_DB[i]->node->pins["P36"] = (buf[6] >> 4)& 0x01;
            fsm.Node_SCM_DB[i]->node->pins["P37"] = (buf[6] >> 5)& 0x01;

        }
    }


    cJSON_Delete(root);
}

void stateMachine::generateMQTTMsg(int ipID, std::string rawMsg)
{
    for(int i = 0; i < Node_TD_IP.size(); i++)
    {

        if(Node_TD_IP[i]->id == ipID)
        {

            cJSON * root = cJSON_CreateObject();
            cJSON_AddItemToObject(root, "IP", cJSON_CreateString(Node_TD_IP[i]->IPAddr.c_str()));
            cJSON_AddItemToObject(root, "port", cJSON_CreateNumber(Node_TD_IP[i]->Port));
            std::string Topic = "/Private/" + Node_TD_IP[i]->IPAddr + ":" + std::to_string(Node_TD_IP[i]->Port) + "/ToDev";
            //std::string Topic = "/Private/192.168.0.48:20108/ToDev";

            cJSON_AddItemToObject(root, "PrivateTopic", cJSON_CreateString(Topic.c_str()));
            cJSON_AddItemToObject(root, "devType", cJSON_CreateString("ioBoard1.0"));
            cJSON_AddItemToObject(root, "MsgType", cJSON_CreateString("1"));//1: control IO 2: cmd
            cJSON_AddItemToObject(root, "devProperty", cJSON_CreateString(rawMsg.c_str()));


            char *printBuf = cJSON_Print(root);

            ToMQTTQueue.putMsg(printBuf);
            free(printBuf);
            cJSON_Delete(root);

        }

    }
}

DB_SCM* stateMachine::search_DB_SCM_by_ID(std::string SCM_ID)
{
    for(int i = 0; i < Node_DB_SCM.size(); i++)
    {
        if(Node_DB_SCM[i]->node->SCM_addr  == SCM_ID )
        {
            return Node_DB_SCM[i]->node;
        }
    }
    return NULL;
}
int stateMachine::createNode( QGraphicsView *in, QGraphicsScene *s, QMenu *menu, QMenu *edgeM )

{
    graphicsView = in;
    mymenu = menu;
    scene = s;
    edgeMenu = edgeM;

    load_TD_Task_D();
    load_TD_Task_D_IF();
    load_TD_Task_D_Do();

    load_TD_IP();

}
int stateMachine::createBoard( QGraphicsView *in, QGraphicsScene *s, QMenu *menu )
{
    IOBoardGraphicsView = in;
    IOBoardScene = s;
    IOBoardmenu = menu;

    load_TD_DB_TO_SCM();
    load_TD_SCM_TO_DB();


}
static int firstTime = 1;
int stateMachine::tick()
{

    if(firstTime == 1)
    {
        for(int i = 0; i < Node_D.size(); i++)
        {
            for(int ii = 0; ii < Node_D[i].size(); ii++)
            {
                if( Node_D[i][ii]->node.isBegin == 1 )
                {
                    Node_D[i][ii]->node.isPlaying = 1;
                }
            }

        }
        firstTime = 0;
    }



    for(int k = 0; k < Node_Do.size(); k++)
    {
        if ( Node_Do[k]->NeedDo == 1 )
        {
               Node_Do[k]->elapseTimes += 100;
        }

        if(Node_Do[k]->elapseTimes > Node_Do[k]->waitss)
        {
            //update DB_SCM
            std::vector<int> pin;
            update_DB_SCM_IOBoard(Node_Do[k]);

            Node_Do[k]->NeedDo = 0;
            Node_Do[k]->elapseTimes = 0;
        }
    }

    //update DB_SCM
    //update_DB_SCM_IOBoard();




  for(int i = 0; i < Node_D.size(); i++)
  {


      for(int ii = 0; ii < Node_D[i].size(); ii++)
      {
          if( Node_D[i][ii]->node.isPlaying == 1 )  //This node running, check all IF nodes, ElapsedTime+100
          {

              for(int j = 0; j < Node_IF.size(); j++)
              {

                  if(Node_D[i][ii]->node.id == Node_IF[j]->IF.From_Task_D_ID)
                  {

                      if( Node_IF[j]->ElapsedTime < Node_IF[j]->IF.waitss )
                      {
                          Node_IF[j]->ElapsedTime += 100;
                      }
                      else
                      {
                        Node_IF[j]->ElapsedTime = 0;
                        int fromNodeID = Node_IF[j]->IF.From_Task_D_ID;
                        int toNodeID = Node_IF[j]->IF.To_Task_D_ID;

                        Node *fromNodeD = NULL;
                        Node *toNodeD = NULL;
                        getNode_D_by_ID(fromNodeID, &fromNodeD);
                        if (fromNodeD != NULL)
                        {
                            fromNodeD->node.isPlaying = 0;
                            fromNodeD->setColor(Qt::red);
                        }

                        getNode_D_by_ID(toNodeID, &toNodeD);
                        if (toNodeD != NULL)
                        {
                            toNodeD->node.isPlaying = 1;
                            toNodeD->setColor(Qt::green);


                            //Task_D_ID
                            for(int k = 0; k < Node_Do.size(); k++)
                            {
                                if ( Node_Do[k]->Task_D_ID == toNodeD->node.id )
                                {
                                       Node_Do[k]->NeedDo = 1;
                                       Node_Do[k]->elapseTimes = 0;
                                }
                            }
                            //update DB_SCM
                            //update_DB_SCM_IOBoard();
                        }
                      }

                  }

              }



              break;
          }
      }


//    for(int ii = 0; i<Node_D[i].size(); ii++)
//    {
//        if( Node_D[i][ii]->node.isBegin == 1 )
//        {

//        }
//    }



  }
}



int stateMachine::backup()
{
    QDate currentData = QDate::currentDate();
    QTime *time = new QTime();

    QString dirname = currentData.toString("yyyy-MM-dd") + "-" + time->currentTime().toString("hh-mm-ss");
    QDir dir;
    dir.mkpath(dirname);


    save_TD_Task_D_Do(dirname.toStdString() + "//TD_Task_D_Do.json");

    save_TD_Task_D_IF(dirname.toStdString() + "//TD_Task_D_IF.json");

    save_TD_Task_D(dirname.toStdString() + "//TD_Task_D.json");

    save_TD_DB_TO_SCM(dirname.toStdString() + "//TD_DB_TO_SCM.json");

    save_TD_SCM_TO_DB(dirname.toStdString() + "//TD_SCM_TO_DB.json");

    save_TD_IP(dirname.toStdString() + "//TD_IP.json");
}


