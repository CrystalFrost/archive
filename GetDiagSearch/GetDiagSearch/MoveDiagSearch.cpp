// Поиск пар диагональных латинских квадратов методом перетасовки строк

# include "MoveDiagSearch.h"

// Конструктор по умолчанию
MoveDiagSearch::MoveDiagSearch()
{
	totalProcessedSquaresSmall = 0;
	totalProcessedSquaresLarge = 0;
	resultFileName = "result.txt";

	InitializeMoveSearch();
}


// Инициализация поиска перетасовкой строк
void MoveDiagSearch::InitializeMoveSearch()
{
	// Зачистка матриц квадратов
	for (int i = 0; i < rank; i++)
	{
		for (int j = 0; j < rank; j++)
		{
			squareA[i][j] = -1;
			squareB[i][j] = -1;
			rowsHistory[i][j] = 1;
		}
	}

	// Зачистка вектров использования строк в очередной перестановке
	// и номеров строк, использованных для текущего квадрата
	for (int i = 0; i < rank; i++)
	{
		rowsUsage[i] = 1;
		currentSquareRows[i] = -1;
	}

	// Сбрасываем счётчик найденных в перестановках квадратов
	squaresCount = 0;
}


// Запуск поиска
void MoveDiagSearch::StartMoveSearch()
{
	// Подписываемся на событие нахождения очередного ДЛК
	__hook(&GetDiagSearch::FoundTheSquare, &squareAGenerator, &MoveDiagSearch::OnFoundTheSquare);

	// Запускаем генерацию ДЛК
	squareAGenerator.Start();

	// Отписываемся от события нахождения очередного ДЛК
	__unhook(&GetDiagSearch::FoundTheSquare, &squareAGenerator, &MoveDiagSearch::OnFoundTheSquare);
}


// Обработка нахождения очередного ДЛК
void MoveDiagSearch::OnFoundTheSquare(int newSquare[GetDiagSearch::rank][GetDiagSearch::rank])
{
	// Запуск поиска парных квадратов методом перестановки строк
		// Очистка значений перед началом поиск
		InitializeMoveSearch();

		// Запоминание найденного квадрата
		for (int i = 0; i < rank; i++)
		{
			for (int j = 0; j < rank; j++)
			{
				squareA[i][j] = newSquare[i][j];
			}
		}

		// Собирание статистики по обработанным квадратам
		totalProcessedSquaresSmall++;
		if (totalProcessedSquaresSmall > 0 && totalProcessedSquaresSmall % 1000000000 == 0)
		{
			totalProcessedSquaresLarge++;
			totalProcessedSquaresSmall = 0;
		}

		// Вывод информации о ходе обработки "в целом"
		if ((totalProcessedSquaresSmall - 1) % 100000 == 0)
		{
			cout << "------------------------" << endl;
			cout << "Processed " << totalProcessedSquaresLarge << " milliards and " << totalProcessedSquaresSmall << " squares." << endl;
			cout << "Now processed the square:" << endl;
			cout << endl;
			PrintSquare(squareA);
			cout << "------------------------" << endl;
		}

		// Запуск перетасовки
		MoveRows();
}


