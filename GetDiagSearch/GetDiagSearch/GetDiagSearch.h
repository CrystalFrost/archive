// Поиск диагональных латинских квадратов методом Получения возможных значений клеток с предварительным подбором диагоналей

# if !defined GetDiagSearch_h
# define GetDiagSearch_h

# include <iostream>
# include <fstream>

using namespace std;

// Класс "Искатель диагональных латинских квадратов"
class GetDiagSearch
{
public:
	GetDiagSearch();						// Конструктор по умолчанию
	void Start();							// Запуск поиска

	static const int rank = 9;				// Ранг искомых квадратов

	__event void FoundTheSquare(int newSquare[rank][rank]);	// Событие нахождения нового диагонального квадрата
protected:
	void static StepForwardSDiag(int* sDiagIndex);			// Сделать шаг вперёд вдоль побочной диагонали
	void static StepBackwardSDiag(int* sDiagIndex);			// Сделать шаг назад вдоль побочной диагонали
	void static StepForward(int* rowId, int* columnId);		// Сделать шаг вперёд вдоль строки
	void static StepBackward(int* rowId, int* columnId);	// Сделать шаг назад вдоль строки

	virtual int GetCell(int rowId, int columnId);			// Получение следующего значения для клетки (rowid, columnId)
	void InitializeSearch();								// Функция инициализации структур перед поиском квадрата
	virtual void ProcessSquare();							// Функция обработки найденного квадрата
	void PrintSquare(int printedSquare[rank][rank]);		// Функция вывода квадрата в консоль
	void SaveSquare(int printedSquare[rank][rank], fstream& resultFile);	// Функция записи квадрата в файл

	int square[rank][rank];					// Матрица квадрата
	int squaresCount;						// Число построенных квадратов
	char* resultFileName;					// Имя файла с результатами

	int primary[rank];						// Содержимое главной диагонали
	int secondary[rank];					// Содержимое побочной диагонали
	int columns[rank][rank];				// Матрица значений, использовавшихся в столбцах - columns[значение][столбец] = 0|1. 0 - значение занято. 1 - свободно.
	int rows[rank][rank];					// Матрица значений, использовавшихся в строках - rows[строка][значение] = 0|1
	int cellsHistory[rank][rank][rank];		// Куб значений, которые использовались для формирования построенной части квадрата - cellsHistory[строка][столбец][значение]

	int pDiagIndex;							// Номер обрабатываемой клетки на главной диагонали
	int sDiagIndex;							// Номер обрабатываемой клетки на побочной диагонали
	int rowId;								// Номер строки с обрабатываемым элементом rowId = 0 .. rank - 1
	int columnId;							// Номер столбца с обрабатываемым элементом columnId = 0 .. rank - 1
private:
};

# endif