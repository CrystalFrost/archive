// Поиск диагональных латинских квадратов методом Получения возможных значений клеток с предварительным подбором диагоналей

# include "GetDiagSearch.h"

using namespace std;

// Конструктор по умолчанию - инициализация структур поиска квадратов
GetDiagSearch::GetDiagSearch()
{
	InitializeSearch();
}

// Инициализация структур перед поиском квадрата
void GetDiagSearch::InitializeSearch()
{
	// Заполняем вектора диагоналей флагами свободных элементов
	for (int i = 0; i < rank; i++)
	{
		primary[i] = 1;
		secondary[i] = 1;
	}

	// Заполняем матрицы столбцов и строк флагами свободных элементов, а квадрат - "-1"
	for (int i = 0; i < rank; i++)
	{
		for (int j = 0; j < rank; j++)
		{
			columns[i][j] = 1;
			rows[i][j] = 1;
			square[i][j] = -1;
		}
	}

	// Очищаем историю использоваия значений в клетках квадратов
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

	// Заполняем первую строку квадрата и отмечаем использование этих значений 
	for (int i = 0; i < rank; i++)
	{
		square[0][i] = i;
		rows[0][i] = 0;
		columns[i][i] = 0;

		cellsHistory[0][i][i] = 0;
	}

	// Отмечаем использование элемента "0" в рамках главной и "9" - в рамках побочной диагонали
	primary[0] = 0;
	secondary[rank - 1] = 0;

	// Выставляем счётчик квадратов в "0"
	squaresCount = 0;

	// Задаём имя файла с результатами
	resultFileName = "result.txt";
}

// Поиск следующего значения, которое можно было бы поставить в клетку (rowId, columndId)
// Возвращает удачность операции:
//		0 - значение найти не удалось, в квадрат записано "-1";
//		1 - значение найдено и записано в квадрат.

int GetDiagSearch::GetCell(int rowId, int columnId)
{
	int isGet;			// Флаг нахождения подходящего значения
	int cellValue;		// Найденное значение для записи в клетку (rowId, columnId)
	int oldCellValue;	// Предыдущее значение, стоявшее в этой клетку

	// Поиск нового значения для клетки (rowId, columnId)
	isGet = 0;
	cellValue = -1;
	for (int i = 0; i < rank && !isGet; i++)
	{
		// Проверка i на возможность записи в клетку (rowId; columnId)
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
	}
	else
	{
		// Обработка факта ненахождения нового значения в клетке (rowId; columnId)
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
					// Зачищаем историю клетки (rowId; columnId)
					for (int i = 0; i < rank; i++)
					{
						cellsHistory[rowId][columnId][i] = 1;
					}
				}
	}

	return isGet;
}


// Шаг вперёд вдоль побочной диагонали с текущей позиции *sDiagIndex
void GetDiagSearch::StepForwardSDiag(int* sDiagIndex)
{
	// Перемещаемся на ближайший следующий элемент, не попадающий на главную диагональ
	do
	{
		(*sDiagIndex)++;
	}
	while (rank%2 == 1 && (*sDiagIndex)*2 + 1 == rank);
}


// Шаг назад вдоль побочной диагонали с текущей позиции *sDiagIndex
void GetDiagSearch::StepBackwardSDiag(int* sDiagIndex)
{
	// Перемещаемся на ближайший предыдущий элемент, не попадающий на главную диагональ
	do
	{
		(*sDiagIndex)--;
	}
	while (rank%2 == 1 && (*sDiagIndex)*2 + 1 == rank);
}


// Шаг вперёд вдоль строки c текущей позиции (*rowId, *columnId)
void GetDiagSearch::StepForward(int* rowId, int* columnId)
{
	// Перемещаемся на ближайщий следующий элемент, не принадлежащий диагоналям,
	// при необходимости опускаясь на строку ниже.
	do
	{
		if ((*columnId) < rank - 1)
		{
			(*columnId)++;
		}
		else
		{
			(*columnId) = 0;
			(*rowId)++;
		}
	}
	while ((*rowId) == (*columnId) || (*rowId) == rank - (*columnId) - 1);
}


// Шаг назад вдоль строки c текущей позиции (*rowId, *columnId)
void GetDiagSearch::StepBackward(int* rowId, int* columnId)
{
	// Перемещаемся на ближайший предыдущий элемент, не принадлежащий диагоналям,
	// при необходимости поднимаясь на строку выше
	do
	{
		if ((*columnId) > 0)
		{
			(*columnId)--;
		}
		else
		{
			(*columnId) = rank - 1;
			(*rowId)--;
		}
	}
	while((*rowId) == (*columnId) || (*rowId) == rank - (*columnId) - 1);
}


