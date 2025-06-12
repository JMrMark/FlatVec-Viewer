#include "camerawindow.h"
#include "interface/ui_camerawindow.h"

CameraWindow::CameraWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CameraWindow)
{
    ui->setupUi(this);
}

CameraWindow::~CameraWindow()
{
    delete ui;
}
