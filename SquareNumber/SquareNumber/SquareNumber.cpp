# include <iostream>
# include <fstream>
# include <cstdio>

using namespace std;

struct Symbol
{
	int Value;
	int IsUsed;
};

inline int Factorial(int base)
{
	/*return (base == 0 || base == 1) ? 1 : Factorial(base - 1)*base;*/

	int result = 0;
	int table[12] = { 1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880, 3628800, 39916800 };

	/*if (base > 10)
	{
		result = Factorial(base - 1)*base;
	}
	else
	{
		result = table[base];
	}

	return result;*/

	return table[base];
}

// Построение заданной версии вектора по его рангу и словарю
// vector - указатель на массив длинной vectorRank
// vectorRank - длинна вектора
// version - номер версии вектора
// dictionary - словарь по которому строится вектор
void CreateVectorVersion(int* vector, int vectorRank, int version, Symbol* dictionary)
{
	int componentNumber = -1;		// Номер незанятого компонента словаря, который добавляется в вектор
	int subversion = version;		// Номер подверсии вектора
	int subrank = vectorRank;		// Длина обрабатываемой части вектора
	int divider = -1;				// Делитель, используемый для вычисления
	int dictionarySymbolIndex = -1;	// Идентификатор символа в словаре
	int freeSymbolNumber = -1;		// Номер незанятого элемента в словаре с учётом того, что уже занятые - пропускаются!

	// Построение вектора по его версии
	if (version >= 0)
	{
		// Построение вектора по заданной версии
			// Очистка элементов вектора и флагов использования элементов словаря
			for (int i = 0; i < vectorRank; i++)
			{
				dictionary[i].IsUsed = 0;
				vector[i] = -1;
			}

			// Построение версии вектора
			for (int i = 0; i < vectorRank; i++)
			{
				// Определение i-го компонента вектора
					// Вычисление номера i-го компонента вектора
					divider = Factorial(subrank - 1);
					componentNumber = subversion / divider;
					// Вычисление версии и ранга для следующего шага
					subversion = subversion % divider;
					subrank--;

					// Поиск в словаре свободного компонента с вычисленным номером - componentNumber
						// Задание первоначальных значений
						dictionarySymbolIndex = -1;
						freeSymbolNumber = -1;
						// Перебор элементов словаря
						do
						{
							// Обработка очередного элемента словаря
								// Наращивание счётчика элементов
								dictionarySymbolIndex++;
								// Подсчёт ещё незанятых элементов
								if (dictionary[dictionarySymbolIndex].IsUsed == 0)
								{
									freeSymbolNumber++;
								}
								// Считывание искомого символа словаря
								if (freeSymbolNumber == componentNumber)
								{
									vector[i] = dictionary[dictionarySymbolIndex].Value;
									dictionary[dictionarySymbolIndex].IsUsed = 1;
								}
						}
						while (freeSymbolNumber != componentNumber && dictionarySymbolIndex < vectorRank);
			}
	}
	else
	{
		// Построение вектора - "заглушки" для отрицательной версии
		for (int i = 0; i < vectorRank; i++)
		{
			vector[i] = -1;
		}
	}
}

// Построение версии квадрата
// square - указатель на массив, в который будут записываться элементы квадрата
// rank - размер квадрата
// version - указатель на массив, описывающий версию квадрата
// dictionary - указатель на массив со словарём
void CreateSquareVersion(int* square, int rank, int* version, Symbol* dictionary)
{
	int* row;		// Вспомогательный вектор, используемый для формирования строки
	int rowVersion;	// Версия обрабатываемой строки

	// Выделение памяти
	row = new int[rank];
	
	// Формирование строк квадрата заданной версии
	for (int rowId = 0; rowId < rank; rowId++)
	{
		// Формирование очередной строки по номеру её версии
			// Считывание версии создаваемой строки
			rowVersion = version[rowId];
			// Создание строки как вектора заданной версии
			CreateVectorVersion(row, rank, rowVersion, dictionary);
			// Запись полученных значений в массив квадрата
			for (int componentId = 0; componentId < rank; componentId++)
			{
				square[rowId*rank + componentId] = row[componentId];
			}
	}

	// Высвобождение занятой памяти
	delete [] row;
}

