#ifndef FLOOR_H
#define FLOOR_H

#include <QWidget>
#include <QStringList>

#include "abyssapi.h"

namespace Ui {
class Floor;
}

class Floor : public QWidget
{
    Q_OBJECT

public:
    explicit Floor(QWidget *parent = nullptr);
    ~Floor();
    //没有判断，注意溢出
    void init(int floor, int all_star, int max_star, int count, QStringList time, int star[3], QStringList icon, QStringList lv);

private:
    Ui::Floor *ui;
};

#endif // FLOOR_H
