#include "mainwindow.h"
#include <QFontDatabase>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFont f;
    f.setFamily(QFontDatabase::applicationFontFamilies (QFontDatabase::addApplicationFont(":/res/fonts.ttf")).at(0));
    a.setFont(f);

    PluginManager::getInstance().load_all_plugin();
    MainWindow w;
    w.resize(QGuiApplication::primaryScreen()->geometry().width() * 0.7, QGuiApplication::primaryScreen()->geometry().height() * 0.7);
    w.load_plugin_widget("overview");
    w.show();
    return a.exec();
}
