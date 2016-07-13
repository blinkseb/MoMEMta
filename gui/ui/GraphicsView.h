#pragma once

#include <QGraphicsView>

class GraphicsConnectionItem;

class GraphicsView: public QGraphicsView {
    using QGraphicsView::QGraphicsView;

    protected:
        void mousePressEvent(QMouseEvent *event) override;
        void mouseReleaseEvent(QMouseEvent *event) override;
        void mouseMoveEvent(QMouseEvent *event) override;
        void wheelEvent(QWheelEvent *e) override;

    private:
        GraphicsConnectionItem* ongoingConnection = nullptr;
};
