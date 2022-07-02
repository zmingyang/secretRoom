#ifndef __FSM__
#define __FSM__

#include <map>
#include <vector>
#include <string>
#include <QGraphicsScene>
#include <QGraphicsView>

#include "diagram.h"
#include "node.h"

#include "inputBoard.h"
#include "outputBoard.h"

class inputIOBoard;
class outputIOBoard;

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

        //other flag;
        int elapseTimes;
        int NeedDo;
};

class stateMachine
{
    public:
        int createNode( QGraphicsView *in, QGraphicsScene *s, QMenu *menu, QMenu *edgeM);
        int createBoard(  QGraphicsView *in, QGraphicsScene *s, QMenu *menu  );

        int getNode_D_by_ID(int id, Node **n);
        int getNode_D_IF_by_ID(int id, Edge **n);

        int sentIOBoardMsg_by_ID(int id);
        int sentIOBoardMsg_by_BoardID(std::string SCM_ID);

        int update_DB_SCM_IOBoard( std::string  scmID, std::vector<int> pins);
        int update_DB_SCM_IOBoard(stateNode_Do * doNode);

        int updateIOBoardMsg_by_ID( std::string msg);


        std::vector <std::vector<Node*>> Node_D;
        std::vector<Edge *> Node_IF;
        std::vector<stateNode_Do *> Node_Do;
        std::vector<inputIOBoard*> Node_SCM_DB;
        std::vector<outputIOBoard*> Node_DB_SCM;
        std::vector<TD_IP*> Node_TD_IP;

        DB_SCM* search_DB_SCM_by_ID(std::string SCM_ID);

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
