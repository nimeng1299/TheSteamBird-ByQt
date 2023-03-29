#ifndef GACHA_H
#define GACHA_H

#include <QWidget>

#include "interfaceplugin.h"

QT_BEGIN_NAMESPACE
namespace Ui { class gacha; }
QT_END_NAMESPACE

class gacha : public QWidget, public InterfacePlugin
{
    Q_OBJECT
    Q_INTERFACES(InterfacePlugin)
    Q_PLUGIN_METADATA(IID InterfacePlugin_iid FILE "description.json")
public:
    gacha(QWidget *parent = nullptr);
    ~gacha();
//接收外部发出的信息的函数
    virtual void accept_message(PluginMetaData) Q_DECL_OVERRIDE;
//用户调用插件时触发的函数
    virtual void init() Q_DECL_OVERRIDE;
//插件与其他插件或主窗口通信的函数
signals:
    void send_message(PluginMetaData) Q_DECL_OVERRIDE;
private:
    Ui::gacha *ui;
};
#endif // GACHA_H
