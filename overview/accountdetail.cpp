#include "accountdetail.h"
#include "ui_accountdetail.h"

#define COUNT 15 //总览标签数量，循环用
#define WORLD 7  //地图数量

//所有processBar都在ui最上层Qwiget中设置了样式
accountDetail::accountDetail(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::accountDetail)
{
    ui->setupUi(this);

    ui->xumi_widget->setStyleSheet("QWidget#xumi_widget{background-image: url(:/world/world/UI_ChapterBackground_Xumi.png);"
                                   "background-position: right;"
                                   "background-repeat:no-repeat;}");

    ui->cengyan_widget->setStyleSheet("QWidget#cengyan_widget{background-image: url(:/world/world/UI_ChapterBackground_ChasmsMaw.png);"
                                   "background-position: right;"
                                   "background-repeat:no-repeat;}");

    ui->enkanomiya_widget->setStyleSheet("QWidget#enkanomiya_widget{background-image: url(:/world/world/UI_ChapterBackground_Enkanomiya.png);"
                                   "background-position: right;"
                                   "background-repeat:no-repeat;}");

    ui->daoqi_widget->setStyleSheet("QWidget#daoqi_widget{background-image: url(:/world/world/UI_ChapterBackground_Daoqi.png);"
                                   "background-position: right;"
                                   "background-repeat:no-repeat;}");

    ui->dragonspine_widget->setStyleSheet("QWidget#dragonspine_widget{background-image: url(:/world/world/UI_ChapterBackground_Dragonspine.png);"
                                   "background-position: right;"
                                   "background-repeat:no-repeat;}");

    ui->liyue_widget->setStyleSheet("QWidget#liyue_widget{background-image: url(:/world/world/UI_ChapterBackground_Liyue.png);"
                                   "background-position: right;"
                                   "background-repeat:no-repeat;}");

    ui->mengde_widget->setStyleSheet("QWidget#mengde_widget{background-image: url(:/world/world/UI_ChapterBackground_Mengde.png);"
                                   "background-position: right;"
                                   "background-repeat:no-repeat;}");
}

accountDetail::~accountDetail()
{
    delete ui;
}

void accountDetail::init(account_detail data)
{
    if(data.platform == 0)
    {
        mys_init(data);
    }else if(data.platform == 1)
    {
        hoyo_init(data);
    }
}

