#ifndef ABYSS_H
#define ABYSS_H


#include <QWidget>
#include <QtPlugin>
#include <QButtonGroup>

#include "abyssapi.h"
#include "floor.h"

QT_BEGIN_NAMESPACE
namespace Ui { class abyss; }
QT_END_NAMESPACE

class abyss : public QWidget, public InterfacePlugin
{
    Q_OBJECT
    Q_INTERFACES(InterfacePlugin)
    Q_PLUGIN_METADATA(IID InterfacePlugin_iid FILE "description.json")
public:
    abyss(QWidget *parent = nullptr);
    ~abyss();

    void account_id_change(int id);
    void set_data_ui(int uid);
    void set_abyss_data(int uid, int id);
    void set_abyss_ui(QJsonObject data);
    virtual void accept_message(PluginMetaData) Q_DECL_OVERRIDE;
    virtual void init() Q_DECL_OVERRIDE;
signals:
    void send_message(PluginMetaData) Q_DECL_OVERRIDE;
private slots:
    void on_pushButton_clicked();

private:
    Ui::abyss *ui;
    QButtonGroup * date_group;

    QList<account_data> datas;

    account_data now_data;
    QString AccountDataPath;
    QStringList data_header;
    QStringList header_type;
};
#endif // ABYSS_H
