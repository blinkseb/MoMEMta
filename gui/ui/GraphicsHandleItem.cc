#include <GraphicsHandleItem.h>

#include <GraphicsConnectionItem.h>

#include <QGraphicsScene>
#include <QPainter>
#include <QtDebug>

GraphicsHandleItem::GraphicsHandleItem(QGraphicsItem* parent): QGraphicsItem(parent) {
    radius_ = 10;
    pen = QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap);
    brush = QBrush(Qt::white);

    boundingBox = QRectF(0. - pen.widthF() / 2.,
            0 - pen.widthF() / 2.,
            radius_ + pen.widthF(),
            radius_ + pen.widthF());

    setAcceptHoverEvents(true);
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
}

QRectF GraphicsHandleItem::boundingRect() const {
    return boundingBox;
}

void GraphicsHandleItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
        QWidget *widget) {
    painter->setBrush(brush);
    painter->setPen(pen);

    painter->drawEllipse(0., 0., radius_, radius_);
}

void GraphicsHandleItem::hoverEnterEvent(QGraphicsSceneHoverEvent* event) {
    return QGraphicsItem::hoverEnterEvent(event);
}

void GraphicsHandleItem::hoverLeaveEvent(QGraphicsSceneHoverEvent* event) {
    return QGraphicsItem::hoverLeaveEvent(event);
}

QVariant GraphicsHandleItem::itemChange(GraphicsItemChange change, const QVariant &value) {

    if (change == ItemScenePositionHasChanged && scene()) {
        for (const auto& connection: connections)
            connection->updatePath();
    }

    return QGraphicsItem::itemChange(change, value);
}

qreal GraphicsHandleItem::radius() const {
    return radius_;
}

void GraphicsHandleItem::addConnection(GraphicsConnectionItem* line) {
    connections.append(line);
}

void GraphicsHandleItem::removeConnection(GraphicsConnectionItem* line) {
    connections.removeOne(line);
}

void GraphicsHandleItem::detachConnections() {
    while (!connections.isEmpty()) {
        auto connection = connections.takeFirst();
        connection->detach();
        scene()->removeItem(connection);
        delete connection;
    }
}

GraphicsConnectionItem* GraphicsHandleItem::startConnection() {
    auto line = new GraphicsConnectionItem(this);
    return line;
}
