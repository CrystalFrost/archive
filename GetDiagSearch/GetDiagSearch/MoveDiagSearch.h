// Поиск пар диагональных латинских квадратов методом перетасовки строк

# if !defined MoveDiagSearch_h
# define MoveDiagSearch_h

# include <iostream>
# include "GetDiagSearch.h"

using namespace std;

class MoveDiagSearch
{
public:
	static const int rank = GetDiagSearch::rank;

	MoveDiagSearch();
	void InitializeMoveSearch();
	void StartMoveSearch();
	void OnFoundTheSquare(int newSquare[GetDiagSearch::rank][GetDiagSearch::rank]);
private:
	void MoveRows();
	int IsSquareLatin();
	int IsSquareDiagonal();
	int IsSquareOrthogonal(int a[rank][rank], int b[rank][rank]);

	void PrintHeader();
	void PrintSquare(int printedSquare[rank][rank]);
	void SaveSquare(int writedSquare[rank][rank], fstream& resultFile);
	void ProcessSquare();

	GetDiagSearch squareAGenerator;
	int squareA[rank][rank];		// Исходный ДЛК, строки в котором будут переставляться
	int squareB[rank][rank];		// Подбираемый ДЛК, строки внутри которого и переставляются
	int rowsUsage[rank];			// Массив флагов возможно задействования строк на данный момент - rowsUsage[номер строки] = 0 | 1, где 0 - строка уже используется, 1 - нет.
	int rowsHistory[rank][rank];	// Массив истории возможного использования строк - rowsHistory[номер строки][значение] = 0 | 1, 
									// где 0 - строка с номером "значение" использовалась для строки "номер строки" формируемого квадрата. 1 - строку ещё можно использовать
	int currentSquareRows[rank];	// Массив с перечнем текущих строк, использованных в квадрате. На i-й позиции - номер используемой строки
	int squaresCount;				// Число обнаруженных диагональных квадратов в перестановках строк из найдеенного squareA
	int totalProcessedSquaresSmall;	// Число обработанных ДЛК, поступивших от генератора в пределах до миллиарда
	int totalProcessedSquaresLarge;	// Число обработанных ДЛК, поступивших от генератора - в миллиардах

	char* resultFileName;						// Имя файла с результатами
	int orthogonalSquares[rank - 1][rank][rank];// Массив для записи группы ортогональных квадратов
};

# endif