#ifndef ACCOUNTMANAGER_H
#define ACCOUNTMANAGER_H

#include <QWidget>

#include "accountapi.h"
#include "loginaccountbrowser.h"
#include <QTabWidget>
#include <QCloseEvent>
namespace Ui {
class accountManager;
}

class accountManager : public QWidget
{
    Q_OBJECT

public:
    explicit accountManager(QWidget *parent = nullptr);
    ~accountManager();
    void login_browser(int id);
    void process_cookie(QVariant cookie, int id);
    void add_table_data(QStringList data);;
    void updata_list();
    void closeEvent(QCloseEvent * event);
private slots:
    void on_mys_login_clicked();

    void on_hoyolab_login_clicked();
    void on_delete_account_clicked();

signals:
    void close();
private:
    Ui::accountManager *ui;
    LoginAccountBrowser *widget;
};

#endif // ACCOUNTMANAGER_H
