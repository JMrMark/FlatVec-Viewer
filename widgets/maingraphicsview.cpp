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
    if (currentRect != nullptr) {
        delete currentRect;
    }

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

void MainGraphicsView::deleteCurrentRect() {
    if (!currentRect) {
        qDebug() << "Немає вибраного об'єкта для видалення";
        return;
    }

    // 1. Прибрати зі сцени
    m_scene->removeItem(currentRect);

    // 2. Видалити з QMap по ключу (currentFilePath)
    if (_AllFiles.contains(currentFilePath)) {
        QVector<Rectangle*>& vec = _AllFiles[currentFilePath];
        auto it = std::find(vec.begin(), vec.end(), currentRect);
        if (it != vec.end()) {
            vec.erase(it);
            qDebug() << "Об'єкт видалено з _AllFiles[" << currentFilePath << "]";
        } else {
            qDebug() << "Об'єкт не знайдено у _AllFiles[" << currentFilePath << "]";
        }
    } else {
        qDebug() << "Ключ" << currentFilePath << "не знайдено у _AllFiles";
    }

    // 3. Видалити сам об’єкт
    delete currentRect;
    currentRect = nullptr;

    qDebug() << "Об'єкт успішно видалено";
}

void MainGraphicsView::setNewSizeToRect(float width, float length){
    if (currentRect != nullptr){
        currentRect->setSize(width, length);
    }
}

void MainGraphicsView::updateRectInArea(Rectangle* rec){
    const auto& rectangles = _AllFiles[currentFilePath];
    for (auto* el : rectangles) {
        if (el->collides(*rec, 8) && el != rec) {
            el->update();
        }
    }
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
            if (currentRect == nullptr) {
                currentRect = includesPointSomeone(m_posBegin);
                if (currentRect != nullptr) {
                    currentRect->setActivatedOn();
                    emit rectangleSelected(currentRect->getWidth(), currentRect->getLength());
                    qDebug() << "Стан змінено";
                }
            }
            else {
                if (currentRect->includesPoint(m_posBegin, 8.0)){
                    auto handle = currentRect->hitHandle(m_posBegin);
                    currentAction = currentRect->createAction(handle);
                    currentActionRender = currentRect->RenderZone(handle);
                }
                else {
                    currentRect->setActivatedOff();
                    currentRect = nullptr;

                    currentRect = includesPointSomeone(m_posBegin);

                    if (currentRect != nullptr) {
                        currentRect->setActivatedOn();
                        emit rectangleSelected(currentRect->getWidth(), currentRect->getLength());
                        qDebug() << "Стан змінено";
                    }
                    else {
                        emit rectangleDeselected();
                    }
                }
            }
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
    if (currentRect != nullptr) {
        QPointF scenePos = mapToScene(event->pos());
        if (currentRect->CurrentState == 1){
            auto handle = currentRect->hitHandle(scenePos, 8.0); // радіус 8px
            switch (handle) {
            case Rectangle::TopMiddle:
            case Rectangle::BottomMiddle:
                setCursor(Qt::SizeVerCursor); // вертикальна ↕
                break;
            case Rectangle::MiddleLeft:
            case Rectangle::MiddleRight:
                setCursor(Qt::SizeHorCursor); // горизонтальна ↔
                break;
            case Rectangle::TopLeft:
            case Rectangle::BottomRight:
                setCursor(Qt::SizeFDiagCursor); // діагональна ↘↖
                break;
            case Rectangle::TopRight:
            case Rectangle::BottomLeft:
                setCursor(Qt::SizeBDiagCursor); // діагональна ↙↗
                break;
            case Rectangle::Center:
                setCursor(Qt::SizeAllCursor); // хрестик
                break;
            default:
                setCursor(Qt::ArrowCursor); // стандартний курсор
                break;
            }
        }
        else {
            if (currentRect->CurrentState == 3 &&
                currentRect->includesPoint(scenePos))
            {
                setCursor(Qt::SizeAllCursor);
            } else {
                setCursor(Qt::ArrowCursor);
            }
        }
    } else {
        setCursor(Qt::ArrowCursor);
    }

    if (m_panning) {
        QPoint delta = event->pos() - m_lastMousePos;
        m_lastMousePos = event->pos();
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - delta.x());
        verticalScrollBar()->setValue(verticalScrollBar()->value() - delta.y());
        event->accept();
    }
    else if (m_tracking && m_tempRectItem) {

        if (currentRect != nullptr) {
            currentRect->setActivatedOff();
            currentRect = nullptr;
        }

        QPointF currentPos = mapToScene(event->pos());

        QRectF rect = QRectF(m_posBegin, currentPos).normalized();
        m_tempRectItem->setRect(rect);

        event->accept();
    }
    else if (currentAction){
        QPointF currentPos = mapToScene(event->pos());
        currentAction(currentPos);

        // Логіка оптимізації пошуку сусідніх об'єктів буде тут
        // Перевірка чи кеш пустий, чи ми вийшли за область рендеренгу
        // currentActionRender() - поверне область пошуку
        // (область пошуку).collides() - отримуємо всі об'єкти в зоні
        // Кешування цих об'єктів

        // Взаємодія з кешом


        // Тимчасово
        // includesRect(currentRect) - буде заміна на перевідку про колайд з об'єктами в кеші
        if (!includesRect(currentRect)) {
            currentRect->update();
        }
        else {
            currentRect->refuseAction();
        }

        emit rectangleSelected(currentRect->getWidth(), currentRect->getLength());
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
    else if (event->button() == Qt::LeftButton) {
        if (m_tracking){
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
                currentRect->normalizeRect();

                if (includesRect(currentRect)){
                    currentRect->setErrorOn();
                }
                else {
                    currentRect->setActivatedOn();
                }
                emit rectangleSelected(currentRect->getWidth(), currentRect->getLength());
            }

            event->accept();
        }
        if (currentAction != nullptr){
            currentAction = nullptr;
            if (includesRect(currentRect)){
                currentRect->setErrorOn();
            }
            emit rectangleSelected(currentRect->getWidth(), currentRect->getLength());
        }
    }
    else {
        QGraphicsView::mouseReleaseEvent(event);
    }
}

