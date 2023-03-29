#ifndef GACHA_H
#define GACHA_H

#include <QWidget>
#include <QMenu>
#include <QtPlugin>
#include <QFile>
#include <QFileDialog>
#include <QInputDialog>

#include "gachaapi.h"
#include "../steambird/interfaceplugin.h"


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
    void init_comboBox();
    // 0、yuanshen 1、genshin impact
    void update_from_file(int platform);
    void update_from_url();
    void update_gacha(QList<QJsonObject> list_obj);
    void update_ui();
    void update_chart();

    virtual void accept_message(PluginMetaData) Q_DECL_OVERRIDE;
    virtual void init() Q_DECL_OVERRIDE;

signals:
    void send_message(PluginMetaData) Q_DECL_OVERRIDE;
private slots:
    void on_pushButton_clicked();

private:
    Ui::gacha *ui;
    QMenu menu;

    QString gacha_json_path;
    QString gacha_data_path;

    QStringList data_header;
    QStringList header_type;
    QStringList data_list;//账号列表

};


#endif // GACHA_H
