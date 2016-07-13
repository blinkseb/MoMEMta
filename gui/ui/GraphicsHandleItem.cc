#include <GraphicsHandleItem.h>

#include <GraphicsConnectionItem.h>

#include <QGraphicsScene>
#include <QPainter>
#include <QtDebug>

GraphicsHandleItem::GraphicsHandleItem(GraphicsHandleItem::Role role, const QString& name, QGraphicsItem* parent): QGraphicsItem(parent) {
    role_ = role;
    name_ = name;
    setToolTip(name_);

    radius_ = 5.;
    //pen = QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap);
    pen = Qt::NoPen;
    brush = QBrush(QColor::fromRgb(143, 129, 100));

    boundingBox = QRectF(-radius_ - pen.widthF() / 2.,
            -radius_ - pen.widthF() / 2.,
            2 * radius_ + pen.widthF(),
            2 * radius_ + pen.widthF());

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

    painter->drawEllipse(QPointF(0, 0), radius_, radius_);
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

GraphicsHandleItem::Role GraphicsHandleItem::role() const {
    return role_;
}

void GraphicsHandleItem::addConnection(GraphicsConnectionItem* line) {
    connections.append(line);
}

void GraphicsHandleItem::removeConnection(GraphicsConnectionItem* line) {
    connections.removeOne(line);
}

void GraphicsHandleItem::detachConnections(GraphicsConnectionItem* except) {
    while (!connections.isEmpty()) {
        auto connection = connections.takeFirst();

        if (connection == except)
            continue;

        connection->detach();
        scene()->removeItem(connection);
        delete connection;
    }
}

GraphicsConnectionItem* GraphicsHandleItem::startConnection() {
    auto line = new GraphicsConnectionItem(this);
    return line;
}
