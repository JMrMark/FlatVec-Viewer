#ifndef TREECONTROLLER_H
#define TREECONTROLLER_H

#pragma once

#include <QObject>
#include <QTreeView>
#include <QFileSystemModel>

class TreeController : public QObject {
    Q_OBJECT

public:
    explicit TreeController(QTreeView *view, QObject *parent = nullptr);

    void showFile(const QString& path);

    QFileSystemModel* getModel() const { return model; }

private:

    QFileSystemModel *model;
    QTreeView *_treeView;

};

#endif // TREECONTROLLER_H
