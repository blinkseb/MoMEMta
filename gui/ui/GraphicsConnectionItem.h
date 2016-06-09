#pragma once

#include <QBrush>
#include <QGraphicsPathItem>
#include <QPen>

class GraphicsHandleItem;

class GraphicsConnectionItem: public QGraphicsPathItem {
    public:
        GraphicsConnectionItem(GraphicsHandleItem*, QGraphicsItem* parent = nullptr);

        void setFinalAnchor(GraphicsHandleItem*);
        void setTemporaryFinalAnchor(const QPointF&);
        void updatePath();

        void detach();

        GraphicsHandleItem* originAnchor() const;
        GraphicsHandleItem* finalAnchor() const;

    private:
        GraphicsHandleItem* from = nullptr;
        GraphicsHandleItem* to = nullptr;
        QPointF temporaryFinalAnchor;

        QRectF boundingBox;
        QBrush brush;
        QPen pen;
};
