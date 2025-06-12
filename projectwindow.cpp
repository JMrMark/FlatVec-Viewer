#include "projectwindow.h"
#include "interface/ui_projectwindow.h"

ProjectWindow::ProjectWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ProjectWindow)
{
    ui->setupUi(this);
}

ProjectWindow::~ProjectWindow()
{
    delete ui;
}
