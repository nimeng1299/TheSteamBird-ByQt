#ifndef OVERVIEW_H
#define OVERVIEW_H

#include <QWidget>
#include <QtPlugin>
#include <QTreeWidgetItem>
#include <QList>

#include "../steambird/interfaceplugin.h"
#include "accountwindet.h"
#include "accountdetail.h"
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget, public InterfacePlugin
{
    Q_OBJECT
    Q_INTERFACES(InterfacePlugin)
    Q_PLUGIN_METADATA(IID InterfacePlugin_iid FILE "description.json")
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    virtual void accept_message(PluginMetaData) Q_DECL_OVERRIDE;
    virtual void init() Q_DECL_OVERRIDE;

signals:
    void send_message(PluginMetaData) Q_DECL_OVERRIDE;


private:
    Ui::Widget *ui;

    QList<account_data> datas;
};
#endif // OVERVIEW_H
