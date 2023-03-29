TEMPLATE        = lib       #表示这个makefile是一个lib的makefile，应用程序则为app
CONFIG         += plugin    #应用程序是一个插件
QT       += core gui sql
TARGET          = OverView  #生成插件的名称
DESTDIR         = ../steambird/debug/plugins/ #生成插件的目录
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ../steambird/api.cpp \
    accountdetail.cpp \
    accountwindet.cpp \
    overview.cpp

HEADERS += \
    ../steambird/api.h \
    accountdetail.h \
    accountwindet.h \
    overview.h

FORMS += \
    accountdetail.ui \
    accountwindet.ui \
    overview.ui

RESOURCES += \
    portraitUI.qrc

LIBS += \
    -LD:\ruanjiangreen\OpenSSL-Win64\lib -llibssl \
    -LD:\ruanjiangreen\OpenSSL-Win64\lib -llibcrypto

INCLUDEPATH += \
    $$quote(D:\ruanjiangreen\OpenSSL-Win64\include)
