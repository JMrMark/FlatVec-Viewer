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

    // Ініціалізація нового файлу та його вектора (Не перевірена)
    bool insertNewFile(const QString &fileName, const QVector<Rectangle*> vec);

    // Метод отримує назву файлу та у результаті повертає вектор даних полотна
    QVector<Rectangle*> getSelectedVector(const QString &filePath);

    // Малює фігуру
    void drawFigure(QGraphicsItem* item);

    Rectangle* includesPointSomeone(const QPointF &pos) const;
    bool includesRect(const Rectangle* rec) const;

    // Повертає вказівник на фігуру (Завжди перевіряти чи не рівний nullptr!!!)
    Rectangle* includesPointRectangle(const QPointF &pos) const;

    bool setCurrentGeometry(GeometryType type);
    Rectangle* createRectangleFromType(QRectF rect);

    void createNewPaintFile(QString file_name);
    QVector<Rectangle*>& currentRectangles();

    QString getCurrentFilePath()
    { return currentFilePath; }

    QMap<QString, QVector<Rectangle*>> getAllFiles() const
    { return _AllFiles; }

    void deleteCurrentRect();

    void setNewSizeToRect(float width, float length);

    // Функція для пошуку сусідніх прямокутників і їх оновлення
    void updateRectInArea(Rectangle* rec);

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

    QString currentFilePath = "Std_file";

    GeometryType m_currentGeometry = GeometryType::None;
    QGraphicsRectItem* m_tempRectItem = nullptr;

    bool m_panning = false;
    QPoint m_lastMousePos;

    bool m_tracking = false;
    QPointF m_posBegin;
    QPointF m_posNow;

    // Змінна для моніторингу активного прямокутника (зменшує навантаження на процесор)
    Rectangle*              currentRect         = nullptr;
    Rectangle::Action       currentAction       = nullptr;
    Rectangle::ActionRender currentActionRender = nullptr;

    // just now for test
    QVector<float> now;

    qreal currentScale = 1.0;

    QGraphicsScene      *m_scene = nullptr;

    QMap<QString, QVector<Rectangle*>> _AllFiles;

    QVector<BoxLine*>   _BoxLines;

};

#endif // MAINGRAPHICSVIEW_H
