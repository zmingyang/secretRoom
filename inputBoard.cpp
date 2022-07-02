#include "inputBoard.h"
#include "msgQueue.hpp"
#include "cJSON.h"
#include "fsm.h"
#include "inputBoardproperty.h"

//IOBoard ioBoard;



inputIOBoard::inputIOBoard(QGraphicsView *graphWidget, QString s, QMenu *ContextMenu):
    graph(graphWidget),text(s),myContextMenu(ContextMenu)
{
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setFlag(QGraphicsItem::ItemIsSelectable, true);


}




void inputIOBoard::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::darkGray);
    //painter->drawEllipse(-7, -7, 90, 20);
    int rad = 10;

    //QRadialGradient gradient(-3, 0, 10);
    //painter->setBrush(gradient);

    painter->setPen(QPen(Qt::black, 0));
    painter->drawRect(-(rad*node->pins.size()/2), -10, (rad*node->pins.size()/2), 30);

    //for(int i=0; i < node->pins.size(); i++)
    int i = 0;
    for (auto& x: node->pins)
    {
         QRect rect;
        if( i < node->pins.size()/2)
        {
           rect = QRect(-(rad* (int)(node->pins.size())/2) + ( i*rad ), -20, rad, 10);

        }
        else
        {
            rect = QRect(-(rad* (int)(node->pins.size())/2) + ( (i -(int)(node->pins.size())/2) *rad ), 20, rad, 10);
        }
        if(x.second == 0 )
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
        i++;
    }
    painter->drawText(QPoint(10,0),text);
}

QRectF inputIOBoard::boundingRect() const
{
    int rad = 10;
    QRectF rec(-(rad* (int)(node->pins.size())/2), -30, (rad*(int)(node->pins.size())), 60);
    return rec;
}

void inputIOBoard::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
//    scene()->clearSelection();
//    setSelected(true);
//    myContextMenu->exec(event->screenPos());

    inputBoardProperty dlg(this);
    dlg.exec();

}

//void IOBoard::updateBoardState(std::string msg)
//{
//    fsm.updateIOBoardMsg_by_ID(msg);
//}

void inputIOBoard::DoUpdate( std::string key, int PinVal)
{

    node->pins[key] = PinVal;

    update();
}

//void IOBoard::sentBoardcmd(std::string msg)
//{
//    ToMQTTQueue.putMsg(msg);
//}
