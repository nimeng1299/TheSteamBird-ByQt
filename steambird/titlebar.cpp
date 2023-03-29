#include "titlebar.h"
#include <QFontDatabase>

QString AccountDataPath;
QString dataTableName = "account";

titlebar::titlebar(QWidget *parent)
    : QWidget{parent}
{
    AccountDataPath = qApp->applicationDirPath() + "/data/account.db";
    init();
    init_data();
}

void titlebar::init()
{
    widget = NULL;

    auto change_font_px = [](auto a, int px){
        QFont font = a->font();
        font.setPointSize(px);
        return font;
    };//设置组件字体大小

    buttonMin = new QPushButton;
    buttonRestore = new QPushButton;
    buttonMax = new QPushButton;
    buttonClose = new QPushButton;

    buttonMin->setToolTip(QStringLiteral("最小化"));
    buttonRestore->setToolTip(QStringLiteral("还原"));
    buttonMax->setToolTip(QStringLiteral("最大化"));
    buttonClose->setToolTip(QStringLiteral("关闭"));

    buttonMin->setObjectName("title_buttonMin");
    buttonRestore->setObjectName("title_buttonRestore");
    buttonMax->setObjectName("title_buttonMax");
    buttonClose->setObjectName("title_buttonClose");

    buttonMin->setFixedSize(QSize(BUTTON_WIDTHT, TITLEBAR_HEIGHT));
    buttonRestore->setFixedSize(QSize(BUTTON_WIDTHT, TITLEBAR_HEIGHT));
    buttonMax->setFixedSize(QSize(BUTTON_WIDTHT, TITLEBAR_HEIGHT));
    buttonClose->setFixedSize(QSize(BUTTON_WIDTHT, TITLEBAR_HEIGHT));

    buttonMin->setIcon(QIcon("://res/minus.png"));
    buttonRestore->setIcon(QIcon("://res/restore.png"));
    buttonMax->setIcon(QIcon("://res/max.png"));
    buttonClose->setIcon(QIcon("://res/close.png"));

    buttonMin->setFlat(true);
    buttonRestore->setFlat(true);
    buttonMax->setFlat(true);
    buttonClose->setFlat(true);

    buttonMin->setStyleSheet("background:transparent;");
    buttonRestore->setStyleSheet("background:transparent;");
    buttonMax->setStyleSheet("background:transparent;");
    buttonClose->setStyleSheet("background:transparent;");

    buttonRestore->hide();

    connect(buttonMin, &QPushButton::clicked, this, [this](){
        emit buttonMinClicked();
    });
    connect(buttonRestore, &QPushButton::clicked, this, [this](){
        buttonRestore->hide();
        buttonMax->show();
        emit buttonRestoreClicked();
    });
    connect(buttonMax, &QPushButton::clicked, this, [this](){
        buttonRestore->show();
        buttonMax->hide();
        emit buttonMaxClicked();
    });
    connect(buttonClose, &QPushButton::clicked, this, [this](){
        emit buttonCloseClicked();
    });

    title = new QLabel(this->titleContent);
    title->setAlignment(Qt::AlignVCenter);
    title->setIndent(10);
    title->setStyleSheet("color:#696969;");
    //font.setPointSize(10);
    title->setFont(change_font_px(title, 10));

    comboAccount = new QComboBox;
    comboAccount->setView(new QListView());
    comboAccount->setObjectName("combo_account");
    comboAccount->setStyleSheet("QComboBox { background-color: 255, 255, 255;border: 0px; padding: 0px; margin: 0px; padding-right: 0px; color: gray;}"
                                "QComboBox QAbstractItemView {outline: 0px; font-size: 11pt; border: none;}"
                                "QComboBox::drop-down{width:0px;}"
                                "QComboBox QAbstractItemView::item {height: 25px;}");
    comboAccount->setMinimumWidth(130);
    comboAccount->setEditable(false);
    comboAccount->setFont(change_font_px(comboAccount, 11));
    connect(comboAccount, &QComboBox::activated, this, &titlebar::manager_widget);

    titleLayout = new QHBoxLayout(this);
    this->setLayout(titleLayout);
    titleLayout->addWidget(this->title);
    titleLayout->addWidget(this->comboAccount);
    titleLayout->addWidget(this->buttonMin);
    titleLayout->addWidget(this->buttonRestore);
    titleLayout->addWidget(this->buttonMax);
    titleLayout->addWidget(this->buttonClose);
    titleLayout->setContentsMargins(5, 0, 5, 0);//边距
    title->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    this->setFixedHeight(TITLEBAR_HEIGHT);
    this->setWindowFlags(Qt::FramelessWindowHint);//无边框

}

