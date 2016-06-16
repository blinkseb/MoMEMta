#include <GraphicsModuleItem.h>

#include <GraphicsHandleItem.h>

#include <QColor>
#include <QPainter>

#include <QtDebug>

GraphicsModuleItem::GraphicsModuleItem(const QString& name) {

    pen = QPen(QColor::fromRgb(79, 99, 115), 1, Qt::SolidLine, Qt::FlatCap);
    brush = QBrush(QColor::fromRgb(20, 33, 43));

    name_ = name;
    width_ = 120;
    height_ = 150;
    labelHeight_ = 0.1 * height_;

    topPadding_ = bottomPadding_ = 16;

    boundingBox = QRectF(0. - pen.widthF() / 2.,
            0 - pen.widthF() / 2.,
            width_ + pen.widthF(),
            height_ + pen.widthF());

    nameBoundingBox = QRectF(0., height_ - labelHeight_, width_, labelHeight_);

    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
}

QRectF GraphicsModuleItem::boundingRect() const {
    return boundingBox;
}

void GraphicsModuleItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
        QWidget *widget) {

    painter->setBrush(brush);
    painter->setPen(pen);
    painter->drawRect(0, 0, width_, height_);

    //painter->setPen(QPen(Qt::white));
    //painter->drawLine(0, height_ - labelHeight_, width_, height_ - labelHeight_);

    //painter->setPen(QPen(Qt::white, 0.5, Qt::DashLine));
    //painter->drawLine(0, topPadding_, width_, topPadding_);
    //painter->drawLine(0, height_ - labelHeight_ - bottomPadding_, width_, height_ - labelHeight_ - bottomPadding_);
    
    painter->setPen(QPen(QColor::fromRgb(217, 215, 172)));
    painter->drawText(nameBoundingBox, Qt::AlignCenter, name_);
}

QVariant GraphicsModuleItem::itemChange(GraphicsItemChange change, const QVariant &value) {

    if (change == ItemPositionChange && scene()) {
        // Ok
    }

    return QGraphicsItem::itemChange(change, value);
}

void GraphicsModuleItem::addInput() {
    inputHandles.append(new GraphicsHandleItem(GraphicsHandleItem::Role::INPUT, this));

    // Re-place handles
    prepareGeometryChange();

    qreal available_height = height_ - labelHeight_ - topPadding_ - bottomPadding_;
    qreal delta = available_height / (inputHandles.size() + 1);

    size_t i = 1;
    for (auto& handle: inputHandles) {
        handle->setPos(handle->radius() + 1.5, topPadding_ + delta * i);
        i++;
    }
}

void GraphicsModuleItem::addOutput() {
    outputHandles.append(new GraphicsHandleItem(GraphicsHandleItem::Role::OUTPUT, this));

    // Re-place handles
    prepareGeometryChange();

    qreal available_height = height_ - labelHeight_ - topPadding_ - bottomPadding_;
    qreal delta = available_height / (outputHandles.size() + 1);

    size_t i = 1;
    for (auto& handle: outputHandles) {
        handle->setPos(width_ - handle->radius() - 1.5, topPadding_ + delta * i);
        i++;
    }
}
