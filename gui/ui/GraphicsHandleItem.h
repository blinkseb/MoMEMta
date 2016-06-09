#pragma once

#include <QBrush>
#include <QGraphicsItem>
#include <QPen>

class GraphicsConnectionItem;

class GraphicsHandleItem: public QGraphicsItem {
    public:
        GraphicsHandleItem(QGraphicsItem* parent = nullptr);

        virtual QRectF boundingRect() const override;
        virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem *option,
                QWidget* widget) override;
        virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

        void setConnectedLine(GraphicsConnectionItem* line);

        const qreal radius = 10.;

    private:
        QRectF boundingBox;
        QBrush brush;
        QPen pen;

        GraphicsConnectionItem* connectedLine = nullptr;
};