void accountDetail::mys_init(account_detail data)
{
    QString value = get_mys_index(data);
    QJsonObject json = api::QString_to_QJsonObject(value);
    QJsonObject data_obj = api::json_read_object(json, "data");
//第一部分
    QStringList name;
    name << "活跃天数" << "成就达成数" << "获得角色数" << "解锁传送点"
         << "风神瞳" << "岩神瞳" << "雷神瞳" << "草神瞳"
         << "解锁秘境" << "深境螺旋" << "华丽宝箱数" << "珍贵宝箱数"
         << "精致宝箱数" << "普通宝箱数" << "奇馈宝箱数";
    QStringList data_value;
    QJsonObject stats = api::json_read_object(data_obj, "stats");

    data_value << QString::number(api::json_read_int(stats, "active_day_number"));
    data_value << QString::number(api::json_read_int(stats, "achievement_number"));
    data_value << QString::number(api::json_read_int(stats, "avatar_number"));
    data_value << QString::number(api::json_read_int(stats, "way_point_number"));
    data_value << QString::number(api::json_read_int(stats, "anemoculus_number"));
    data_value << QString::number(api::json_read_int(stats, "geoculus_number"));
    data_value << QString::number(api::json_read_int(stats, "electroculus_number"));
    data_value << QString::number(api::json_read_int(stats, "dendroculus_number"));
    data_value << QString::number(api::json_read_int(stats, "domain_number"));
    data_value << api::json_read_qstring(stats, "spiral_abyss");
    data_value << QString::number(api::json_read_int(stats, "luxurious_chest_number"));
    data_value << QString::number(api::json_read_int(stats, "precious_chest_number"));
    data_value << QString::number(api::json_read_int(stats, "exquisite_chest_number"));
    data_value << QString::number(api::json_read_int(stats, "common_chest_number"));
    data_value << QString::number(api::json_read_int(stats, "magic_chest_number"));

    int count = 0;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            if(count < 15)
            {
                QLabel * label = new QLabel;
                label->setAlignment(Qt::AlignVCenter);
                label->setAlignment(Qt::AlignCenter);
                label->setText(QString("<font color = #FFA500 >%1</font>").arg(data_value.at(i * 5 + j)) + QString("<br>") + name.at(i * 5 + j));
                label->adjustSize ();
                QFont font = label->font();
                font.setPointSize(10);
                label->setFont(font);
                label->setGeometry(label->x(), label->y(), 400, 600);
                ui->layout->addWidget(label, i, j);
            }
            count++;
        }
    }

    auto exploration_percentage_to_qstring = [](int data){
        QString value = QString::number(data);
        QString right = value.right(1);
        value.chop(1);
        value = value + "." + right + "%";
        return value;
    };

    QJsonArray world_explorations = api::json_read_array(data_obj, "world_explorations");
    QStringList world_name;
    world_name << "蒙德" << "璃月" << "龙脊雪山" << "稻妻" << "渊下宫" << "层岩巨渊" << "层岩巨渊·地下矿区" << "须弥";
    bool real_world[WORLD] = {0, 0, 0, 0, 0, 0};
    for(int i = 0; i < world_explorations.count(); i++)
    {
        QJsonObject world_obj = world_explorations.at(i).toObject();
        QString world = api::json_read_qstring(world_obj, "name");
        int index = world_name.indexOf(world);
        qDebug() << index;
        switch (index) {
        case 0:
            ui->mengde_exploration->setText("探索度：" + QString("<font color = #FFA500 >%1</font>").arg(exploration_percentage_to_qstring(api::json_read_int(world_obj, "exploration_percentage"))));
            ui->mengde_progressBar->setValue(api::json_read_int(world_obj, "exploration_percentage"));
            ui->mengde_reputation->setText("声望：" + QString("<font color = #FFA500 >%1</font>").arg(QString::number(api::json_read_int(world_obj, "level"))) + "级");
            real_world[0] = true;
            break;
        case 1:
            //璃月
            ui->liyue_exploration->setText("探索度：" + QString("<font color = #FFA500 >%1</font>").arg(exploration_percentage_to_qstring(api::json_read_int(world_obj, "exploration_percentage"))));
            ui->liyue_progressBar->setValue(api::json_read_int(world_obj, "exploration_percentage"));
            ui->liyue_reputation->setText("声望：" + QString("<font color = #FFA500 >%1</font>").arg(QString::number(api::json_read_int(world_obj, "level"))) + "级");
            real_world[1] = true;
            break;
        case 2:
            //龙脊雪山
            ui->dragonspine_exploration->setText("探索度：" + QString("<font color = #FFA500 >%1</font>").arg(exploration_percentage_to_qstring(api::json_read_int(world_obj, "exploration_percentage"))));
            ui->dragonspine_progressBar->setValue(api::json_read_int(world_obj, "exploration_percentage"));
            ui->dragonspine_Eldertree->setText("忍冬之树：" + QString("<font color = #FFA500 >%1</font>").arg(QString::number(api::json_read_int(api::json_read_array(world_obj, "offerings").at(0).toObject(),"level"))) + "级");
            real_world[2] = true;
            break;
        case 3:
            //稻妻
            ui->daoqi_exploration->setText("探索度：" + QString("<font color = #FFA500 >%1</font>").arg(exploration_percentage_to_qstring(api::json_read_int(world_obj, "exploration_percentage"))));
            ui->daoqi_progressBar->setValue(api::json_read_int(world_obj, "exploration_percentage"));
            ui->daoqi_Oraionokami->setText("神樱眷顾：" + QString("<font color = #FFA500 >%1</font>").arg(QString::number(api::json_read_int(api::json_read_array(world_obj, "offerings").at(0).toObject(),"level"))) + "级");
            ui->daoqi_reputation->setText("声望：" + QString("<font color = #FFA500 >%1</font>").arg(QString::number(api::json_read_int(world_obj, "level"))) + "级");
            real_world[3] = true;
            break;
        case 4:
            //渊下宫
            ui->enkanomiya_exploration->setText("探索度：" + QString("<font color = #FFA500 >%1</font>").arg(exploration_percentage_to_qstring(api::json_read_int(world_obj, "exploration_percentage"))));
            ui->enkanomiya_progressBar->setValue(api::json_read_int(world_obj, "exploration_percentage"));
            real_world[4] = true;
            break;
        case 5:
            //层岩巨渊
            ui->cengyan_exploration->setText("层岩巨渊探索度：" + QString("<font color = #FFA500 >%1</font>").arg(exploration_percentage_to_qstring(api::json_read_int(world_obj, "exploration_percentage"))));
            ui->cengyan_progressBar->setValue(api::json_read_int(world_obj, "exploration_percentage"));
            ui->cengyan_Lumenstone->setText("流明石触媒：" + QString("<font color = #FFA500 >%1</font>").arg(QString::number(api::json_read_int(api::json_read_array(world_obj, "offerings").at(0).toObject(),"level"))) + "级");
            real_world[5] = true;
            break;
        case 6:
            //层岩巨渊·地下矿区
            ui->cengyan_under_exploration->setText("层岩巨渊·地下矿区探索度：" + QString("<font color = #FFA500 >%1</font>").arg(exploration_percentage_to_qstring(api::json_read_int(world_obj, "exploration_percentage"))));
            ui->cengyan_under_progressBar->setValue(api::json_read_int(world_obj, "exploration_percentage"));
            ui->cengyan_Lumenstone->setText("流明石触媒：" + QString("<font color = #FFA500 >%1</font>").arg(QString::number(api::json_read_int(api::json_read_array(world_obj, "offerings").at(0).toObject(),"level"))) + "级");
            real_world[5] = true;
            break;
        case 7:
            //须弥
            ui->xumi_exploration->setText("探索度：" + QString("<font color = #FFA500 >%1</font>").arg(exploration_percentage_to_qstring(api::json_read_int(world_obj, "exploration_percentage"))));
            ui->xumi_progressBar->setValue(api::json_read_int(world_obj, "exploration_percentage"));
            ui->xumi_DreamTree->setText("梦之树：" + QString("<font color = #FFA500 >%1</font>").arg(QString::number(api::json_read_int(api::json_read_array(world_obj, "offerings").at(0).toObject(),"level"))) + "级");
            ui->xumi_reputation->setText("声望：" + QString("<font color = #FFA500 >%1</font>").arg(QString::number(api::json_read_int(world_obj, "level"))) + "级");
            real_world[6] = true;
            break;
        default:
            break;
        }
    }
    for(int i = 0; i < WORLD; i++)
    {
        if(real_world[i] == false)
        {
            switch (i) {
            case 0:
                ui->mengde_widget->hide();
                break;
            case 1:
                ui->liyue_widget->hide();
                break;
            case 2:
                ui->dragonspine_widget->hide();
                break;
            case 3:
                ui->daoqi_widget->hide();
                break;
            case 4:
                ui->enkanomiya_widget->hide();
                break;
            case 5:
                ui->cengyan_widget->hide();
                break;
            case 6:
                ui->xumi_widget->hide();
                break;
            default:
                break;
            }
        }
    }
}

