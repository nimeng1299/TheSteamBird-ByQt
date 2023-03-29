#include "gachaapi.h"


QList<QJsonObject> gachaapi::from_yuanshen_file(QString file_path)
{
    QFileInfo fileInfo(file_path);
    QString path = fileInfo.absoluteDir().path();
    path = path + "/YuanShen_Data/webCaches/Cache/Cache_Data/data_2";
    QFile file(path);
    QTextCodec * codec = QTextCodec::codecForName("GBK");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString file_value = codec->toUnicode(file.readAll());
    file.close();
    QString url;
    url = file_value.right(file_value.length() - file_value.lastIndexOf("e20190909gacha-v2"));
    url = url.right(url.length() - url.indexOf("hk4e-api.mihoyo.com"));
    url = url.left(url.indexOf("game_biz") + 50);
    url = url.left(url.lastIndexOf("&gacha_type"));
    QList<QJsonObject> gacha_all;
    QStringList gacha_type;
    gacha_type << "200" << "301" << "302" << "400";
    for(int i = 0; i < gacha_type.count(); i++)
    {
        int page = 1;
        int size = GACHA_SIZE;
        QString end_id = "0";
        bool start = true;
        while (start) {
            start = false;//防止网络无响应死循环
            QString now_url = url + "&gacha_type=" + gacha_type.at(i) + "&page=" + QString::number(page) + "&size=" + QString::number(size) + "&end_id=" + end_id;
            QJsonObject gacha_obj = from_yuanshen_url(now_url);
            QJsonObject gacha_data = api::json_read_object(gacha_obj, "data");
            QJsonArray gacha_array = api::json_read_array(gacha_data, "list");
            int count = gacha_array.count();
            if(count == GACHA_SIZE)
                start = true;
            else if(count == 0)
                break;
            gacha_all << gacha_data;
            end_id = api::json_read_qstring(gacha_array.at(count - 1).toObject(), "id");
            qDebug()<<end_id;
            page++;
        }

    }
    gacha_json_file_insert(qApp->applicationDirPath() + "/data/gacha.json", "yuanshen", file_path);
    return gacha_all;
}

QJsonObject gachaapi::from_yuanshen_url(QString url)
{
    QString uurl = url.right(url.length() - url.indexOf("/", 10));
    httplib::Headers headers = {{}};
    QString body = api::network_body(api::network_https_get("hk4e-api.mihoyo.com", uurl, headers));
    QJsonObject obj = api::QString_to_QJsonObject(body);
    return obj;
}

void gachaapi::gacha_json_file_insert(QString path, QString name, QString value)
{
    QFile file(path);
    QJsonObject obj;
    file.open(QIODevice::ReadWrite);
    QString str = file.readAll();
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(str.toUtf8(), &error);
    if(error.error == QJsonParseError::NoError)
    {
        obj = doc.object();
    }
    if(obj.contains(name))
    {
        obj[name] = value;
    }else
    {
        obj.insert(name, value);
    }
    file.resize(0);
    QJsonDocument write_doc;
    write_doc.setObject(obj);
    file.write(write_doc.toJson());
    file.close();
}

QList<QList<QStringList>> gachaapi::get_data_from_database(QString path, QString uid, int header_count)
{
    QVariantList var = api::database_find_all(path, header_count, "'" + uid + "'");
    QList<QStringList> all_data;
    QList<QStringList> data_200;
    QList<QStringList> data_301;
    QList<QStringList> data_302;
    if(var.count() == 0)
    {
        QList<QList<QStringList>> return_0;
        return_0 << data_200 << data_301 << data_302;
        return return_0;
    }
    for(int i = 0; i < (var.count() / header_count); i++)
    {
       QStringList this_data;
       for(int j = 0; j < header_count; j++)
       {
           this_data << var.at(i * header_count + j).toString();
       }
       all_data << this_data;
    }

    auto compare = [](const QStringList &a, const QStringList &b){
        return a.at(0) < b.at(0);
    };

    std::sort(all_data.begin(), all_data.end(), compare);

    //分类 0、200 1、301 2、302 3、400
    QList<QList<QStringList>> data;

    for(int i = 0; i < all_data.count(); i++)
    {
        QString type = all_data.at(i).at(1);
        if(type == "200")
            data_200 << all_data.at(i);
        else if(type == "301" || type == "400")
            data_301 << all_data.at(i);
        else if(type == "302")
            data_302 << all_data.at(i);
    }
    data << data_200 << data_301 << data_302;
    return data;
}

