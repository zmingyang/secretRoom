#ifndef IOBOARD_H
#define IOBOARD_H

#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <string>
#include <vector>
#include "fsm.h"
class IOBoard: public QGraphicsItem //QGraphicsLineItem //
{
public:

    class BoardNode
    {
        public:
            std::vector <int> Pin;

    };
    int type; //0 ouput, 1 input
    void *fsmIOBoard;
    //DB_SCM *outPutBoard;
    //SCM_DB *inPutBoard;
    BoardNode node;

    IOBoard(QGraphicsView *graphWidget, QString s, QMenu *ContextMenu, int Pin);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    QRectF boundingRect() const override;
//    QPainterPath shape() const override;

//    void setColor(QColor c);
protected:
//    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

//    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
//    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;



//    void updateBoardState(std::string msg);
//    void sentBoardcmd(std::string msg );
      void DoUpdate( int number, int PinVal);


private:

    QList<Edge *> edgeList;
    QPointF newPos;
    QGraphicsView *graph;
    //QGraphicsScene scene;
    QString text;
    QColor bashColor;
    QMenu *myContextMenu;
};

//extern IOBoard ioBoard;
#endif // IOBOARD_H
