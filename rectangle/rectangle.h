/*
 * Клас є основою для усіх прямокутників
 * Він наслідується в усі похідні прямокутники,
 * тому має загальну реалізацію
*/

#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <QString>

class Rectangle
{
public:
    Rectangle();

    // Створення прямокутника (ініціалізація)
    void rectangle_create(const double &xlt, const double &ylt, const double &xrb, const double &yrb);

    // Зміна розміру прямокутника
    void rectangle_resize_left(const double &xlt, const double &ylt);
    void rectangle_resize_right(const double &xrb, const double &yrb);

    // Стилізація
    void rectangle_change_color_background(const QString &color);
    void rectangle_change_color_border(const QString &color);

private:

    // Лівий правий кут
    double XLeftTop;
    double YLeftTop;

    // Правий нижній кут
    double XRighBottom;
    double YRighBottom;

};

#endif // RECTANGLE_H
