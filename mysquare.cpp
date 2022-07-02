#include "mysquare.h"

mysquare::mysquare()
{
    pressed = false;

    setFlag(ItemIsMovable);
}

QRectF mysquare::boundingRect() const
{
    return QRectF(0,0,50,50);
}

void mysquare::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF rec = boundingRect();
    QBrush brush(Qt::blue);

    if (pressed == true)
    {
        brush.setColor(Qt::red);
    }
    else
    {
        brush.setColor(Qt::green);
    }

    painter->fillRect(rec, brush);
    painter->drawRect(rec);
}

void mysquare::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
        pressed = true;
        update();
        QGraphicsItem::mousePressEvent(event);

}

void mysquare::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    pressed = false;
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}
