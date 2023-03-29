QT       += core gui sql webenginewidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    accountapi.cpp \
    accountmanager.cpp \
    api.cpp \
    loginaccountbrowser.cpp \
    main.cpp \
    mainwindow.cpp \
    pluginmanager.cpp \
    titlebar.cpp

HEADERS += \
    accountapi.h \
    accountmanager.h \
    api.h \
    httplib.h \
    interfaceplugin.h \
    loginaccountbrowser.h \
    mainwindow.h \
    pluginmanager.h \
    titlebar.h

FORMS += \
    accountmanager.ui \
    loginaccountbrowser.ui \
    mainwindow.ui
#程序版本
VERSION = 1.1.0.0
#程序图标
RC_ICONS = "./res/ico32.ico"
#公司名称
QMAKE_TARGET_COMPANY ="TheSteambird"
#程序说明
QMAKE_TARGET_DESCRIPTION = "TheSteambird"
#程序名称
QMAKE_TARGET_PRODUCT = "TheSteambird"
#程序语言
RC_LANG = 0x0800
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

LIBS += \
    -LD:\ruanjiangreen\OpenSSL-Win64\lib -llibssl \
    -LD:\ruanjiangreen\OpenSSL-Win64\lib -llibcrypto

INCLUDEPATH += \
    $$quote(D:\ruanjiangreen\OpenSSL-Win64\include)

RESOURCES += \
    resource.qrc

