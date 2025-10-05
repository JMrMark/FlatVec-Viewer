/*
 * Клас потрібен для тимчасових данних прямокутників (він є спільним для всіх)
 * Використувається наступним чином:
 * - ініціалізація відбувається, коли стан прямокутника знінюється на '1'
 * - використання при переміщені чи модифікації об'єкта
 * - видалення відбувається, коли стан прямокутника змінюється з "стану" '1'
 *
 * Містить у собі зони рендеренгу:
 * - Зона рендеренгу
 * - Зона оновлення зони рендеренгу
*/

#ifndef TEMPRECTANGLE_H
#define TEMPRECTANGLE_H

#include <QVector>

class Rectangle;

class TempRectangle
{
public:
    TempRectangle();
    ~TempRectangle();

    // Оновлення вектора _renderZones
    bool        RenderZone_update(const QVector<Rectangle*> &renderZones);

    // Перевірка чи містить вектор елементи
    inline bool RenderZoneIsEmpty() { return _renderZones.empty(); }

    // Геттери для отримання елементів вектора
    Rectangle* getRenderZone();
    Rectangle* getRenderZoneUpdate();

    // Змінні не є у полі private, адже це лиш ускладнить їх використання
    float x = 0;
    float y = 0;
    float width = 0;
    float length = 0;

private:

    // Очищає вектор. (порушує RAII, але є необхідним)
    void deleteRenderZone();

    // [0] = RenderZone
    // [1] = RenderZoneUpdate
    QVector<Rectangle*> _renderZones;

};

#endif // TEMPRECTANGLE_H
