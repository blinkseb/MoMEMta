#pragma once

#include <memory>

#include <QMainWindow>
#include <QGraphicsScene>

namespace Ui {
    class MainWindow;
}

class MainWindow: public QMainWindow {

    public:
        MainWindow(QMainWindow* parent = nullptr);

    private:
        std::unique_ptr<Ui::MainWindow> ui;

        QGraphicsScene *scene;
};
