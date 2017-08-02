// Поиск пар Ортогональных Диагональных Латинских Квадратов

# include "GetDiagPairSearch.h"

using namespace std;

// Конструктор по умолчанию - инициализация структур поиска квадратов
GetDiagPairSearch::GetDiagPairSearch() : GetDiagSearch()
{
	InitializePairSearch();
}


// Инициализация структур перед поиском парного квадрата
void GetDiagPairSearch::InitializePairSearch()
{
	InitializeSearch();

	// Обнуление матрицы для записи первого квадрата пары - squareA
	// и матрицы задействованных пар в греко-латинском квадрате
	for (int i = 0; i < rank; i++)
	{
		for (int j = 0; j < rank; j++)
		{
			squareA[i][j] = -1;
			pairsDictionary[i][j] = 1;
		}
	}
}


// Поиск следующего значения, которое можно было бы поставить в клетку (rowId, columndId) искомого парного квадрата
// Возвращает удачность операции:
//		0 - значение найти не удалось, в квадрат записано "-1";
//		1 - значение найдено и записано в квадрат.

int GetDiagPairSearch::GetCell(int rowId, int columnId)
{
	int isGet;			// Флаг нахождения подходящего значения
	int cellValue;		// Найденное значение для записи в клетку (rowId, columnId)
	int oldCellValue;	// Предыдущее значение, стоявшее в этой клетку

	// Поиск нового значения для клетки (rowId, columnId)
	isGet = 0;
	cellValue = -1;
	for (int i = 0; i < rank && !isGet; i++)
	{
		// Проверка i на возможность записи в клетку (rowId; columnId) квадрата-пары
		if (columns[i][columnId] == 1 && rows[rowId][i] == 1 && cellsHistory[rowId][columnId][i] == 1)
		{
			// Значение не занято в столбцах и строках, но надо ещё проверить диагонали
				// Выставляем флаг, который, возможно, будет сброшен диагональной проверкой или проверкой пар
				isGet = 1;

				// Проверяем значение - не попадалось ли оно на диагоналях
					// Проверка первой диагонали
					if(columnId == rowId)
					{
						if (primary[i] != 1)
						{
							isGet = 0;
						}
					}

					// Проверка второй диагонали
					if (rowId == rank - 1 - columnId)
					{
						if (secondary[i] != 1)
						{
							isGet = 0;
						}
					}

				// Проверяем получающуюся пару на её задействование другими ячейками
				// первого квадрата - squareA и проверяемого значения для второго - i
				if (pairsDictionary[squareA[rowId][columnId]][i] == 0)
				{
					isGet = 0;
				}
		}

		// Запоминание значения, найденного в цикле
		if (isGet)
		{
			cellValue = i;
		}
	}

	// Обработка результата поисков
	if (isGet)
	{
		// Обработка найденного нового значения
			// Считывание текущего значения
			oldCellValue = square[rowId][columnId];

			// Запись нового значения
				// Записываем значение в квадрат
				square[rowId][columnId] = cellValue;
				// Отмечаем значение в столбцах
				columns[cellValue][columnId] = 0;
				// Отмечаем значение в строках
				rows[rowId][cellValue] = 0;
				// Отмечаем значение в диагоналях
				if (rowId == columnId)
				{
					primary[cellValue] = 0;
				}
				if (rowId == rank - 1 - columnId)
				{
					secondary[cellValue] = 0;
				}
				// Отмечаем значение в истории значений клетки
				cellsHistory[rowId][columnId][cellValue] = 0;

			// Возвращение предыдущего значения без зачистки истории (так как мы работаем с этой клеткой)
			if (oldCellValue != -1)
			{
				// Возвращаем значение в столбцы
				columns[oldCellValue][columnId] = 1;
				// Возвращаем значение в строки
				rows[rowId][oldCellValue] = 1;
				// Возвращаем значение в диагонали
				if (rowId == columnId)
				{
					primary[oldCellValue] = 1;
				}
				if (rowId == rank - 1 - columnId)
				{
					secondary[oldCellValue] = 1;
				}
			}

			// Отмечаем значение найденной пары в использованных
				// Отмечаем использованние новой пары
				pairsDictionary[squareA[rowId][columnId]][square[rowId][columnId]] = 0;

				// Снимаем отметку с использования предыдущй пары
				if (oldCellValue != -1)
				{
					pairsDictionary[squareA[rowId][columnId]][oldCellValue] = 1;
				}
	}
	else
	{
		// Обработка факта ненахождения нового значения в клетке (rowId; columnId)
			// Возвращаем текущее значение из квадрата в массивы
				// Считываем текущее значение
				cellValue = square[rowId][columnId];

				// Возвращаем считанное значение в служебные массивы
				if (cellValue != -1)
				{
					// Возвращаем значение в столбцы
					columns[cellValue][columnId] = 1;

					// Возвращаем значение в строки
					rows[rowId][cellValue] = 1;

					// Возвращаем значение в диагонали
					if (rowId == columnId)
					{
						primary[cellValue] = 1;
					}
					if (rowId == rank - 1 - columnId)
					{
						secondary[cellValue] = 1;
					}

					// Записываем -1 в квадрат
					square[rowId][columnId] = -1;

					// Зачищаем историю клетки (rowId; columnId)
					for (int i = 0; i < rank; i++)
					{
						cellsHistory[rowId][columnId][i] = 1;
					}

					// Отмечаем освобождения значения в использованных парах
					pairsDictionary[squareA[rowId][columnId]][cellValue] = 1;
				}
	}

	return isGet;
}


