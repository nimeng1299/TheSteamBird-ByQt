TEMPLATE        = lib       #表示这个makefile是一个lib的makefile，应用程序则为app
CONFIG         += plugin    #应用程序是一个插件
QT       += core gui
TARGET          = Gacha  #生成插件的名称
DESTDIR         = ../steambird/debug/plugins/ #生成插件的目录
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    gacha.cpp

HEADERS += \
    gacha.h

FORMS += \
    gacha.ui