/*// Увеличение версии квадрата на 1
// version - указатель на массив с версиями строк
// rank - ранг квадрата, длина массива с версиями строк
// component - номер компонента вектора, который надо увеличить
// Примечание: Если квадрат достиг максимальной версии,
// то будет "переполнение" - его версия будет сброшена на 1
void IncrementSquareVersion(int* version, int rank, int component)
{
	int isIncremented = 0;
	int componentId = component;
	int maxVersion = Factorial(rank) - 1;

	// Наращивание версии одной из строк на 1
	do
	{
		if (version[componentId] < maxVersion)
		{
			version[componentId]++;
			isIncremented = 1;
		}
		else
		{
			version[componentId] = 0;
			componentId--;
		}
	}
	while (!isIncremented && componentId >= 0);
}*/

/*// Проверка того, достигла ли версия квадрата максимального значения
// version - указатель на массив с версиями строк квадрата
// rank - ранг квадрата, длина массива с версиями строк квадрата
int IsMaxSquareVersion(int* version, int rank)
{
	int isVersionMax = 1;				// Флаг максимальности заданной версии квадрата
	int maxVersion = Factorial(rank) - 1;	// Номер максимальной версии, которую могут получать отдельные вектора в рамках квадрата

	// Проверка компонентов вектора на то, что значения в каждом из него соответствуют максимальной версии вектора
	for (int componentId = 0; componentId < rank && isVersionMax; componentId++)
	{
		// Проверка очередного компонента вектора
		if (version[componentId] != maxVersion)
		{
			isVersionMax = 0;
		}
	}

	return isVersionMax;
}*/

/*// Проверка того, что версии всех строк в версии квадрата - различны
// version - указатель на массив с версиями строк квадрата
// rank - ранг квадрата, длина массива с версиями строк квадрата
int IsRowVersionsDistinct(int* version, int rank)
{
	int duplicationDetected = 0;

	// Проверка номеров версий строк на совпадение между собой
	for (int componentId = 0; componentId < rank && !duplicationDetected; componentId++)
	{
		// Проверка версии строки componentId на совпадение с оставшимися
		for (int comparedId = componentId + 1; comparedId < rank && !duplicationDetected; comparedId++)
		{
			if (version[componentId] == version[comparedId])
			{
				duplicationDetected = 1;
			}
		}
	}

	return !duplicationDetected;
}*/

/*// Проверка корректности первого стобца квадрата, соответствующего заданной версии
// version - указатель на массив с версиями строк квадрата
// rank - ранг квадрата, длина массива с версиями строк квадрата
// rowA - первая строка из пары дублей
// rowB - вторая строка из пары дублей
int IsFirstColumnCorrect(int* version, int rank, int* rowA, int* rowB)
{
	int* column = new int[rank];
	int divider = Factorial(rank - 1);
	int duplicationDetected = 0;

	// Сброс значений индексов дублирующихся строк
	*rowA = -1;
	*rowB = -1;

	// Формирование значений первого столбца
	for (int columnId = 0; columnId < rank; columnId++)
	{
		column[columnId] = version[columnId] / divider;
	}

	// Проверка уникальности значений в первом столбце
	for (int componentId = 0; componentId < rank && !duplicationDetected; componentId++)
	{
		for (int comparedId = componentId + 1; comparedId < rank && !duplicationDetected; comparedId++)
		{
			if (column[componentId] == column[comparedId])
			{
				duplicationDetected = 1;
				*rowA = componentId;
				*rowB = comparedId;
			}
		}
	}

	delete [] column;

	return !duplicationDetected;
}*/

