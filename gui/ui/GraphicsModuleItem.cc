#include <GraphicsModuleItem.h>

#include <GraphicsHandleItem.h>

#include <QPainter>

#include <QtDebug>

GraphicsModuleItem::GraphicsModuleItem() {
    // FIXME: Take into account pen width
    boundingBox = QRectF(0, 0, 100, 200);

    auto child = new GraphicsHandleItem(this);
    child->setPos(5, 100);
    
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
}

QRectF GraphicsModuleItem::boundingRect() const {
    return boundingBox;
}

void GraphicsModuleItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
        QWidget *widget) {
    painter->drawRect(boundingBox);
}

QVariant GraphicsModuleItem::itemChange(GraphicsItemChange change, const QVariant &value) {

    if (change == ItemPositionChange && scene()) {
        // Ok
    }

    return QGraphicsItem::itemChange(change, value);
}
