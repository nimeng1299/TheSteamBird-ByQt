#ifndef LOGINACCOUNTBROWSER_H
#define LOGINACCOUNTBROWSER_H

#include <QWidget>

namespace Ui {
class LoginAccountBrowser;
}

class LoginAccountBrowser : public QWidget
{
    Q_OBJECT

public:
    explicit LoginAccountBrowser(QWidget *parent = nullptr);
    ~LoginAccountBrowser();
    void way(int id);//登录方式，0、米游社，1、HoloLab
    void get_cookie(QVariant cookie);


private slots:
    void on_pushButton_clicked();

signals:
    void send_cookie(QVariant cookie, int id);

private:
    Ui::LoginAccountBrowser *ui;

    int id = -1;
};

#endif // LOGINACCOUNTBROWSER_H