void accountDetail::hoyo_init(account_detail data)
{
    QString value = get_hoyo_index(data);
    QJsonObject json = api::QString_to_QJsonObject(value);
    QJsonObject data_obj = api::json_read_object(json, "data");
//第一部分
    QStringList name;
    name << "活跃天数" << "成就达成数" << "获得角色数" << "解锁传送点"
         << "风神瞳" << "岩神瞳" << "雷神瞳" << "草神瞳"
         << "解锁秘境" << "深境螺旋" << "华丽宝箱数" << "珍贵宝箱数"
         << "精致宝箱数" << "普通宝箱数" << "奇馈宝箱数";
    QStringList data_value;
    QJsonObject stats = api::json_read_object(data_obj, "stats");

    data_value << QString::number(api::json_read_int(stats, "active_day_number"));
    data_value << QString::number(api::json_read_int(stats, "achievement_number"));
    data_value << QString::number(api::json_read_int(stats, "avatar_number"));
    data_value << QString::number(api::json_read_int(stats, "way_point_number"));
    data_value << QString::number(api::json_read_int(stats, "anemoculus_number"));
    data_value << QString::number(api::json_read_int(stats, "geoculus_number"));
    data_value << QString::number(api::json_read_int(stats, "electroculus_number"));
    data_value << QString::number(api::json_read_int(stats, "dendroculus_number"));
    data_value << QString::number(api::json_read_int(stats, "domain_number"));
    data_value << api::json_read_qstring(stats, "spiral_abyss");
    data_value << QString::number(api::json_read_int(stats, "luxurious_chest_number"));
    data_value << QString::number(api::json_read_int(stats, "precious_chest_number"));
    data_value << QString::number(api::json_read_int(stats, "exquisite_chest_number"));
    data_value << QString::number(api::json_read_int(stats, "common_chest_number"));
    data_value << QString::number(api::json_read_int(stats, "magic_chest_number"));

    int count = 0;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            if(count < 15)
            {
                QLabel * label = new QLabel;
                label->setAlignment(Qt::AlignVCenter);
                label->setAlignment(Qt::AlignCenter);
                label->setText(QString("<font color = #FFA500 >%1</font>").arg(data_value.at(i * 5 + j)) + QString("<br>") + name.at(i * 5 + j));
                label->adjustSize ();
                QFont font = label->font();
                font.setPointSize(10);
                label->setFont(font);
                label->setGeometry(label->x(), label->y(), 400, 600);
                ui->layout->addWidget(label, i, j);
            }
            count++;
        }
    }

    auto exploration_percentage_to_qstring = [](int data){
        QString value = QString::number(data);
        QString right = value.right(1);
        value.chop(1);
        value = value + "." + right + "%";
        return value;
    };

    QJsonArray world_explorations = api::json_read_array(data_obj, "world_explorations");
    QStringList world_name;
    world_name << "蒙德" << "璃月" << "龙脊雪山" << "稻妻" << "渊下宫" << "层岩巨渊" << "层岩巨渊·地下矿区" << "须弥";
    bool real_world[WORLD] = {0, 0, 0, 0, 0, 0};
    for(int i = 0; i < world_explorations.count(); i++)
    {
        QJsonObject world_obj = world_explorations.at(i).toObject();
        QString world = api::json_read_qstring(world_obj, "name");
        int index = world_name.indexOf(world);
        qDebug() << index;
        switch (index) {
        case 0:
            ui->mengde_exploration->setText("探索度：" + QString("<font color = #FFA500 >%1</font>").arg(exploration_percentage_to_qstring(api::json_read_int(world_obj, "exploration_percentage"))));
            ui->mengde_progressBar->setValue(api::json_read_int(world_obj, "exploration_percentage"));
            ui->mengde_reputation->setText("声望：" + QString("<font color = #FFA500 >%1</font>").arg(QString::number(api::json_read_int(world_obj, "level"))) + "级");
            real_world[0] = true;
            break;
        case 1:
            //璃月
            ui->liyue_exploration->setText("探索度：" + QString("<font color = #FFA500 >%1</font>").arg(exploration_percentage_to_qstring(api::json_read_int(world_obj, "exploration_percentage"))));
            ui->liyue_progressBar->setValue(api::json_read_int(world_obj, "exploration_percentage"));
            ui->liyue_reputation->setText("声望：" + QString("<font color = #FFA500 >%1</font>").arg(QString::number(api::json_read_int(world_obj, "level"))) + "级");
            real_world[1] = true;
            break;
        case 2:
            //龙脊雪山
            ui->dragonspine_exploration->setText("探索度：" + QString("<font color = #FFA500 >%1</font>").arg(exploration_percentage_to_qstring(api::json_read_int(world_obj, "exploration_percentage"))));
            ui->dragonspine_progressBar->setValue(api::json_read_int(world_obj, "exploration_percentage"));
            ui->dragonspine_Eldertree->setText("忍冬之树：" + QString("<font color = #FFA500 >%1</font>").arg(QString::number(api::json_read_int(api::json_read_array(world_obj, "offerings").at(0).toObject(),"level"))) + "级");
            real_world[2] = true;
            break;
        case 3:
            //稻妻
            ui->daoqi_exploration->setText("探索度：" + QString("<font color = #FFA500 >%1</font>").arg(exploration_percentage_to_qstring(api::json_read_int(world_obj, "exploration_percentage"))));
            ui->daoqi_progressBar->setValue(api::json_read_int(world_obj, "exploration_percentage"));
            ui->daoqi_Oraionokami->setText("神樱眷顾：" + QString("<font color = #FFA500 >%1</font>").arg(QString::number(api::json_read_int(api::json_read_array(world_obj, "offerings").at(0).toObject(),"level"))) + "级");
            ui->daoqi_reputation->setText("声望：" + QString("<font color = #FFA500 >%1</font>").arg(QString::number(api::json_read_int(world_obj, "level"))) + "级");
            real_world[3] = true;
            break;
        case 4:
            //渊下宫
            ui->enkanomiya_exploration->setText("探索度：" + QString("<font color = #FFA500 >%1</font>").arg(exploration_percentage_to_qstring(api::json_read_int(world_obj, "exploration_percentage"))));
            ui->enkanomiya_progressBar->setValue(api::json_read_int(world_obj, "exploration_percentage"));
            real_world[4] = true;
            break;
        case 5:
            //层岩巨渊
            ui->cengyan_exploration->setText("层岩巨渊探索度：" + QString("<font color = #FFA500 >%1</font>").arg(exploration_percentage_to_qstring(api::json_read_int(world_obj, "exploration_percentage"))));
            ui->cengyan_progressBar->setValue(api::json_read_int(world_obj, "exploration_percentage"));
            ui->cengyan_Lumenstone->setText("流明石触媒：" + QString("<font color = #FFA500 >%1</font>").arg(QString::number(api::json_read_int(api::json_read_array(world_obj, "offerings").at(0).toObject(),"level"))) + "级");
            real_world[5] = true;
            break;
        case 6:
            //层岩巨渊·地下矿区
            ui->cengyan_under_exploration->setText("层岩巨渊·地下矿区探索度：" + QString("<font color = #FFA500 >%1</font>").arg(exploration_percentage_to_qstring(api::json_read_int(world_obj, "exploration_percentage"))));
            ui->cengyan_under_progressBar->setValue(api::json_read_int(world_obj, "exploration_percentage"));
            ui->cengyan_Lumenstone->setText("流明石触媒：" + QString("<font color = #FFA500 >%1</font>").arg(QString::number(api::json_read_int(api::json_read_array(world_obj, "offerings").at(0).toObject(),"level"))) + "级");
            real_world[5] = true;
            break;
        case 7:
            //须弥
            ui->xumi_exploration->setText("探索度：" + QString("<font color = #FFA500 >%1</font>").arg(exploration_percentage_to_qstring(api::json_read_int(world_obj, "exploration_percentage"))));
            ui->xumi_progressBar->setValue(api::json_read_int(world_obj, "exploration_percentage"));
            ui->xumi_DreamTree->setText("梦之树：" + QString("<font color = #FFA500 >%1</font>").arg(QString::number(api::json_read_int(api::json_read_array(world_obj, "offerings").at(0).toObject(),"level"))) + "级");
            ui->xumi_reputation->setText("声望：" + QString("<font color = #FFA500 >%1</font>").arg(QString::number(api::json_read_int(world_obj, "level"))) + "级");
            real_world[6] = true;
            break;
        default:
            break;
        }
    }
    for(int i = 0; i < WORLD; i++)
    {
        if(real_world[i] == false)
        {
            switch (i) {
            case 0:
                ui->mengde_widget->hide();
                break;
            case 1:
                ui->liyue_widget->hide();
                break;
            case 2:
                ui->dragonspine_widget->hide();
                break;
            case 3:
                ui->daoqi_widget->hide();
                break;
            case 4:
                ui->enkanomiya_widget->hide();
                break;
            case 5:
                ui->cengyan_widget->hide();
                break;
            case 6:
                ui->xumi_widget->hide();
                break;
            default:
                break;
            }
        }
    }
}

