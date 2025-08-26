#include "filemanager.h"

#include <QDebug>
#include <QFile>
#include <QDateTime>
#include <QFileInfo>

FileManager::FileManager(QString filepath)
    : _FilePath(filepath) {}

bool FileManager::File_Create(const QString &path, const QString &stddata) {

    File_SetPath(path);

    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    QTextStream out(&file);

    QDateTime now = QDateTime::currentDateTime();
    out << "Author: " << stddata << "\n";
    out << "Created: " << now.toString(Qt::ISODate) << "\n";
    out << "Version: 1.0\n";
    out << "---DATA---\n";

    // Можна буде ще додати структуру чи пустий шаблон (треба подумати!)

    file.close();
    return true;
}

QVector<Rectangle*> FileManager::File_Read(const QString &path) {

    File_SetPath(path);

    QVector<Rectangle*> vec;

    vec.append(new OrdinaryRectangle(150, 150, 100, 250));
    vec.append(new OrdinaryRectangle(10, 10, 10, 20));

    return vec;
}

bool FileManager::File_Save(const QString &path, const QVector<Rectangle*> vecData) {
    if (!path.isNull() && !path.isEmpty()) {
        File_SetPath(path);
    }

    return File_Write(vecData);
}


bool FileManager::File_Write(QVector<Rectangle*> vecData){

    if (_FilePath == nullptr){
        qDebug() << "No file name found";
        return false;
    }

    QFile file(_FilePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    QTextStream out(&file);

    //QDateTime now = QDateTime::currentDateTime();
    //out << "Author: " << stddata << "\n";
    //out << "Created: " << now.toString(Qt::ISODate) << "\n";
    //out << "Version: 1.0\n";
    //out << "---DATA---\n";

    if (vecData.empty()){
        qDebug() << "No data found (Vector is empty)";
        file.close();
        return true;
    }

    //char output[256] = {};
    //compress_bytes("input", strlen("input"), output);

    // Метод File_Compress(vecData) - має викликати файл compress.asm
    //if (!File_Compress(vecData)) return false;

    // Метод File_Encode(vecData) - має виклика файл encode.asm
    //QString data = File_Encode(vecData);

    //if (data == "") return false;

    // Запис наступної інформації у файл:
    // Назва комп'ютера, час, дата, геолокація (якщо можливо),
    // 8 біт (10XXX10X) - це для ускладнення дешефрування для зловмисників та для розуміння де починається інформація
    // data
    // 8 біт (01XX11X0) - це для ускладнення дешефрування для зловмисників та для перевірки правильності отриманої інформації

    file.close();
    return true;
}

bool FileManager::File_Compress(QVector<Rectangle*> &vecData){
    return true;
}

QString FileManager::File_Encode(QVector<Rectangle*> &vecData) const{
    return "";
}
