#include "accountapi.h"

QStringList accountApi::account_count()
{
    QStringList aa;
    return aa;
}

QString accountApi::get_mys_DS(QString region, QString uid)
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

QList<account_data> accountApi::get_account_info(QString path)
{
    QList<account_data> data;
    QString table_name = "account";
    int count = api::database_count(path, table_name);
    QVariantList info = api::database_find_all(path, 6, table_name);
    if(count != 0)
    {
        for (int i = 0; i < count ; i++)
        {
            int begin = i * 6;
            QString name = info.at(begin).toString();
            QString server = info.at(begin + 1).toString();
            int platform = info.at(begin + 2).toInt();
            int id = info.at(begin + 3).toInt();
            int uid = info.at(begin + 4).toInt();
            QByteArray cookie = info.at(begin + 5).toByteArray();
            account_data account = {name, server, platform, id, uid, cookie};
            data << account;
        }
    }
    return data;
}

account_data accountApi::get_mys_info(QByteArray cookie)
{ 
    account_data data;
    int id;
    QString cookie_id = cookie;
    QStringList cookies = cookie_id.split(";");
    auto cookie_value = [](QString strcookie){
        int index = strcookie.indexOf("=");
        QString value = strcookie.right(strcookie.length() - index - 1);
        return value;
    };
    //检查是否包含id
    QStringList account_id_list = cookies.filter("account_id");
    if(account_id_list.isEmpty())
    {
        QStringList login_uid_list = cookies.filter("login_uid");
        if(login_uid_list.isEmpty())
        {
            QStringList stuid = cookies.filter("stuid");
            if(login_uid_list.isEmpty())
            {
                data.id = -1;
                return data;
            }else
            {
                id = cookie_value(stuid.at(0)).toInt();
            }

        }else
        {
            id = cookie_value(login_uid_list.at(0)).toInt();
        }

    }else
    {
        id = cookie_value(account_id_list.at(0)).toInt();
    }
    //另外的参数
    QStringList login_ticket_list = cookies.filter("login_ticket");
    QString login_ticket;
    if(login_ticket_list.isEmpty())
    {
        data.id = -1;
        return data;
    }else
    {
        login_ticket = cookie_value(login_ticket_list.at(0));
    }

    QStringList stoken_list = cookies.filter("stoken");
    QString stoken;
    if(stoken_list.isEmpty())
    {
        httplib::Headers stoken_headers = {
            {"x-rpc-app_version","2.11.2"},
            {"User-Agent","Mozilla/5.0 (iPhone; CPU iPhone OS 13_2_3 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) miHoYoBBS/2.11.1"},
            {"x-rpc-client_type","5"},
            {"Referer","https://webstatic.mihoyo.com/"},
            {"Origin","https://webstatic.mihoyo.com"},
            {"Cookie",cookie_id.toStdString()}
        };
        QString stoken_path = "/auth/api/getMultiTokenByLoginTicket?login_ticket=" + login_ticket + "&token_types=3&uid=" + QString::number(id);
        QString stoken_get = api::network_body(api::network_https_get("api-takumi.mihoyo.com", stoken_path, stoken_headers));
        QJsonObject stoken_obj = api::QString_to_QJsonObject(stoken_get);
        stoken = api::json_read_qstring(api::json_read_array(api::json_read_object(stoken_obj, "data"), "list").at(0).toObject(), "token");
        cookie_id = cookie_id + "; stoken=" + stoken;
    }else
    {
        stoken = cookie_value(stoken_list.at(0));
    }

    QStringList cookie_token_list = cookies.filter("cookie_token");
    QString cookie_token;
    if(cookie_token_list.isEmpty())
    {
        httplib::Headers cookie_token_headers = {
            {"x-rpc-app_version","2.11.2"},
            {"User-Agent","Mozilla/5.0 (iPhone; CPU iPhone OS 13_2_3 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) miHoYoBBS/2.11.1"},
            {"x-rpc-client_type","5"},
            {"Referer","https://webstatic.mihoyo.com/"},
            {"Origin","https://webstatic.mihoyo.com"},
            {"Cookie",cookie_id.toStdString()}
        };
        QString cookie_token_path = "/auth/api/getCookieAccountInfoBySToken?uid=" + QString::number(id) + "&stoken=" + stoken;
        QString cookie_token_get = api::network_body(api::network_https_get("api-takumi.mihoyo.com", cookie_token_path, cookie_token_headers));
        QJsonObject cookie_token_obj = api::QString_to_QJsonObject(cookie_token_get);
        cookie_token = api::json_read_qstring(api::json_read_object(cookie_token_obj, "data"), "cookie_token");
        cookie_id = cookie_id + "; cookie_token=" + cookie_token;
    }else
    {
        cookie_token = cookie_value(cookie_token_list.at(0));
    }
    //游戏数据
    httplib::Headers game_data_headers = {
        {"Cookie",cookie_id.toStdString()}
    };
    QString game_data_path = "/binding/api/getUserGameRolesByCookie?game_biz=hk4e_cn";
    QString game_data_get = api::network_body(api::network_https_get("api-takumi.mihoyo.com", game_data_path, game_data_headers));
    QJsonObject game_data_obj = api::QString_to_QJsonObject(game_data_get);
    QJsonObject game_data = api::json_read_array(api::json_read_object(game_data_obj, "data"), "list").at(0).toObject();
    data.name = api::json_read_qstring(game_data, "nickname");
    data.server = api::json_read_qstring(game_data, "region");
    data.platform = 0;
    data.id = id;
    data.uid = api::json_read_qstring(game_data, "game_uid").toInt();
    data.cookie = cookie_id;
    return data;
}



