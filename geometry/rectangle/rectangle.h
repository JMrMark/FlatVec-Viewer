#pragma once

/*
 * Радіус, який застосовується для пошуку сусідніх об'єктів
 * Застосування: оптимізація взаємодії з об'єктами
*/
#define RENDER_RADIUS_ZONE 150

/*
 * Радіус, який застосувується для перерахунку області
 * взаємодії з іншими об'єктами.
 * Повинен бути меншим за RENDER_RADIUS_ZONE
 * Застосування: оптимізація взаємодії з об'єктами
*/
#define RENDER_RADIUS_UPDATE_ZONE 100

/*
 * Клас є абстрактним
 * Він наслідується в усі похідні прямокутники,
 * Використовується для подвійної диспетчеризації (double dispatch)
*/

#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <QString>
#include <QPointF>

#include <QGraphicsItem>
#include <QPainter>

#include "temprectangle.h"

class CurvedRectangle;
class OrdinaryRectangle;
class SlantedRectangle;

class Rectangle : public QGraphicsItem
{
public:
    using QGraphicsItem::QGraphicsItem;
    Rectangle();

    // ---= Розмір фігури =--- //
    virtual float   getWidth()  const = 0;
    virtual float   getLength() const = 0;
    virtual void    setSize(float width, float length) = 0;

    // ---= Взаємодія прямокутника з точкою =--- //
    virtual bool includesPoint(const QPointF &point) const = 0;
    virtual bool includesPoint(const QPointF &point, float overSize) const = 0;

    // ---= Стан прямокутника =--- //
    inline void setActivatedOff()   {CurrentState = 0; TempRectangleDelete(); update();}
    inline void setActivatedOn()    {CurrentState = 1; TempRectangleCreate(); TempRectangleFill(); update();}
    inline void setSelectedOn()     {CurrentState = 2; TempRectangleCreate(); TempRectangleFill(); update();}
    inline void setErrorOn()        {CurrentState = 3; TempRectangleCreate(); TempRectangleFill(); update();}

    // Стан потрібен для підвантаження стилю
    // 0 - StdStyleRect
    // 1 - ActivatedStyleRect
    // 2 - SelectedStyleRect
    // 3 - ErrorStyleRect
    unsigned short int CurrentState = 0;

    // ---= Модифікація прямокутника =--- //
    enum HandleType {
        TopLeft,    TopMiddle,    TopRight,
        MiddleLeft, Center,       MiddleRight,
        BottomLeft, BottomMiddle, BottomRight,
        None
    };

    /*
    * Вказівник необхідний для перехідного етапу прямокутника.
    * Зберігає у собі тимчасові значення прямокутників. (включно з зонами рендеренгу)
    * Застосовується при зміні станів:
    *  * Стан "не 0" - створення та запис тимчасових даних
    *  * Стан 0 - видалення об'єкту та очищення пам'яті
    */
    TempRectangle*  _TempRectangle = nullptr;

    virtual void    TempRectangleFill() = 0;
    virtual void    TempRectangleNormalize() = 0;
    void            TempRectangleCreate();
    void            TempRectangleDelete();

    /*
     * Обрахунок зони рендеру. Повертає:
     *  - Перший прямокутник: Зона взаємодії з об'єктами
     *  - Другий прямокутник: Зона оновлення "Зони взаємодії з об'єктами"
    */
    using   ActionRender = std::function<QVector<Rectangle*>()>;
    virtual ActionRender RenderZone(const HandleType handle) const = 0;

    // Перевіряємо чи потрібно обновляти зону рендеренгу (якщо true - то так)
    virtual bool renderZoneUpdateValidation() const = 0;

    // Перевіряємо чи зони рендеренгу ініціалізовані
    bool RenderZoneExist();

    // Оновлюємо зони рендеренгу
    void RenderZoneUpdate(const QVector<Rectangle*> &vrz);

    // Отримуємо зону рендеренгу [0]
    Rectangle* RenderZoneGet();

    /*
     * Модифікація прямокутника за рахунок взаємодії з
     * хендлами (точками на прямокутнику)
    */
    using   Action = std::function<void(const QPointF &mousePos)>;
    virtual Action createAction(HandleType handle) = 0;
    virtual void   refuseAction() = 0;

    virtual QVector<QPointF> handles() const = 0;
    virtual HandleType hitHandle(const QPointF &point, qreal radius = 8.0) const = 0;

    virtual void normalizeRect() = 0;

    // ---= Взаємодія прямокутників =--- //
    virtual bool collides(const Rectangle& other, float overSize = 0) const = 0;

    virtual bool collidesWithCurvedRectangle(const CurvedRectangle& a, float overSize = 0) const = 0;
    virtual bool collidesWithOrdinaryRectangle(const OrdinaryRectangle& b, float overSize = 0) const = 0;
    virtual bool collidesWithSlantedRectangle(const SlantedRectangle& c, float overSize = 0) const = 0;

    virtual ~Rectangle();

};

#endif // RECTANGLE_H
