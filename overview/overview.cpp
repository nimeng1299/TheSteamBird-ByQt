#include "overview.h"
#include "ui_overview.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    ui->treeWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    ui->treeWidget->setSelectionMode(QAbstractItemView::NoSelection);
    ui->treeWidget->setFocusPolicy(Qt::NoFocus);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::accept_message(PluginMetaData metadata)
{
    if(metadata.from == "overview")
    {
        this->datas = metadata.datas;
        int count = this->datas.count();
        if(count != 0)
        {

            ui->treeWidget->clear();
            for(int i = 0; i < count; i++)
            {
                account_data data = this->datas.at(i);

                QTreeWidgetItem * item = new QTreeWidgetItem();
                ui->treeWidget->addTopLevelItem(item);
                accountWindet * widget = new accountWindet;
                account data_data = {
                    data.name,
                    data.server,
                    data.platform,
                    data.id,
                    data.uid,
                    data.cookie
                };
                widget->init(data_data);
                ui->treeWidget->setItemWidget(item, 0, widget);

                QTreeWidgetItem *childItem = new QTreeWidgetItem( );
                item->addChild(childItem);
                accountDetail * widget_detail = new accountDetail;
                account_detail data_detail = {
                    data.name,
                    data.server,
                    data.platform,
                    data.id,
                    data.uid,
                    data.cookie
                };
                widget_detail->init(data_detail);
                ui->treeWidget->setItemWidget(childItem, 0, widget_detail);
            }
        }
    }
    qDebug() << metadata.dest;
}

void Widget::init()
{
    PluginMetaData metadata;
    metadata.from = "overview";
    metadata.dest = "";
    metadata.type = 0;
    emit send_message(metadata);
}


