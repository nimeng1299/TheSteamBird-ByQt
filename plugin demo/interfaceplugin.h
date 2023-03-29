#include <QString>
#include <QtPlugin>
#include <QObject>
//定义接口

struct account_data
{
    QString name;       //游戏名字
    QString server;     //服务器
    int platform;       //平台 0、米游社 1、HoyoLab
    int id;             //平台id //-1则为账号不存在
    int uid;            //游戏uid
    QString cookie;  //账户cookie
};

struct PluginMetaData
{
    QString from;
    QString dest;
    int type;//0、获取所有账号列表 1、获取当前账号列表
    //数据
    QVariantList data_list;
    QList<account_data> datas;
    QObject *object = nullptr;
};
Q_DECLARE_METATYPE(PluginMetaData);



class InterfacePlugin
{

public:
    virtual ~InterfacePlugin() {}
//初始化
    virtual void init() = 0;

//接受信号
    virtual void accept_message(PluginMetaData) = 0;
//signals:
    virtual void send_message(PluginMetaData) = 0;
};


#define InterfacePlugin_iid "Plugin.TheSteamBird"

Q_DECLARE_INTERFACE(InterfacePlugin, InterfacePlugin_iid)

