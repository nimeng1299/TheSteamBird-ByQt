#include "mainwindow.h"
#include "ui_mainwindow.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("TheSteamBird");
    this->setMinimumSize(45,45);
    this->setMouseTracking(true);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
    ui->label->setFixedSize(ui->label->size().width(), 30);//为标题栏占位
    plugin_button_group = new QButtonGroup(this);
    titleBar = titlebar::create(this);
    initTitleBar();

    padding = 8;
    moveEnable = false;
    resizeEnable = true;
    mousePressed = false;
    mousePoint = QPoint(0, 0);
    mouseRect = QRect(0, 0, 0, 0);
    for (int i = 0; i < 8; ++i) {
        pressedArea << false;
        pressedRect << QRect(0, 0, 0, 0);
    }
    this->setMouseTracking(true);
    this->installEventFilter(this);
    this->setAttribute(Qt::WA_Hover, true);

    initPlugin();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initTitleBar()
{
    titleBar->move(0, 5);
    titleBar->setTitleWidth(this->width());
    connect(titleBar, &titlebar::buttonMinClicked, this, [this](){
        showMinimized();
    });
    connect(titleBar, &titlebar::buttonRestoreClicked, this, [this](){
        showNormal();
        resizeEnable = true;
    });
    connect(titleBar, &titlebar::buttonMaxClicked, this, [this](){
        showMaximized();
        resizeEnable = false;
    });
    connect(titleBar, &titlebar::buttonCloseClicked, this, [this](){
        close();
    });


}

