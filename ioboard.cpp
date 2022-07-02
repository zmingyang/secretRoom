#include "ioboard.h"
#include "msgQueue.hpp"
#include "cJSON.h"
#include "fsm.h"
#include "ioboardproperty.h"

//IOBoard ioBoard;



IOBoard::IOBoard(QGraphicsView *graphWidget, QString s, QMenu *ContextMenu, int Pin):
    graph(graphWidget),text(s),myContextMenu(ContextMenu)
{
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    fsmIOBoard = NULL;

    while(Pin>0)
    {
        node.Pin.push_back(0);
        Pin--;
    }

}




void IOBoard::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::darkGray);
    //painter->drawEllipse(-7, -7, 90, 20);
    int rad = 10;

    //QRadialGradient gradient(-3, 0, 10);
    //painter->setBrush(gradient);

    painter->setPen(QPen(Qt::black, 0));
    painter->drawRect(-(rad*node.Pin.size()/2), -10, (rad*node.Pin.size()/2), 30);

    for(int i=0; i < node.Pin.size(); i++)
    {
         QRect rect;
        if( i < node.Pin.size()/2)
        {
           rect = QRect(-(rad* (int)(node.Pin.size())/2) + ( i*rad ), -20, rad, 10);

        }
        else
        {
            rect = QRect(-(rad* (int)(node.Pin.size())/2) + ( (i -(int)(node.Pin.size())/2) *rad ), 20, rad, 10);
        }
        if(node.Pin[i] == 0 )
        {
            painter->setPen(QPen(Qt::black, 0));
            painter->setBrush(Qt::yellow);
            painter->drawEllipse(rect);
        }
        else
        {
            painter->setPen(QPen(Qt::red, 0));
            painter->setBrush(Qt::green);
            painter->drawEllipse(rect);
        }
    }
    painter->drawText(QPoint(10,0),text);
}

QRectF IOBoard::boundingRect() const
{
    int rad = 10;
    QRectF rec(-(rad* (int)(node.Pin.size())/2), -30, (rad*(int)(node.Pin.size())), 60);
    return rec;
}

void IOBoard::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
//    scene()->clearSelection();
//    setSelected(true);
//    myContextMenu->exec(event->screenPos());

    IOBoardProperty dlg(this);
    dlg.exec();



}

//void IOBoard::updateBoardState(std::string msg)
//{
//    fsm.updateIOBoardMsg_by_ID(msg);
//}

void IOBoard::DoUpdate( int number, int PinVal)
{

    if(number < node.Pin.size())
    {
        node.Pin[number] = PinVal;
    }
    update();
}

//void IOBoard::sentBoardcmd(std::string msg)
//{
//    ToMQTTQueue.putMsg(msg);
//}
