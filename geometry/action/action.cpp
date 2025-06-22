#include "action.h"
#include <QDebug>

Action::Action() {}

bool Action::Rectangle_Collides_With_Point(QPointF &pos, QVector<float> &vec) const {

    //qDebug() << vec[0] << "<=" << pos.x() << "<=" << vec[2];
    //qDebug() << vec[1] << "<=" << pos.y() << "<=" << vec[3];

    if (vec[0] <= pos.x() && pos.x() <= vec[2] && vec[1] <= pos.y() && pos.y() <= vec[3]) {
        return true;
    }
    return false;
}

// Не працює!
bool Action::Rectangle_Collides_With_Rectangle(QVector<float> &rect1, QVector<float> &rect2) const {

    /*
     * rect1[0] x1
     * rect1[1] y1
     * rect1[2] x2
     * rect1[3] y2
     *
     * rect2[0] x1
     * rect2[1] y1
     * rect2[2] x2
     * rect2[3] y2
     *
    */

    if ((Area_Includes_Point(rect1[0], rect1[2], rect2[0]) || Area_Includes_Point(rect1[0], rect1[2], rect2[2])) &&
        (Area_Includes_Point(rect1[1], rect1[3], rect2[1]) || Area_Includes_Point(rect1[1], rect1[3], rect2[3])))
    {
        return true;
    }
    return false;
}

bool Action::Area_Includes_Point(float &ax1, float &ax2, float &bx) const {
    if ((ax1 <= bx) && (bx <= ax2)){
        return true;
    }
    return false;
}
