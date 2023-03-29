
#include <QObject>
#include <QApplication>
#include <QPluginLoader>
#include <QLibrary>
#include <QDir>
#include "titlebar.h"
#include "accountapi.h"


class PluginManager : public QObject
{
    Q_OBJECT

public:
    static PluginManager &getInstance()
    {
        static PluginManager m_instance;
        return m_instance;
    }


public:
    PluginManager();
    //获取插件目录
    QDir get_plugin_path();
    //加载某个插件
    void load_plugin(const QString &filepath);
    //加载所有插件
    void load_all_plugin();
    //扫描json数据
    void scan(const QString &filepath);
    //卸载某个插件
    void unload_plugin(const QString &filepath);
    //卸载所有插件
    void unload_all_plugin();
    //获取某个插件
    QPluginLoader * get_plugin(const QString &name);
    //获取所有插件
    QList<QPluginLoader *> get_all_plugins();

    //第一个参数为插件路径
    QHash<QString, QVariant> m_names;//插件名称
    QHash<QString, QVariant> m_nicknames;//显示的名称
    QHash<QString, QVariant> m_authors;//显示的名称
    QHash<QString, QVariant> m_versions;//插件版本
    QHash<QString, QVariant> m_platforms;//插件支持的服务器 0、米游社 1、HoyoLab 2、都支持
    QHash<QString, QVariant> m_descriptions;//插件说明
    QHash<QString, QPluginLoader *>m_loaders; //QPluginLoader实例

public slots:
    void accept_plugin_message(PluginMetaData);



};


