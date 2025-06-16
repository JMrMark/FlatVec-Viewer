#ifndef MAINGRAPHICSVIEW_H
#define MAINGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QMouseEvent>
#include <QDebug>

class MainGraphicsView : public QGraphicsView {
    Q_OBJECT
public:
    using QGraphicsView::QGraphicsView; // Наслідування конструкторів
    explicit MainGraphicsView(QWidget *parent = nullptr);

    bool CurrentGeometry_Set(char cg);

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:

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
