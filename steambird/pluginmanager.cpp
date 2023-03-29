#include "pluginmanager.h"

PluginManager::PluginManager()
{

}

QDir PluginManager::get_plugin_path()
{
    QDir path = QDir(qApp->applicationDirPath() + "/plugins");
    return path;
}

void PluginManager::load_plugin(const QString &filepath)
{
    // 判断是否是库
    if (!QLibrary::isLibrary(filepath))
        return;

    // 加载插件
    QPluginLoader *loader = new QPluginLoader(filepath);
    if (loader->load()) {
        // 如果继承自 Plugin，则认为是自己的插件（防止外部插件注入）。
        InterfacePlugin *plugin = qobject_cast<InterfacePlugin *>(loader->instance());
        if (plugin)
        {
            this->m_loaders.insert(filepath, loader);
            connect(loader->instance(),SIGNAL(send_message(PluginMetaData)),this,SLOT(accept_plugin_message(PluginMetaData)),Qt::UniqueConnection);
            //connect(loader->instance(), &InterfacePlugin::send_message, this, &PluginManager::accept_plugin_message);
        }
        else
        {
            delete loader;
            loader = Q_NULLPTR;
        }
    }
}

void PluginManager::load_all_plugin()
{
    QDir path = get_plugin_path();

    // 初始化插件中的元数据
    foreach (QFileInfo info, path.entryInfoList(QDir::Files | QDir::NoDotAndDotDot))
    {
            scan(info.absoluteFilePath());
    }
    // 加载插件
    foreach (QFileInfo info, path.entryInfoList(QDir::Files | QDir::NoDotAndDotDot))
    {
            load_plugin(info.absoluteFilePath());
    }
}

void PluginManager::scan(const QString &filepath)
{
    if (!QLibrary::isLibrary(filepath))
        return;

    // 获取元数据（名称、版本、依赖）
    QPluginLoader *loader = new QPluginLoader(filepath);
    QJsonObject json = loader->metaData().value("MetaData").toObject();

    this->m_names.insert(filepath, json.value("name").toVariant());
    this->m_nicknames.insert(filepath, json.value("nickname").toVariant());
    this->m_authors.insert(filepath, json.value("author").toVariant());
    this->m_versions.insert(filepath, json.value("version").toVariant());
    this->m_platforms.insert(filepath, json.value("platform").toVariant());
    this->m_descriptions.insert(filepath, json.value("description").toVariant());

    delete loader;
    loader = Q_NULLPTR;
}

void PluginManager::unload_plugin(const QString &filepath)
{
    QPluginLoader *loader = this->m_loaders.value(filepath);

    // 卸载插件，并从内部数据结构中移除
    if (loader->unload())
    {
        this->m_loaders.remove(filepath);
        delete loader;
        loader = Q_NULLPTR;
    }
}

void PluginManager::unload_all_plugin()
{
    foreach (const QString &filepath, this->m_loaders.keys())
        unload_plugin(filepath);
}

QPluginLoader *PluginManager::get_plugin(const QString &name)
{
    QPluginLoader *loader = this->m_loaders.value(this->m_names.key(name));
    return loader;
}

QList<QPluginLoader *> PluginManager::get_all_plugins()
{
    return this->m_loaders.values();
}



void PluginManager::accept_plugin_message(PluginMetaData metadata)
{
    if(metadata.dest == "" || metadata.dest == "main")
    {
        if(metadata.type == 0)
        {
            metadata.datas = accountApi::get_account_info(qApp->applicationDirPath() + "/data/account.db");
        }else if (metadata.type == 1)
        {
            QList<account_data> datas;
            account_data data = titlebar::create(NULL)->get_account();
            datas << data;
        }
        auto loader = get_plugin(metadata.from);
            if(loader)
            {
                auto interfaces = qobject_cast<InterfacePlugin*>(loader->instance());;
                if(interfaces)
                {
                    interfaces->accept_message(metadata);
                }
            }
    }else
    {
        auto loader = get_plugin(metadata.dest); //目标插件
        if(loader) {

            auto interfaces = qobject_cast<InterfacePlugin*>(loader->instance());
            if(interfaces) {
                interfaces->accept_message(metadata); //转发给对应的插件
            }
        }
    }
}
