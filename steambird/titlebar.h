#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QListView>
#include <QMouseEvent>
#include "accountmanager.h"
#define TITLEBAR_HEIGHT 25 //窗口高度
#define BUTTON_WIDTHT 40 // 按钮宽度
#define TITLE_HEIGHT 20 //标题高度



class titlebar : public QWidget
{
    Q_OBJECT
public:

    static  titlebar * create(QWidget *parent)
    {
        return new titlebar(parent);
    }
    //初始化
    void init();
    void init_data();
    void setTitleWidth(int width);
    void changeLength(int length);
    void manager_widget(int index);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    //获取当前选中的账号
    account_data get_account();

signals:
    void buttonMinClicked();
    void buttonRestoreClicked();
    void buttonMaxClicked();
    void buttonCloseClicked();

private:
    explicit titlebar(QWidget *parent);


    QHBoxLayout * titleLayout;
    QLabel * title;                  // 标题
    QString titleContent = "蒸汽鸟";  // 标题内容
    QComboBox * comboAccount;        // 账号下拉框
    QPushButton* buttonMin;          // 最小化;
    QPushButton* buttonRestore;      // 还原;
    QPushButton* buttonMax;          // 最大化;
    QPushButton* buttonClose;        // 关闭;

    bool isPress = false; //是否可以移动窗口了
    QPoint startMovePos; //开始移动窗口时的鼠标位置

    int old_user_id = -1;

    accountManager * widget;
    QList<account_data> account_info;

};

#endif // TITLEBAR_H
