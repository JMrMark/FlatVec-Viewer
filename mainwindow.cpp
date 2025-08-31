#include <QApplication>
#include <QToolBar>
#include <QAction>
#include <QDebug>
#include <QFile>
#include <QMessageBox>

#include "mainwindow.h"
#include "interface/ui_mainwindow.h"
#include "geometrytype/geometrytype.h"
#include "styles/stylemanager.h"

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

    _MainGraphicsView = qobject_cast<MainGraphicsView*>(ui->graphicsView);

    _MainGraphicsView->setCurrentGeometry(GeometryType::None);

    _FileManager = new FileManager(nullptr);

    _TreeController = new TreeController(ui->treeView, this);

    // Upload styles from style.qss for rectangles
    StyleManager::loadStyles(":/styles/style.qss");

    connect(ui->actionCreateNewFile, &QAction::triggered, this, &MainWindow::onCreateNewFile);
    connect(ui->treeView, &QTreeView::clicked, this, &MainWindow::onTreeFileClicked);
    //connect(ui->actionSaveFile, &QAction::triggered, this, &MainWindow::on_actionSaveFile_triggered);
    //connect(ui->actionSaveFileAs, &QAction::triggered, this, &MainWindow::on_actionSaveFileAs_triggered);

    //ui->graphicsView->setScene(new QGraphicsScene(this));

    //ui->pushButton_curved->setObjectName("style1Button");
    //ui->pushButton_invisible->setObjectName("style2Button");
}

MainWindow::~MainWindow()
{
    delete _MainGraphicsView;
    delete _FileManager;
    delete _TreeController;
    delete ui;
}

void MainWindow::onCreateNewFile()
{
    QString filePath = QFileDialog::getSaveFileName(this, "Новий файл", QDir::homePath(), "FlatVec (*.vppc)");

    if (filePath.isEmpty())
        return;

    if (!filePath.endsWith(".vppc"))
        filePath += ".vppc";

    if (!_FileManager->File_Create(filePath, "Артем")) {
        QMessageBox::warning(this, "Помилка", "Файл не створено");
        return;
    }

    _FileManager->File_SetPath(filePath);
    _MainGraphicsView->createNewPaintFile(filePath);
    _TreeController->showFile(filePath);  // показуємо в TreeView
}

void MainWindow::openFile(const QString &filePath) {
    if (filePath.isEmpty() || !QFileInfo::exists(filePath)) return;

    if (_MainGraphicsView->getCurrentFilePath() == "Std_file") {
        if (_MainGraphicsView->getSelectedVector("Std_file").empty()) {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(
                this,
                tr("Незбережені зміни"),
                tr("Ви працюєте на порожньому аркуші. Якщо відкриєте інший файл, усі зміни буде втрачено.\n"
                   "Бажаєте зберегти поточну сцену перед відкриттям нового файлу?"),
                QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel
                );

            if (reply == QMessageBox::Yes) {
                // Користувач хоче зберегти — викликаємо збереження
                on_actionSaveFileAs_triggered();
            } else if (reply == QMessageBox::Cancel) {
                // Користувач передумав відкривати файл — повністю виходимо
                return;
            }
        }
    }

    QVector<Rectangle*> figures = _FileManager->File_Read(filePath);
    if (figures.isEmpty()) return;

    QFileInfo info(filePath);

    if (_MainGraphicsView->insertNewFile(filePath, figures)) {
        _FileManager->File_SetPath(filePath);
        _TreeController->showFile(filePath);
    }
}

void MainWindow::on_action_SaveAll_triggered()
{
    const auto& allFiles = _MainGraphicsView->getAllFiles();

    int successCount = 0;
    int failCount = 0;

    for (auto it = allFiles.constBegin(); it != allFiles.constEnd(); ++it) {
        const QString& filePath = it.key();
        const QVector<Rectangle*>& vec = it.value();

        // Пропускаємо Std_file, бо він не є дійсним шляхом
        if (filePath == "Std_file")
            continue;

        if (_FileManager->File_Save(filePath, vec)) {
            ++successCount;
        } else {
            ++failCount;
        }
    }

    QMessageBox::information(this, tr("Збереження завершено"),
                             tr("Файли збережено успішно: %1\nНе вдалося зберегти: %2").arg(successCount).arg(failCount));
}

