#pragma once

#include <QBrush>
#include <QGraphicsPathItem>
#include <QPen>

class GraphicsHandleItem;

class GraphicsConnectionItem: public QGraphicsPathItem {
    public:
        GraphicsConnectionItem(GraphicsHandleItem*, GraphicsHandleItem*, QGraphicsItem* parent = nullptr);

        void updatePath();

    private:
        GraphicsHandleItem* from;
        GraphicsHandleItem* to;

        QRectF boundingBox;
        QBrush brush;
        QPen pen;
};
