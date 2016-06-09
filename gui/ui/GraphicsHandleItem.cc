#include <GraphicsHandleItem.h>

#include <GraphicsConnectionItem.h>

#include <QPainter>
#include <QtDebug>

GraphicsHandleItem::GraphicsHandleItem(QGraphicsItem* parent): QGraphicsItem(parent) {
    pen = QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap);
    brush = QBrush(Qt::white);

    boundingBox = QRectF(0. - pen.widthF() / 2.,
            0 - pen.widthF() / 2.,
            radius + pen.widthF(),
            radius + pen.widthF());

    setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
}

QRectF GraphicsHandleItem::boundingRect() const {
    return boundingBox;
}

void GraphicsHandleItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
        QWidget *widget) {
    painter->setBrush(brush);
    painter->setPen(pen);

    painter->drawEllipse(0., 0., radius, radius);
}

QVariant GraphicsHandleItem::itemChange(GraphicsItemChange change, const QVariant &value) {

    if (change == ItemScenePositionHasChanged && scene()) {
        if (connectedLine)
            connectedLine->updatePath();

    }

    return QGraphicsItem::itemChange(change, value);
}

void GraphicsHandleItem::setConnectedLine(GraphicsConnectionItem* line) {
    connectedLine = line;
}
