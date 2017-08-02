# include <iostream>
# include <fstream>

using namespace std;

struct Symbol
{
	int Value;
	int IsUsed;
};

int Factorial(int base)
{
	/* Формально - наиболее корректный вариант, вместо которого выбрали фактически - верный, и более быстрый
	return (base == 0 || base == 1) ? 1 : Factorial(base - 1)*base;
	*/

	int result = 0;
	int table[12] = { 1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880, 3628800, 39916800 };

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

// checkpointVersion - версия квадрата из контрольной точки
// version - указатель на массив с версиями строк
// squareRank - ранг квадрата, длина массива с версиями строк

// Примечание: Если версия квадрата не законченная (т.е. есть строки с версией -1)
// то стартовая версия должна совпадать с версией из контрольной точки.
// Если же версия квадрата - законченная, то производится инкремент версии квадрата
// Если версия квадрата дошла до максимальной, то первая строка станет равной 1
void CreateStartVersion(int* checkpointVersion, int* version, int squareRank)
{
	int rowId;									// Идентификатор строки 
	int maxVersion = Factorial(squareRank) - 1;	// Номер максимальной версии

	// Первичное копирование версии. Если версия не законченная - она останется без изменений
	for (int i = 0; i < squareRank; i++)
	{
		version[i] = checkpointVersion[i];
	}

	// Формирование версии в зависимости от законченности версии квадрата
	if (checkpointVersion[squareRank - 1] != -1)
	{
		// Текущая версия = Версия из контрольной точки + 1
			rowId = squareRank - 1;

			// Ищем строку, версию которой ещё можно увеличивать
			while (version[rowId] + 1 > maxVersion)
			{
				// Если версия дошла до максимальной - выставление -1 и переход к вышестоящей строке
				version[rowId] = -1;
				rowId--;
			}
			// Увеличиваем на единицу версию строки, на которой остановились
			version[rowId]++;
	}
}

// Отображение информации и параметрах начала расчётов
// squareRank - ранг квадрата
// version - номер версии, с которой начат расчёт
void ShowStartParameters(int squareRank, int* version)
{
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
}

// Проверка части квадрата на перспективность
// Так как конфликты в рамках строки исключены - здесь строки не проверяются
// Осуществляется проверка совпадений элементов заданной строки (checkedRowId) с остальными
// squareRank - ранг квадрата
// version - проверяемая версия
// rowVersions - перечень возможных версий
// checkedRowId - проверяемая строка
int CheckSquarePart(int squareRank, int* square, int checkedRowId)
{
	int duplicationDetected = 0;

	if (checkedRowId != 0)
	{
		// Проверка корректности части квадрата
			// Проверка первой диагонали - диагонали [0; 0] - [maxRowId; maxRowId]
			for (int rowId = checkedRowId - 1; rowId >= 0  && duplicationDetected == 0; rowId--)
			{
				// Проверка совпадения элемента [checkedRowId; checkedRowId] и элемента [rowId; rowId]
				if (square[checkedRowId*squareRank + checkedRowId] == square[rowId*squareRank + rowId])
				{
					duplicationDetected = 1;
				}
			}

			// Проверка второй диагонали - диагонали [0; squareRank - 1] - [checkedRowId; squareRank - checkedRowId - 1]
			for (int rowId = checkedRowId - 1; rowId >= 0 && duplicationDetected == 0; rowId--)
			{
				// Проверка совпадения элемента [checkedRowId; squareRank - checkedRowId - 1] с элементом [rowId; squareRank - rowId - 1]
				if (square[checkedRowId*squareRank + squareRank - checkedRowId - 1] == square[rowId*squareRank + squareRank - rowId - 1])
				{
					duplicationDetected = 1;
				}
			}

			// Проверка столбцов квадрата
			for (int itemId = squareRank - 1; itemId >= 0 && duplicationDetected == 0; itemId--)
			{
				// Проверка столбца itemId
				for (int rowId = checkedRowId - 1; rowId >= 0  && duplicationDetected == 0; rowId--)
				{
					// Проверка на совпадение элемента [checkedRowId; itemId] и [rowid; itemId]
					if (square[checkedRowId*squareRank + itemId] == square[rowId*squareRank + itemId])
					{
						duplicationDetected = 1;
					}
				}
			}
	}

	return !duplicationDetected;
}


// Проверка квадрата на то, что он является латинским
// squareRank - ранг квадрата
// version - версия квадрата
// rowVersions - массив с ссылками на массивы с возможными значениями строк
// Примечание: так как квадрат формируется построчно из векторов,
// то проверка по внутри строк - необязательна, но она оставлена
// для того, чтобы функция была универсальнее, а для более быстрого
// отсеивания не латинских квадратов, сначала выполняется проверка
// внутри столбцов
int IsSquareLatin(int squareRank, int* square)
{
	int duplicationDetected = 0;

	// Проверка столбцов квадрата
	for (int columnId = 0; columnId < squareRank && !duplicationDetected; columnId++)
	{
		// Проверка корректности столбца columnId
		for (int rowId = 0; rowId < squareRank && !duplicationDetected; rowId++)
		{
			// Проверка на совпадение элемента [rowId; columnId] со всеми остальными элементами столбца columndId
			for (int comparedRowId = rowId + 1; comparedRowId < squareRank && !duplicationDetected; comparedRowId++)
			{
				if (square[comparedRowId*squareRank + columnId] == square[rowId*squareRank + columnId])
				{
					duplicationDetected = 1;
				}
			}
		}
	}

	// Проверка строк квадрата
	for (int rowId = 0; rowId < squareRank && !duplicationDetected; rowId++)
	{
		// Проверка корректности строки rowId
		for (int columnId = 0; columnId < squareRank && !duplicationDetected; columnId++)
		{
			// Проверка на совпадение элемента [rowId; columnId] со всеми остальными элементами строки rowId
			for (int comparedColumnId = columnId + 1; comparedColumnId < squareRank && !duplicationDetected; comparedColumnId++)
			{
				if (square[rowId*squareRank + columnId] == square[rowId*squareRank + comparedColumnId])
				{
					duplicationDetected = 1;
				}
			}
		}
	}

	return !duplicationDetected;
}

// Проверка квадрата на то, что он является диагональным
// squareRank - ранг квадрата
// version - версия квадрата
// rowVersions - массив с ссылками на массивы с возможными значениями строк
int IsSquareDiagonal(int squareRank, int* square)
{
	int duplicationDetected = 0;

	// Проверка первой диагонали - диагонали [0;0] - [rank;rank]
	for (int itemId = 0; itemId < squareRank && !duplicationDetected; itemId++)
	{
		// Проверка на совпадение элемента [itemId; itemId] со всеми остальными элементами диагонали
		for (int comparedId = itemId + 1; comparedId < squareRank && !duplicationDetected; comparedId++)
		{
			if (square[itemId*squareRank + itemId] == square[comparedId*squareRank + comparedId])
			{
				duplicationDetected = 1;
			}
		}
	}

	// Проверка второй диагонали - диагонали [rank - itemId - 1; itemId]
	for (int itemId = 0; itemId < squareRank && !duplicationDetected; itemId ++)
	{
		// Проверка на совпадение элемента [rank - itemId - 1; itemId] со всеми остальными элементами диагонали
		for (int comparedId = itemId + 1; comparedId < squareRank && !duplicationDetected; comparedId++)
		{
			if (square[(squareRank - itemId - 1)*squareRank + itemId] == square[(squareRank - comparedId - 1)*squareRank + comparedId])
			{
				duplicationDetected = 1;
			}
		}
	}

	return !duplicationDetected;
}

// Формирование контрольной точки
// completedCheckpointFileName - имя файла с завершённой контрольной точкой
// newCheckpointFileName - имя файла с новой контрольной точкой
// squareRank - ранг квадрата
// version - версия квадрата
void CreateCheckpoint(char* completedCheckpointFileName, char* newCheckpointFileName, int squareRank, int* version)
{
	fstream newCheckpointFile;

	// Формирование контрольной точки
		// Создание нового файла контрольной точки
		newCheckpointFile.open(newCheckpointFileName, std::ios_base::out);
		newCheckpointFile << squareRank;

		for (int i = 0; i < squareRank; i++)
		{
			newCheckpointFile << " " << version[i];
		}

		newCheckpointFile.close();

		// Удаление старого файла контрольной точки
		remove(completedCheckpointFileName);
								
		// Переименование нового файла контрольной точки
		rename(newCheckpointFileName, completedCheckpointFileName);
}

// Запись найденного квадрата в файл
// resultFileName - имя файла в который дописываем результат
// squareRank - ранг квадрата
// version - версия квадрата
// rowVersions - массив с ссылками на массивы с возможными значениями строк
void SaveSquare(char* resultFileName, int squareRank, int* version, int* square)
{
	fstream resultFile;

	// Вывод квадрата в файл с результатами
	resultFile.open(resultFileName, std::ios_base::app);
	resultFile << "Version" << '\t' << "Square" << endl;

	// Вывод версий строк и элементов строк квадратов
	for (int printedRowId = 0; printedRowId < squareRank; printedRowId++)
	{
		// Вывод очередной строки
			// Вывод версии строки
			resultFile << version[printedRowId] << '\t';
			// Вывод элементов строки
			for (int printedColumnId = 0; printedColumnId < squareRank; printedColumnId++)
			{
				resultFile << square[printedRowId*squareRank + printedColumnId] << " ";
			}
			// Закрытие строки
			resultFile << endl;
	}
	resultFile << endl;
	
	resultFile.close();
}

// Формирование версий квадратов
// squareRank - ранг квадрата
// version - версия, с которой начинаем генерацию
// rowVersions - список версий, которые могут быть в строке
void GenerateSquares(int squareRank, int* version, int* rowVersions, Symbol* dictionary, char* completedCheckpointFileName, char* newCheckpointFileName, char* resultFileName, int checkpointInterval)
{
	int rowId;									// Обрабатываемая строка
	int maxVersion = Factorial(squareRank) - 1;	// Наибольший номер версии
	int checkpointCounter = 0;					// Счётчик итераций между контрольными точками
	int* square;								// Массив - квадрат

	// Выделяем память под массив
	square = new int[squareRank*squareRank];
	// "Зануляем" память с квадратом
	for (int i = 0; i < squareRank*squareRank; i++)
	{
		square[i] = -1;
	}

	// Определяем текущую строку
	rowId = squareRank - 1;
	while (version[rowId] == -1)
	{
		rowId--;
	}
	
	// Формируем начальный квадрат
	for (int i = 0; i < squareRank; i++)
	{
		// Формируем i-ю строку квадрата
		CreateVectorVersion(square + i*squareRank, squareRank, version[i], dictionary);
	}

	// Генерируем все возможные квадраты для 0-й версии первой строки (а больше - и не надо)
	while (version[0] == 0)
	{
		// Обрабатываем интересные нам версии квадратов
		if (rowId == squareRank - 1)
		{
			// Работа с последней строкой квадрата
				// Проверка и сохранение найденного квадрата
				if (CheckSquarePart(squareRank, square, rowId))
				{
					// Обработка найденного квадрата
					if (IsSquareLatin(squareRank, square) && IsSquareDiagonal(squareRank, square))
					{
						// Обработка найденного и проверенного квадрата
							// Запись найденного квадрата в файл
							SaveSquare(resultFileName, squareRank, version, square);
							// Создание контрольной точки
							CreateCheckpoint(completedCheckpointFileName, newCheckpointFileName, squareRank, version);
					}
				}

				// Переход к следующему квадрату
					// Наращивание версии строки rowId
						// Обработка переполнения версии
						while (version[rowId] + 1 > maxVersion)
						{
							// Если версия дошла до максимальной - "обнуление" строки в квадрате, выставление -1 в персии и переход к вышестоящей строке
							for (int i = 0; i < squareRank; i++)
							{
								square[rowId*squareRank + i] = -1;
							}
	
							version[rowId] = -1;
							rowId--;
						}	

						// Переход к следующей версии
							// Увеличение номера версии
							version[rowId]++;
							// Генерация строки квадрата
							CreateVectorVersion(square + rowId*squareRank, squareRank, version[rowId], dictionary);
		}
		else
		{
			// Работа с промежуточными строками
				// Проверка промежуточного квадрата
				if (CheckSquarePart(squareRank, square, rowId))
				{
					// Продолжение поиска в перспективном квадрата
						// Переходим к следующей строке и генерируем её в квадрате
						rowId++;
						version[rowId] = 0;
						CreateVectorVersion(square + rowId*squareRank, squareRank, version[rowId], dictionary);
				}
				else
				{
					// Переход к следующей версии в последней из определённых на данный момент строк (код аналогичен коду выше)
						// Обработка переполнения версии
						while (version[rowId] + 1 > maxVersion)
						{
							// Если версия дошла до максимальной - "обнуление" строки в квадрате, выставление -1 в персии и переход к вышестоящей строке
							for (int i = 0; i < squareRank; i++)
							{
								square[rowId*squareRank + i] = -1;
							}
	
							version[rowId] = -1;
							rowId--;
						}
						
						// Переход к следующей версии
							// Увеличение номера версии
							version[rowId]++;
							// Генерация строки квадрата
							CreateVectorVersion(square + rowId*squareRank, squareRank, version[rowId], dictionary);
				}
		}

		// Формирование контрольной точки
		checkpointCounter++;
		if (checkpointCounter % checkpointInterval == 0)
		{
			CreateCheckpoint(completedCheckpointFileName, newCheckpointFileName, squareRank, version);
		}
	}

	// Фиксация завершения работы
	CreateCheckpoint(completedCheckpointFileName, newCheckpointFileName, squareRank, version);
}

int main(int argumentsCount, char* argumentsValues[])
{
	// Переменные входных параметров
	int parametersIsRead = 0;									// Флаг считывания входных параметров
	int checkpointInterval = 10000000;							// Число итераций между контрольными точками
	char* completedCheckpointFileName = "checkpoint.txt\0";		// Файл завершённой контрольной точки
	char* newCheckpointFileName = "checkpoint_new.txt\0";		// Файл создаваемой контрольной точки
	char* resultFileName = "result.txt";						// Файл с результатами расчётов
	fstream checkpointFile;										// Поток для работы с файлом контрольной точки

	// Структуры данных поиска квадратов
	int squareRank;				// Ранг квадрата
	Symbol* dictionary;			// Массив со словарём
	int* rowVersions;			// Массив указателей на все возможные комбинации, которые могут составлять строку
	int rowCombinations;		// Число возможных комбинаций

	int* versionFromCheckpoint;	// Версия квадрата, считанная из файла контрольной точки
	int* version;				// Текущая обрабатываемая версия

	// Cчитывание параметров из файла
		// Открытие файла c данными контрольной точки
		checkpointFile.open(completedCheckpointFileName, std::fstream::in);
		// Считывание входных параметров
		if (checkpointFile.is_open())
		{
			// Считывание входных параметров из открытого файла контрольной точки
				// Считывание ранга квадрата
				checkpointFile >> squareRank;

				// Считывание версии квадрата
				versionFromCheckpoint = new int[squareRank];
				for (int i = 0; i < squareRank; i++)
				{
					checkpointFile >> versionFromCheckpoint[i];
				}

				// Закрытие файла контрольной точки
				checkpointFile.close();

				// Выставление флага считанности параметров
				parametersIsRead = 1;
	}

	// Формирование структур, зависимых от параметров из файла 
	if (parametersIsRead)
	{
		// Формирование структур по считанным параметрам
			// Формирование словаря
			dictionary = new Symbol[squareRank];
			for (int i = 0; i < squareRank; i++)
			{
				dictionary[i].Value = i;
				dictionary[i].IsUsed = 0;
			}

			// Построение всех возможных версий строк
				// Формирование массива указателей на версии
				rowCombinations = Factorial(squareRank);
				rowVersions = new int [rowCombinations*squareRank];

				// Формирование версий
				for (int i = 0; i < rowCombinations; i++)
				{
					// Формирование i-й версии
					CreateVectorVersion(rowVersions + squareRank*i, squareRank, i, dictionary);
				}

			// Формирование массива текущей версии
			version = new int[squareRank];
			CreateStartVersion(versionFromCheckpoint, version, squareRank);
		}

	// Основной блок работы программы - поиск квадратов
	if (parametersIsRead)
	{
		// Вывод информации о начале расчёта
		ShowStartParameters(squareRank, versionFromCheckpoint);
		
		// Формирование квадратов
		GenerateSquares(squareRank, version, rowVersions, dictionary, completedCheckpointFileName, newCheckpointFileName, resultFileName, checkpointInterval);
	}

	// Высвобождение памяти
	if (parametersIsRead)
	{
		// Удаление массивов - версии из файла контрольной точки и текущей версии
		delete [] versionFromCheckpoint;
		delete [] version;

		// Удаление массива с перечнем версий
		delete [] rowVersions;
		// Удаление словаря
		delete [] dictionary;
	}

	// Завершение работы
	cout << "Press any key to continue..." << endl;
	cin.get();

	return 0;
}