#include "maingraphicsview.h"

MainGraphicsView::MainGraphicsView(QWidget *parent)
    : QGraphicsView(parent) {
}

void MainGraphicsView::mousePressEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton) {
        QPoint viewPos = event->pos(); // Координати у виді (відносно QGraphicsView)
        QPointF scenePos = mapToScene(viewPos); // Перетворення в координати сцени

        qDebug() << "Натискання миші у координатах view:" << viewPos;
        qDebug() << "Натискання миші у координатах сцени:" << scenePos;
        qDebug() << "CurrentGeometry =" << CurrentGeometry;
    }

    QGraphicsView::mousePressEvent(event); // Не забувай викликати базовий обробник
}

// Оновлюємо значення CurrentGeometry
bool MainGraphicsView::CurrentGeometry_Set(char cg){

    if (CurrentGeometry == cg){
        CurrentGeometry = 'n'; // Вносимо стандартне значення
        return true;
    }
    else if (cg >= 'a' && cg <= 'f' || cg == 'n') {
        CurrentGeometry = cg;
        return true;
    }
    return false;
}
