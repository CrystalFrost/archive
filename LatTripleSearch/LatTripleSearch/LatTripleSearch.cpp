// Поиск триплетов латинских квадратов

# include <iostream>
# include <fstream>

using namespace std;

int main()
{
	const int rank = 10;
	const int height = 3;

	int columns[height][rank][rank];
	int rows[height][rank][rank];
	int square[height][rank][rank];
	int cellsHistory[height][rank][rank][rank];
	int pairsABDictionary[rank][rank];
	int pairsACDictionary[rank][rank];
	int pairsBCDictionary[rank][rank];

	int rowId;
	int columnId;
	int layerId;
	int isGet;
	int cellValue;
	int oldCellValue;

	fstream resultFile;
	char* resultFileName = "result.txt";

	int stop = 0;
	int squaresCount = 0;

	// Заполняем массивы учёта использованных значений разрешительными метками
	for (int h = 0; h < height; h++)
	{
		for (int i = 0; i < rank; i++)
		{
			for (int j = 0; j < rank; j++)
			{
				columns[h][i][j] = 1;
				rows[h][i][j] = 1;
				square[h][i][j] = -1;
			}
		}
	}

	// Очищаем массив истории использования значений в ячейках
	for (int h = 0; h < height; h++)
	{
		for (int i = 0; i < rank; i++)
		{
			for (int j = 0; j < rank; j++)
			{
				for (int k = 0; k < rank; k++)
				{
					cellsHistory[h][i][j][k] = 1;
				}
			}
		}
	}

	// Очищаем массивы использованных комбинаций пар элементов квадратов
	for (int i = 0; i < rank; i++)
	{
		for (int j = 0; j < rank; j++)
		{
			pairsABDictionary[i][j] = 1;
			pairsACDictionary[i][j] = 1;
			pairsBCDictionary[i][j] = 1;
		}
	}

	rowId = 0;
	columnId = 0;
	layerId = 0;

	while (!stop)
	{
		// Ищем значение для ячейки (layerId; rowId; columnId)
		isGet = 0;
		cellValue = -1;

		for (int i = 0; i < rank; i++)
		{
			// Проверяем возможность взятия значения i в ячейку (rowId, columnId) квадрата уровня layerId
			if (columns[layerId][i][columnId] == 1 && rows[layerId][rowId][i] == 1 && cellsHistory[layerId][rowId][columnId][i] == 1)
			{
				// Обработка найденного значения
					// Обработка значения для квадрата первого слоя
					if (layerId == 0)
					{
						cellValue = i;
						isGet = 1;
					
						break;
					}
					// Обработка значения для квадрата второго слоя
					if (layerId == 1)
					{
						// Проверка получающейся пары A || B на совместимость с другими парами этих квадратов
						if (pairsABDictionary[square[0][rowId][columnId]][i] == 1)
						{
							cellValue = i;
							isGet = 1;

							break;
						}
					}

					if (layerId == 2)
					{
						if (pairsACDictionary[square[0][rowId][columnId]][i] == 1 && pairsBCDictionary[square[1][rowId][columnId]][i] == 1)
						{
							cellValue = i;
							isGet = 1;

							break;
						}
					}

			}
		}

		// Обработка результата поисков
		if (isGet)
		{
			// Обработка найденного нового значения
				// Считывание текущего значения
				oldCellValue = square[layerId][rowId][columnId];

				// Запись нового значения
					// Записываем значение в квадрат
					square[layerId][rowId][columnId] = cellValue;
					// Отмечаем значение в столбцах
					columns[layerId][cellValue][columnId] = 0;
					// Отмечаем значение в строках
					rows[layerId][rowId][cellValue] = 0;
					// Отмечаем значение в истории значений ячейки
					cellsHistory[layerId][rowId][columnId][cellValue] = 0;

				// Возвращение предыдущего значения без зачистки истории (так как мы работаем с этой ячейкой)
				if (oldCellValue != -1)
				{
					// Возвращаем значение в столбцы
					columns[layerId][oldCellValue][columnId] = 1;
					// Возвращаем значение в строки
					rows[layerId][rowId][oldCellValue] = 1;
				}

			// Отмечаем найденные пары в использованных
				// Отмечаем найденную пару A || B
				if (layerId == 1)
				{
					pairsABDictionary[square[0][rowId][columnId]][square[1][rowId][columnId]] = 0;
				}
				// Отмечаем найденные пары A || C и B || C
				if (layerId == 2)
				{
					pairsACDictionary[square[0][rowId][columnId]][square[2][rowId][columnId]] = 0;
					pairsBCDictionary[square[1][rowId][columnId]][square[2][rowId][columnId]] = 0;
				}

				// Возвращаем отметки об использовании пар из-за смены значений
				if (oldCellValue != -1)
				{
					// "Возвращаем" пару A || B
					if (layerId == 1)
					{
						pairsABDictionary[square[0][rowId][columnId]][oldCellValue] = 1;
					}
					
					// "Возвращаем" пары A || C и B || C
					if (layerId == 2)
					{
						pairsACDictionary[square[0][rowId][columnId]][oldCellValue] = 1;
						pairsBCDictionary[square[1][rowId][columnId]][oldCellValue] = 1;
					}
				}

			// Переход к следующей ячейке, если не достигли конца квадрата
			if (rowId == rank - 1 && columnId == rank - 1 && layerId == height - 1)
			{
				// Обработка достижения окончания квадрата
				squaresCount++;
				
					// Блок определения свойств квадратов
					int squareLatin[height];
					int squareDiagonal[height];

					for (int h = 0; h < height; h++)
					{
						int isSquareLatin = 0;
						int isSquareDiagonal = 0;

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
										if (square[h][comparedRowId][columnId] == square[h][rowId][columnId])
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
										if (square[h][rowId][columnId] == square[h][rowId][comparedColumnId])
										{
											duplicationDetected = 1;
										}
									}
								}
							}

							isSquareLatin = !duplicationDetected;
						}

						{
							int duplicationDetected = 0;

							// Проверка первой диагонали - диагонали [0;0] - [rank;rank]
							for (int itemId = 0; itemId < rank && !duplicationDetected; itemId++)
							{
								// Проверка на совпадение элемента [itemId; itemId] со всеми остальными элементами диагонали
								for (int comparedId = itemId + 1; comparedId < rank && !duplicationDetected; comparedId++)
								{
									if (square[h][itemId][itemId] == square[h][comparedId][comparedId])
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
									if (square[h][(rank - itemId - 1)][itemId] == square[h][(rank - comparedId - 1)][comparedId])
									{
										duplicationDetected = 1;
									}
								}
							}

							isSquareDiagonal = !duplicationDetected;
						}

						squareLatin[h] = isSquareLatin;
						squareDiagonal[h] = isSquareDiagonal;
					}

				resultFile.open(resultFileName, std::ios_base::app);

				resultFile << "------------------------" << endl;
				resultFile << "Triple # " << squaresCount << ":" << endl;
				resultFile << "Squares {A; B; C}: {"
						<< (squareDiagonal[0] == 1 ? "Diagonal" : (squareLatin[0] == 1 ? "Latin" : "???" )) << "; "
						<< (squareDiagonal[1] == 1 ? "Diagonal" : (squareLatin[1] == 1 ? "Latin" : "???" )) << "; "
						<< (squareDiagonal[2] == 1 ? "Diagonal" : (squareLatin[2] == 1 ? "Latin" : "???" )) << "}" << endl;
				resultFile << "------------------------" << endl;

				cout << "------------------------" << endl;
				cout << "Triple # " << squaresCount << ":" << endl;
				cout << "Squares {A; B; C}: {"
						<< (squareDiagonal[0] == 1 ? "Diagonal" : (squareLatin[0] == 1 ? "Latin" : "???" )) << "; "
						<< (squareDiagonal[1] == 1 ? "Diagonal" : (squareLatin[1] == 1 ? "Latin" : "???" )) << "; "
						<< (squareDiagonal[2] == 1 ? "Diagonal" : (squareLatin[2] == 1 ? "Latin" : "???" )) << "}" << endl;
				cout << "------------------------" << endl;
				

				for (int h = 0; h < height; h++)
				{
					for (int i = 0; i < rank; i++)
					{
						for (int j = 0; j < rank; j++)
						{
							cout << square[h][i][j] << " ";
							resultFile << square[h][i][j] << " ";
						}
						cout << endl;
						resultFile << endl;
					}

					cout << endl;
					resultFile << endl;
				}

				resultFile.close();
			}
			else
			{
				// Смена ячейки
				if (layerId < height - 1)
				{
					// Переход на уровень выше в рамках вертикали
					layerId++;
				}
				else
				{
					// Смена обрабатываемой вертикали
						// Переход на нулевой уровень новой вертикали при движении вперёд
						layerId = 0;
						// Изменение вертикали
						if (columnId < rank - 1)
						{
							// Смена столбца
							columnId++;
						}
						else
						{
							// Смена строки и столбца
							columnId = 0;
							rowId++;
						}
				}
			}
		}
		else
		{
			// Обработка факта ненахождения нового значения в ячейке (rowId; columnId)
				// Считываем текущее значение
				cellValue = square[layerId][rowId][columnId];

				// Возвращаем значение в служебные массивы
				if (cellValue != -1)
				{
					// Возвращаем значение в столбцы
					columns[layerId][cellValue][columnId] = 1;
					// Возвращаем значение в строки
					rows[layerId][rowId][cellValue] = 1;
					// Записываем -1 в квадрат
					square[layerId][rowId][columnId] = -1;

					// Зачищаем историю ячейки (layerId; rowId; columnId)
					for (int i = 0; i < rank; i++)
					{
						cellsHistory[layerId][rowId][columnId][i] = 1;
					}
				}

				if (cellValue != -1)
				{
					// "Освобождаем" пару A || B
					if (layerId == 1)
					{
						pairsABDictionary[square[0][rowId][columnId]][cellValue] = 1;
					}
					
					// "Освобождаем" пары A || C и B || C
					if (layerId == 2)
					{
						pairsACDictionary[square[0][rowId][columnId]][cellValue] = 1;
						pairsBCDictionary[square[1][rowId][columnId]][cellValue] = 1;
					}
				}

				// Смена ячейки
				if (layerId > 0)
				{
					// Смена слоя в рамках вертикали
					layerId--;
				}
				else
				{
					// Смена обрабатываемой вертикали
						// Переход на верхний уровень вертикали после перехода назад
						layerId = height - 1;
						// Смена вертикали
						if (columnId > 0)
						{
							// Выполнение возможного перехода к предыдущему столбцу
							columnId--;
						}
						else
						{
							// Переход к нулевому стобцу предыдущей строки
							columnId = rank - 1;
							rowId--;

							// Проверка выхода из слоя квадратов
							if (rowId < 0)
							{
								stop = 1;
							}
						}
				}
		}
	}

	cout << "------------------------" << endl;
	cout << "Total tripes found: " << squaresCount << endl;
	cout << "Press any key to exit ... " << endl;
	cin.get();

	return 0;
}