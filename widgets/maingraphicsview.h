#ifndef MAINGRAPHICSVIEW_H
#define MAINGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QMouseEvent>
#include <QDebug>
#include <QVector>

#include "../geometry/rectangle/curvedrectangle.h"
#include "../geometry/rectangle/invisiblerectangle.h"
#include "../geometry/rectangle/ordinaryrectangle.h"
#include "../geometry/rectangle/slantedrectangle.h"
#include "../geometry/line/boxline.h"

#include "../datamanagement/localdatamanager.h"

class MainGraphicsView : public QGraphicsView {
    Q_OBJECT
public:
    using QGraphicsView::QGraphicsView; // Наслідування конструкторів
    explicit MainGraphicsView(QWidget *parent = nullptr);

    bool CurrentGeometry_Set(char cg);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:

    // just now for test
    QVector<float> now;

    qreal currentScale = 1.0;

    QGraphicsScene      *m_scene             = nullptr;

    // Не забувати очищати після використання
    CurvedRectangle     *_CurvedRectangle    = nullptr;
    InvisibleRectangle  *_InvisibleRectangle = nullptr;
    OrdinaryRectangle   *_OrdinaryRectangle  = nullptr;
    SlantedRectangle    *_SlantedRectangle   = nullptr;
    BoxLine             *_BoxLine            = nullptr;

    // Робота з локальною пам'ятю
    LocalDataManager    *_LocalDataManager   = nullptr;

    // n = none
    // a = _OrdinaryRectangle
    // b = _CurvedRectangle
    // c = _SlantedRectangle
    // d = _InvisibleRectangle
    // f = _LineBox
    // Для зміни значення є CurrentGeometry_Set
    char CurrentGeometry = 'n';
};

#endif // MAINGRAPHICSVIEW_H
