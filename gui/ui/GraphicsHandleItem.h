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

        virtual void hoverEnterEvent(QGraphicsSceneHoverEvent*);
        virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent*);

        void detachConnections();
        void addConnection(GraphicsConnectionItem* line);
        void removeConnection(GraphicsConnectionItem* line);

        GraphicsConnectionItem* startConnection();

        qreal radius() const;

    private:
        QRectF boundingBox;
        QBrush brush;
        QPen pen;
        qreal radius_;

        QList<GraphicsConnectionItem*> connections;
};

