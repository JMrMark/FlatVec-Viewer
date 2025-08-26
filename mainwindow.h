#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QFileDialog>

#include "interface/ui_mainwindow.h"
#include "widgets/treecontroller.h"
#include "datamanagement/filemanager.h"

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

    void onCreateNewFile();

    void openFile(const QString &filePath);

private slots:
    void on_pushButton_ordinary_clicked();

    void on_pushButton_curved_clicked();

    void on_pushButton_slanted_clicked();

    void on_pushButton_invisible_clicked();

    void on_pushButton_start_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void on_lineEdit_height_textEdited(const QString &arg1);

    void on_lineEdit_width_textEdited(const QString &arg1);

    void on_pushButton_addNewFile_clicked();

    void on_actionOpenNewFile_triggered();

    void on_actionSaveFile_triggered();

    void on_actionSaveFileAs_triggered();

    void onTreeFileClicked(const QModelIndex &index);

    void on_pushButton_openNewFile_clicked();

    void on_action_SaveAll_triggered();

private:

    Ui::MainWindow *ui;

    // Вказівник на кнопку, для динамічного задання їх стилю
    QPushButton         *_ButtonNow          = nullptr;

    TreeController      *_TreeController     = nullptr;
    FileManager         *_FileManager        = nullptr;

    MainGraphicsView    *_MainGraphicsView   = nullptr;

};
#endif // MAINWINDOW_H
