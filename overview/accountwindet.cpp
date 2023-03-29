#include "accountwindet.h"
#include "ui_accountwindet.h"

accountWindet::accountWindet(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::accountWindet)
{
    ui->setupUi(this);
    ui->tabWidget->tabBar()->hide();

    ui->FragileResinUI->setPixmap(QPixmap(":/res/res/FragileResin.png").scaled(ui->FragileResinUI->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));
    ui->SereniteaPotUI->setPixmap(QPixmap(":/res/res/SereniteaPot.png").scaled(ui->FragileResinUI->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));
    ui->CommissionUI->setPixmap(QPixmap(":/res/res/Commission.png").scaled(ui->FragileResinUI->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));
    ui->ParametricTransformerUI->setPixmap(QPixmap(":/res/res/ParametricTransformer.png").scaled(ui->FragileResinUI->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));
    ui->DomainsUI->setPixmap(QPixmap(":/res/res/Domains.png").scaled(ui->FragileResinUI->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));

    ui->FragileResinTitle->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    ui->FragileResinValue->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    ui->SereniteaPotValue->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    ui->CommissionValue->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    ui->ParametricTransformerValue->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    ui->DomainsValue->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
}

accountWindet::~accountWindet()
{
    delete ui;
}

void accountWindet::init(account data)
{
    if(data.platform == 0)
    {
        mys_init(data);
    }else if(data.platform == 1)
    {
        hoyo_init(data);
    }
}

void accountWindet::mys_init(account data)
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
    QString url = "/game_record/app/genshin/api/dailyNote?role_id=" + QString::number(data.uid) + "&server=" + data.server;
    httplib::Headers headers = {
        {"Access-Control-Request-Headers","ds,x-rpc-app_version,x-rpc-client_type,x-rpc-page"},
        {"Access-Control-Request-Method","GET"},
        {"Accept","application/json, text/plain, */*"},
        {"Accept-Encoding","deflate"},
        {"Accept-Language","zh-CN,zh;q=0.9,en-US;q=0.8,en;q=0.7"},
        {"Connection","keep-alive"},
        {"DS",get_DS(data.server, QString::number(data.uid)).toStdString()},
        {"Host","api-takumi-record.mihoyo.com"},
        {"Origin","https://webstatic.mihoyo.com"},
        {"Referer","https://webstatic.mihoyo.com/"},
        {"Sec-Fecth-Dest","empty"},
        {"Sec-Fecth-Mode","cors"},
        {"Sec-Fetch-Site","cookiessame-site"},
        {"X-Requested-With","com.mihoyo.hyperion"},
        {"x-rpc-app_version","2.35.2"},
        {"x-rpc-client_type","5"},
        {"x-rpc-page","/ys/daily/"},
        {"Cookie",data_cookie.toStdString()}
    };
    QString value = api::network_body(api::network_https_get("api-takumi-record.mihoyo.com", url, headers));
    QJsonObject json = api::QString_to_QJsonObject(value);
    QString message = api::json_read_qstring(json, "message");
    if(message == "OK")
    {
        QJsonObject data_obj = api::json_read_object(json, "data");
        QString current_resin = QString::number(api::json_read_int(data_obj, "current_resin"));
        QString max_resin = QString::number(api::json_read_int(data_obj, "max_resin"));
        QString resin_recovery_time = api::json_read_qstring(data_obj, "resin_recovery_time");
        QString finished_task_num = QString::number(api::json_read_int(data_obj, "finished_task_num"));
        QString total_task_num = QString::number(api::json_read_int(data_obj, "total_task_num"));
        QString current_expedition_num = QString::number(api::json_read_int(data_obj, "current_expedition_num"));
        QString max_expedition_num = QString::number(api::json_read_int(data_obj, "max_expedition_num"));
        bool is_extra_task_reward_received = data_obj.value("is_extra_task_reward_received").isBool();
        QString current_home_coin = QString::number(api::json_read_int(data_obj, "current_home_coin"));
        QString max_home_coin = QString::number(api::json_read_int(data_obj, "max_home_coin"));

        ui->FragileResinValue->setText(QString("<font color = #FFA500 >%1</font>").arg(current_resin) + " / " + max_resin);
        ui->SereniteaPotValue->setText(QString("<font color = #FFA500 >%1</font>").arg(current_home_coin) + " / " + max_home_coin);
        ui->CommissionValue->setText(QString("<font color = #FFA500 >%1</font>").arg(finished_task_num) + " / " + total_task_num);
        if(is_extra_task_reward_received)
            ui->ParametricTransformerValue->setText(QString("<font color = #FFA500 >%1</font>").arg("1") + " / 1");
        else
            ui->ParametricTransformerValue->setText(QString("<font color = #FFA500 >%1</font>").arg("0") + " / 1");
        ui->DomainsValue->setText(QString("<font color = #FFA500 >%1</font>").arg(current_expedition_num) + " / " + max_expedition_num);

        int time = resin_recovery_time.toInt();
        int H = time / (60*60);
        int M = (time - (H * 60 * 60)) / 60;
        //int S = (time - (H * 60 * 60)) - M * 60;
        QString hour = QString::number(H);
        QString min = QString::number(M);
        //QString sec = QString::number(S);
        ui->FragileResinTime->setText(QString("<font color = #FFA500 >%1</font>").arg(hour) + " 小时 " + QString("<font color = #FFA500 >%1</font>").arg(min) + " 分钟 ");
        ui->name_label->setText(data.name + "\t" + QString::number(data.uid));


    }else if(message == "")
    {
        ui->tabWidget->setCurrentIndex(1);
        ui->error_label->setText("error: 网络错误");
    }else
    {
        ui->tabWidget->setCurrentIndex(1);
        ui->error_label->setText("error:" + message);
    }
}

