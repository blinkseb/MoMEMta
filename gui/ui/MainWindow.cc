#include <MainWindow.h>

#include <ui_main.h>

#include <GraphicsModuleItem.h>
#include <GraphicsHandleItem.h>
#include <GraphicsConnectionItem.h>

#include <QGraphicsItem>

#include <momemta/ModuleDescription.h>
#include <momemta/ModuleFactory.h>

MainWindow::MainWindow(QMainWindow* parent): QMainWindow(parent) {
    ui.reset(new Ui::MainWindow());
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setRenderHints(QPainter::Antialiasing);

    auto module = new GraphicsModuleItem("BlockD");
    scene->addItem(module);

    auto module2 = new GraphicsModuleItem("BuildInitialState");
    module2->setPos(200, 0);
    scene->addItem(module2);

    //ui->graphicsView->scale(1.5, 1.5);
    
    module->addInput("s13");
    module->addInput("s134");
    module->addInput("s25");
    module->addInput("s256");

    module->addOutput("solutions");

    module2->addInput("solution");
    module2->addOutput("partons");
}
