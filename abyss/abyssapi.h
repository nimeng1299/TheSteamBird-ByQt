#ifndef ABYSSAPI_H
#define ABYSSAPI_H

#include <QString>
#include <QStringList>
#include <QDateTime>
#include <QRandomGenerator>
#include <QCryptographicHash>
#include <QPixmap>
#include <tchar.h>

#include "../steambird/interfaceplugin.h"
#include "../steambird/api.h"

#pragma comment(lib, "Urlmon.lib")
#pragma once
namespace abyssapi {
    QString get_mys_abyss_ds(int schedule_type, int uid, QString server);
    QString get_hoyo_abyss_ds();
    QStringList update(account_data data);
    QStringList update_mys_abyss(account_data data);
    QStringList update_hoyo_abyss(account_data data);
    QPixmap get_icon(QString url);
}

#endif // ABYSSAPI_H
