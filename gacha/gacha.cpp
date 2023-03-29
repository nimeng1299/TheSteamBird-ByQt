#include "gacha.h"
#include "ui_gacha.h"

gacha::gacha(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::gacha)
{
    gacha_json_path = qApp->applicationDirPath() + "/data/gacha.json";
    gacha_data_path = qApp->applicationDirPath() + "/data/gacha.db";

    this->data_header << "id" << "gacha_type" << "time" << "name" << "item_type" << "rank_type";
    this->header_type << "text" << "text" << "text" << "text" << "text" << "text";

    ui->setupUi(this);

    ui->webWidget->setContextMenuPolicy(Qt::NoContextMenu);
    ui->webWidget->load(QUrl("qrc:/echarts/gacha_echarts/echart.html"));

    init_comboBox();
    QAction * from_yuanshen_update = menu.addAction("从国服缓存中更新");
    QAction * from_genshin_update = menu.addAction("从国际服缓存中更新");
    menu.addSeparator();
    QAction * from_url_update = menu.addAction("输入网址更新");
    connect(from_yuanshen_update, &QAction::triggered, this, [=](){
        update_from_file(0);
    });
    connect(from_genshin_update, &QAction::triggered, this, [=](){
        update_from_file(1);
    });
    connect(from_url_update, &QAction::triggered, this, [=](){
        update_from_url();
    });

    connect(ui->account_comboBox, &QComboBox::currentIndexChanged, this, &gacha::update_ui);
}


gacha::~gacha()
{
    delete ui;
}

void gacha::init_comboBox()
{
    ui->account_comboBox->clear();
    this->data_list.clear();
    this->data_list = api::databast_get_all_table(gacha_data_path);
    int count = this->data_list.count();
    if(count >= 0)
    {
        ui->account_comboBox->addItems(this->data_list);
    }
}

void gacha::update_from_file(int platform)
{
    ui->stats->setText("正在获取数据，界面可能会卡死");
    QString file_path;
    auto get_file_path = [platform](){
        QStringList name;
        name << "Yuanshen.exe" << "GenshinImpact.exe";
        QString caption = "请选中游戏文件。为启动器目录下的\\Genshin Impact Game\\" + name.at(platform) + "文件";
        QString filter = "原神启动文件(" + name.at(platform) + ");;所有文件(*.*)";

        QString get_file_path = QFileDialog::getOpenFileName(Q_NULLPTR,
                                                             caption,
                                                             qApp->applicationDirPath(),
                                                             filter);
        return get_file_path;
    };

    QFile file(gacha_json_path);
    QStringList name;
    name << "yuanshen" << "GenshinImpact";
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        file_path = get_file_path();
    }else
    {
        QByteArray array = file.readAll();
        QJsonParseError json_error;
        QJsonDocument json_doc = QJsonDocument::fromJson(array, &json_error);
        if(json_error.error != QJsonParseError::NoError || api::json_read_qstring(json_doc.object(), name.at(platform)) == "")
        {
            file_path = get_file_path();
        }else
        {   
            file_path = api::json_read_qstring(json_doc.object(), name.at(platform));
        }
    }
    file.close();
    if(platform == 0)
        update_gacha(gachaapi::from_yuanshen_file(file_path));
    else if(platform == 1)
        update_gacha(gachaapi::from_genshin_file(file_path));
    ui->stats->setText("");
}

void gacha::update_from_url()
{
    bool ok;
    QString url = QInputDialog::getText(this, tr("手动输入祈愿网址"), tr("输入网址，以https://开头"), QLineEdit::Normal,tr("admin"), &ok);
    if(ok)
    {
        ui->stats->setText("正在获取数据，界面可能会卡死");
        update_gacha(gachaapi::from_url(url));
    }else
    {
        ui->stats->setText("");
    }
    ui->stats->setText("");
}

