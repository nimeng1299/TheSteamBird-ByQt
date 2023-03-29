#ifndef ACCOUNTWINDET_H
#define ACCOUNTWINDET_H

#include <QWidget>
#include <QTabWidget>
#include <QTabBar>
#include <QLabel>
#include <QLayout>

#include "../steambird/api.h"

struct account{
    QString name;       //游戏名字
    QString server;     //服务器
    int platform;       //平台 0、米游社 1、HoyoLab
    int id;             //平台id //-1则为账号不存在
    int uid;            //游戏uid
    QString cookie;  //账户cookie
};

namespace Ui {
class accountWindet;
}

class accountWindet : public QWidget
{
    Q_OBJECT

public:
    explicit accountWindet(QWidget *parent = nullptr);
    ~accountWindet();

    void init(account);
    void mys_init(account);
    void hoyo_init(account);
    QString get_DS(QString region , QString uid, int platform = 0);
    QString get_hoyo_daily_DS();
private:
    Ui::accountWindet *ui;
};

#endif // ACCOUNTWINDET_H