void MainGraphicsView::wheelEvent(QWheelEvent* event) {
    const double zoomInFactor = 1.15;
    const double zoomOutFactor = 1.0 / zoomInFactor;
    const double maxScale = 4.0;

    // Поточний масштаб
    double currentScale = transform().m11();
    double factor = (event->angleDelta().y() > 0) ? zoomInFactor : zoomOutFactor;
    double newScale = currentScale * factor;

    // Обмеження масштабу
    QRectF sceneBounds = sceneRect();

    // Мінімальний масштаб: коли viewBounds ≈ sceneBounds
    double minScaleX = viewport()->width() / sceneBounds.width();
    double minScaleY = viewport()->height() / sceneBounds.height();
    double minScale = std::min(minScaleX, minScaleY);

    if (newScale < minScale) {
        //qDebug() << "Досягли мінімального масштабу";
        // Центруємо сцену
        centerOn(sceneBounds.center());
        return;
    }

    if (newScale > maxScale) {
        //qDebug() << "Досягли максимального масштабу";
        return;
    }

    // Масштабуємо відносно курсора
    scale(factor, factor);

    // Після трансформації
    QRectF viewBoundsAfter = mapToScene(viewport()->rect()).boundingRect();

    // Якщо viewBounds > sceneBounds → центруємо
    if (viewBoundsAfter.width() > sceneBounds.width() ||
        viewBoundsAfter.height() > sceneBounds.height()) {
        //qDebug() << "View більше за сцену — центруємо";
        centerOn(sceneBounds.center());
    } else {
        // Притискаємо до меж
        qreal dx = 0, dy = 0;
        if (viewBoundsAfter.left() < sceneBounds.left())
            dx = sceneBounds.left() - viewBoundsAfter.left();
        if (viewBoundsAfter.top() < sceneBounds.top())
            dy = sceneBounds.top() - viewBoundsAfter.top();
        if (viewBoundsAfter.right() > sceneBounds.right())
            dx = sceneBounds.right() - viewBoundsAfter.right();
        if (viewBoundsAfter.bottom() > sceneBounds.bottom())
            dy = sceneBounds.bottom() - viewBoundsAfter.bottom();

        if (dx != 0 || dy != 0) {
            //qDebug() << "Притискаємо камеру: dx=" << dx << " dy=" << dy;
            translate(dx, dy);
        }
    }

    // Вивід для відладки
    /*qDebug() << "----- Wheel Event -----";
    qDebug() << "Cursor Scene Pos:" << scenePos;
    qDebug() << "Scale:" << transform().m11();
    qDebug() << "Scene bounds:" << sceneBounds;
    qDebug() << "View bounds after zoom:" << viewBoundsAfter;
    qDebug() << "-----------------------";*/
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

void MainGraphicsView::drawBackground(QPainter *painter, const QRectF &rect) {
    // Колір та стиль сітки
    QColor gridColor(200, 200, 200, 125); // світло-сірий напівпрозорий
    QPen pen(gridColor);
    pen.setWidth(0); // 0 = найтонша лінія незалежно від масштабу
    painter->setPen(pen);

    // Крок сітки (наприклад 20 пікселів)
    constexpr qreal gridStep = 100;

    // Межі видимої області
    qreal left   = std::floor(rect.left()   / gridStep) * gridStep;
    qreal top    = std::floor(rect.top()    / gridStep) * gridStep;
    qreal right  = std::ceil (rect.right()  / gridStep) * gridStep;
    qreal bottom = std::ceil (rect.bottom() / gridStep) * gridStep;

    // Вертикальні лінії
    for (qreal x = left; x <= right; x += gridStep) {
        painter->drawLine(QLineF(x, top, x, bottom));
    }

    // Горизонтальні лінії
    for (qreal y = top; y <= bottom; y += gridStep) {
        painter->drawLine(QLineF(left, y, right, y));
    }
}

void MainGraphicsView::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Delete) {
        deleteCurrentRect();
    } else {
        QGraphicsView::keyPressEvent(event);
    }
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
        if (el->collides(*rec)) {
            if (el == rec) continue;
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