// Проверка квадрата на то, что он является латинским
// square - указатель на массив с элементами квадрата
// rank - ранг квадрата
// Примечание: так как квадрат формируется построчно из векторов,
// то проверка по внутри строк - необязательна, но она оставлена
// для того, чтобы функция была универсальнее, а для более быстрого
// отсеивания не латинских квадратов, сначала выполняется проверка
// внутри столбцов
int IsSquareLatin(int* square, int rank)
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
				if (square[rowId*rank + columnId] == square[comparedRowId*rank + columnId])
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
				if (square[rowId*rank + columnId] == square[rowId*rank + comparedColumnId])
				{
					duplicationDetected = 1;
				}
			}
		}
	}

	return !duplicationDetected;
}

// Проверка квадрата на то, что он является диагональным
// square - указатель на массив с элементами квадрата
// rank - ранг квадрата
int IsSquareDiagonal(int* square, int rank)
{
	int duplicationDetected = 0;

	// Проверка первой диагонали - диагонали [0;0] - [rank;rank]
	for (int itemId = 0; itemId < rank && !duplicationDetected; itemId++)
	{
		// Проверка на совпадение элемента [itemId; itemId] со всеми остальными элементами диагонали
		for (int comparedId = itemId + 1; comparedId < rank && !duplicationDetected; comparedId++)
		{
			if (square[itemId*rank + itemId] == square[comparedId*rank + comparedId])
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
			if (square[(rank - itemId - 1)*rank + itemId] == square[(rank - comparedId - 1)*rank + comparedId])
			{
				duplicationDetected = 1;
			}
		}
	}

	return !duplicationDetected;
}

// Проверка соответствия между частью квадрата от 0-й строки до maxRowId и строки row
// Предполагается, что строка row должна занять строку maxRowId + 1
// square - указатель на массив с содержимым квадрата
// rank - ранг квадрата
// row - указатель на массив с новой строкой
// maxRowId - номер строки до которой определён квадрат
int CheckSquarePart(int* square, int rank, int* row, int maxRowId)
{
	int duplicationDetected = 0;

	// Проверка первой диагонали - диагонали [0;0] - [maxRowId;maxRowId]
	for (int itemId = 0; itemId <= maxRowId && !duplicationDetected; itemId++)
	{
		// Проверка на совпадение элемента [itemId; itemId] со всеми остальными элементами диагонали
		for (int comparedId = itemId + 1; comparedId <= maxRowId && !duplicationDetected; comparedId++)
		{
			if (square[itemId*rank + itemId] == square[comparedId*rank + comparedId])
			{
				duplicationDetected = 1;
			}
		}
	}

	// Проверка второй диагонали - диагонали [maxRowId - itemId - 1; itemId]
	for (int itemId = 0; itemId <= maxRowId && !duplicationDetected; itemId ++)
	{
		// Проверка на совпадение элемента [maxRowId - itemId - 1; itemId] со всеми остальными элементами диагонали
		for (int comparedId = itemId + 1; comparedId <= maxRowId && !duplicationDetected; comparedId++)
		{
			if (square[itemId*rank + (rank - 1) - itemId] == square[comparedId*rank + (rank - 1) - comparedId])
			{
				duplicationDetected = 1;
			}
		}
	}

	// Проверка столбцов квадрата
	for (int columnId = 0; columnId < rank && !duplicationDetected; columnId++)
	{
		// Проверка корректности столбца columnId
		for (int rowId = 0; rowId <= maxRowId && !duplicationDetected; rowId++)
		{
			if (square[rowId*rank + columnId] == row[columnId])
			{
				duplicationDetected = 1;
			}
		}
	}

	/*for (int columnId = rank - 1; columnId >= 0 && !duplicationDetected; columnId--)
	{
		// Проверка корректности столбца columnId
		for (int rowId = maxRowId; rowId >= 0 && !duplicationDetected; rowId--)
		{
			if (square[rowId*rank + columnId] == row[columnId])
			{
				duplicationDetected = 1;
			}
		}
	}*/

	return !duplicationDetected;
}

