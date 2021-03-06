#ifndef GDS_H
#define GDS_H
/*!
 * \file
 * \brief Файл, описывающий раздел 2.
 *
 * Данный файл содержит в себе определения раздела описания сетки,
 * содержащегося в формате GRIB 1
 */
#include <reader.h>
/*!
 * \brief Раздел 2. Раздел описания сетки.
 * \details  Данный отдел не является обязательный.
 * Содержание:
 * Длина раздела, геометрия сетки, если необходимо
 */
class GDS: public Reader
{
protected:
    /*!
     * \brief Структура данных, описывающая содержание раздела описания сетки
     */
    struct Data{
        ///< Длина раздела
        long sectionLength;
        int NV;///< Количество параметров вертикальной координаты
        int PV_LV;///< Номер октета перечня параметров верткальной координат
        string DataType;///< Тип представления данных
        int Ni;///< Количество точек вдоль параллель
        int Nj;///< Количество тчоек вдоль меридиана
        int pointQuantity;///< Общее количество точек
        int La1;///<Широта первой точки сетки
        bool southLa1;
        int Lo1;///<Долгота первой точки сетки
        bool westLo1;
        int flags;///< Флаг разрешения и компонентов
        int La2;///< Широта последней точки сетки
        bool southLa2;
        int Lo2;///< Долгота последней точки сетки
        bool westLo2;
        int Di;///<Приращение в направлении i
        int Dj_N;///< Приращение в направлении j
        int scanFlags;///< Режим сканирования
    };
public:
    friend struct uniData;
    GDS(){};
    ~GDS(){};
    /*!
     * Чтение секции
     * \param[in] file Указатель на файл кодировки grb
     * \param[in] d Указатель на структуру Data
     */
    void readSec(ifstream *file, Data* d);
    /*!
     * Определение вида сетки в соответсвии с типом представления данных
     * \param[in] dataType тип представления данных
     */
    string dataType(char dataType);
    /*!
     * Определение широтно-долготной сетки
     * \param[in] file Указатель на файл кодировки grb
     * \param[in] d Указатель на структуру Data
     */
    void typeLanLon(ifstream *file, Data* d);

};

#endif // GDS_H