QString accountDetail::get_mys_DS(QString region, QString uid)
{
    QString salt = "xV8v4Qu54lUKrEYFZkJhB8cuOh9Asafs";
    QString t = QString::number(QDateTime::currentSecsSinceEpoch());
    int random = QRandomGenerator::global()->bounded(100000);
    QString r = QString::number(random + 100000);
    QString q = "role_id="+ uid + "&server=" + region;
    QString DS = "salt=" + salt + "&t=" + t + "&r=" + r +"&b=&q=" + q;
    QString DSMD5 = QCryptographicHash::hash(DS.toLatin1(),QCryptographicHash::Md5).toHex();
    QString DSresult = t + "," + r + "," + DSMD5;
    return DSresult;
}

QString accountDetail::get_hoyo_ds()
{
    QString salt = "6s25p5ox5y14umn1p61aqyyvbvvl3lrt";
    int t = QDateTime::currentSecsSinceEpoch();
    QString r = "";
    QRandomGenerator generator(t);
    for (int i = 0; i < 6; i++)
    {
        int v8 = generator.bounded(0, 32768) % 26;
        int v9 = 87;
        if (v8 < 10)
        {
            v9 = 48;
        }
        r = r + ((char)(v8 + v9));
    }
    QString md5_result = api::MD5("salt=" + salt + "&t=" + QString::number(t) + "&r=" + r);
    return QString::number(t) + "," + r + "," + md5_result;
}

