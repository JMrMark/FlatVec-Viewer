#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QString>
#include <QVector>

#include "asm_interface.h"

#include "../geometry/rectangle/curvedrectangle.h"
#include "../geometry/rectangle/invisiblerectangle.h"
#include "../geometry/rectangle/ordinaryrectangle.h"
#include "../geometry/rectangle/slantedrectangle.h"
#include "../geometry/line/boxline.h"

#include "../geometrytype/geometrytype.h"

class FileManager
{
public:
    FileManager(QString filename);

    // Встановлюємо назву файлу з яким працюємо
    void File_SetPath(QString filepath)
    {_FilePath = filepath;}

    QString File_GetPath()
    {return _FilePath;}

    // Створюємо новий файл (Доробити алгортм зберізання даних!)
    bool File_Create(const QString &path, const QString &stddata);

    // Читаємо файл (Не зроблений, тимчасова заглушка!)
    QVector<Rectangle*> File_Read(const QString &path);

    // Зберігаємо файл (перезаписуємо існуючий чи створюємо новий)
    bool File_Save(const QString &path, const QVector<Rectangle*> vecData);

    // Записуємо інформацію у файл
    bool File_Write(QVector<Rectangle*> vecData);

    // Стиснення файлу
    bool File_Compress(QVector<Rectangle*> &vecData);

    // Розтиснення файлу
    bool File_Decompress(QVector<Rectangle*> &vecData);

    // Закодовуємо інформацію
    QString File_Encode(QVector<Rectangle*> &vecData) const;

    // Розкодовуємо інформацію
    QVector<Rectangle*> File_Decode(QString &data) const;

    // Отримуємо ключ користувача для розшифрування
    QString Key_GetFromFile();

private:

    QString _FilePath;

};

#endif // FILEMANAGER_H