void accountWindet::hoyo_init(account data)
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
    QString url = "/game_record/genshin/api/dailyNote?server="  + data.server + "&role_id=" + QString::number(data.uid);
    httplib::Headers headers = {
        {"User-Agent","Mozilla/5.0 (Linux; Android 9; OPPO R11 Plus Build/NMF26X; wv) AppleWebKit/537.36 (KHTML, like Gecko) Version/4.0 Chrome/92.0.4515.131 Mobile Safari/537.36"},
        {"ds",get_hoyo_daily_DS().toStdString()},
        {"x-rpc-app_version","1.5.0"},
        {"x-rpc-client_type","5"},
        {"Cookie",data_cookie.toStdString()}
    };
    QString value = api::network_body(api::network_https_get("bbs-api-os.hoyolab.com", url, headers));
    QJsonObject json = api::QString_to_QJsonObject(value);
    QString message = api::json_read_qstring(json, "message");
    if(message == "OK")
    {
        QJsonObject data_obj = api::json_read_object(json, "data");
        QString current_resin = QString::number(api::json_read_int(data_obj, "current_resin"));
        QString max_resin = QString::number(api::json_read_int(data_obj, "max_resin"));
        QString resin_recovery_time = api::json_read_qstring(data_obj, "resin_recovery_time");
        QString finished_task_num = QString::number(api::json_read_int(data_obj, "finished_task_num"));
        QString total_task_num = QString::number(api::json_read_int(data_obj, "total_task_num"));
        QString current_expedition_num = QString::number(api::json_read_int(data_obj, "current_expedition_num"));
        QString max_expedition_num = QString::number(api::json_read_int(data_obj, "max_expedition_num"));
        bool is_extra_task_reward_received = data_obj.value("is_extra_task_reward_received").isBool();
        QString current_home_coin = QString::number(api::json_read_int(data_obj, "current_home_coin"));
        QString max_home_coin = QString::number(api::json_read_int(data_obj, "max_home_coin"));

        ui->FragileResinValue->setText(QString("<font color = #FFA500 >%1</font>").arg(current_resin) + " / " + max_resin);
        ui->SereniteaPotValue->setText(QString("<font color = #FFA500 >%1</font>").arg(current_home_coin) + " / " + max_home_coin);
        ui->CommissionValue->setText(QString("<font color = #FFA500 >%1</font>").arg(finished_task_num) + " / " + total_task_num);
        if(is_extra_task_reward_received)
            ui->ParametricTransformerValue->setText(QString("<font color = #FFA500 >%1</font>").arg("1") + " / 1");
        else
            ui->ParametricTransformerValue->setText(QString("<font color = #FFA500 >%1</font>").arg("0") + " / 1");
        ui->DomainsValue->setText(QString("<font color = #FFA500 >%1</font>").arg(current_expedition_num) + " / " + max_expedition_num);

        int time = resin_recovery_time.toInt();
        int H = time / (60*60);
        int M = (time - (H * 60 * 60)) / 60;
        //int S = (time - (H * 60 * 60)) - M * 60;
        QString hour = QString::number(H);
        QString min = QString::number(M);
        //QString sec = QString::number(S);
        ui->FragileResinTime->setText(QString("<font color = #FFA500 >%1</font>").arg(hour) + " 小时 " + QString("<font color = #FFA500 >%1</font>").arg(min) + " 分钟 ");
        ui->name_label->setText(data.name + "\t" + QString::number(data.uid));


    }else if(message == "")
    {
        ui->tabWidget->setCurrentIndex(1);
        ui->error_label->setText("error: 网络错误");
    }else
    {
        ui->tabWidget->setCurrentIndex(1);
        ui->error_label->setText("error:" + message);
    }
}

QString accountWindet::get_DS(QString region, QString uid, int platform)
{
    QString salt;
    if (platform == 0)
    {
        salt = "xV8v4Qu54lUKrEYFZkJhB8cuOh9Asafs";
    }else
    {
        salt = "6cqshh5dhw73bzxn20oexa9k516chk7s";
        //salt = "6s25p5ox5y14umn1p61aqyyvbvvl3lrt";
    }
    QString t = QString::number(QDateTime::currentSecsSinceEpoch());
    int random = QRandomGenerator::global()->bounded(100000);
    QString r = QString::number(random + 100000);
    QString q = "role_id="+ uid + "&server=" + region;
    QString DS = "salt=" + salt + "&t=" + t + "&r=" + r +"&b=&q=" + q;
    QString DSMD5 = QCryptographicHash::hash(DS.toLatin1(),QCryptographicHash::Md5).toHex();
    QString DSresult = t + "," + r + "," + DSMD5;
    return DSresult;
}

QString accountWindet::get_hoyo_daily_DS()
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