// Обработчик события - нахождения первого квадрата пары объектов squareAGenerator
void GetDiagPairSearch::OnFoundTheSquare(int newSquare[rank][rank])
{
	fstream resultFile;		// Поток для I/O в файл с результатами

	// Выполняем подотовку к запуску поиска парного квадрата,
	// Зачищая все переменные и значения
	InitializePairSearch();

	// Запоминаем найденный квадрат
	for (int i = 0; i < rank; i++)
	{
		for (int j = 0; j < rank; j++)
		{
			squareA[i][j] = newSquare[i][j];
		}
	}

	// Обработка нахождения первого квадрата пары
		// Вывод информации в консоль
		cout << "------------------------" << endl;
		cout << "Starting analyze a square: " << endl;
		PrintSquare(squareA);
		cout << "------------------------" << endl;

	// Запускаем поиск пары к нему
	Start();

	// Выводим результат поиска
	if (squaresCount > 0)
	{
		PrintSearchFooter();
	}
}


// Перехватчик обработки найденног латинского квадрата в пару к квадрату A - squareA
void GetDiagPairSearch::ProcessSquare()
{
	// Обрабатываем найденную пару
	ProcessSquaresPair();
}


// Обработка нахождения пары диагональных латинских квадратов
void GetDiagPairSearch::ProcessSquaresPair()
{
	fstream resultFile;		// Поток для I/O в файл с результатами

	// Вывод информации о найденной паре
		// Вывод заголовка
		if (squaresCount == 1)
		{
			// Вывод информации о первом квадрате пары в виде заголовка
			cout << "------------------------" << endl;
			cout << "Detected pair for the square: " << endl;
			cout << "------------------------" << endl;
			PrintSquare(squareA);
			cout << "------------------------" << endl;

			// Вывод информации в файл
			resultFile.open(resultFileName, std::ios_base::app);
			resultFile << "------------------------" << endl;
			resultFile << "Detected pair for the square: " << endl;
			resultFile << "------------------------" << endl;
			SaveSquare(squareA, resultFile);
			resultFile.close();
		}

		// Вывод информации в консоль
		cout << "------------------------" << endl;
		cout << "Pair # " << squaresCount << endl;
		cout << "------------------------" << endl;

		PrintSquare(squareA);
		PrintSquare(square);

		// Вывод информации в файл
		resultFile.open(resultFileName, std::ios_base::app);
		resultFile << "------------------------" << endl;
		resultFile << "Pair # " << squaresCount << endl;
		resultFile << "------------------------" << endl;
		SaveSquare(squareA, resultFile);
		SaveSquare(square, resultFile);
		resultFile.close();

		/* Усечённый вывод
		PrintSquare(square);

		resultFile.open(resultFileName, std::ios_base::app);
		SaveSquare(square, resultFile);
		resultFile.close();*/
}


// Вывод информации об итогах поиска
void GetDiagPairSearch::PrintSearchFooter()
{
	fstream resultFile;

	// Вывод итогов в консоль
	cout << "------------------------" << endl;
	cout << "Total pairs found: " << squaresCount << endl;
	cout << "------------------------" << endl;

	// Вывод итогов в файл
	resultFile.open(resultFileName, std::ios_base::app);
	resultFile << "------------------------" << endl;
	resultFile << "Total pairs found: " << squaresCount << endl;
	resultFile << "------------------------" << endl;
	resultFile.close();
}


// Запуск поиска пар диагональных латинских квадратов
void GetDiagPairSearch::StartPairSearch()
{
	// Подписываемся на событие нахождения ДЛК
	__hook(&GetDiagSearch::FoundTheSquare, &squareAGenerator, &GetDiagPairSearch::OnFoundTheSquare);

	// Запускаем поиск Диагональных Латинских Квадратов
	squareAGenerator.Start();

	// Отписываемся от события нахождения ДЛК
	__unhook(&GetDiagSearch::FoundTheSquare, &squareAGenerator, &GetDiagPairSearch::OnFoundTheSquare);
}


// Поик ортогональных квадратов к заданному квадрату
void GetDiagPairSearch::CheckSeparateSquare(int square[rank][rank])
{
	// Выполняем подотовку к запуску поиска парного квадрата,
	// Зачищая все переменные и значения
	InitializePairSearch();

	// Запоминаем найденный квадрат
	for (int i = 0; i < rank; i++)
	{
		for (int j = 0; j < rank; j++)
		{
			squareA[i][j] = square[i][j];
		}
	}

	// Обработка нахождения первого квадрата пары
		// Вывод информации в консоль
		cout << "------------------------" << endl;
		cout << "Starting analyze a square: " << endl;
		PrintSquare(squareA);
		cout << "------------------------" << endl;

	// Запускаем поиск пары к нему
	Start();

	// Выводим результат поиска
	if (squaresCount > 0)
	{
		PrintSearchFooter();
	}
}