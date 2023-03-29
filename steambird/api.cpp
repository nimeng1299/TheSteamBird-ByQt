#include "api.h"


void api::check_dir(QString path)
{
    QDir dir(path);
    if(!dir.exists())
        dir.mkdir(path);
}

bool api::isFileExist(QString path)
{
    QFileInfo fileInfo(path);
    if(fileInfo.isFile())
        return true;
    else
        return false;
}

QSqlDatabase api::database_create_connect(QString path)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(path);
    return db;
}

bool api::database_create_file(QString path)
{
    QSqlDatabase db = database_create_connect(path);
    if(db.open())
    {
        db.close();
        return true;
    }else
        return false;
}

bool api::database_create_table(QString path, QString table_name, QStringList head_name, QStringList type)
{
    QSqlDatabase db = database_create_connect(path);
    if(db.open())
    {
        QSqlQuery query(db);
        int count = head_name.count();
        QString sql = "create table " + table_name + " (";
        for (int i = 0; i < count; i++)
        {
            sql = sql + head_name.at(i) + " " + type.at(i);
            if(i != count - 1)
            {
                sql = sql + ", ";
            }else
            {
                sql = sql + ");";
            }
        }
        query.prepare(sql);
        if(query.exec())
        {
            db.close();
            return true;
        }
        else
        {
            db.close();
            return false;
        }
    }else
        return false;
}

QVariantList api::database_find(QString path, QString table_name, QStringList head_name)
{
    QVariantList data;
    QSqlDatabase db = database_create_connect(path);
    if(db.open())
    {
        QSqlQuery query(db);
        QString sql = "select ";
        int count = head_name.count();
        for(int i = 0; i < count; i++)
        {
            sql = sql + head_name.at(i);
            if(i != count - 1)
            {
                sql = sql + ", ";
            }
        }
        sql = sql + " from " + table_name + ";";
        if(!query.exec(sql))
        {
            qDebug() << query.lastError();
        }else
        {
            while(query.next())
            {
                QVariantList this_data;
                this_data.clear();
                for(int i = 0; i < count; i++)
                {
                    this_data << query.value(i);
                }
                data << this_data;
            }
        }
        db.close();
    }
    return data;
}

QVariantList api::database_find_all(QString path, int count, QString table_name)
{
    QVariantList data;
    QSqlDatabase db = database_create_connect(path);
    if(db.open())
    {

        QSqlQuery query(db);
        QString sql = "select * from " + table_name + ";";
        query.prepare(sql);
        if(query.exec())
        {
            while(query.next())
            {
                QVariantList this_data;
                for(int i = 0; i < count; i++)
                {
                    this_data << query.value(i);
                }
                data << this_data;
            }
        }else
        {
            qDebug() << query.lastError();
        }
    }
    return data;
}


int api::database_count(QString path, QString table_name)
{
    int count;
    QSqlDatabase db = database_create_connect(path);
    if(db.open())
    {
        QSqlQuery query(db);
        QString queryString=QString("SELECT count(*) FROM %1").arg(table_name);
        query.exec(queryString);
        query.next();
        count = query.value(0).toInt();
        db.close();
        return count;
    }else
        return -1;
}

bool api::database_insert(QString path, QString table_name, QString head_name_all, QString value_all)
{
    QSqlDatabase db = database_create_connect(path);
    if(db.open())
    {
        QSqlQuery query(db);
        QString sql = "insert into " + table_name + "(" + head_name_all + ") values (" + value_all + ");";
        qDebug() << sql;
        query.prepare(sql);
        if(query.exec())
        {
            db.close();
            return true;
        }
        else
        {
            db.close();
            qDebug() << query.lastError();
            return false;
        }
    }else
    {
        return false;
    }
}


QJsonObject api::QString_to_QJsonObject(QString json)
{
    QJsonDocument document = QJsonDocument::fromJson(json.toUtf8().data());
    return document.object();
}

QString api::json_read_qstring(QJsonObject obj, QString name)
{
    return  obj.value(name).toString();
}

QJsonObject api::json_read_object(QJsonObject obj, QString name)
{
    return obj.value(name).toObject();
}

int api::json_read_int(QJsonObject obj, QString name)
{
    return obj.value(name).toInt();
}

QJsonArray api::json_read_array(QJsonObject obj, QString name)
{
    return obj[name].toArray();
}

QString api::MD5(QString str)
{
    QString md5_str;
    QByteArray byte_str = QCryptographicHash::hash(str.toLatin1(),QCryptographicHash::Md5);
    md5_str.append(byte_str.toHex());
    return md5_str;
}

httplib::Result api::network_https_get(QString host, QString path, httplib::Headers headers)
{
    httplib::SSLClient cli(host.toStdString());
    httplib::Result res = cli.Get(path.toStdString(), headers);
    return res;
}

int api::network_status(httplib::Result res)
{
    return res->status;
}

QString api::network_body(httplib::Result res)
{
    return QString::fromStdString(res->body);
}

QString api::network_header(httplib::Result res, QString header)
{
    return QString::fromStdString(res->get_header_value(header.toStdString()));
}

bool api::database_delete(QString path, QString table_name, QString condition)
{
    QSqlDatabase db = database_create_connect(path);
    if(db.open())
    {
        QSqlQuery query(db);
        QString sql = "delete from " + table_name + " where " + condition + ";";
        qDebug() << sql;
        query.prepare(sql);
        if(query.exec())
        {
            db.close();
            return true;
        }
        else
        {
            db.close();
            qDebug() << query.lastError();
            return false;
        }
    }else
    {
        return false;
    }
}

bool api::database_table_isExist(QString path, QString table_name)
{
    QSqlDatabase db = database_create_connect(path);
    db.open();
    QSqlQuery query(db);
    QString sql = QString("select count(*) from sqlite_master where type = 'table' name='%1'").arg(table_name);
    query.exec(sql);
    bool isExist = query.next();
    db.close();
    return isExist;
}

QString api::timestamp_to_time(int timestamp, QString str)
{
    QDateTime time;
    time.setSecsSinceEpoch(timestamp);
    return time.toString(str);
}

int api::time_to_stamp(QString time, QString str)
{
    QDateTime datetime = QDateTime::fromString(time, str);
    int timestamp = static_cast<int>(datetime.toSecsSinceEpoch());
    return timestamp;
}

QStringList api::databast_get_all_table(QString path)
{
    QStringList list;
    QSqlDatabase db = database_create_connect(path);
    if(db.open())
    {
        list = db.tables();
    }
    return list;
}
