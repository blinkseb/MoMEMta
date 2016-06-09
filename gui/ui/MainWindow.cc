#include <MainWindow.h>

#include <ui_main.h>

#include <GraphicsModuleItem.h>
#include <GraphicsHandleItem.h>
#include <GraphicsConnectionItem.h>

#include <QGraphicsItem>

MainWindow::MainWindow(QMainWindow* parent): QMainWindow(parent) {
    ui.reset(new Ui::MainWindow());
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setRenderHints(QPainter::Antialiasing);

    auto module = new GraphicsModuleItem();
    scene->addItem(module);

    auto module2 = new GraphicsModuleItem();
    module2->setPos(200, 0);
    scene->addItem(module2);

    ui->graphicsView->scale(2., 2.);
}
