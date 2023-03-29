
#define CPPHTTPLIB_OPENSSL_SUPPORT

#include <QDir>
#include <QCryptographicHash>
#include <QJsonDocument>
#include <QJsonObject>
#include <QtSql>
#include <QDateTime>
#include "httplib.h"
  
#pragma once

namespace api {

    //检查文件夹是否存在，不存在就创建
    void check_dir(QString path);
    //检查文件是否存在，用绝对路径
    bool isFileExist(QString path);
    //MD5加密
    QString MD5(QString str);
    //时间戳转时间,毫秒级
    QString timestamp_to_time(int timestamp, QString str = "yyyy-MM-dd hh:mm:ss");
    //时间转时间戳
    int time_to_stamp(QString time, QString str = "yyyy-MM-dd hh:mm:ss");

    //json类
    //QString 转jsonObject
    QJsonObject QString_to_QJsonObject (QString json);
    //读取字符串
    QString json_read_qstring(QJsonObject obj, QString name);
    //读取下一层obj
    QJsonObject json_read_object(QJsonObject obj, QString name);
    //读取int
    int json_read_int(QJsonObject obj, QString name);
    //读取集合
    QJsonArray json_read_array(QJsonObject obj, QString name);

    //SQLite数据库操作类
    //创建数据库连接
    QSqlDatabase database_create_connect(QString path);
    //创建数据库文件
    bool database_create_file(QString path);
    //创建表
    bool database_create_table(QString path, QString table_name, QStringList head_name, QStringList type);
    //查询表是否存在
    bool database_table_isExist(QString path, QString table_name);
    //查询部分数据
    QVariantList database_find(QString path, QString table_name, QStringList head_name);
    //查询全部数据， count：列的数量
    QVariantList database_find_all(QString path, int count, QString table_name);
    //获取数据行数
    int database_count(QString path, QString table_name);
    //插入数据
    bool database_insert(QString path, QString table_name, QString head_name_all, QString value_all);
    //删除数据，利用条件
    bool database_delete(QString path, QString table_name, QString condition);
    //获取所有表名
    QStringList databast_get_all_table(QString path);

    //网络类
    //https get
    httplib::Result network_https_get(QString host, QString path, httplib::Headers headers);
    //status
    int network_status(httplib::Result res);
    //body
    QString network_body(httplib::Result res);
    //header内容
    QString network_header(httplib::Result res, QString header);
}
