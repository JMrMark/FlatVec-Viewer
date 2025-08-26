#include <QFileInfo>
#include <QDir>

#include "treecontroller.h"

TreeController::TreeController(QTreeView *view, QObject *parent)
    : QObject(parent), _treeView(view) {

    model = new QFileSystemModel(this);
    model->setRootPath(QDir::rootPath());

    // Показувати лише один стовпець
    _treeView->setModel(model);
    for (int i = 1; i < model->columnCount(); ++i)
        _treeView->hideColumn(i);

    // Фільтр: тільки папки і .vppc-файли
    model->setFilter(QDir::AllDirs | QDir::Files | QDir::NoDotAndDotDot);
    model->setNameFilters(QStringList() << "*.vppc");
    model->setNameFilterDisables(false); // Показувати лише .vppc, а не все

    _treeView->setHeaderHidden(true); // Не обов’язково, але для мінімалізму
}

void TreeController::showFile(const QString &filePath) {
    QFileInfo info(filePath);
    QString dirPath = info.absoluteDir().absolutePath();

    QModelIndex rootIndex = model->index(dirPath);
    _treeView->setRootIndex(rootIndex);
}
