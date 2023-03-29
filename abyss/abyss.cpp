#include "abyss.h"
#include "ui_abyss.h"

abyss::abyss(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::abyss)
{
    AccountDataPath = qApp->applicationDirPath() + "/data/abyss.db";

    ui->setupUi(this);
    connect(ui->account_comboBox, &QComboBox::currentIndexChanged, this, &abyss::account_id_change);
    this->date_group = new QButtonGroup(this);
    this->data_header << "id" << "data";
    this->header_type << "int" << "text";

    ui->listWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    ui->listWidget->setSelectionMode(QAbstractItemView::NoSelection);
    ui->listWidget->setFocusPolicy(Qt::NoFocus);
}

abyss::~abyss()
{
    delete ui;
}

void abyss::account_id_change(int id)
{
    if (ui->account_comboBox->count() < 1) return;
    this->now_data = this->datas.at(id);
    set_data_ui(this->now_data.uid);
}

void abyss::set_data_ui(int uid)
{
    //清除原来所有的按钮
    QList<QAbstractButton *> buttonList = date_group->buttons();
    for (int i = 0; i < buttonList.size(); ++i)
    {
        date_group->removeButton(buttonList[i]);
        ui->button_Layout->removeWidget(buttonList[i]);
        delete buttonList[i];
    }

    QVariantList list = api::database_find_all(AccountDataPath, 1,"'" + QString::number(uid) + "'");
    if(list.count() > 0)
    {
        QList<int> id;
        for(int i = 0; i < list.count(); i++)
        {
            id << list.at(i).toInt();
        }
        std::sort(id.begin(), id.end());
        for(int i = 0; i < list.count(); i++)
        {
            int this_id = id.at(list.count() - i - 1);

            QPushButton * button = new QPushButton("第 " + QString::number(this_id) + " 期");
            connect(button, &QPushButton::clicked, this ,[=](){
                set_abyss_data(uid, this_id);
            });
            button->setFixedSize(125, 25);
            ui->button_Layout->addWidget(button);
            date_group->addButton(button, list.at(i).toInt());
            if(i == 0)
            {
                button -> click();
            }
        }
    }
}

void abyss::set_abyss_data(int uid, int id)
{
    QVariantList var_data = api::database_find_all(AccountDataPath, 2, "'" + QString::number(uid) + "'");
    QString data;
    for(int i = 0; i < var_data.count(); i++)
    {
        if(i % 2 == 0)
        {
            if(var_data.at(i).toInt() == id)
            {
                data = var_data.at(i + 1).toString();
                break;
            }
        }
    }
    QJsonObject data_obj = api::QString_to_QJsonObject(data);
    set_abyss_ui(data_obj);
}

