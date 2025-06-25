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

#include "../geometrytype/geometrytype.h"

//#include "../datamanagement/localdatamanager.h"

class MainGraphicsView : public QGraphicsView {
    Q_OBJECT
public:
    using QGraphicsView::QGraphicsView; // Наслідування конструкторів
    explicit MainGraphicsView(QWidget *parent = nullptr);

    ~MainGraphicsView();

    // Малює фігуру
    void drawFigure(QGraphicsItem* item);

    // Шукає фігуру з якою перетинається
    template<typename T1>
    bool collidesWithSomeone(const T1* rect) const;

    bool includesPointSomeone(const QPointF &pos) const;

    bool setCurrentGeometry(GeometryType type);
    Rectangle* createRectangleFromType(QRectF rect);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    void wheelEvent(QWheelEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:

    GeometryType m_currentGeometry = GeometryType::None;
    QGraphicsRectItem* m_tempRectItem = nullptr;

    bool m_panning = false;
    QPoint m_lastMousePos;

    bool m_tracking = false;
    QPointF m_posBegin;
    QPointF m_posNow;

    // just now for test
    QVector<float> now;

    qreal currentScale = 1.0;

    QGraphicsScene      *m_scene = nullptr;

    QVector<Rectangle*> _AllRectangles;

    QVector<BoxLine*>   _BoxLines;

    // Робота з локальною пам'ятю
    //LocalDataManager    *_LocalDataManager = nullptr;

};

#endif // MAINGRAPHICSVIEW_H
