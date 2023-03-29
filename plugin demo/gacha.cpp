#include "gacha.h"
#include "ui_gacha.h"

gacha::gacha(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::gacha)
{
    ui->setupUi(this);
}

gacha::~gacha()
{
    delete ui;
}

void gacha::accept_message(PluginMetaData)
{

}

void gacha::init()
{
    PluginMetaData metadata;
    metadata.from = "";
    metadata.dest = "";
    metadata.type = 0;
    emit send_message(metadata);
}

