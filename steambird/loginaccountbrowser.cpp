#include "loginaccountbrowser.h"
#include "ui_loginaccountbrowser.h"
#include <QWebEngineProfile>
LoginAccountBrowser::LoginAccountBrowser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginAccountBrowser)
{
    ui->setupUi(this);
    ui->myWeb->page()->profile()->setHttpUserAgent("Mozilla/5.0 miHoYoBBS/2.43.1");
    this->setAttribute(Qt::WA_QuitOnClose,false);
    this->setWindowTitle("登录页面");
}

LoginAccountBrowser::~LoginAccountBrowser()
{
    delete ui;
}

void LoginAccountBrowser::way(int id)
{
    this->id = id;
    if(id == 0)
    {
        ui->myWeb->setUrl(QUrl("https://user.mihoyo.com/#/login/captcha"));
    }else if(id == 1)
    {
        ui->myWeb->setUrl(QUrl("https://www.hoyolab.com/"));
    }
}

void LoginAccountBrowser::get_cookie(QVariant cookie)
{
    emit send_cookie(cookie, this->id);

    this->close();
}

void LoginAccountBrowser::on_pushButton_clicked()
{
    ui->myWeb->page()->runJavaScript("function GetCookie(){return document.cookie}");
    ui->myWeb->page()->runJavaScript("GetCookie();",[this](const QVariant &v) {
    qDebug() << v;
    get_cookie(v);
    });
}

