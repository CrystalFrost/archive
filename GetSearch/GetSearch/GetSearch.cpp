// Поиск диагональных латинских квадратов методом Получения возможных значений ячеек

# include <iostream>

using namespace std;

int main()
{
	const int rank = 5;

	int primary[rank];
	int secondary[rank];
	int columns[rank][rank];
	int rows[rank][rank];
	int square[rank][rank];
	int cellsHistory[rank][rank][rank];

	int rowId;
	int columnId;
	int isGet;
	int cellValue;
	int oldCellValue;

	int stop = 0;
	int squaresCount = 0;

	for (int i = 0; i < rank; i++)
	{
		primary[i] = 1;
		secondary[i] = 1;
	}

	for (int i = 0; i < rank; i++)
	{
		for (int j = 0; j < rank; j++)
		{
			columns[i][j] = 1;
			rows[i][j] = 1;
			square[i][j] = -1;
		}
	}

	for (int i = 0; i < rank; i++)
	{
		for (int j = 0; j < rank; j++)
		{
			for (int k = 0; k < rank; k++)
			{
				cellsHistory[i][j][k] = 1;
			}
		}
	}

	rowId = 0;
	columnId = 0;
	while (!stop)
	{
		// Запрашиваем значение ячейки rowId, columnId
		isGet = 0;
		cellValue = -1;
		for (int i = 0; i < rank && !isGet; i++)
		{
			// Проверяем возможность взятия значения i в ячейку (rowId, columnId)
			if (columns[i][columnId] == 1 && rows[rowId][i] == 1 && cellsHistory[rowId][columnId][i] == 1)
			{
				// Значение не занято в столбцах и строках, но надо ещё проверить диагонали
					// Выставляем флаг, который, возможно, будет сброшен диагональной проверкой
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
			}

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
					// Отмечаем значение в истории значений ячейки
					cellsHistory[rowId][columnId][cellValue] = 0;

				// Возвращение предыдущего значения без зачистки истории (так как мы работаем с этой ячейкой)
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
				
				// Переход к следующей ячейке, если не достигли конца квадрата
				if (rowId == rank - 1 && columnId == rank - 1)
				{
					squaresCount++;
					
					cout << "------------------------" << endl;
					for (int i = 0; i < rank; i++)
					{
						for (int j = 0; j < rank; j++)
						{
							cout << square[i][j] << " ";
						}
						cout << endl;
					}
				}
				else
				{
					if (columnId < rank - 1)
					{
						columnId++;
					}
					else
					{
						columnId = 0;
						rowId++;
					}
				}
		}
		else
		{
			// Обработка факта ненахождения нового значения в ячейке (rowId; columnId)
				// Возвращаем текущее значение из квадрата в массивы
					// Считываем текущее значение
					cellValue = square[rowId][columnId];
					// Возвращаем значение в служебные массивы
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
						// Зачищаем историю ячейки (rowId; columnId)
						for (int i = 0; i < rank; i++)
						{
							cellsHistory[rowId][columnId][i] = 1;
						}
					}

				// Возвращаемся к предыдущей ячейке
				if (columnId > 0)
				{
					columnId--;
				}
				else
				{
					columnId = rank - 1;
					rowId--;

					if (rowId == 0)
					{
						stop = 1;
					}
				}
		}
	}

	cout << "------------------------" << endl;
	cout << "Total squares found: " << squaresCount << endl;
	cout << "Press any key to exit ... " << endl;
	cin.get();

	return 0;
}