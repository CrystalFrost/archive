// Генератор Диагональных латинских квадратов

# if !defined Generator_h
# define Generator_h

# include <iostream>
# include <fstream>
# include "Square.h"

using namespace std;

class Generator
{
public:
	__event void SquareGenerated(Square generatedSquare);	// Событие генерирования нового диагонального квадрата

	Generator();							// Конструктор по умолчанию
	Generator(Generator& source);			// Конструктор копировния
	void Start();							// Запуск генерации квадратов
	void Reset();							// Сброс всех значений внутренних структур
	void SetFileNames(char* start, char* result, char* checkpoint, char* temp);	// Заданием имен файлов параметров и контрольной точки
	void Initialize(char* start, char* result, char* checkpoint, char* temp);	// Инициализация поиска

	Generator& operator = (Generator&  value);									// Оператор копирования
	friend std::ostream& operator << (std::ostream& os, Generator& value);		// Оператор записи состояние генератора
	friend std::istream& operator >> (std::istream& is, Generator& value);		// Оператор считывания состояния генератора

protected:
	static const int Rank = Square::Rank;	// Ранг квадрата (для удобства)
	static const int Free = 1;				// Флаг свободного для использования значения
	static const int Used = 0;				// Флаг задействования значения в какой-либо клетке диагонали | строки | столбца
	static const int CellsInPath = (Rank - 1)*Rank;	// Число обрабатываемых клеток
	static const int Yes = 1;				// Флаг "Да"
	static const int No = 0;				// Флаг "Нет"

	void CopyState(Generator& source);		// Копирование состояния
	void Read(std::istream& is);			// Считывание состояние генератора из потока
	void Write(std::ostream& os);			// Запись состояния генератора в поток

	Square newSquare;						// Генерируемый квадрат

	int path[CellsInPath][2];				// Путь заполнения матрицы квадрата - path[i][0] - строка на шаге i, path[i][1] - столбец
	int keyRowId;							// Идентификатор строки ключевой клетки - по значению которой расчёт будет останавливаться
	int keyColumnId;						// Идентификатор столбца ключевой клетки
	int keyValue;							// Значение ключевой клетки, по достижению которого расчёт будет останавливаться

	int primary[Rank];						// Содержимое главной диагонали
	int secondary[Rank];					// Содержимое побочной диагонали
	int columns[Rank][Rank];				// Матрица значений, использовавшихся в столбцах - columns[значение][столбец] = 0|1. 0 - значение занято. 1 - свободно.
	int rows[Rank][Rank];					// Матрица значений, использовавшихся в строках - rows[строка][значение] = 0|1
	int cellsHistory[Rank][Rank][Rank];		// Куб значений, которые использовались для формирования построенной части квадрата - cellsHistory[строка][столбец][значение]

	char* startParametersFileName;			// Название файла с параметрами запуска расчёта
	char* resultFileName;					// Название файла с результатами
	char* checkpointFileName;				// Название файла контрольной точки
	char* tempCheckpointFileName;			// Временное название файла новой контрольной точки

	int isInitialized;						// Флаг успешной инициализации поиска
	int squaresCount;						// Число обнаруженных ДЛК
	int rowId;								// Идентификатор строки обратываемой клетки
	int columnId;							// Идентификатор столбца обрабатываемой клетки
	int cellId;								// Идентификатор клетки в перечне шагов обхода квадрата

private:
	void CreateCheckpoint();				// Создание контрольной точки
	void ProcessSquare();					// Обработка найденного квадрата

	char* generatorStateHeader;				// Заголовок, после которого в файле параметров или контрольной точки идёт состояние генератора диагональных квадратов
};

# endif