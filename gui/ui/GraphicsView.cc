#include <GraphicsView.h>

#include <GraphicsConnectionItem.h>
#include <GraphicsHandleItem.h>

#include <QMouseEvent>
#include <QtDebug>

void GraphicsView::mousePressEvent(QMouseEvent *event) {
    auto child = itemAt(event->pos());

    if (event->button() == Qt::LeftButton) {

        auto handle = dynamic_cast<GraphicsHandleItem*>(child);
        if (handle) {
            ongoingConnection = handle->startConnection();
            scene()->addItem(ongoingConnection);

            handle->parentItem()->setFlag(QGraphicsItem::ItemIsMovable, false);
        }

    } 

    QGraphicsView::mousePressEvent(event);
}

void GraphicsView::mouseMoveEvent(QMouseEvent *event) {
    if (ongoingConnection) {
        ongoingConnection->setTemporaryFinalAnchor(mapToScene(event->pos()));
    }

    QGraphicsView::mouseMoveEvent(event);
}

void GraphicsView::mouseReleaseEvent(QMouseEvent *event) {

    auto children = items(event->pos());

    if (event->button() == Qt::RightButton) {

        for (const auto& child: children) {
            auto connection = dynamic_cast<GraphicsConnectionItem*>(child);
            if (connection) {
                connection->detach();
                scene()->removeItem(connection);
                delete connection;

                break;
            }
        }

    } else if (event->button() == Qt::LeftButton) {

        if (! ongoingConnection) {
            QGraphicsView::mouseReleaseEvent(event);
            return;
        }

        // Restore mobility of origin module
        ongoingConnection->originAnchor()->parentItem()->setFlag(QGraphicsItem::ItemIsMovable);

        auto children = items(event->pos());

        for (const auto& child: children) {
            auto handle = dynamic_cast<GraphicsHandleItem*>(child);
            if (handle) {
                // Do not connect an handle to itself
                if (handle == ongoingConnection->originAnchor())
                    continue;

                // Do not connect an handle to another handle from the same module
                if (handle->parentItem() == ongoingConnection->originAnchor()->parentItem())
                    continue;

                // Only connect an input to an output or the opposite
                if (handle->role() == ongoingConnection->originAnchor()->role())
                    continue;

                ongoingConnection->setFinalAnchor(handle);
                ongoingConnection = nullptr;

                QGraphicsView::mouseReleaseEvent(event);
                return;
            }
        }

        ongoingConnection->detach();
        scene()->removeItem(ongoingConnection);
        delete ongoingConnection;
        ongoingConnection = nullptr;

    }

    QGraphicsView::mouseReleaseEvent(event);
}
