#ifndef ACCOUNTDETAIL_H
#define ACCOUNTDETAIL_H

#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include <QPixmap>
#include "../steambird/api.h"
struct account_detail{
    QString name;       //游戏名字
    QString server;     //服务器
    int platform;       //平台 0、米游社 1、HoyoLab
    int id;             //平台id //-1则为账号不存在
    int uid;            //游戏uid
    QString cookie;     //账户cookie
};

namespace Ui {
class accountDetail;
}

class accountDetail : public QWidget
{
    Q_OBJECT

public:
    explicit accountDetail(QWidget *parent = nullptr);
    ~accountDetail();
    void init(account_detail);
    void mys_init(account_detail);
    void hoyo_init(account_detail);
    QString get_mys_DS(QString region, QString uid);
    QString get_hoyo_ds();
    QString get_mys_index(account_detail data);
    QString get_hoyo_index(account_detail data);
private:
    Ui::accountDetail *ui;

};

#endif // ACCOUNTDETAIL_H
