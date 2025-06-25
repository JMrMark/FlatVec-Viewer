#include <QApplication>
#include <QToolBar>
#include <QAction>
#include <QDebug>
#include <QFile>

#include "mainwindow.h"
#include "interface/ui_mainwindow.h"
#include "geometrytype/geometrytype.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Flat Viewer");

    // Застосовуємо стилі
    QFile styleFile(":/style/style.qss");
    if (styleFile.open(QFile::ReadOnly)) {
        QString style = QLatin1String(styleFile.readAll());
        qApp->setStyleSheet(style);
        styleFile.close();
    }

    ui->graphicsView->setCurrentGeometry(GeometryType::None);
    //ui->graphicsView->setScene(new QGraphicsScene(this));

    //ui->pushButton_curved->setObjectName("style1Button");
    //ui->pushButton_invisible->setObjectName("style2Button");
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Натиснуто кнопку для побудови горизонтального чи вертикального прямокутника
// Натиснуто кнопку для побудови лінії
void MainWindow::on_pushButton_ordinary_clicked()
{
    if (ui->comboBox->currentIndex() == 0){
        if(ui->graphicsView->setCurrentGeometry(GeometryType::Ordinary)){
            Button_CurrentPaint(ui->pushButton_ordinary);
        }
    }
    else {
        if(ui->graphicsView->setCurrentGeometry(GeometryType::Invisible)){
            Button_CurrentPaint(ui->pushButton_ordinary);
        }
    }
}

// Натиснуто кнопку для побудови згинаючого прямокутника
void MainWindow::on_pushButton_curved_clicked()
{
    if(ui->graphicsView->setCurrentGeometry(GeometryType::Curved)){
        Button_CurrentPaint(ui->pushButton_curved);
    }
}

// Натиснуто кнопку для побудови похилого прямокутника
void MainWindow::on_pushButton_slanted_clicked()
{
    if(ui->graphicsView->setCurrentGeometry(GeometryType::Slanted)){
        Button_CurrentPaint(ui->pushButton_slanted);
    }
}

// Натиснуто кнопку для побудови невидимого прямокутника
// (Цей прямокутник потрібен для створення пустих просторів у стінах:
// двері, проходи і т.д.)
void MainWindow::on_pushButton_invisible_clicked()
{
    if(ui->graphicsView->setCurrentGeometry(GeometryType::Invisible)){
        Button_CurrentPaint(ui->pushButton_invisible);
    }
}

// Натиснуто кнопку для запуску проекту
void MainWindow::on_pushButton_start_clicked()
{
    // Реалізація
}

// Стилізація буде зміненна [ПЕРЕРОБИТИ СТИЛЬ]
void MainWindow::Button_CurrentPaint(QPushButton *buttonNext) {
    if (_ButtonNow) {
        _ButtonNow->setStyleSheet("background-color: transparent;");
        if (_ButtonNow == buttonNext){
            return;
        }
    }
    if (buttonNext) {
        buttonNext->setStyleSheet("background-color: #78f5e9;");
        _ButtonNow = buttonNext;
    }
}

// Змінено шар перегляду
// 0 = Шар поверхні
// 1 = Шар вологості
// 2 = Шар температури
void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    if (_ButtonNow){
        _ButtonNow->setStyleSheet("background-color: transparent;");
    }
    _ButtonNow = nullptr;

    ui->graphicsView->setCurrentGeometry(GeometryType::None);

    if (index == 0){
        ui->pushButton_invisible->show();
        ui->pushButton_curved->show();
        ui->pushButton_slanted->show();

        ui->pushButton_curved->setText("Зігнутий");
        ui->pushButton_invisible->setText("Невидимий");
        ui->pushButton_ordinary->setText("Прямокутник");
        ui->pushButton_slanted->setText("Похилий");
    }
    else if (index == 1){
        ui->pushButton_invisible->hide();
        ui->pushButton_curved->hide();
        ui->pushButton_slanted->hide();

        ui->pushButton_ordinary->setText("Лінія");
    }
    else {
        ui->pushButton_invisible->hide();
        ui->pushButton_curved->hide();
        ui->pushButton_slanted->hide();

        ui->pushButton_ordinary->setText("Лінія");
    }
}

// Висота змінена
void MainWindow::on_lineEdit_height_textEdited(const QString &arg1)
{

}

// Ширина змінена
void MainWindow::on_lineEdit_width_textEdited(const QString &arg1)
{

}