// Перестановка строк квадрата
// Первая строка в рамках перестановки не затрагивается, 
// т.к. мы ищем ДЛК в канонической форме
void MoveDiagSearch::MoveRows()
{
	int currentRowId = 1;
	int isRowGet = 0;
	int gettingRowId = -1;
	int oldRowId = -1;

	int diagonalValues[rank];
	int duplicationDetected = 0;

	// Записываем первую строку квадрата A в B с целью поиска нормализованных квадратов
	for (int j = 0; j < rank; j++)
	{
		squareB[0][j] = squareA[0][j];
	}

	// Отмечаем задействование первой строки, т.к. она фиксированная
	rowsUsage[0] = 0;
	rowsHistory[0][0] = 0;
	currentSquareRows[0] = 0;

	while (currentRowId > 0)
	{
		// Подбираем строку из исходного квадрата на позицию currentRowId формируемого квадрата
		isRowGet = 0;
		gettingRowId = -1;
		for (int i = 0; i < rank; i++)
		{
			// Проверяем i-ю строку исходного квадрата
			if (rowsUsage[i] && rowsHistory[currentRowId][i])
			{
				isRowGet = 1;
				gettingRowId = i;

				break;
			}
		}

		// Обрабатываем результат поиска
		if (isRowGet)
		{
			// Обрабатываем нахождение новой строки
				// Заносим в квадрат новую строку
					// Считываем номер строки, которая сейчас стоит в квадрате
					oldRowId = currentSquareRows[currentRowId];
					// Записываем новую строку в квадрат, массив флагов использованных строк, в историю использованных строк и массив текущих строк квадрата
						// Записываем новую строку в квадрат
						for (int j = 0; j < rank; j++)
						{
							squareB[currentRowId][j] = squareA[gettingRowId][j];
						}
						// Отмечаем строку в массие используемых строк
						rowsUsage[gettingRowId] = 0;
						// Отмечаем строку в истории использования строки
						rowsHistory[currentRowId][gettingRowId] = 0;
						// Записываем строку в массив текущих строк квадрата
						currentSquareRows[currentRowId] = gettingRowId;

				// Очищаем для предыдущей строки флаги использования
					// Убираем отметку в массиве используемых строк
					if (oldRowId != -1)
					{
						rowsUsage[oldRowId] = 1;
					}

				// Проверяем диагональность получающейся части квадрата
					// Сбрасываем флаг сигнализирующий и дубликатах на диагоналях
					duplicationDetected = 0;
					// Проверка главной диагонали
						// Сбрасываем флаги использованных строк
						for (int i = 0; i < rank; i++)
						{
							diagonalValues[i] = 1;
						}
						// Проверка значений главной диагонали
						for (int i = 0; i <= currentRowId; i++)
						{
							// Проверка i-го элемента главной диагонали - клетки (i, i)
							if (diagonalValues[squareB[i][i]])
							{
								diagonalValues[squareB[i][i]] = 0;
							}
							else
							{
								duplicationDetected = 1;
								break;
							}
						}
					// Проверка побочной диагонали, если это имеет смысл
					if (!duplicationDetected)
					{
						// Проверка побочной диагонали
							// Сбрасываем флаги использованных строк
							for (int i = 0; i < rank; i++)
							{
								diagonalValues[i] = 1;
							}
							// Проверка значений побочной диагонали начиная с "её хвоста"
							for (int i = 0; i <= currentRowId; i++)
							{
								// Проверка i-го значения побочной диагонали - элемента (i, rank - 1 - i)
								if (diagonalValues[squareB[i][rank - 1 - i]])
								{
									diagonalValues[squareB[i][rank - 1 - i]] = 0;
								}
								else
								{
									duplicationDetected = 1;
									break;
								}
							}
					}

				// Обработка итогов проверки диагональности квадрата
				if (!duplicationDetected)
				{
					// Делаем следующий шаг вперёд в зависимости от текущего положения
					if (currentRowId == rank - 1)
					{
						// Обрабатываем найденный квадрат
						ProcessSquare();
					}
					else
					{
						// Делаем шаг вперёд
						currentRowId++;
					}
				}
		}
		else
		{
			// Обрабатываем ненахождение новой строки - делаем шаг назад, зачищая флаги задействования, 
			// историю использования, перечень текущих строк квадрата и зачищая сам квадрат
				// Считываем номер текущей строки
				oldRowId = currentSquareRows[currentRowId];
				// Зачищаем текущую строку в квадрате
				for (int j = 0; j < rank; j++)
				{
					squareB[currentRowId][j] = -1;
				}
				// Зачищаем текущий состав квадрата
				currentSquareRows[currentRowId] = -1;
				// Зачищаем флаг возможного задействования
				rowsUsage[oldRowId] = 1;
				// Зачищаем историю работы с этой строкой
				for (int i = 0; i < rank; i++)
				{
					rowsHistory[currentRowId][i] = 1;
				}
				// Делаем шаг назад
				currentRowId--;
		}
	}
}


// Обработка найденного квадрата
void MoveDiagSearch::ProcessSquare()
{
	int isDifferent = 0;			// Число отличий в строках от исходного квадрата (для отсева формирования его копии)
	fstream resultFile;				// Поток для I/O в файл с результатами

	// Проверяем его на то, что он копия исходного
	isDifferent = 0;
	
	for (int i = 0; i < rank; i++)
	{
		if (currentSquareRows[i] != i)
		{
			isDifferent = 1;
			break;
		}
	}

	// Обработка найденного квадрата
	if (isDifferent && IsSquareLatin() && IsSquareDiagonal() && IsSquareOrthogonal(squareA, squareB))
	{
		// Запись информации о найденном квадрате

		// Увеличение счётчика квадратов
		squaresCount++;

		// Запоминание базового квадрата
		if (squaresCount == 1)
		{
			for (int i = 0; i < rank; i++)
			{
				for (int j = 0; j < rank; j++)
				{
					orthogonalSquares[squaresCount - 1][i][j] = squareA[i][j];
				}
			}
		}

		// Запоминание квадрата - пары
		for (int i = 0; i < rank; i++)
		{
			for (int j = 0; j < rank; j++)
			{
				orthogonalSquares[squaresCount][i][j] = squareB[i][j];
			}
		}

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

		// Вывод информации о найденной паре
			// Вывод информации в консоль
			cout << "------------------------" << endl;
			cout << "Pair # " << squaresCount << endl;
			cout << "------------------------" << endl;

			PrintSquare(squareA);
			PrintSquare(squareB);

			// Вывод информации в файл
			resultFile.open(resultFileName, std::ios_base::app);
			resultFile << "------------------------" << endl;
			resultFile << "Pair # " << squaresCount << endl;
			resultFile << "------------------------" << endl;
			SaveSquare(squareA, resultFile);
			SaveSquare(squareB, resultFile);

			// Проверка взаимной ортогональности набора квадратов
			for (int i = 0; i < squaresCount; i++)
			{
				for (int j = i + 1; j <= squaresCount; j++)
				{
					if (IsSquareOrthogonal(orthogonalSquares[i], orthogonalSquares[j]))
					{
						cout << "Square " << i << " # " << j << endl;
						resultFile << "Square " << i << " # " << j << endl;
					}
				}
			}

			resultFile.close();
	}
}

