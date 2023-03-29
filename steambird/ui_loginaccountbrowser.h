/********************************************************************************
** Form generated from reading UI file 'loginaccountbrowser.ui'
**
** Created by: Qt User Interface Compiler version 6.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINACCOUNTBROWSER_H
#define UI_LOGINACCOUNTBROWSER_H

#include <QtCore/QVariant>
#include <QtWebEngineWidgets/QWebEngineView>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LoginAccountBrowser
{
public:
    QGridLayout *gridLayout;
    QPushButton *pushButton;
    QVBoxLayout *verticalLayout;
    QWebEngineView *myWeb;

    void setupUi(QWidget *LoginAccountBrowser)
    {
        if (LoginAccountBrowser->objectName().isEmpty())
            LoginAccountBrowser->setObjectName(QString::fromUtf8("LoginAccountBrowser"));
        LoginAccountBrowser->resize(618, 350);
        gridLayout = new QGridLayout(LoginAccountBrowser);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        pushButton = new QPushButton(LoginAccountBrowser);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        QFont font;
        font.setPointSize(16);
        pushButton->setFont(font);

        gridLayout->addWidget(pushButton, 0, 0, 1, 1);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        myWeb = new QWebEngineView(LoginAccountBrowser);
        myWeb->setObjectName(QString::fromUtf8("myWeb"));

        verticalLayout->addWidget(myWeb);


        gridLayout->addLayout(verticalLayout, 1, 0, 1, 1);


        retranslateUi(LoginAccountBrowser);

        QMetaObject::connectSlotsByName(LoginAccountBrowser);
    } // setupUi

    void retranslateUi(QWidget *LoginAccountBrowser)
    {
        LoginAccountBrowser->setWindowTitle(QCoreApplication::translate("LoginAccountBrowser", "Form", nullptr));
        pushButton->setText(QCoreApplication::translate("LoginAccountBrowser", "\347\231\273\345\275\225\345\220\216\347\202\271\345\207\273\350\277\231\351\207\214", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LoginAccountBrowser: public Ui_LoginAccountBrowser {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINACCOUNTBROWSER_H
