#ifndef ACCOUNTAPI_H
#define ACCOUNTAPI_H


#include <QStringList>
#include "api.h"
#include "interfaceplugin.h"

namespace accountApi {
    QStringList account_count();
    QString get_DS();
    QString get_mys_DS(QString region, QString uid);

    QList<account_data> get_account_info(QString path);
    void data_insert(QString path, account_data data);
    account_data get_mys_info(QByteArray cookie);
    account_data get_hoyo_info(QByteArray cookie);
}


#endif // ACCOUNTAPI_H
