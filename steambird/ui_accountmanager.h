/********************************************************************************
** Form generated from reading UI file 'accountmanager.ui'
**
** Created by: Qt User Interface Compiler version 6.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ACCOUNTMANAGER_H
#define UI_ACCOUNTMANAGER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_accountManager
{
public:
    QGridLayout *gridLayout;
    QPushButton *mys_login;
    QPushButton *hoyolab_login;
    QPushButton *delete_account;
    QTableWidget *account_table;

    void setupUi(QWidget *accountManager)
    {
        if (accountManager->objectName().isEmpty())
            accountManager->setObjectName(QString::fromUtf8("accountManager"));
        accountManager->resize(655, 410);
        gridLayout = new QGridLayout(accountManager);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        mys_login = new QPushButton(accountManager);
        mys_login->setObjectName(QString::fromUtf8("mys_login"));

        gridLayout->addWidget(mys_login, 1, 1, 1, 1);

        hoyolab_login = new QPushButton(accountManager);
        hoyolab_login->setObjectName(QString::fromUtf8("hoyolab_login"));

        gridLayout->addWidget(hoyolab_login, 1, 2, 1, 1);

        delete_account = new QPushButton(accountManager);
        delete_account->setObjectName(QString::fromUtf8("delete_account"));

        gridLayout->addWidget(delete_account, 1, 0, 1, 1);

        account_table = new QTableWidget(accountManager);
        account_table->setObjectName(QString::fromUtf8("account_table"));
        account_table->setTextElideMode(Qt::ElideMiddle);

        gridLayout->addWidget(account_table, 0, 0, 1, 3);


        retranslateUi(accountManager);

        QMetaObject::connectSlotsByName(accountManager);
    } // setupUi

    void retranslateUi(QWidget *accountManager)
    {
        accountManager->setWindowTitle(QCoreApplication::translate("accountManager", "Form", nullptr));
        mys_login->setText(QCoreApplication::translate("accountManager", " \344\275\277\347\224\250\347\261\263\346\270\270\347\244\276\347\231\273\345\275\225", nullptr));
        hoyolab_login->setText(QCoreApplication::translate("accountManager", "\344\275\277\347\224\250HoyoLab\347\231\273\345\275\225", nullptr));
        delete_account->setText(QCoreApplication::translate("accountManager", "\345\210\240\351\231\244\350\264\246\345\217\267", nullptr));
    } // retranslateUi

};

namespace Ui {
    class accountManager: public Ui_accountManager {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ACCOUNTMANAGER_H
