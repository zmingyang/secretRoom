#ifndef OUTPUT_IOBOARD_H
#define OUTPUT_IOBOARD_H

#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <string>
#include <vector>
#include "diagram.h"
#include "edge.h"
class QGraphicsView;

class DB_SCM
{
public:
    int id;
    std::string IP_addr;
    std::string SCM_addr;
    std::string type;
    std::string dscr;
    std::map<std::string,int> pins;

    std::string OldSendStr;
    std::string titles;
    std::string camp;
    int Ipid;
    int num;
    //outputIOBoard *boardPanel;
};

class outputIOBoard: public QGraphicsItem //QGraphicsLineItem //
{
public:



    DB_SCM *node;
    outputIOBoard(QGraphicsView *graphWidget, QString s, QMenu *ContextMenu);
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
      void DoUpdate( std::string key, int PinVal);


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