// Печать заголовка о нахождении ДЛК из перестановки строк
void MoveDiagSearch::PrintHeader()
{
	cout << "------------------------" << endl;
	cout << "Detected a pair for the square: " << endl;
	cout << "------------------------" << endl;
	for (int i = 0; i < rank; i++)
	{
		for (int j = 0; j < rank; j++)
		{
			cout << squareA[i][j] << " ";
		}
		cout << endl;
	}
	cout << "------------------------" << endl;
}


// Функция вывода квадрата в консоль
void MoveDiagSearch::PrintSquare(int printedSquare[rank][rank])
{
	// Вывод квадрата в консоль
	for (int i = 0; i < rank; i++)
	{
		for (int j = 0; j < rank; j++)
		{
			cout << printedSquare[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}


// Функция записи квадрата в файл
void MoveDiagSearch::SaveSquare(int writedSquare[rank][rank], fstream& resultFile)
{
	// Запись квадрата файловый поток
	for (int i = 0; i < rank; i++)
	{
		for (int j = 0; j < rank; j++)
		{
			resultFile << writedSquare[i][j] << " ";
		}
		resultFile << endl;
	}
	resultFile << endl;
}


// Проверка квадрата на то, что он является латинским
// Примечание: так как квадрат формируется построчно из векторов,
// то проверка по внутри строк - необязательна, но она оставлена
// для того, чтобы функция была универсальнее, а для более быстрого
// отсеивания не латинских квадратов, сначала выполняется проверка
// внутри столбцов
int MoveDiagSearch::IsSquareLatin()
{
	int duplicationDetected = 0;

	// Проверка столбцов квадрата
	for (int columnId = 0; columnId < rank && !duplicationDetected; columnId++)
	{
		// Проверка корректности столбца columnId
		for (int rowId = 0; rowId < rank && !duplicationDetected; rowId++)
		{
			// Проверка на совпадение элемента [rowId; columnId] со всеми остальными элементами столбца columndId
			for (int comparedRowId = rowId + 1; comparedRowId < rank && !duplicationDetected; comparedRowId++)
			{
				if (squareB[comparedRowId][columnId] == squareB[rowId][columnId])
				{
					duplicationDetected = 1;
				}
			}
		}
	}

	// Проверка строк квадрата
	for (int rowId = 0; rowId < rank && !duplicationDetected; rowId++)
	{
		// Проверка корректности строки rowId
		for (int columnId = 0; columnId < rank && !duplicationDetected; columnId++)
		{
			// Проверка на совпадение элемента [rowId; columnId] со всеми остальными элементами строки rowId
			for (int comparedColumnId = columnId + 1; comparedColumnId < rank && !duplicationDetected; comparedColumnId++)
			{
				if (squareB[rowId][columnId] == squareB[rowId][comparedColumnId])
				{
					duplicationDetected = 1;
				}
			}
		}
	}

	return !duplicationDetected;
}


// Проверка квадрата на то, что он является диагональным
int MoveDiagSearch::IsSquareDiagonal()
{
	int duplicationDetected = 0;

	// Проверка первой диагонали - диагонали [0;0] - [rank;rank]
	for (int itemId = 0; itemId < rank && !duplicationDetected; itemId++)
	{
		// Проверка на совпадение элемента [itemId; itemId] со всеми остальными элементами диагонали
		for (int comparedId = itemId + 1; comparedId < rank && !duplicationDetected; comparedId++)
		{
			if (squareB[itemId][itemId] == squareB[comparedId][comparedId])
			{
				duplicationDetected = 1;
			}
		}
	}

	// Проверка второй диагонали - диагонали [rank - itemId - 1; itemId]
	for (int itemId = 0; itemId < rank && !duplicationDetected; itemId ++)
	{
		// Проверка на совпадение элемента [rank - itemId - 1; itemId] со всеми остальными элементами диагонали
		for (int comparedId = itemId + 1; comparedId < rank && !duplicationDetected; comparedId++)
		{
			if (squareB[(rank - itemId - 1)][itemId] == squareB[(rank - comparedId - 1)][comparedId])
			{
				duplicationDetected = 1;
			}
		}
	}

	return !duplicationDetected;
}


// Проверка квадратов squareA и squareB на ортогональность
int MoveDiagSearch::IsSquareOrthogonal(int a[rank][rank], int b[rank][rank])
{
	int isOrthogonal = 1;
	int pair[rank][rank];

	for (int i = 0; i < rank; i++)
	{
		for (int j = 0; j < rank; j++)
		{
			pair[i][j] = 1;
		}
	}

	for (int i = 0; i < rank && isOrthogonal; i++)
	{
		for (int j = 0; j < rank && isOrthogonal; j++)
		{
			if (pair[a[i][j]][b[i][j]])
			{
				pair[a[i][j]][b[i][j]] = 0;
			}
			else
			{
				isOrthogonal = 0;
			}
		}
	}

	return isOrthogonal;
}