void abyss::set_abyss_ui(QJsonObject data)
{
    //挑战回顾
    ui->star_count->setText(QString::number(api::json_read_int(data, "total_star")));
    ui->max_floor->setText(api::json_read_qstring(data, "max_floor"));
    ui->total_battle_times->setText(QString::number(api::json_read_int(data, "total_battle_times")));
    //战斗数据榜
    QJsonObject damage_rank_obj = api::json_read_array(data, "damage_rank").at(0).toObject();
    ui->damage_rank_icon->setPixmap(abyssapi::get_icon(api::json_read_qstring(damage_rank_obj, "avatar_icon")).scaled(45, 45, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->damage_rank->setText(QString::number(api::json_read_int(damage_rank_obj, "value")));

    QJsonObject defeat_rank = api::json_read_array(data, "defeat_rank").at(0).toObject();
    ui->defeat_rank_icon->setPixmap(abyssapi::get_icon(api::json_read_qstring(defeat_rank, "avatar_icon")).scaled(45, 45, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->defeat_rank->setText(QString::number(api::json_read_int(defeat_rank, "value")));

    QJsonObject energy_skill_rank = api::json_read_array(data, "energy_skill_rank").at(0).toObject();
    ui->energy_skill_rank_icon->setPixmap(abyssapi::get_icon(api::json_read_qstring(energy_skill_rank, "avatar_icon")).scaled(45, 45, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->energy_skill_rank->setText(QString::number(api::json_read_int(energy_skill_rank, "value")));

    QJsonObject normal_skill_rank = api::json_read_array(data, "normal_skill_rank").at(0).toObject();
    ui->normal_skill_rank_icon->setPixmap(abyssapi::get_icon(api::json_read_qstring(normal_skill_rank, "avatar_icon")).scaled(45, 45, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->normal_skill_rank->setText(QString::number(api::json_read_int(normal_skill_rank, "value")));

    QJsonObject take_damage_rank = api::json_read_array(data, "take_damage_rank").at(0).toObject();
    ui->take_damage_rank_icon->setPixmap(abyssapi::get_icon(api::json_read_qstring(take_damage_rank, "avatar_icon")).scaled(45, 45, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->take_damage_rank->setText(QString::number(api::json_read_int(take_damage_rank, "value")));
    //出战次数
    QJsonArray reveal_rank_array = api::json_read_array(data, "reveal_rank");
    int reveal_rank_array_count = reveal_rank_array.count() < 4 ? reveal_rank_array.count() : 4;
    for(int i = 0; i < reveal_rank_array_count; i++)
    {
        QJsonObject reveal_rank_obj = reveal_rank_array.at(i).toObject();
        switch (i) {
        case 0:
            ui->reveal_rank_1->setPixmap(abyssapi::get_icon(api::json_read_qstring(reveal_rank_obj, "avatar_icon")).scaled(128, 128, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            ui->reveal_rank_count_1->setText(QString::number(api::json_read_int(reveal_rank_obj, "value")));
            break;
        case 1:
            ui->reveal_rank_2->setPixmap(abyssapi::get_icon(api::json_read_qstring(reveal_rank_obj, "avatar_icon")).scaled(128, 128, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            ui->reveal_rank_count_2->setText(QString::number(api::json_read_int(reveal_rank_obj, "value")));
            break;
        case 2:
            ui->reveal_rank_3->setPixmap(abyssapi::get_icon(api::json_read_qstring(reveal_rank_obj, "avatar_icon")).scaled(128, 128, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            ui->reveal_rank_count_3->setText(QString::number(api::json_read_int(reveal_rank_obj, "value")));
            break;
        case 3:
            ui->reveal_rank_4->setPixmap(abyssapi::get_icon(api::json_read_qstring(reveal_rank_obj, "avatar_icon")).scaled(128, 128, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            ui->reveal_rank_count_4->setText(QString::number(api::json_read_int(reveal_rank_obj, "value")));
            break;
        default:
            break;
        }
    }

    //详情
    ui->listWidget->clear();
    if(api::json_read_qstring(data, "max_floor") == "0-0")
    {
        //没打深渊
        return;
    }
    QJsonArray floor_array = api::json_read_array(data, "floors");
    for(int i = 0; i < floor_array.count(); i++)
    {
        QJsonObject floor_obj = floor_array.at(i).toObject();
        QListWidgetItem * ListWidgetItem = new QListWidgetItem;
        Floor * floor_widget = new Floor;

        int floor = api::json_read_int(floor_obj, "index");
        int all_star = api::json_read_int(floor_obj, "star");
        int max_star = api::json_read_int(floor_obj, "max_star");
        int count = 0;
        QStringList time;
        int star[3] = {0, 0, 0};
        QStringList icon;
        QStringList lv;

        QJsonArray levels_array = api::json_read_array(floor_obj, "levels");
        for(int j = 0; j < 3; j++)
        {
            if(j < levels_array.count())
            {
                QJsonObject levels_obj = levels_array.at(j).toObject();
                QJsonArray battles_array = api::json_read_array(levels_obj, "battles");
                QString floor_time = "";
                for(int k = 0; k < 2; k++)
                {
                    if(k < battles_array.count())
                    {
                        QJsonObject battles_obj = battles_array.at(k).toObject();
                        QJsonArray avatars_array = api::json_read_array(battles_obj, "avatars");
                        for(int l = 0; l < 4; l++)
                        {
                            if(l < avatars_array.count())
                            {
                                QJsonObject avatars_obj = avatars_array.at(l).toObject();
                                icon << api::json_read_qstring(avatars_obj, "icon");
                                lv << QString::number(api::json_read_int(avatars_obj, "level"));
                            }else
                            {
                                icon << "";
                                lv << "";
                            }
                        }
                        if(floor_time == "")
                            floor_time = api::timestamp_to_time(api::json_read_qstring(battles_obj, "timestamp").toInt());
                    }else
                    {
                        icon << "" << "" << "" << "";
                        lv << "" << "" << "" << "";
                    }
                }
                time << floor_time;
                star[j] = api::json_read_int(levels_obj, "star");
                count++;
            }else
            {
                icon << "" << "" << "" << "" << "" << "" << "" << "";
                lv << "" << "" << "" << "" << "" << "" << "" << "";
                time << "";
            }
        }

        floor_widget->init(floor, all_star, max_star, count, time, star, icon, lv);
        ui->listWidget->addItem(ListWidgetItem);
        ui->listWidget->setItemWidget(ListWidgetItem, floor_widget);
    }
}

void abyss::accept_message(PluginMetaData data)
{
    if(data.from == "abyss")
    {
        this->datas = data.datas;
        int count = this->datas.count();
        ui->account_comboBox->clear();
        if(count >= 0)
        {
            for(int i = 0; i < count; i++)
            {
                ui->account_comboBox->addItem("  " + this->datas.at(i).name + "  ");
                if(!api::database_table_isExist(AccountDataPath, QString::number(this->datas.at(i).uid)))
                {
                    api::database_create_table(AccountDataPath, "'" + QString::number(this->datas.at(i).uid) + "'", this->data_header, this->header_type);
                }
            }
        }
    }
}

void abyss::init()
{
    PluginMetaData metadata;
    metadata.from = "abyss";
    metadata.dest = "";
    metadata.type = 0;
    emit send_message(metadata);
}


void abyss::on_pushButton_clicked()
{
    QStringList data = abyssapi::update(this->now_data);

    for(int i = 0; i < data.count(); i++)
    {
        QJsonObject obj = api::QString_to_QJsonObject(data.at(i));
        if(api::json_read_qstring(obj, "message") == "OK")
        {
            QJsonObject data_obj = api::json_read_object(obj, "data");
            int schedule_id = api::json_read_int(data_obj, "schedule_id");
            QString value = QString::number(schedule_id) + ", '" + QString(QJsonDocument(data_obj).toJson(QJsonDocument::Compact)) + "'";
            api::database_delete(AccountDataPath, "'" + QString::number(this->now_data.uid) + "'", "id=" + QString::number(schedule_id));
            api::database_insert(AccountDataPath, "'" + QString::number(this->now_data.uid) + "'", "id, data", value);
        }
    }
    set_data_ui(this->now_data.uid);
}