QString accountApi::get_DS()
{
    QString salt = "V1PYbXKQY7ysdx3MNCcNbsE1LtY2QZpW";
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

void accountApi::data_insert(QString path, account_data data)
{
    QString table, value;
    table = "name, server, platform, platform_id, uid, cookie";
    value = "'" + data.name + "','" + data.server + "'," + QString::number(data.platform) + "," + QString::number(data.id) + "," + QString::number(data.uid) + ",'" + data.cookie + "'";
    api::database_insert(path, "account", table, value);
}

account_data accountApi::get_hoyo_info(QByteArray cookie)
{
    account_data data;
    int id;
    QString cookie_id = cookie;
    QStringList cookies = cookie_id.split(";");
    auto cookie_value = [](QString strcookie){
        int index = strcookie.indexOf("=");
        QString value = strcookie.right(strcookie.length() - index - 1);
        return value;
    };

    QStringList account_id_list = cookies.filter("account_id");
    if(account_id_list.isEmpty())
    {
        QStringList login_uid_list = cookies.filter("login_uid");
        if(login_uid_list.isEmpty())
        {
            QStringList stuid = cookies.filter("stuid");
            if(login_uid_list.isEmpty())
            {
                data.id = -1;
                return data;
            }else
            {
                id = cookie_value(stuid.at(0)).toInt();
            }

        }else
        {
            id = cookie_value(login_uid_list.at(0)).toInt();
        }

    }else
    {
        id = cookie_value(account_id_list.at(0)).toInt();
    }

    QStringList cookie_token_list = cookies.filter("cookie_token");
    QString cookie_token;
    if(cookie_token_list.isEmpty())
    {
        data.id = -1;
        return data;
    }else
    {
        cookie_token = cookie_value(cookie_token_list.at(0));
    }

    //游戏数据
    httplib::Headers game_data_headers = {
        {"Cookie",cookie_id.toStdString()}
    };
    QString game_data_path = "/binding/api/getUserGameRolesByCookie?game_biz=hk4e_global";
    QString game_data_get = api::network_body(api::network_https_get("api-os-takumi.hoyoverse.com", game_data_path, game_data_headers));
    QJsonObject game_data_obj = api::QString_to_QJsonObject(game_data_get);
    QJsonObject game_data = api::json_read_array(api::json_read_object(game_data_obj, "data"), "list").at(0).toObject();
    data.name = api::json_read_qstring(game_data, "nickname");
    data.server = api::json_read_qstring(game_data, "region");
    data.platform = 1;
    data.id = id;
    data.uid = api::json_read_qstring(game_data, "game_uid").toInt();
    data.cookie = cookie_id;
    return data;
}
