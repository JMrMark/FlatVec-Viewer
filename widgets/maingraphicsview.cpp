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

    createNewPaintFile(currentFilePath);

    currentRectangles().append(new OrdinaryRectangle(100, 100, 100, 50));
    drawFigure(currentRectangles()[0]);

}

void MainGraphicsView::createNewPaintFile(QString file_name){
    QVector<Rectangle*> _AllRectangles;
    _AllFiles.insert(file_name, _AllRectangles);
    currentFilePath = file_name;
}

QVector<Rectangle*>& MainGraphicsView::currentRectangles() {
    return _AllFiles[currentFilePath];
}

MainGraphicsView::~MainGraphicsView(){
    for (auto& scene : _AllFiles) {
        for (auto* rect : scene) {
            m_scene->removeItem(rect);
            delete rect;
        }
        scene.clear();
    }
    _AllFiles.clear();
}

QVector<Rectangle*> MainGraphicsView::getSelectedVector(const QString &filePath){

    if (currentFilePath == "Std_file") {
        if (_AllFiles.contains("Std_file")) {
            // Копіюємо вміст тимчасового файла під новий шлях
            _AllFiles[filePath] = _AllFiles.value("Std_file");
            // Видаляємо старий ключ
            _AllFiles.remove("Std_file");
            currentFilePath = filePath;
        }
    }

    if (_AllFiles.contains(filePath)) {
        return _AllFiles.value(filePath);
    }
    return {};
}

bool MainGraphicsView::insertNewFile(const QString &fileName, const QVector<Rectangle*> vec) {

    if (fileName == nullptr) return false;

    currentFilePath = fileName;
    _AllFiles.insert(fileName, vec);

    m_scene->clear();

    for (auto &rec : vec){
        drawFigure(rec);
    }

    return true;
}

void MainGraphicsView::mousePressEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton) {
        m_posBegin = mapToScene(event->pos());
        if (!(m_currentGeometry == GeometryType::None)){
            m_tracking = true;
            m_tempRectItem = new QGraphicsRectItem();
            m_tempRectItem->setPen(QPen(Qt::blue, 1, Qt::DashLine));
            m_scene->addItem(m_tempRectItem);
            event->accept();
        }
        else {

            if (currentRect != nullptr) {
                currentRect->CurrentState = 0;
                currentRect = nullptr;
            }

            currentRect = includesPointSomeone(m_posBegin);

            if (currentRect == nullptr) {
                qDebug() << "Ззовні, currentRect == nullptr";
            }
            else {
                currentRect->CurrentState = 1;
                qDebug() << "Стан змінено";
            }

            m_scene->update();
        }
    }
    else {
        QGraphicsView::mousePressEvent(event);
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
    }
    else if (m_tracking && m_tempRectItem) {

        if (currentRect != nullptr) {
            currentRect->CurrentState = 0;
            currentRect = nullptr;
            m_scene->update();
        }

        QPointF currentPos = mapToScene(event->pos());

        QRectF rect = QRectF(m_posBegin, currentPos).normalized();
        m_tempRectItem->setRect(rect);

        event->accept();
    }
    else {
        QGraphicsView::mouseMoveEvent(event);
    }
}

void MainGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton && m_panning) {
        m_panning = false;
        setCursor(Qt::ArrowCursor);
        event->accept();
    }
    else if (event->button() == Qt::LeftButton && m_tracking) {
        m_tracking = false;

        QPointF posEnd = mapToScene(event->pos());
        QRectF rect = QRectF(m_posBegin, posEnd).normalized();

        // видаляємо тимчасовий прямокутник
        if (m_tempRectItem) {
            m_scene->removeItem(m_tempRectItem);
            delete m_tempRectItem;
            m_tempRectItem = nullptr;
        }
        mapToScene(event->pos());

        // Тимчасово!
        if (m_currentGeometry == GeometryType::Ordinary){
            currentRectangles().append(createRectangleFromType(rect));
            drawFigure(currentRectangles().last());

            if (currentRect != nullptr) {
                currentRect->CurrentState = 0;
                currentRect = nullptr;
            }

            currentRect = currentRectangles().last();

            if (includesRect(currentRect)){
                currentRect->CurrentState = 3;
            }
            else {
                currentRect->CurrentState = 1;
            }
            m_scene->update();
        }

        event->accept();
    }
    else {
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
bool MainGraphicsView::collidesWithSomeone(const T1* rect) const {
    const auto& rectangles = _AllFiles[currentFilePath];
    for (auto* el : rectangles) {
        if (el != rect && rect->collides(el)) {
            return true;
        }
    }
    return false;
}

Rectangle* MainGraphicsView::includesPointSomeone(const QPointF &pos) const {
    const auto& rectangles = _AllFiles[currentFilePath];
    for (auto* el : rectangles) {
        if (el->includesPoint(pos)) {
            return el;
        }
    }
    return nullptr;
}

bool MainGraphicsView::includesRect(const Rectangle* rec) const{
    const auto& rectangles = _AllFiles[currentFilePath];
    for (auto* el : rectangles) {
        if (el->collides(*rec) && el != rec) {
            return true;
        }
    }
    return false;
}

Rectangle* MainGraphicsView::includesPointRectangle(const QPointF &pos) const {
    const auto& rectangles = _AllFiles[currentFilePath];
    for (auto* el : rectangles) {
        if (el->includesPoint(pos)) {
            return el;
        }
    }
    return nullptr;
}

void MainGraphicsView::drawFigure(QGraphicsItem* item) {
    if (!item) return;

    if (!m_scene->items().contains(item))
        m_scene->addItem(item);
}

bool MainGraphicsView::setCurrentGeometry(GeometryType type)
{
    if (m_currentGeometry == type) {
        m_currentGeometry = GeometryType::None;
        return true;
    }

    m_currentGeometry = type;
    return true;
}

Rectangle* MainGraphicsView::createRectangleFromType(QRectF rect)
{
    switch (m_currentGeometry) {
    case GeometryType::Ordinary:
        return new OrdinaryRectangle(rect.x(), rect.y(), rect.width(), rect.height());
    case GeometryType::Curved:
        //return new CurvedRectangle(rect.x(), rect.y(), rect.width(), rect.height());
    case GeometryType::Slanted:
        //return new SlantedRectangle(rect.x(), rect.y(), rect.width(), rect.height());
    case GeometryType::Invisible:
        //return new InvisibleRectangle(rect.x(), rect.y(), rect.width(), rect.height());
    case GeometryType::LineBox:
        //return new BoxLine(rect.x(), rect.y(), rect.width(), rect.height());
    default:
        return nullptr;
    }
}