void MainWindow::initPlugin()
{
    plugin_names.clear();
    foreach (QVariant name, PluginManager::getInstance().m_names) {
        plugin_names << name.toString();
    }
    foreach (QVariant name, PluginManager::getInstance().m_nicknames) {
        plugin_nicknames<< name.toString();
    }
    for(int i = 0; i < plugin_names.size(); i++)
    {//plugin_names[i]
        QPushButton * button = new QPushButton(plugin_nicknames[i]);
        connect(button, &QPushButton::clicked, this, [=](){
            load_plugin_widget(plugin_names[i]);
        });
        button->setFixedSize(125, 25);
        ui->verticalLayout->addWidget(button);
        plugin_button_group->addButton(button, i);
    }
    ui->verticalLayout->addStretch();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    titleBar->changeLength(this->width());

    return QMainWindow::resizeEvent(event);
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::Resize) {
            int width = this->width();
            int height = this->height();
            //左侧描点区域
            pressedRect[0] = QRect(0, padding, padding, height - padding * 2);
            //右侧描点区域
            pressedRect[1] = QRect(width - padding, padding, padding, height - padding * 2);
            //上侧描点区域
            pressedRect[2] = QRect(padding, 0, width - padding * 2, padding);
            //下侧描点区域
            pressedRect[3] = QRect(padding, height - padding, width - padding * 2, padding);
            //左上角描点区域
            pressedRect[4] = QRect(0, 0, padding, padding);
            //右上角描点区域
            pressedRect[5] = QRect(width - padding, 0, padding, padding);
            //左下角描点区域
            pressedRect[6] = QRect(0, height - padding, padding, padding);
            //右下角描点区域
            pressedRect[7] = QRect(width - padding, height - padding, padding, padding);
        } else if (event->type() == QEvent::HoverMove) {
            QHoverEvent *hoverEvent = (QHoverEvent *)event;
            QPoint point = hoverEvent->pos();
            if (resizeEnable) {
                if (pressedRect.at(0).contains(point)) {
                    this->setCursor(Qt::SizeHorCursor);
                } else if (pressedRect.at(1).contains(point)) {
                    this->setCursor(Qt::SizeHorCursor);
                } else if (pressedRect.at(2).contains(point)) {
                    this->setCursor(Qt::SizeVerCursor);
                } else if (pressedRect.at(3).contains(point)) {
                    this->setCursor(Qt::SizeVerCursor);
                } else if (pressedRect.at(4).contains(point)) {
                    this->setCursor(Qt::SizeFDiagCursor);
                } else if (pressedRect.at(5).contains(point)) {
                    this->setCursor(Qt::SizeBDiagCursor);
                } else if (pressedRect.at(6).contains(point)) {
                    this->setCursor(Qt::SizeBDiagCursor);
                } else if (pressedRect.at(7).contains(point)) {
                    this->setCursor(Qt::SizeFDiagCursor);
                } else {
                    this->setCursor(Qt::ArrowCursor);
                }
            }
            int offsetX = point.x() - mousePoint.x();
            int offsetY = point.y() - mousePoint.y();
            if (moveEnable && mousePressed) {
                this->move(this->x() + offsetX, this->y() + offsetY);
            }

            if (resizeEnable) {
                int rectX = mouseRect.x();
                int rectY = mouseRect.y();
                int rectW = mouseRect.width();
                int rectH = mouseRect.height();

                if (pressedArea.at(0)) {
                    int resizeW = this->width() - offsetX;
                    if (this->minimumWidth() <= resizeW) {
                        this->setGeometry(this->x() + offsetX, rectY, resizeW, rectH);
                    }
                } else if (pressedArea.at(1)) {
                    this->setGeometry(rectX, rectY, rectW + offsetX, rectH);
                } else if (pressedArea.at(2)) {
                    int resizeH = this->height() - offsetY;
                    if (this->minimumHeight() <= resizeH) {
                        this->setGeometry(rectX, this->y() + offsetY, rectW, resizeH);
                    }
                } else if (pressedArea.at(3)) {
                    this->setGeometry(rectX, rectY, rectW, rectH + offsetY);
                } else if (pressedArea.at(4)) {
                    int resizeW = this->width() - offsetX;
                    int resizeH = this->height() - offsetY;
                    if (this->minimumWidth() <= resizeW) {
                        this->setGeometry(this->x() + offsetX, this->y(), resizeW, resizeH);
                    }
                    if (this->minimumHeight() <= resizeH) {
                        this->setGeometry(this->x(), this->y() + offsetY, resizeW, resizeH);
                    }
                } else if (pressedArea.at(5)) {
                    int resizeW = rectW + offsetX;
                    int resizeH = this->height() - offsetY;
                    if (this->minimumHeight() <= resizeH) {
                        this->setGeometry(this->x(), this->y() + offsetY, resizeW, resizeH);
                    }
                } else if (pressedArea.at(6)) {
                    int resizeW = this->width() - offsetX;
                    int resizeH = rectH + offsetY;
                    if (this->minimumWidth() <= resizeW) {
                        this->setGeometry(this->x() + offsetX, this->y(), resizeW, resizeH);
                    }
                    if (this->minimumHeight() <= resizeH) {
                        this->setGeometry(this->x(), this->y(), resizeW, resizeH);
                    }
                } else if (pressedArea.at(7)) {
                    int resizeW = rectW + offsetX;
                    int resizeH = rectH + offsetY;
                    this->setGeometry(this->x(), this->y(), resizeW, resizeH);
                }
            }
        }else if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouseEvent = (QMouseEvent *)event;
        mousePoint = mouseEvent->pos();
        mouseRect = this->geometry();
        if (pressedRect.at(0).contains(mousePoint)) {
            pressedArea[0] = true;
        } else if (pressedRect.at(1).contains(mousePoint)) {
            pressedArea[1] = true;
        } else if (pressedRect.at(2).contains(mousePoint)) {
            pressedArea[2] = true;
        } else if (pressedRect.at(3).contains(mousePoint)) {
            pressedArea[3] = true;
        } else if (pressedRect.at(4).contains(mousePoint)) {
            pressedArea[4] = true;
        } else if (pressedRect.at(5).contains(mousePoint)) {
            pressedArea[5] = true;
        } else if (pressedRect.at(6).contains(mousePoint)) {
            pressedArea[6] = true;
        } else if (pressedRect.at(7).contains(mousePoint)) {
            pressedArea[7] = true;
        } else {
            mousePressed = true;
        }
    } else if (event->type() == QEvent::MouseMove) {
    } else if (event->type() == QEvent::MouseButtonRelease) {
        this->setCursor(Qt::ArrowCursor);
        mousePressed = false;
        for (int i = 0; i < 8; ++i) {
            pressedArea[i] = false;
        }
    }
    return QObject::eventFilter(watched, event);
}

void MainWindow::load_plugin_widget(QString name)
{
    QPluginLoader *loader =  PluginManager::getInstance().get_plugin(name);
     if(loader)
     {
         auto obj = loader->instance();
         if(obj->isWidgetType())
         {
             if(plugin_widget != NULL)
                 plugin_widget -> close();
             plugin_widget = qobject_cast<QWidget*>(obj);
             ui->widget_verticalLayout->addWidget(plugin_widget);
             plugin_widget->show();
             auto interfaces = qobject_cast<InterfacePlugin*>(loader->instance());
             interfaces->init();
         }
     }
     else
     {
      qDebug() << "未能找到插件";
     }
}