QList<QJsonObject> gachaapi::from_genshin_file(QString file_path)
{
    QFileInfo fileInfo(file_path);
    QString path = fileInfo.absoluteDir().path();
    path = path + "/GenshinImpact_Data/webCaches/Cache/Cache_Data/data_2";
    QFile file(path);
    QTextCodec * codec = QTextCodec::codecForName("GBK");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString file_value = codec->toUnicode(file.readAll());
    file.close();
    QString url;
    url = file_value.right(file_value.length() - file_value.lastIndexOf("e20190909gacha-v2"));
    url = url.right(url.length() - url.indexOf("hk4e-api-os.hoyoverse.com"));
    url = url.left(url.indexOf("game_biz") + 50);
    url = url.left(url.lastIndexOf("&gacha_type"));
    QList<QJsonObject> gacha_all;
    QStringList gacha_type;
    gacha_type << "200" << "301" << "302" << "400";
    for(int i = 0; i < gacha_type.count(); i++)
    {
        int page = 1;
        int size = GACHA_SIZE;
        QString end_id = "0";
        bool start = true;
        while (start) {
            start = false;//防止网络无响应死循环
            QString now_url = url + "&gacha_type=" + gacha_type.at(i) + "&page=" + QString::number(page) + "&size=" + QString::number(size) + "&end_id=" + end_id;
            QJsonObject gacha_obj = from_genshin_url(now_url);
            QJsonObject gacha_data = api::json_read_object(gacha_obj, "data");
            QJsonArray gacha_array = api::json_read_array(gacha_data, "list");
            int count = gacha_array.count();
            if(count == GACHA_SIZE)
                start = true;
            else if(count == 0)
                break;
            gacha_all << gacha_data;
            end_id = api::json_read_qstring(gacha_array.at(count - 1).toObject(), "id");
            qDebug()<<end_id;
            page++;
        }

    }
    gacha_json_file_insert(qApp->applicationDirPath() + "/data/gacha.json", "GenshinImpact", file_path);
    return gacha_all;
}

QJsonObject gachaapi::from_genshin_url(QString url)
{
    QString uurl = url.right(url.length() - url.indexOf("/", 10));
    httplib::Headers headers = {{}};
    QString body = api::network_body(api::network_https_get("hk4e-api-os.hoyoverse.com", uurl, headers));
    QJsonObject obj = api::QString_to_QJsonObject(body);
    return obj;
}

QList<QJsonObject> gachaapi::from_url(QString url)
{
    auto get_gacha = [](QString url, QString uurl){
        httplib::Headers headers = {{}};
        QString body = api::network_body(api::network_https_get(url, uurl, headers));
        QJsonObject obj = api::QString_to_QJsonObject(body);
        return obj;
    };

    url.remove(0,8);
    int index = url.indexOf("/");
    QString host = url.left(index);
    QString uurl = url.right(url.length() - index);
    uurl = uurl.left(url.lastIndexOf("&gacha_type"));

    QList<QJsonObject> gacha_all;
    QStringList gacha_type;
    gacha_type << "200" << "301" << "302" << "400";
    for(int i = 0; i < gacha_type.count(); i++)
    {
        int page = 1;
        int size = GACHA_SIZE;
        QString end_id = "0";
        bool start = true;
        while (start) {
            start = false;//防止网络无响应死循环
            QString now_url = uurl + "&gacha_type=" + gacha_type.at(i) + "&page=" + QString::number(page) + "&size=" + QString::number(size) + "&end_id=" + end_id;
            QJsonObject gacha_obj = get_gacha(host, now_url);
            QJsonObject gacha_data = api::json_read_object(gacha_obj, "data");
            QJsonArray gacha_array = api::json_read_array(gacha_data, "list");
            int count = gacha_array.count();
            if(count == GACHA_SIZE)
                start = true;
            else if(count == 0)
                break;
            gacha_all << gacha_data;
            end_id = api::json_read_qstring(gacha_array.at(count - 1).toObject(), "id");
            qDebug()<<end_id;
            page++;
        }

    }
    return gacha_all;


}
