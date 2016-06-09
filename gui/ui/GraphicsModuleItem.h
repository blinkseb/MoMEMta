#pragma once

#include <QGraphicsItem>

class GraphicsHandleItem;

class GraphicsModuleItem: public QGraphicsItem {
    public:
        GraphicsModuleItem();

        virtual QRectF boundingRect() const override;
        virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem *option,
                QWidget* widget) override;

        virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

        GraphicsHandleItem* handle() const;

    private:
        QRectF boundingBox;
};
