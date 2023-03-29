#include "accountmanager.h"
#include "ui_accountmanager.h"

accountManager::accountManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::accountManager)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_QuitOnClose,false);
    this->setWindowTitle("账号管理");
    widget = NULL;

    ui->account_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->account_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->account_table->setColumnCount(3);
    ui->account_table->setHorizontalHeaderLabels(QStringList() << tr("名称") << tr("id") << tr("uid"));
    updata_list();
}

accountManager::~accountManager()
{
    delete ui;
}

void accountManager::login_browser(int id)
{
    if(!widget)
    {
        widget = new LoginAccountBrowser;
    }
    widget->show();
    widget->way(id);
    connect(widget, &LoginAccountBrowser::send_cookie, this, &accountManager::process_cookie, Qt::UniqueConnection);
}

void accountManager::process_cookie(QVariant cookie, int id)
{
    account_data data;
    if(id == 0)
    {
        data = accountApi::get_mys_info(cookie.toByteArray());
    }else if(id == 1)
    {
        data = accountApi::get_hoyo_info(cookie.toByteArray());
    }
    accountApi::data_insert(qApp->applicationDirPath() + "/data/account.db", data);
    updata_list();
}

void accountManager::add_table_data(QStringList data)
{
    int rows = ui->account_table->rowCount();
    ui->account_table->setRowCount(rows + 1);
    for(int i = 0; i < data.count(); i++)
    {
        ui->account_table->setItem(rows, i, new QTableWidgetItem(data.at(i)));
    }
}

void accountManager::updata_list()
{

    ui->account_table->clearContents();
    ui->account_table->setRowCount(0);
    QList<account_data> data_list = accountApi::get_account_info(qApp->applicationDirPath() + "/data/account.db");
    for(int i = 0; i < data_list.count(); i++)
    {
        QStringList list;
        list << data_list.at(i).name << QString::number(data_list.at(i).platform) << QString::number(data_list.at(i).uid);
        add_table_data(list);
    }
    for(int i = 0; i < ui->account_table->rowCount(); i++)
    {
        for(int j = 0; j < 3; j++)
            ui->account_table->item(i, j)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    }
}

void accountManager::closeEvent(QCloseEvent *event)
{
    emit close();
    event->accept();
}

void accountManager::on_mys_login_clicked()
{
    login_browser(0);
}


void accountManager::on_hoyolab_login_clicked()
{
    login_browser(1);
}


void accountManager::on_delete_account_clicked()
{
    QList<QTableWidgetSelectionRange> ranges = ui->account_table->selectedRanges();
    int count=ranges.count();
    if(count > 0)
    {
        QTableWidgetSelectionRange range=ranges.at(0);
        int row = range.topRow();
        QString conditon = "uid = " + ui->account_table->item(row, 2)->text();
        api::database_delete(qApp->applicationDirPath() + "/data/account.db", "account", conditon);
        updata_list();
    }
}

