#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QEvent>
#include <QMouseEvent>
#include <QRect>
#include <QRectF>
#include <QButtonGroup>
#include <QPoint>
#include "pluginmanager.h"
#include "titlebar.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void initTitleBar();
    void initPlugin();
    void resizeEvent (QResizeEvent * event );

    bool eventFilter(QObject *watched, QEvent *event);

    void load_plugin_widget(QString name);
private:
    Ui::MainWindow *ui;


protected:
    titlebar* titleBar;

    int padding;
    bool moveEnable;//是否允许移动
    bool resizeEnable;//是否允许拉伸
    bool mousePressed;
    QPoint mousePoint;
    QRect mouseRect;
    //依次为 左侧+右侧+上侧+下侧+左上侧+右上侧+左下侧+右下侧
    QList<bool> pressedArea;
    QList<QRect> pressedRect;

    QVector<QString> plugin_names;
    QVector<QString> plugin_nicknames;
    QButtonGroup * plugin_button_group;
    QWidget *plugin_widget = NULL;
};
#endif // MAINWINDOW_H
