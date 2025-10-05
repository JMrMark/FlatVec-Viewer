#ifndef MAINGRAPHICSVIEW_H
#define MAINGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QMouseEvent>
#include <QDebug>
#include <QVector>

#include <QMap>

#include "../geometry/rectangle/curvedrectangle.h"
#include "../geometry/rectangle/invisiblerectangle.h"
#include "../geometry/rectangle/ordinaryrectangle.h"
#include "../geometry/rectangle/slantedrectangle.h"
#include "../geometry/line/boxline.h"

#include "../geometrytype/geometrytype.h"

class MainGraphicsView : public QGraphicsView {
    Q_OBJECT
public:
    using QGraphicsView::QGraphicsView; // Наслідування конструкторів
    explicit MainGraphicsView(QWidget *parent = nullptr);

    ~MainGraphicsView();

    /* --- Робота з файлами --- */

    // Повернення усіх файлів та їх даних
    QMap<QString, QVector<Rectangle*>> getAllFiles() const
    { return _AllFiles; }

    // Створення нового файлу
    void createNewPaintFile(QString file_name);

    // Повернення розміщення об'єктів на сцені терерішнього файлу
    QVector<Rectangle*>& currentRectangles();

    // Повернення теперішнього файлу
    QString getCurrentFilePath()
    { return currentFilePath; }

    // Ініціалізація нового файлу та його вектора (Не перевірена)
    bool insertNewFile(const QString &fileName, const QVector<Rectangle*> vec);

    // Метод отримує назву файлу та у результаті повертає вектор даних полотна
    QVector<Rectangle*> getSelectedVector(const QString &filePath);

    /* --- Логіка взаємодії фігур --- */

    /* Перевірка чи містить хоч один прямокутник задану точку.
     * Якщо так, то повертає вказівник на цей об'єкт.
     * Якщо ні, то повертає nullptr (обов'язково перевіряйте!). */
    Rectangle*  includesPointSomeone(const QPointF &pos) const;

    /* Перевірка чи перетинаються (налазать) об'єкти з заданим об'єктом.
     * Якщо так, то повертається значення true
     * Якщо ні, то повертається значення false */
    bool        includesRect(const Rectangle* rec) const;

    /* --- Зображення об'єктів на сцені --- */

    // Малює фігуру на сцені
    void drawFigure(QGraphicsItem* item);

    // Видаляє фігуру зі сцени
    void deleteCurrentRect();

    /* --- Оптимізація взаємодії між об'єктами --- */

    // Використовує CurrentRect для заповнення RectanglesInRenderZone.
    // (оновлення RectanglesInRenderZone)
    void updateRectanglesInRenderZone();

    // Функція для пошуку сусідніх прямокутників і їх оновлення
    void updateRectInArea(Rectangle* rec);

    bool setCurrentGeometry(GeometryType type);
    Rectangle* createRectangleFromType(QRectF rect);

    void setNewSizeToRect(float width, float length);

protected:
    // Відслідковування миші
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    // Відследковування миші та масштабування полотна
    void wheelEvent(QWheelEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

    // Задній план сцени
    void drawBackground(QPainter *painter, const QRectF &rect) override;

    // Відслідковування натиснутої кнопки на клавіатурі
    void keyPressEvent(QKeyEvent *event) override;

signals:
    void rectangleSelected(float width, float length);
    void rectangleDeselected();

    //void rectangleError() {m_currentGeometry = GeometryType::None; };

private:

    // Шлях до файлу, який зараз відкрито
    // Std_file - ніякий файл не відкрито
    QString currentFilePath = "Std_file";

    /* --- Створення об'єктів на сцені --- */

    // Обраний тип прямокутника користувачем
    GeometryType m_currentGeometry = GeometryType::None;

    // Тимчасово! (QGraphicsRectItem буде замінено на Rectangle*)
    // Використовується як тимчасовий об'єкт під час створення нового об'єкту
    QGraphicsRectItem* m_tempRectItem = nullptr;

    /* --- Змінні, які потрібні для корегування взаємодії користувача зі сценою з використанням миші --- */
    bool m_panning  = false;
    bool m_tracking = false;
    QPoint  m_lastMousePos;
    QPointF m_posBegin;
    QPointF m_posNow;

    /* --- Оптимізація модифікації, переміщення та взаємодії об'єктів --- */
    // Змінна для моніторингу активного прямокутника (зменшує навантаження на процесор)
    Rectangle*              currentRect         = nullptr;
    Rectangle::Action       currentAction       = nullptr;
    Rectangle::ActionRender currentActionRender = nullptr;
    QVector<Rectangle*>     RectanglesInRenderZone;

    qreal currentScale = 1.0;

    // Вказівник на сцену
    QGraphicsScene      *m_scene = nullptr;

    // Основний компонент! Зберігає у собі назви файлів та об'єкти сцен
    QMap<QString, QVector<Rectangle*>> _AllFiles;

    QVector<BoxLine*>   _BoxLines;

};

#endif // MAINGRAPHICSVIEW_H
