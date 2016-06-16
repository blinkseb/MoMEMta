#include <GraphicsConnectionItem.h>

#include <GraphicsHandleItem.h>

#include <QPainterPath>
#include <QtDebug>

GraphicsConnectionItem::GraphicsConnectionItem(GraphicsHandleItem* from, QGraphicsItem* parent): QGraphicsPathItem(parent) {
    this->from = from;

    from->addConnection(this);
    temporaryFinalAnchor = from->scenePos() + QPointF(from->radius() / 2., from->radius() / 2.);
    
    setPen(QPen(QColor::fromRgb(217, 215, 172), 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

    updatePath();
}

void GraphicsConnectionItem::setFinalAnchor(GraphicsHandleItem* to_) {
    to = to_;

    if (to) {

        // If needed, swap from & to so that from is always an OUTPUT and to an INPUT
        if (to->role() == GraphicsHandleItem::Role::OUTPUT) {
            std::swap(from, to);
        }

        // An INPUT can only be connected to one OUTPUT
        to->detachConnections(this);
        from->removeConnection(this);

        from->addConnection(this);
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
    path.moveTo(from->scenePos());

    if (to)
        path.lineTo(to->scenePos());
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
