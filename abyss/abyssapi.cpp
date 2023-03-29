#include "abyssapi.h"


QString abyssapi::get_mys_abyss_ds(int schedule_type, int uid, QString server)
{
    QString salt = "xV8v4Qu54lUKrEYFZkJhB8cuOh9Asafs";

    QString t = QString::number(QDateTime::currentSecsSinceEpoch());
    int random = QRandomGenerator::global()->bounded(100000);
    QString r = QString::number(random + 100000);
    QString q = "role_id="+ QString::number(uid) + "&schedule_type=" + QString::number(schedule_type) + "&server=" + server;
    QString DS = "salt=" + salt + "&t=" + t + "&r=" + r +"&b=&q=" + q;
    QString DSMD5 = QCryptographicHash::hash(DS.toLatin1(),QCryptographicHash::Md5).toHex();
    QString DSresult = t + "," + r + "," + DSMD5;
    return DSresult;
}

QString abyssapi::get_hoyo_abyss_ds()
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

QStringList abyssapi::update(account_data data)
{
    QStringList list;
    if(data.platform == 0)
    {
        list = update_mys_abyss(data);
    }else
    {
        list = update_hoyo_abyss(data);
    }
    return list;
}

QStringList abyssapi::update_mys_abyss(account_data data)
{
    QStringList data_list;

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

    auto get_abyss = [](int schedule_type, QString cookie, account_data data){
        httplib::Headers header = {
            {"Cookie", cookie.toStdString()},
            {"x-rpc-app_version", "2.45.1"},
            {"x-rpc-client_type", "5"},
            {"DS", get_mys_abyss_ds(schedule_type, data.uid, data.server).toStdString()}
        };
        QString url;
        url = "/game_record/app/genshin/api/spiralAbyss?schedule_type=" + QString::number(schedule_type) + "&role_id=" + QString::number(data.uid) + "&server=" + data.server;
        QString res = api::network_body(api::network_https_get("api-takumi-record.mihoyo.com", url, header));
        return res;
    };
    data_list << get_abyss(1, data_cookie, data)
              << get_abyss(2, data_cookie, data);
    return data_list;
}

QStringList abyssapi::update_hoyo_abyss(account_data data)
{
    QStringList data_list;
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

    auto get_abyss = [](int schedule_type, QString cookie, account_data data){
        httplib::Headers header = {
            {"Cookie", cookie.toStdString()},
            {"x-rpc-app_version", "2.45.1"},
            {"x-rpc-client_type", "5"},
            {"DS", get_hoyo_abyss_ds().toStdString()}
        };
        QString url;
        url = "/game_record/genshin/api/spiralAbyss?schedule_type=" + QString::number(schedule_type) + "&role_id=" + QString::number(data.uid) + "&server=" + data.server;
        QString res = api::network_body(api::network_https_get("bbs-api-os.hoyolab.com", url, header));
        return res;
    };
    data_list << get_abyss(1, data_cookie, data)
              << get_abyss(2, data_cookie, data);
    return data_list;
}

QPixmap abyssapi::get_icon(QString url)
{
    QString Path = qApp->applicationDirPath();
    api::check_dir(Path + "/icon");
    QString pic_name = url.right(url.length() - url.lastIndexOf("/") - 1);
    QString pic_path = Path + "/icon/" + pic_name;
    if(!api::isFileExist(pic_path))
    {
        std::string std_url = url.toStdString();
        size_t len = std_url.length();//获取字符串长度
        int nmlen = MultiByteToWideChar(CP_ACP, 0, std_url.c_str(), len + 1, NULL, 0);//如果函数运行成功，并且cchWideChar为零，
        //返回值是接收到待转换字符串的缓冲区所需求的宽字符数大小。
        wchar_t* buffer = new wchar_t[nmlen];
        const wchar_t* wchar_t_pic_path = reinterpret_cast<const wchar_t *>(pic_path.utf16());
        MultiByteToWideChar(CP_ACP, 0, std_url.c_str(), len + 1, buffer, nmlen);
        HRESULT hr = URLDownloadToFile(NULL, buffer, wchar_t_pic_path, 0, NULL);
        //if (hr == S_OK)
    }

    QPixmap pic(pic_path);
    return pic;
}
