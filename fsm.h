#ifndef __FSM__
#define __FSM__

#include <map>
#include <vector>
#include <string>
#include <QGraphicsScene>
#include <QGraphicsView>

#include "edge.h"
#include "node.h"

class TD_IP
{
public:
    int id;
    std::string IPAddr;
    int iNum;
    int Dispose;
    std::string dscr;
    int isConn;
    int Port;
    int num;
};

class DB_SCM
{
public:
    int id;
    std::string IP_addr;
    std::string SCM_addr;
    std::string type;
    std::string dscr;
    int P00;
    int P01;
    int P02;
    int P03;
    int P04;
    int P05;
    int P06;
    int P07;

    int P10;
    int P11;
    int P12;
    int P13;
    int P14;
    int P15;
    int P16;
    int P17;

    int P20;
    int P21;
    int P22;
    int P23;
    int P24;
    int P25;
    int P26;
    int P27;

    int P30;
    int P31;
    int P32;
    int P33;
    int P34;
    int P35;
    int P36;
    int P37;

    int V01;
    int V02;
    int V03;
    int V04;
    int V05;
    int V06;
    int V07;
    int V08;
    int V09;
    int V10;
    std::string OldSendStr;
    std::string titles;
    std::string camp;
    int Ipid;
    int num;
};

class SCM_DB
{
public:
    int id;
    std::string IP_addr;
    std::string SCM_addr;
    std::string type;
    std::string dscr;
    int P00;
    int P01;
    int P02;
    int P03;
    int P04;
    int P05;
    int P06;
    int P07;

    int P10;
    int P11;
    int P12;
    int P13;
    int P14;
    int P15;
    int P16;
    int P17;

    int P20;
    int P21;
    int P22;
    int P23;
    int P24;
    int P25;
    int P26;
    int P27;

    int P30;
    int P31;
    int P32;
    int P33;
    int P34;
    int P35;
    int P36;
    int P37;

    int V01;
    int V02;
    int V03;
    int V04;
    int V05;
    int V06;
    int V07;
    int V08;
    int V09;
    int V10;
    std::string OldSendStr;
    std::string titles;
    std::string camp;
    int Ipid;
    int num;
};




class stateNode_Do
{
    public:

    //private:
        int id;
        int Task_D_ID;
        std::string DB_TO_SCM_Addr;
        std::string P;
        std::string V;
        std::string Dscr;
        int waitss;
        int isDo;
        int SCM_ID;
};

class stateMachine
{
    public:
        int createNode( QGraphicsView *in, QGraphicsScene *s, QMenu *menu, QMenu *edgeM);
        int createBoard(  QGraphicsView *in, QGraphicsScene *s, QMenu *menu  );

        int getNode_D_by_ID(int id, Node **n);
        int sentIOBoardMsg_by_ID(int id);
        int sentIOBoardMsg_by_BoardID(int id);
        int update_DB_SCM_IOBoard(int id, std::vector<int> pins);

        int updateIOBoardMsg_by_ID( std::string msg);

        std::vector <std::vector<Node*>> Node_D;
        std::vector<Edge *> Node_IF;
        std::vector<stateNode_Do *> Node_Do;
        std::vector<SCM_DB*> Node_SCM_DB;
        std::vector<DB_SCM*> Node_DB_SCM;
        std::vector<TD_IP*> Node_TD_IP;

        DB_SCM* search_DB_SCM_by_ID(int SCM_ID);

        int tick();

    private:
        int load_TD_Task_D_Do();
        int load_TD_Task_D_IF();
        int load_TD_Task_D();
        int load_TD_DB_TO_SCM();
        int load_TD_SCM_TO_DB();
        int load_TD_IP();



        void generateMQTTMsg(int ipID, std::string rawMsg);


        QGraphicsView *graphicsView;
        QGraphicsScene *scene;
        QMenu *mymenu;
        QMenu *edgeMenu;

        QGraphicsView *IOBoardGraphicsView;
        QGraphicsScene *IOBoardScene;
        QMenu *IOBoardmenu;



};

extern stateMachine fsm;

#endif
