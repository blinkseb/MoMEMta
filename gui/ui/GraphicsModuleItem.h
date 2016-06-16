#pragma once

#include <QGraphicsItem>

#include <QBrush>
#include <QPen>
#include <QString>

class GraphicsHandleItem;

class GraphicsModuleItem: public QGraphicsItem {
    public:
        GraphicsModuleItem(const QString& name);

        virtual QRectF boundingRect() const override;
        virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem *option,
                QWidget* widget) override;

        virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

        void addInput();
        void addOutput();

    private:
        QRectF boundingBox;
        QRectF nameBoundingBox;
        QBrush brush;
        QPen pen;

        QString name_;
        qreal width_;
        qreal height_;
        qreal labelHeight_;
        qreal topPadding_;
        qreal bottomPadding_;

        QList<GraphicsHandleItem*> inputHandles;
        QList<GraphicsHandleItem*> outputHandles;
};
