#include <QApplication>
#include <QToolBar>
#include <QAction>

#include "mainwindow.h"
#include "interface/ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Flat Viewer");

}

MainWindow::~MainWindow()
{
    delete ui;
}
