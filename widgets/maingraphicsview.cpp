#include "maingraphicsview.h"
#include <QScrollBar>
#include <QWheelEvent>
//#include <QRect>

MainGraphicsView::MainGraphicsView(QWidget *parent)
    : QGraphicsView(parent) {

    setDragMode(QGraphicsView::NoDrag); // вимикаємо стандартне перетягування
    setMouseTracking(true); // слідкуємо за мишею навіть без натискання

    setTransformationAnchor(QGraphicsView::AnchorUnderMouse); // масштабування під курсором
    setResizeAnchor(QGraphicsView::AnchorUnderMouse);

    // 1. Скидаємо масштаб
    this->resetTransform();

    // 2. Встановлюємо сцену
    m_scene = new QGraphicsScene(this);
    this->setScene(m_scene);

    // 3. Встановлюємо прямокутник сцени — обов’язково з (0,0)
    m_scene->setSceneRect(0, 0, 1000, 1000);

    // 4. Скидаємо прокрутку
    this->horizontalScrollBar()->setValue(0);
    this->verticalScrollBar()->setValue(0);

    _OrdinaryRectangles.append(new OrdinaryRectangle(100, 100, 100, 50));
    drawFigure(_OrdinaryRectangles[0]);

    //_OrdinaryRectangles.append(new OrdinaryRectangle(200, 200, 50, 100));
    //m_scene->addItem(_OrdinaryRectangles.last());

}

MainGraphicsView::~MainGraphicsView(){
    for (auto rect : _OrdinaryRectangles) {
        m_scene->removeItem(rect);
        delete rect;
    }
    _OrdinaryRectangles.clear();
}

void MainGraphicsView::mousePressEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton) {
        QPoint viewPos = event->pos(); // Координати у виді (відносно QGraphicsView)
        QPointF scenePos = mapToScene(viewPos); // Перетворення в координати сцени

        //qDebug() << "Кількість об'єктів у сцені:" << m_scene->items().size();
        //qDebug() << "Натискання миші у координатах view:" << viewPos;
        qDebug() << "Натискання миші у координатах сцени:" << scenePos;
        if (_OrdinaryRectangles[0]->includesPoint(scenePos)){
            qDebug() << "Всередині";
        }
        else {
            qDebug() << "Ззовні";
        }
    }

    if (event->button() == Qt::RightButton) {
        m_panning = true;
        m_lastMousePos = event->pos();
        setCursor(Qt::ClosedHandCursor); // курсор у вигляді "руки"
        event->accept();
    } else {
        QGraphicsView::mousePressEvent(event);
    }

    QGraphicsView::mousePressEvent(event); // Не забувай викликати базовий обробник
}

void MainGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    if (m_panning) {
        QPoint delta = event->pos() - m_lastMousePos;
        m_lastMousePos = event->pos();
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - delta.x());
        verticalScrollBar()->setValue(verticalScrollBar()->value() - delta.y());
        event->accept();
    } else {
        QGraphicsView::mouseMoveEvent(event);
    }
}

void MainGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton && m_panning) {
        m_panning = false;
        setCursor(Qt::ArrowCursor);
        event->accept();
    } else {
        QGraphicsView::mouseReleaseEvent(event);
    }
}

void MainGraphicsView::wheelEvent(QWheelEvent *event) {
    constexpr double scaleStep = 1.15;
    constexpr double minScale = 100.0 / 1000.0;   // Мінімум: видимо 100x100 (якщо сцена 1000x1000)
    constexpr double maxScale = 5000.0 / 1000.0;  // Максимум: видимо 5000x5000

    QPointF oldPos = mapToScene(event->position().toPoint());

    // Обрахунок нового масштабу:
    if (event->angleDelta().y() > 0) {
        if (currentScale * scaleStep <= maxScale) {
            scale(scaleStep, scaleStep);
            currentScale *= scaleStep;
        }
    } else {
        if (currentScale / scaleStep >= minScale) {
            scale(1.0 / scaleStep, 1.0 / scaleStep);
            currentScale /= scaleStep;
        }
    }

    // Зберігаємо точку під курсором
    QPointF newPos = mapToScene(event->position().toPoint());
    QPointF delta = newPos - oldPos;
    translate(delta.x(), delta.y());
    //qDebug() << "Current view area:" << mapToScene(viewport()->rect()).boundingRect();
}

void MainGraphicsView::resizeEvent(QResizeEvent *event) {
    // Запам’ятовуємо позицію сцени в центрі екрана перед resize
    QPointF oldCenter = mapToScene(viewport()->rect().center());

    // Викликаємо стандартну поведінку (щоб змінився viewport)
    QGraphicsView::resizeEvent(event);

    // Центруємо знову ту ж точку після resize
    centerOn(oldCenter);
    //qDebug() << "Current view area:" << mapToScene(viewport()->rect()).boundingRect();
}

template<typename T1>
bool MainGraphicsView::collidesWithSomeone(T1* rect){
    for (auto &el : _CurvedRectangles){
        if (el != rect){
            if (rect->collides(el)){
                return true;
            }
        }
    }
    for (auto &el : _OrdinaryRectangles){
        if (el != rect){
            if (rect->collides(el)){
                return true;
            }
        }
    }
    for (auto &el : _SlantedRectangles){
        if (el != rect){
            if (rect->collides(el)){
                return true;
            }
        }
    }
    return false;
}

template<typename T>
void MainGraphicsView::drawFigure(T* rect){
    if (!rect) return;

    m_scene->addItem(rect);
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
