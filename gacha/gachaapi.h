#ifndef GACHAAPI_H
#define GACHAAPI_H
#pragma once

#include <QTextCodec>

#include "../steambird/api.h"

#define GACHA_SIZE 10
namespace gachaapi {
    QList<QJsonObject> from_yuanshen_file(QString file_path);
    QJsonObject from_yuanshen_url(QString url);
    void gacha_json_file_insert(QString path, QString name, QString value);
    QList<QList<QStringList>> get_data_from_database(QString path, QString uid, int header_count);
    QList<QJsonObject> from_genshin_file(QString file_path);
    QJsonObject from_genshin_url(QString url);
    QList<QJsonObject> from_url(QString url);
}

#endif // GACHAAPI_H