// Генерация подквадрата начиная со строки rowId
// square - указатель на массив с содержимым квадрата
// rank - ранг квадрата
// version - вектор-версия квадрата
// dictionary - словарь символов
// rowId - строка с которой начинает генерироваться подквадрат
// row - указатель на массив с новой строкой
// checkpointCounter - счётчик для запоминания числа обработанных версий от последней контрольной точки
// completedCheckpointFileName - название файла с завершённой контрольной точкой
// newCheckpointFileName - название файла с новой контрольной точкой
// checkpointInterval - интервал (в виде числа версий) через который надо делать контрольную точку
// resultFileName - название файла в который надо выводить найденные квадраты
void GenerateSubsquare(int* square, int rank, int* version, Symbol* dictionary, int rowId, int* row,
					   int* checkpointCounter, int checkpointInterval, char* completedCheckpointFileName, char* newCheckpointFileName, char* resultFileName)
{
	int maxRowVersion = Factorial(rank);
	int rowVersion;
	int startRowVersion = 0;

	fstream newCheckpointFile;
	fstream resultFile;

	// Выполнение генерации версий подквадратов
	if (rowId == 0)
	{
		// Запуск генерации начиная с указанной версии (первую строку мы не меняем, просто запуская генерацию остальных)
			// Построение версии квадрата
			CreateSquareVersion(square, rank, version, dictionary);

			// Генерация последующих строк
			GenerateSubsquare(square, rank, version, dictionary, rowId + 1, row, checkpointCounter, checkpointInterval, completedCheckpointFileName, newCheckpointFileName, resultFileName);
	}
	else
	{
		// Генерация начиная с заданной строки квадрата
			// Задание начальной версии строки по переданным данным
			if (rowId < rank - 1)
			{
				startRowVersion = version[rowId] == -1 ? 0 : version[rowId];
			}
			else
			{
				startRowVersion = version[rowId] + 1;
			}

			// Генерация строк квадрата
			for (rowVersion = startRowVersion; rowVersion < maxRowVersion; rowVersion++)
			{
				// Генерация очередной версии строки и её подквадрата
				if (rowId + 1 < rank && version[rowId + 1] != -1)
				{
					// Переход к продолжению генерации подквадрата на очередной уровень
					GenerateSubsquare(square, rank, version, dictionary, rowId + 1, row, checkpointCounter, checkpointInterval, completedCheckpointFileName, newCheckpointFileName, resultFileName);
				}
				else
				{
					// Генерация версии текущей строки
						// Создание строки, соответствующей новой версии
						CreateVectorVersion(row, rank, rowVersion, dictionary);
						// Проверка получающегося квадрата
						if (CheckSquarePart(square, rank, row, rowId - 1))
						{
							if (rowId + 1 == rank)
							{
								// Дополнительная проверка квадрата на то, что он латинский и диагональный
									// Запись текущей строки квадрат
									for (int i = 0; i < rank; i++)
									{
										square[rowId*rank + i] = row[i];
									}
							
									// Запись текущей версии строки в вектор версии квадрата
									version[rowId] = rowVersion;

									// Выполнение проверки
									if (IsSquareLatin(square, rank) && IsSquareDiagonal(square, rank))
									{
										// Вывод квадрата, прошедшего проверку, в файл с результатами
										resultFile.open(resultFileName, std::ios_base::app);
										resultFile << "Version" << '\t' << "Square" << endl;

										for (int printedRowId = 0; printedRowId < rank; printedRowId++)
										{
											resultFile << version[printedRowId] << '\t';
											for (int printedColumnId = 0; printedColumnId < rank; printedColumnId++)
											{
												resultFile << square[printedRowId*rank + printedColumnId] << " ";
											}
											resultFile << endl;
										}
										resultFile << endl;

										resultFile.close();
									}

									/*// Формирование контрольной точки при необходимости
									(*checkpointCounter)++;
									if (*checkpointCounter == checkpointInterval)
									{
										// Формирование контрольной точки
											// Создание нового файла контрольной точки
											newCheckpointFile.open("checkpoint_new.txt", std::ios_base::out);
											newCheckpointFile << rank;

											for (int i = 0; i < rank; i++)
											{
												newCheckpointFile << " " << version[i];
											}

											newCheckpointFile.close();

											// Удаление старого файла контрольной точки
											remove("checkpoint.txt");

											// Переименование нового файла контрольной точки
											rename("checkpoint_new.txt", "checkpoint.txt");

											// Сброс флага необходимости формирования контрольной точки
											*checkpointCounter = 0;
									}*/
							}
							else
							{
								// Генерация остальных подстрок квадрата
									// Запись текущей строки квадрат
									for (int i = 0; i < rank; i++)
									{
										square[rowId*rank + i] = row[i];
									}
									// Запись текущей версии
									version[rowId] = rowVersion;
									// Подбор нижележащих строк
									GenerateSubsquare(square, rank, version, dictionary, rowId + 1, row, checkpointCounter, checkpointInterval, completedCheckpointFileName, newCheckpointFileName, resultFileName);
							}
						}

						// Формирование контрольной точки при необходимости
						(*checkpointCounter)++;
						if (*checkpointCounter == checkpointInterval)
						{
							// Формирование контрольной точки
								// Создание нового файла контрольной точки
								newCheckpointFile.open("checkpoint_new.txt", std::ios_base::out);
								newCheckpointFile << rank;

								for (int i = 0; i < rank; i++)
								{
									newCheckpointFile << " " << version[i];
								}

								newCheckpointFile.close();

								// Удаление старого файла контрольной точки
								remove("checkpoint.txt");
								
								// Переименование нового файла контрольной точки
								rename("checkpoint_new.txt", "checkpoint.txt");

								// Сброс флага необходимости формирования контрольной точки
								*checkpointCounter = 0;
						}
				}
			}
			// Сброс версии обработанной строки
			version[rowId] = -1;
	}
}

