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

    ~MainGraphicsView();

    // Малює фігуру
    template<typename T>
    void drawFigure(T* rect);

    // Шукає фігуру з якою перетинається
    // Реалізацію змінити!!!
    template<typename T1>
    bool collidesWithSomeone(T1* rect);

    bool CurrentGeometry_Set(char cg);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    void wheelEvent(QWheelEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:

    bool m_panning = false;
    QPoint m_lastMousePos;

    // just now for test
    QVector<float> now;

    qreal currentScale = 1.0;

    QGraphicsScene      *m_scene             = nullptr;

    // Не забувати очищати після використання
    QVector<CurvedRectangle*>       _CurvedRectangles;
    QVector<InvisibleRectangle*>    _InvisibleRectangles;
    QVector<OrdinaryRectangle*>     _OrdinaryRectangles;
    QVector<SlantedRectangle*>      _SlantedRectangles;
    QVector<BoxLine*>               _BoxLines;

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
