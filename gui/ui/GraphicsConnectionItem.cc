#include <GraphicsConnectionItem.h>

#include <GraphicsHandleItem.h>

#include <QPainterPath>
#include <QtDebug>

GraphicsConnectionItem::GraphicsConnectionItem(GraphicsHandleItem* from, GraphicsHandleItem* to, QGraphicsItem* parent): QGraphicsPathItem(parent) {
    this->from = from;
    this->to = to;

    from->setConnectedLine(this);
    to->setConnectedLine(this);
    
    setPen(QPen(Qt::black, 5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

    updatePath();
}

void GraphicsConnectionItem::updatePath() {
    prepareGeometryChange();

    QPainterPath path;
    path.moveTo(from->scenePos() + QPointF(from->radius / 2., from->radius / 2.));
    path.lineTo(to->scenePos() + QPointF(to->radius / 2., to->radius / 2.));

    setPath(path);
}
