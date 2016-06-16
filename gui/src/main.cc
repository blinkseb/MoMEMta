#include <QApplication>

#include <MainWindow.h>

#include <momemta/ModuleDescription.h>
#include <momemta/ModuleFactory.h>

#include <iostream>

int main(int argc, char **argv) {

    auto plugins = ModuleDescriptionFillerFactory::get().getPluginsList();
    std::cout << "Available plugins: " << std::endl;
    for (const auto& plugin: plugins)
        std::cout << "    - " << plugin << std::endl;

    auto test = ModuleDescriptionFillerFactory::get().create("BlockD");

    ModuleDescription description;
    test->fillDescription(description);

    QApplication app(argc, argv);

    MainWindow *window = new MainWindow;
    window->show();

    return app.exec();
}