QString accountDetail::get_mys_index(account_detail data)
{
    QString cookie_id = data.cookie;
    QStringList cookies = cookie_id.split(";");
    auto cookie_value = [](QString strcookie){
        int index = strcookie.indexOf("=");
        QString value = strcookie.right(strcookie.length() - index - 1);
        return value;
    };
    QStringList cookie_token_list = cookies.filter("cookie_token");
    QString data_cookie;
    if(!cookie_token_list.isEmpty())
        data_cookie = "account_id=" + QString::number(data.id) + ";cookie_token=" + cookie_value(cookie_token_list.at(0));
    QString url = "/game_record/app/genshin/api/index?server=" + data.server + "&role_id=" + QString::number(data.uid);
    httplib::Headers headers = {
        {"Access-Control-Request-Headers","ds,x-rpc-app_version,x-rpc-client_type,x-rpc-page"},
        {"Access-Control-Request-Method","GET"},
        {"Accept","application/json, text/plain, */*"},
        {"Accept-Encoding","deflate"},
        {"Accept-Language","zh-CN,zh;q=0.9,en-US;q=0.8,en;q=0.7"},
        {"Connection","keep-alive"},
        {"DS",get_mys_DS(data.server, QString::number(data.uid)).toStdString()},
        {"Host","api-takumi-record.mihoyo.com"},
        {"Origin","https://webstatic.mihoyo.com"},
        {"Referer","https://webstatic.mihoyo.com/"},
        {"Sec-Fecth-Dest","empty"},
        {"Sec-Fecth-Mode","cors"},
        {"Sec-Fetch-Site","cookiessame-site"},
        {"X-Requested-With","com.mihoyo.hyperion"},
        {"x-rpc-app_version","2.35.2"},
        {"x-rpc-client_type","5"},
        {"x-rpc-page","/ys"},
        {"Cookie",data_cookie.toStdString()}
    };
    QString value = api::network_body(api::network_https_get("api-takumi-record.mihoyo.com", url, headers));
    return value;
}

QString accountDetail::get_hoyo_index(account_detail data)
{
    QString cookie_id = data.cookie;
    QStringList cookies = cookie_id.split(";");
    auto cookie_value = [](QString strcookie){
        int index = strcookie.indexOf("=");
        QString value = strcookie.right(strcookie.length() - index - 1);
        return value;
    };
    QStringList ltoken_list = cookies.filter("ltoken");
    QString data_cookie;
    if(!ltoken_list.isEmpty())
        data_cookie = "ltuid=" + QString::number(data.id) + ";ltoken=" + cookie_value(ltoken_list.at(0));
    QString url = "/game_record/genshin/api/index?server=" + data.server + "&role_id=" + QString::number(data.uid);
    httplib::Headers headers = {
        {"ds",get_hoyo_ds().toStdString()},
        {"x-rpc-app_version","1.5.0"},
        {"x-rpc-client_type","5"},
        {"Cookie",data_cookie.toStdString()}
    };
    QString value = api::network_body(api::network_https_get("bbs-api-os.hoyolab.com", url, headers));
    return value;
}
