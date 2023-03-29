#include "floor.h"
#include "ui_floor.h"

Floor::Floor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Floor)
{
    ui->setupUi(this);

}

Floor::~Floor()
{
    delete ui;
}

void Floor::init(int floor, int all_star, int max_star, int count, QStringList time, int star[3], QStringList icon, QStringList lv)
{
    QStringList number;
    number << "一" << "二" << "三" << "四" << "五" << "六" << "七" << "八" << "九" << "十" << "十一" << "十二";
    ui->title->setText("深境螺旋第" + number.at(floor - 1) + "层");
    ui->star->setText(QString::number(all_star) + "/" + QString::number(max_star));

    auto star_tr = [](int count){
        QString str;
        switch (count) {
        case 1:
            str = tr("<html><head/><body><p><img src=\":/res/res/star.png\"/></p></body></html>");
            break;
        case 2:
            str = tr("<html><head/><body><p><img src=\":/res/res/star.png\"/><img src=\":/res/res/star.png\"/></p></body></html>");
            break;
        case 3:
            str = tr("<html><head/><body><p><img src=\":/res/res/star.png\"/><img src=\":/res/res/star.png\"/><img src=\":/res/res/star.png\"/></p></body></html>");
            break;
        default:
            break;
        }
        return str;
    };
    auto scaled_pixmap = [](QPixmap pixmap){
        return pixmap.scaled(128, 192, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    };

    if(count < 1)
        ui->groupBox_1->hide();
    else
    {
        ui->floor_1_time->setText("第一间\t" + time.at(0));
        ui->floor_1_star->setText(star_tr(star[0]));
        ui->character_1_1->setPixmap(scaled_pixmap(abyssapi::get_icon(icon.at(0))));
        ui->character_lv_1_1->setText("Lv " + lv.at(0));
        ui->character_1_2->setPixmap(scaled_pixmap(abyssapi::get_icon(icon.at(1))));
        ui->character_lv_1_2->setText("Lv " + lv.at(1));
        ui->character_1_3->setPixmap(scaled_pixmap(abyssapi::get_icon(icon.at(2))));
        ui->character_lv_1_3->setText("Lv " + lv.at(2));
        ui->character_1_4->setPixmap(scaled_pixmap(abyssapi::get_icon(icon.at(3))));
        ui->character_lv_1_4->setText("Lv " + lv.at(3));
        ui->character_1_5->setPixmap(scaled_pixmap(abyssapi::get_icon(icon.at(4))));
        ui->character_lv_1_5->setText("Lv " + lv.at(4));
        ui->character_1_6->setPixmap(scaled_pixmap(abyssapi::get_icon(icon.at(5))));
        ui->character_lv_1_6->setText("Lv " + lv.at(5));
        ui->character_1_7->setPixmap(scaled_pixmap(abyssapi::get_icon(icon.at(6))));
        ui->character_lv_1_7->setText("Lv " + lv.at(6));
        ui->character_1_8->setPixmap(scaled_pixmap(abyssapi::get_icon(icon.at(7))));
        ui->character_lv_1_8->setText("Lv " + lv.at(7));
    }

    if(count < 2)
        ui->groupBox_2->hide();
    else
    {
        ui->floor_2_time->setText("第二间\t" + time.at(1));
        ui->floor_2_star->setText(star_tr(star[0]));
        ui->character_2_1->setPixmap(scaled_pixmap(abyssapi::get_icon(icon.at(8))));
        ui->character_lv_2_1->setText("Lv " + lv.at(8));
        ui->character_2_2->setPixmap(scaled_pixmap(abyssapi::get_icon(icon.at(9))));
        ui->character_lv_2_2->setText("Lv " + lv.at(9));
        ui->character_2_3->setPixmap(scaled_pixmap(abyssapi::get_icon(icon.at(10))));
        ui->character_lv_2_3->setText("Lv " + lv.at(10));
        ui->character_2_4->setPixmap(scaled_pixmap(abyssapi::get_icon(icon.at(11))));
        ui->character_lv_2_4->setText("Lv " + lv.at(11));
        ui->character_2_5->setPixmap(scaled_pixmap(abyssapi::get_icon(icon.at(12))));
        ui->character_lv_2_5->setText("Lv " + lv.at(12));
        ui->character_2_6->setPixmap(scaled_pixmap(abyssapi::get_icon(icon.at(13))));
        ui->character_lv_2_6->setText("Lv " + lv.at(13));
        ui->character_2_7->setPixmap(scaled_pixmap(abyssapi::get_icon(icon.at(14))));
        ui->character_lv_2_7->setText("Lv " + lv.at(14));
        ui->character_2_8->setPixmap(scaled_pixmap(abyssapi::get_icon(icon.at(15))));
        ui->character_lv_2_8->setText("Lv " + lv.at(15));
    }

    if(count < 3)
        ui->groupBox_3->hide();
    else
    {
        ui->floor_3_time->setText("第三间\t" + time.at(2));
        ui->floor_3_star->setText(star_tr(star[0]));
        ui->character_3_1->setPixmap(scaled_pixmap(abyssapi::get_icon(icon.at(16))));
        ui->character_lv_3_1->setText("Lv " + lv.at(16));
        ui->character_3_2->setPixmap(scaled_pixmap(abyssapi::get_icon(icon.at(17))));
        ui->character_lv_3_2->setText("Lv " + lv.at(17));
        ui->character_3_3->setPixmap(scaled_pixmap(abyssapi::get_icon(icon.at(18))));
        ui->character_lv_3_3->setText("Lv " + lv.at(18));
        ui->character_3_4->setPixmap(scaled_pixmap(abyssapi::get_icon(icon.at(19))));
        ui->character_lv_3_4->setText("Lv " + lv.at(19));
        ui->character_3_5->setPixmap(scaled_pixmap(abyssapi::get_icon(icon.at(20))));
        ui->character_lv_3_5->setText("Lv " + lv.at(20));
        ui->character_3_6->setPixmap(scaled_pixmap(abyssapi::get_icon(icon.at(21))));
        ui->character_lv_3_6->setText("Lv " + lv.at(21));
        ui->character_3_7->setPixmap(scaled_pixmap(abyssapi::get_icon(icon.at(22))));
        ui->character_lv_3_7->setText("Lv " + lv.at(22));
        ui->character_3_8->setPixmap(scaled_pixmap(abyssapi::get_icon(icon.at(23))));
        ui->character_lv_3_8->setText("Lv " + lv.at(23));
    }
}
