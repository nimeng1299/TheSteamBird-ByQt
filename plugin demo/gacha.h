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
//�����ⲿ��������Ϣ�ĺ���
    virtual void accept_message(PluginMetaData) Q_DECL_OVERRIDE;
//�û����ò��ʱ�����ĺ���
    virtual void init() Q_DECL_OVERRIDE;
//��������������������ͨ�ŵĺ���
signals:
    void send_message(PluginMetaData) Q_DECL_OVERRIDE;
private:
    Ui::gacha *ui;
};
#endif // GACHA_H
