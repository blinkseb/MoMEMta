#include <GraphicsConnectionItem.h>

#include <GraphicsHandleItem.h>

#include <QPainterPath>
#include <QtDebug>

GraphicsConnectionItem::GraphicsConnectionItem(GraphicsHandleItem* from, QGraphicsItem* parent): QGraphicsPathItem(parent) {
    this->from = from;

    from->addConnection(this);
    temporaryFinalAnchor = from->scenePos() + QPointF(from->radius() / 2., from->radius() / 2.);
    
    setPen(QPen(Qt::black, 2.5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

    updatePath();
}

void GraphicsConnectionItem::setFinalAnchor(GraphicsHandleItem* to) {
    this->to = to;

    if (to) {
        to->detachConnections();
        to->addConnection(this);
    }

    updatePath();
}

void GraphicsConnectionItem::setTemporaryFinalAnchor(const QPointF& point) {
    temporaryFinalAnchor = point;
    updatePath();
}

void GraphicsConnectionItem::updatePath() {
    prepareGeometryChange();

    QPainterPath path;
    path.moveTo(from->scenePos() + QPointF(from->radius() / 2., from->radius() / 2.));

    if (to)
        path.lineTo(to->scenePos() + QPointF(to->radius() / 2., to->radius() / 2.));
    else
        path.lineTo(temporaryFinalAnchor);

    setPath(path);
}

void GraphicsConnectionItem::detach() {
    from->removeConnection(this);
    if (to)
        to->removeConnection(this);

    from = nullptr;
    to = nullptr;
}

GraphicsHandleItem* GraphicsConnectionItem::originAnchor() const {
    return from;
}

GraphicsHandleItem* GraphicsConnectionItem::finalAnchor() const {
    return to;
}
