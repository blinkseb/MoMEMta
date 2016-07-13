#pragma once

#include <QBrush>
#include <QGraphicsItem>
#include <QPen>

class GraphicsConnectionItem;

class GraphicsHandleItem: public QGraphicsItem {
    public:
        enum class Role {
            INPUT,
            OUTPUT
        };

        GraphicsHandleItem(Role role, const QString& name, QGraphicsItem* parent = nullptr);

        virtual QRectF boundingRect() const override;
        virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem *option,
                QWidget* widget) override;
        virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

        virtual void hoverEnterEvent(QGraphicsSceneHoverEvent*);
        virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent*);

        void detachConnections(GraphicsConnectionItem* except);
        void addConnection(GraphicsConnectionItem* line);
        void removeConnection(GraphicsConnectionItem* line);

        GraphicsConnectionItem* startConnection();

        qreal radius() const;

        Role role() const;

    private:
        QRectF boundingBox;
        QBrush brush;
        QPen pen;
        qreal radius_;
        Role role_;
        QString name_;

        QList<GraphicsConnectionItem*> connections;
};