// Обработка найденного квадрата
void GetDiagSearch::ProcessSquare()
{
	// Генерируем событие нахождения квадрата 
	__raise FoundTheSquare(square);
}


// Функция вывода квадрата в консоль
void GetDiagSearch::PrintSquare(int printedSquare[rank][rank])
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
void GetDiagSearch::SaveSquare(int printedSquare[rank][rank], fstream& resultFile)
{
	// Запись квадрата файловый поток
	for (int i = 0; i < rank; i++)
	{
		for (int j = 0; j < rank; j++)
		{
			resultFile << printedSquare[i][j] << " ";
		}
		resultFile << endl;
	}

	resultFile << endl;
}


// Поиск Диагональных Латинских Квадратов
void GetDiagSearch::Start()
{
	int stop = 0;			// Флаг окончания поиска в целом
	int sDiagStop = 0;		// Флаг окончания поиска вдоль побочной диагонали
	int cellsStop = 0;		// Флаг окончания поиска по строкам

	int isPDiag = 0;		// Флаг нахождения варианта главной диагонали
	int isSDiag = 0;		// Флаг нахождения варианта побочной диагонали

	pDiagIndex = 1;			// Номер обрабатываемой клетки на главной диагонали
	sDiagIndex = 0;			// Номер обрабатываемой клетки на побочной диагонали

	rowId = 0;				// Номер строки с обрабатываемой клеткой
	columnId = 0;			// Номер столбца с обрабатываемой клеткой

	// Поиск квадратов по клеткам
	while (!stop)
	{
		// Поиск квадратов по главной и побочной диагонали и по строкам
			// Подбор главной диагонали
			isPDiag = 0;

			while (!isPDiag && !stop)
			{
				// Поиск значения для элемента pDiagIndex главной диагонали
				if (GetCell(pDiagIndex, pDiagIndex))
				{
					// Обработка нахождения значения
					if (pDiagIndex == rank - 1)
					{
						// Выставление флага завешения главной диагонали
						isPDiag = 1;
					}
					else
					{
						// Переход к следующему элементу
						pDiagIndex++;
					}
				}
				else
				{
					// Обработка отсутствия возможного значения
					if (pDiagIndex == 1)
					{
						// Завершаем поиск, если вернулись в самую первую клетку
						stop = 1;
					}
					else
					{
						// Переходим к предыдущему элементу
						pDiagIndex--;
					}
				}
			}

			// Подбор остальной части квадрата
			sDiagStop = 0;
			while (isPDiag && !sDiagStop && !stop)
			{
				// Подбор побочной диагонали и оставшихся клеток квадрата к найденной главной диагонали
					// Подбор побочной диагонали
					isSDiag = 0;

					while (isPDiag && !isSDiag && !sDiagStop && !stop)
					{
						// Поиск значения для элемента sDiagIndex побочной диагонали
						if (GetCell(rank - sDiagIndex - 1, sDiagIndex))
						{
							// Обработка нахождения значения
							if (sDiagIndex == rank - 2)
							{
								// Обработка достижения предпоследнего элемента (последний - в зафиксированной строке)
								isSDiag = 1;
							}
							else
							{
								// Перемещаемся вдоль побочной диагонали
								StepForwardSDiag(&sDiagIndex);
							}
						}
						else
						{
							// Обработка отсутствия возможного значения
							if (sDiagIndex == 0)
							{
								// По возвращении в начало диагонали - выставляем флаг отсутствия подходящей побочной диагонали
								sDiagStop = 1;
							}
							else
							{
								// Переходим к предыдущему элементу
								StepBackwardSDiag(&sDiagIndex);
							}
						}
					}

					// Подбор значений для оставшихся клеток квадрата
						// Задание первоначальных значений
						rowId = 1;
						columnId = 0;
						cellsStop = 0;

						while (isPDiag && isSDiag && !stop && !cellsStop)
						{
							// Поиск нового значения для клетки (rowId, columnId)
							if (GetCell(rowId, columnId))
							{
								// Обработка нахождения значения
								if (rowId == rank - 1 && columnId == rank - 2)
								{
									// Обработка полученного диагонального латинского квадрата по достижению клетки (rank - 1, rank - 2).
									// "-2" - потому, что самая последняя клетка квадрата - это окончание главной диагонали
									squaresCount++;
									ProcessSquare();
								}
								else
								{
									// Делаем следующий шаг
									StepForward(&rowId, &columnId);
								}
							}
							else
							{
								// Обработка отсутствия значения
									// Делаем шаг назад
									StepBackward(&rowId, &columnId);
									// Проверяем возврат к фиксированной строке
									if (rowId == 0)
									{
										// Выставляем флаг завершения поиска по недиагональным клеткам
										cellsStop = 1;
									}
							}
						}
			}
	}
}