void titlebar::init_data()
{
    api::check_dir(qApp->applicationDirPath() + "/data");
    if(!api::isFileExist(AccountDataPath))
    {
        api::database_create_file(AccountDataPath);
        QStringList head_name;
        head_name <<  "name" << "server" << "platform" << "platform_id" << "uid" << "cookie";
        QStringList type;
        type <<  "text" << "text" << "int" << "int" << "int" << "text";
        qDebug() << api::database_create_table(AccountDataPath, dataTableName, head_name, type);
    }
    /*
    QString head_name1 = "name, server, platform, platform_id, uid, cookie";
    QString value = "'aaa', 'www', 123, 123123, 1111, 'wewwe'";
    api::database_insert(AccountDataPath, "account", head_name1, value);
    */

    //获取用户信息
    account_info = accountApi::get_account_info(AccountDataPath);
    int count = account_info.count();
    QStringList strlist;
    if(count != 0)
    {
        for(int i = 0 ; i < count ; i++)
        {
            strlist << account_info.at(i).name;
        }
    }
    strlist << "账号管理";
    comboAccount->clear();
    comboAccount->addItems(strlist);
}

void titlebar::setTitleWidth(int width)
{
    this->setFixedWidth(width);
}

void titlebar::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(buttonMax->isVisible())
        buttonMax->click();
    else
        buttonRestore->click();
    return QWidget::mouseDoubleClickEvent(event);
}

void titlebar::mousePressEvent(QMouseEvent *event)
{
    if(buttonMax->isVisible())
    {
        this->isPress = true;
        startMovePos = event->globalPos();
    }
    return QWidget::mousePressEvent(event);
}

void titlebar::mouseMoveEvent(QMouseEvent *event)
{
    if(this->isPress)
    {
        QPoint movePoint = event->globalPos() - startMovePos;
        QPoint widgetPos = this->parentWidget()->pos();
        startMovePos = event->globalPos();
        this->parentWidget()->move(widgetPos.x() + movePoint.x(), widgetPos.y() + movePoint.y());
    }
    return QWidget::mouseMoveEvent(event);
}

void titlebar::mouseReleaseEvent(QMouseEvent *event)
{
    this->isPress = false;
    return QWidget::mouseReleaseEvent(event);
}

account_data titlebar::get_account()
{
    QList<account_data> datas = accountApi::get_account_info(qApp->applicationDirPath() + "/data/account.db");
    account_data data;
    if(datas.count() == 0)
    {
        data.id = -1;
        return data;
    }
    int index = this->comboAccount->currentIndex();
    if(index == this->comboAccount->count() || index == -1)
    {
        data.id = -1;
        return data;
    }
    return datas.at(index);
}

void titlebar::changeLength(int length)
{
    this->setFixedSize(length, TITLEBAR_HEIGHT);
}

void titlebar::manager_widget(int index)
{
    int nums = comboAccount->count();
    if(index == nums - 1)
    {//加载管理页面
        comboAccount->setCurrentIndex(old_user_id);
        if(!widget)
        {
            widget = new accountManager;
            connect(widget, &accountManager::close, this, [=](){
                init_data();
            });
        }
        widget->show();

    }else if(index == -1)
    {
        comboAccount->setCurrentIndex(old_user_id);
    }else
    {
        old_user_id = index;
    }
}