void MainWindow::on_actionOpenNewFile_triggered() {
    QString filePath = QFileDialog::getOpenFileName(
        this,
        tr("Open .vppc File"),
        QDir::homePath(),
        tr("Vppc Files (*.vppc)")
        );

    openFile(filePath);
}

void MainWindow::on_actionSaveFile_triggered() {
    if (!_FileManager || _FileManager->File_GetPath().isEmpty()) {
        // Якщо шлях до файлу ще не встановлено — спрацьовує Save As
        on_actionSaveFileAs_triggered();
        return;
    }

    QString filePath = _FileManager->File_GetPath();

    if (_FileManager->File_Save(nullptr, _MainGraphicsView->getSelectedVector(filePath))) {
        QMessageBox::information(this, "Збережено", "Файл успішно збережено.");
    } else {
        QMessageBox::warning(this, "Помилка", "Не вдалося зберегти файл.");
    }
}

void MainWindow::on_actionSaveFileAs_triggered() {
    QString selectedPath = QFileDialog::getSaveFileName(this, "Зберегти файл як", "", "FlatVec Files (*.vppc)");

    if (selectedPath.isEmpty()) return;

    if (_FileManager->File_Save(selectedPath, _MainGraphicsView->getSelectedVector(selectedPath))) {
        QMessageBox::information(this, "Збережено", "Файл успішно збережено як.");
    } else {
        QMessageBox::warning(this, "Помилка", "Не вдалося зберегти файл.");
    }
}

void MainWindow::onTreeFileClicked(const QModelIndex &index) {
    QString path = _TreeController->getModel()->filePath(index);
    if (QFileInfo(path).isFile() && path.endsWith(".vppc", Qt::CaseInsensitive)) {
        openFile(path);
    }
}

// Натиснуто кнопку для побудови горизонтального чи вертикального прямокутника
// Натиснуто кнопку для побудови лінії
void MainWindow::on_pushButton_ordinary_clicked()
{
    if (ui->comboBox->currentIndex() == 0){
        if(_MainGraphicsView->setCurrentGeometry(GeometryType::Ordinary)){
            Button_CurrentPaint(ui->pushButton_ordinary);
        }
    }
    else {
        if(_MainGraphicsView->setCurrentGeometry(GeometryType::Invisible)){
            Button_CurrentPaint(ui->pushButton_ordinary);
        }
    }
}

// Натиснуто кнопку для побудови згинаючого прямокутника
void MainWindow::on_pushButton_curved_clicked()
{
    if(_MainGraphicsView->setCurrentGeometry(GeometryType::Curved)){
        Button_CurrentPaint(ui->pushButton_curved);
    }
}

// Натиснуто кнопку для побудови похилого прямокутника
void MainWindow::on_pushButton_slanted_clicked()
{
    if(_MainGraphicsView->setCurrentGeometry(GeometryType::Slanted)){
        Button_CurrentPaint(ui->pushButton_slanted);
    }
}

// Натиснуто кнопку для побудови невидимого прямокутника
// (Цей прямокутник потрібен для створення пустих просторів у стінах:
// двері, проходи і т.д.)
void MainWindow::on_pushButton_invisible_clicked()
{
    if(_MainGraphicsView->setCurrentGeometry(GeometryType::Invisible)){
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

    _MainGraphicsView->setCurrentGeometry(GeometryType::None);

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


void MainWindow::on_pushButton_addNewFile_clicked()
{
    onCreateNewFile();
}


void MainWindow::on_pushButton_openNewFile_clicked()
{
    on_actionOpenNewFile_triggered();
}