int main(int argumentsCount, char* argumentsValues[])
{
	int squareRank;
	int* square;
	int* version;
	int* row;
	Symbol* dictionary;

	int parametersIsRead = 0;
	int checkpointInterval = 10000000;
	int checkpointCounter = 0;
	char* completedCheckpointFileName = "checkpoint.txt\0";
	char* newCheckpointFileName = "checkpoint_new.txt\0";
	char* resultFileName = "result.txt";
	fstream checkpointFile;

	// Считывание входных параметров из файла
	checkpointFile.open(completedCheckpointFileName, std::fstream::in);
	
	if (checkpointFile.is_open())
	{
		// Считывание входных параметров из файла контрольной точки
			// Считывание ранга квадрата
			checkpointFile >> squareRank;

			// Формирование структур, зависимых от ранга квадрата
			square = new int[squareRank*squareRank];
			version = new int[squareRank];
			row = new int[squareRank];
			dictionary = new Symbol[squareRank];

			// Формирование словаря
			for (int i = 0; i < squareRank; i++)
			{
				dictionary[i].Value = i;
				dictionary[i].IsUsed = 0;
			}

			// Считывание версии квадрата
			for (int i = 0; i < squareRank; i++)
			{
				checkpointFile >> version[i];
			}

			// Закрытие файла контрольной точки
			checkpointFile.close();

			// Выставление флага считанности параметров
			parametersIsRead = 1;
	}

	// Построение латинского квадрата путём наращивания его версии
	if (parametersIsRead)
	{
		// Построение латинского квадрата по считанным параметрам
			// Вывод сообщения о начале расчётов
			cout << "Starting computations with the following parameters:" << endl;
			cout << "Square rank: " << squareRank << endl;
			cout << "Square version: { ";

			for (int i = 0; i < squareRank; i++)
			{
				cout << version[i];
				if (i < squareRank - 1)
				{
					cout << "; ";
				}
			}

			cout << " }" << endl;

			// Построение латинского квадрата по строкам
			GenerateSubsquare(square, squareRank, version, dictionary, 0, row, &checkpointCounter, checkpointInterval, completedCheckpointFileName, newCheckpointFileName, resultFileName);

			// Освобождение выделенной памяти
			delete [] square;
			delete [] version;
			delete [] row;
			delete [] dictionary;
	}

	cout << "Computations finished! Press eny key to exit..." << endl;
	cin.get();

	return 0;
}