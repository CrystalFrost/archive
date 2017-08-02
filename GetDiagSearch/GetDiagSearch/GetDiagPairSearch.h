// Поиск пар Ортогональных Диагональных Латинских Квадратов

# if !defined GetDiagPairSearch_h
# define GetDiagPairSearch_h

# include <iostream>
# include <fstream>
# include "GetDiagSearch.h"

using namespace std;

// Класс "Искатель пар ортогональных диагональных латинских квадратов"
class GetDiagPairSearch : public GetDiagSearch
{
public:
	GetDiagPairSearch();								// Конструктор по умолчанию
	void StartPairSearch();								// Запуск поиска квадрата-пары
	void CheckSeparateSquare(int square[rank][rank]);	// Поик ортогональных квадратов к заданному квадрату

protected:
	virtual int GetCell(int rowId, int columnId);		// Получение следующего значения для клетки (rowid, columnId) искомого парного квадрата
	void InitializePairSearch();						// Инициализация структур перед поиском парного квадрата
	void OnFoundTheSquare(int newSquare[rank][rank]);	// Обработчик события - нахождения первого квадрата пары объектов squareAGenerator
	virtual void ProcessSquare();						// Перехватчик обработки нахождения первого квадрата
	virtual void ProcessSquaresPair();					// Обработка найденной пары квадратов

	GetDiagSearch squareAGenerator;			// Генератор первых квадратов пары
	int squareA[rank][rank];				// Матрица первого квадрата в паре
	int pairsDictionary[rank][rank];		// Матрица задействования пар в формируемой паре квадратов
private:
	void PrintSearchFooter();				// Вывод информации об итогах поиска
};

# endif