void gacha::update_gacha(QList<QJsonObject> list_obj)
{
    ui->stats->setText("正在更新数据");
    int count = list_obj.count();
    for(int i = 0; i < count; i++)
    {
        QJsonObject data_obj = list_obj.at(i);
        //<< "id" << "gacha_type" << "time" << "name" << "item_type" << "rank_type";

        QJsonArray list = api::json_read_array(data_obj, "list");
        for (int j = 0; j < list.count(); j++) {
            QJsonObject obj = list.at(j).toObject();
            QString uid = api::json_read_qstring(obj, "uid");
            QString id = api::json_read_qstring(obj, "id");
            QString gacha_type = api::json_read_qstring(obj, "gacha_type");
            QString time = api::json_read_qstring(obj, "time");
            time = QString::number(api::time_to_stamp(time));
            QString name = api::json_read_qstring(obj, "name");
            QString item_type = api::json_read_qstring(obj, "item_type");
            QString rank_type = api::json_read_qstring(obj, "rank_type");

            QString value = "'" + id + "', '" + gacha_type + "', '" + time + "', '" + name + "', '" + item_type + "', '" + rank_type + "'";

            if(!api::database_table_isExist(gacha_data_path, uid))
                api::database_create_table(gacha_data_path, "'" + uid + "'", data_header, header_type);
            api::database_delete(gacha_data_path, "'" + uid + "'", "id='" + id + "'");
            api::database_insert(gacha_data_path, "'" + uid + "'", "'id', 'gacha_type', 'time', 'name', 'item_type', 'rank_type'", value);
        }
    }
    ui->stats->setText("");
    init_comboBox();
}

void gacha::update_ui()
{
    if(ui->account_comboBox->count() < 1)
        return;
    update_chart();

}

void gacha::update_chart()
{
    ui->stats->setText("正在绘制数据");
    if(ui->account_comboBox->count() < 1)
        return;
    QString uid = this->data_list.at(ui->account_comboBox->currentIndex());
    QList<QList<QStringList>> data = gachaapi::get_data_from_database(gacha_data_path, uid, data_header.count());



    int star_5_count = 0;
    QList<QList<QVariantList>> all_view;

    for(int i = 0; i < data.count(); i++)
    {
        QList<QStringList> view_data = data.at(i);
        QList<QVariantList> view;
        for(int j = 0; j < view_data.count(); j++)
        {
            star_5_count++;
            if(view_data.at(j).at(5) == "5")
            {
                QVariantList list;
                // list << 名称 << 次数 << 时间戳（传给charts需要*1000）<< 池子
                list << view_data.at(j).at(3) << star_5_count << view_data.at(j).at(2) << view_data.at(j).at(1);
                star_5_count = 0;
                view << list;
            }
        }
        all_view << view;
    }

    QJsonObject chart_data;
    for(int i = 0; i < all_view.count(); i++)
    {
        int index = 0;
        QJsonArray value, datas;
        QList<QVariantList> view_data = all_view.at(i);
        long long time = 0;
        int count_time = 0;
        for(int j = 0; j < view_data.count(); j++)
        {
            QVariantList data_value = view_data.at(j);
            long long timestamp = data_value.at(2).toLongLong();
            if(time == timestamp)
            {
                count_time++;
                timestamp = timestamp + count_time * 60;
            }else
            {
                count_time = 0;
            }
            time = data_value.at(2).toLongLong();
            value.append(data_value.at(0).toString());
            datas.append(QJsonArray({timestamp * 1000, data_value.at(1).toString()}));
            switch (data_value.at(3).toInt()) {
            case 200:
                index = 3;
                break;
            case 301:
                index = 1;
                break;
            case 302:
                index = 2;
                break;
            case 400:
                index = 1;
                break;
            default:
                break;
            }
        }
        QString value_name = "value" + QString::number(index);
        QString data_name = "data" + QString::number(index);
        chart_data.insert(value_name, value);
        chart_data.insert(data_name, datas);
    }
    QString str = QJsonDocument(chart_data).toJson();
    QString js = QString("init2(%1)").arg(str);
    ui->webWidget->page()->runJavaScript(js);
    ui->stats->setText("");
}

void gacha::accept_message(PluginMetaData metadata)
{
    if(metadata.from == "gacha")
        init_comboBox();
}

void gacha::init()
{
    PluginMetaData metadata;
    metadata.from = "gacha";
    metadata.dest = "";
    metadata.type = 0;
    emit send_message(metadata);
}


void gacha::on_pushButton_clicked()
{
    QPoint pos = ui->pushButton->pos();
    pos.setX(pos.x() + 1);
    pos.setY(pos.y() + ui->pushButton->height() - 1);
    menu.exec(mapToGlobal(pos));
}

