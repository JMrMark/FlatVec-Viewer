#ifndef ACTION_H
#define ACTION_H

#include <QPointF>
#include <QVector>

class Action
{
public:
    Action();

    // Перевірка чи містить прямокутник задану точку
    bool Rectangle_Collides_With_Point(QPointF &pos, QVector<float> &vec) const;

    // Перевірка чи перетинаються прямокутники
    bool Rectangle_Collides_With_Rectangle(QVector<float> &rect1, QVector<float> &rect2) const;


    bool Area_Includes_Point(float &ax1, float &ax2, float &bx) const;
};

#endif // ACTION_H
