#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>

#include "widgets/maingraphicsview.h"
#include "interface/ui_mainwindow.h"

#include "geometry/rectangle/curvedrectangle.h"
#include "geometry/rectangle/invisiblerectangle.h"
#include "geometry/rectangle/ordinaryrectangle.h"
#include "geometry/rectangle/slantedrectangle.h"
#include "geometry/line/boxline.h"

#include "datamanagement/localdatamanager.h"



QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void Button_CurrentPaint(QPushButton *buttonNext);

private slots:
    void on_pushButton_ordinary_clicked();

    void on_pushButton_curved_clicked();

    void on_pushButton_slanted_clicked();

    void on_pushButton_invisible_clicked();

    void on_pushButton_start_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void on_lineEdit_height_textEdited(const QString &arg1);

    void on_lineEdit_width_textEdited(const QString &arg1);

private:
    Ui::MainWindow *ui;

    // Не забувати очищати після використання
    CurvedRectangle     *_CurvedRectangle    = nullptr;
    InvisibleRectangle  *_InvisibleRectangle = nullptr;
    OrdinaryRectangle   *_OrdinaryRectangle  = nullptr;
    SlantedRectangle    *_SlantedRectangle   = nullptr;
    BoxLine             *_BoxLine            = nullptr;

    // Робота з локальною пам'ятю
    LocalDataManager    *_LocalDataManager   = nullptr;

    // Вказівник на кнопку, для динамічного задання їх стилю
    QPushButton         *_ButtonNow          = nullptr;
};
#endif // MAINWINDOW_H
