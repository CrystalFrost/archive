# include <iostream>
# include <fstream>

using namespace std;

// Структура - символ словаря
struct Symbol
{
	int Value;
	int IsUsed;
};

// Класс - искатель квадратов
class SquareFinder
{
public:
	SquareFinder();		// Конструктор по умолчанию, считывает данные из checkpoint-а
	~SquareFinder();	// Деструктор
	void Start();		// Запуск процесса вычислений

private:
	static int Factorial(int base);																	// Вычисление факториала
	static void CreateVectorVersion(int* vector, int vectorRank, int version, Symbol* dictionary);	// Создание вектора по версии перестановки его компонентов
	void Commit();					// Формирование контрольной точки
	void Restore();					// Восстановление процесса расчёта из контрольной точки
	void SaveSquare();				// Сохранение найденного квадрата в файл результатов
	void ShowStartParameters();		// Вывод в консоль параметров запуска
	int CheckPrimaryDiagonalCompatibility(int diagonalPVersion);							// Проверка соответствия главной диагонали фиксированной строке
	int CheckSecondaryDiagonalCompatibility(int diagonalPVersion, int diagonalSVersion);	// Проверка соответствия побочной диагонали главной и фиксированной строке
	int CheckColumnCompatibility(int ColumnIndex, int ColumnVersion);						// Проверка совместимости заданой версии для заданного столбца с остальными элементами квадрата
	int IsSquareLatin();			// Проверка того, что квадрат - латинский
	int IsSquareDiagonal();			// Проверка диагональности квадрата

	int Rank;						// Ранг квадрата
	int FixedLineVersion;			// Версия фиксированной строки
	int DiagonalPVersion;			// Версия главной диагонали
	int DiagonalSVersion;			// Версия побочной диагонали
	int* ColumnsVersions;			// Массив с версиями столбцов

	int ColumnIndex;				// Номер столбца, обрабатываемого в данный момент
	int ColumnVersion;				// Номер версии столбца, обрабатываемой в данный момент

	int* FixedLine;					// Массив с элементами фиксированной строки
	int* DiagonalP;					// Массив с элементами главной диагонали
	int* DiagonalS;					// Массив с элементами побочной диагонали
	int** Matrix;					// Массив с указателями на массивы - строки матрицы квадрата

	int* VectorVersions;			// Массив с перечнем версий векторов

	char* ResultFileName;			// Строка с названием файла с результатами
	char* NewCheckpointFileName;	// Строка с названием файла новой контрольной точки
	char* CheckpointFileName;		// Строка с названием файла завершённой контрольной точки

	Symbol* Dictionary;				// Массив со словарём

	int CheckpointInterval;			// Интервал создания контрольной точки в числе итераций
};


// Конструктор по умолчанию
SquareFinder::SquareFinder()
{
	// Задаём названия файлов с контрольной точкой и результатами расчёта
	ResultFileName = "result.txt";
	NewCheckpointFileName = "checkpoint_new.txt";
	CheckpointFileName = "checkpoint.txt";

	// Задаём интервал создания контрольных точек
	CheckpointInterval = 1000000000;

	// Восстанавливаем состояние поиска
	Restore();
}


// Деструктор
SquareFinder::~SquareFinder()
{
	// Высвобождение занятой памяти
		// Удаление словаря
		delete [] Dictionary;

		// Удаление версий векторов
		delete [] VectorVersions;

		// Удаление фиксированной строки, главной и побочной диагоналей
		delete [] FixedLine;
		delete [] DiagonalP;
		delete [] DiagonalS;

		// Удаление матрицы квадрата
		for (int i = 0; i < Rank; i++)
		{
			delete [] Matrix[i];
		}
		
		delete [] Matrix;
}


// Вычисление функции факториала
int SquareFinder::Factorial(int base)
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
void SquareFinder::CreateVectorVersion(int* vector, int vectorRank, int version, Symbol* dictionary)
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


// Запуск поиска
void SquareFinder::Start()
{
	int maxVersion = Factorial(Rank) - 1;		// Максимальный номер версии
	int checkpointCounter = 0;					// Счётчик циклов для создания контрольной точки

	// Вывод информации о точке начала поиска
	ShowStartParameters();

	// Запуск поиска по данным из контрольной точки
	while (VectorVersions[DiagonalPVersion*Rank] == 0)
	{
		// Проверка очередного варианта главной диагонали
		if (CheckPrimaryDiagonalCompatibility(DiagonalPVersion))
		{
			// Обработка подходящего варианта главной диагонали
				// Запись диагонали в матрицу и выделенный массив
				for (int i = 0; i < Rank; i++)
				{
					DiagonalP[i] = VectorVersions[DiagonalPVersion*Rank + i];
					Matrix[i][i] = VectorVersions[DiagonalPVersion*Rank + i];
				}

				// Перебор вариантов побочной диагонали
				while (DiagonalSVersion < maxVersion)
				{
					// Проверка очередного варианта побочной диагонали
					if (CheckSecondaryDiagonalCompatibility(DiagonalPVersion, DiagonalSVersion))
					{
						// Обработка подходящего вариантв побочной диагонали
							// Запись диагонали в матрицу и выделенный массив
							for (int i = 0; i < Rank; i++)
							{
								DiagonalS[i] = VectorVersions[DiagonalSVersion*Rank + i];
								Matrix[Rank - 1 - i][i] = VectorVersions[DiagonalSVersion*Rank + i];
							}

							// Подбор столбцов для комбинации {"Первая фиксированная строка"; "Главная диагональ"; "Побочная диагональ"}
							// Примечание: В программе есть следующие структуры работы со столбцами:
							// ColumnsVersions - массив с версиями уже проверенных версий. По мере хода к 10-му столбцу и обратно - то зачищается, то заполняется снова
							// ColumnIndex и ColumnVersion - текущие проверяемые значения столбца и его версии. ColumnVersion не обязательно должна быть в ColumnsVersion
							do
							{
								// Проверка очередной версии очередного столбца
									// Проверка текущей версии
									if (CheckColumnCompatibility(ColumnIndex, ColumnVersion))
									{
										// Запись столбца в матрицу, проверка квадрата
											// Запись версии в столбец
											ColumnsVersions[ColumnIndex] = ColumnVersion;

											// Запись столбца в квадрат
											for (int i = 0; i < Rank; i++)
											{
												Matrix[i][ColumnIndex] = VectorVersions[ColumnVersion*Rank + i];
											}

											// Дополнительная проверка квадрата, если он построен целиком
											if (ColumnIndex == Rank - 1 && IsSquareLatin() && IsSquareDiagonal())
											{
												SaveSquare();
											}
											else
											{
												// Переход к следующему столбцу (вправо)
												ColumnIndex++;
												// Изначальный вариант - ColumnVersion = -1; - заменён на вариант предсказания номера с которого можно начинать поиск,
												// так как, версии столбцов должны соответствовать ColumnIndex-элементу первой строки.
												// Но если она не нулевой версии, то это не будет работать корректно!
												// Аналогичная оптимизация в блоке "Переход к следующей версии"
												ColumnVersion = ColumnIndex * (maxVersion + 1) / Rank - 1 >= 0 ? ColumnIndex * (maxVersion + 1) / Rank - 1 : 0;
											}
									}

									// Переход к следующей версии
									// Оптимизация, схожая с "Переходом к следующему столбцу (вправо)"
									// Первоначальный вариант - if (ColumnVersion < maxVersion)
									if (ColumnVersion < maxVersion && ColumnVersion < (ColumnIndex + 1) * (maxVersion + 1) / Rank + 1)
									{
										// Наращивание версии
										ColumnVersion++;
									}
									else
									{
										// Зачистка текущего столбца, переход к предыдущему
											// Зачищаем версию столбца в массиве версий
											ColumnsVersions[ColumnIndex] = 0;
											// Зачищаем столбец (кроме первой строки и диагоналей) в матрице
											for (int i = 0; i < Rank; i++)
											{
												if (i != 0 && i != ColumnIndex && i != Rank - 1 - ColumnIndex)
												{
													Matrix[i][ColumnIndex] = 0;
												}
											}
											// Переходим к предыдущему столбцу, считывая его версию и наращивая её
											ColumnIndex--;
											ColumnVersion = ColumnsVersions[ColumnIndex] + 1;
									}

									// Создание контрольной точки
									if (++checkpointCounter == CheckpointInterval)
									{
										checkpointCounter = 0;
										Commit();
									}
							}
							while (ColumnIndex != 0 || VectorVersions[ColumnVersion*Rank + 0] == 0);

							// Зачистка первого столбца для новых версий диагоналей
							for (int i = 0; i < Rank; i++)
							{
								if (i != 0 && i != ColumnIndex && i != Rank - 1 - ColumnIndex)
								{
									Matrix[i][ColumnIndex] = 0;
								}
							}
							
							ColumnVersion = 0;
					}

					// Переход к следующей версии
					DiagonalSVersion++;
				}

				// Сброс номера версии побочной диагонали для нового цикла
				DiagonalSVersion = 0;

				// Переход к следующей версии главной диагонали
				DiagonalPVersion++;
		}
		else
		{
			// Переход к следующей версии
			DiagonalPVersion++;
		}
	}
}

// Формирование контрольной точки
void SquareFinder::Commit()
{
	fstream checkpointFile;	// Поток для работы с файлом контрольной точки

	// Формирование контрольной точки
		// Открытие файла
		checkpointFile.open(NewCheckpointFileName, std::ios_base::out);

		// Запись состояния расчёта
		checkpointFile << Rank << " ";
		checkpointFile << FixedLineVersion << " ";
		checkpointFile << DiagonalPVersion << " ";
		checkpointFile << DiagonalSVersion << " ";

		for (int i = 0; i < Rank; i++)
		{
			checkpointFile << ColumnsVersions[i] << " ";
		}

		checkpointFile << ColumnIndex << " ";
		checkpointFile << ColumnVersion;

		// Закрытие файла
		checkpointFile.close();

		// Удаление старого файла контрольной точки
		remove(CheckpointFileName);
								
		// Переименование нового файла контрольной точки
		rename(NewCheckpointFileName, CheckpointFileName);
}


// Восстановление состояния поиска из файла контрольной точки
void SquareFinder::Restore()
{
	fstream checkpointFile;				// Поток для работы с файлом контрольной точки
	int parametersIsRead = 0;			// Флаг считывания входных параметров
	int versionsCount = 0;				// Максимально возможное число версий векторов

	// Cчитывание параметров из файла
		// Открытие файла c данными контрольной точки
		checkpointFile.open(CheckpointFileName, std::fstream::in);
		// Считывание входных параметров
		if (checkpointFile.is_open())
		{
			// Считывание входных параметров из открытого файла контрольной точки
				// Считывание ранга квадрата
				checkpointFile >> Rank;

				// Считывание номера версии фиксированной строки
				checkpointFile >> FixedLineVersion;

				// Считывание версий диагоналей
				checkpointFile >> DiagonalPVersion;
				checkpointFile >> DiagonalSVersion;

				// Считывание версии квадрата
				ColumnsVersions = new int[Rank];
				for (int i = 0; i < Rank; i++)
				{
					checkpointFile >> ColumnsVersions[i];
				}

				// Считывание номера обрабатываемого столбца
				checkpointFile >> ColumnIndex;

				// Считывание последней обработанной версии текущего столбца
				checkpointFile >> ColumnVersion;

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
			Dictionary = new Symbol[Rank];
			for (int i = 0; i < Rank; i++)
			{
				Dictionary[i].Value = i;
				Dictionary[i].IsUsed = 0;
			}

			// Формирование версий векторов
				// Формирование массива c версиями
				versionsCount = Factorial(Rank);
				VectorVersions = new int [versionsCount*Rank];

				// Формирование версий
				for (int i = 0; i < versionsCount; i++)
				{
					// Формирование i-й версии
					CreateVectorVersion(VectorVersions + Rank*i, Rank, i, Dictionary);
				}

			// Формирование фиксированной строки, главной и побочной диагоналей
			FixedLine = new int[Rank];
			DiagonalP = new int[Rank];
			DiagonalS = new int[Rank];

			for (int i = 0; i < Rank; i++)
			{
				FixedLine[i] = VectorVersions[FixedLineVersion*Rank + i];
				DiagonalP[i] = VectorVersions[DiagonalPVersion*Rank + i];
				DiagonalS[i] = VectorVersions[DiagonalSVersion*Rank + i];
			}

			// Выделение памяти под матрицу квадрата
			Matrix = new int*[Rank];
			for (int i = 0; i < Rank; i++)
			{
				Matrix[i] = new int[Rank];
			}

			// Заполнение первой строки матрицы
			for (int i = 0; i < Rank; i++)
			{
				Matrix[0][i] = FixedLine[i];
			}

			// Заполнение столбцов матрицы
			for (int j = 0; j < Rank; j++)
			{
				// Заполение элементов строк j-го столбца, кроме 1-й (она - фиксирована)
				for (int i = 1; i < Rank; i++)
				{
					// Заполнение элемента i-й строки и j-го столбца
					Matrix[i][j] = VectorVersions[ColumnsVersions[j]*Rank + i];
				}
			}
	}
}


// Запись найденного квадрата в файл
void SquareFinder::SaveSquare()
{
	fstream resultFile;					// Поток для работы с файлов результатов

	// Вывод квадрата в файл с результатами
	resultFile.open(ResultFileName, std::ios_base::app);

	// Вывод версий диагоналей
	resultFile << "Diagonals: " << DiagonalPVersion << " " << DiagonalSVersion << "; ";

	// Вывод версий столбцов
	resultFile << "Columns: ";
	for (int i = 0; i < Rank; i++)
	{
		resultFile << ColumnsVersions[i] << " ";
	}
	resultFile << endl;

	// Вывод матрицы квадрата
	for (int printedRowId = 0; printedRowId < Rank; printedRowId++)
	{
		// Вывод очередной строки
			// Вывод элементов строки
			for (int printedColumnId = 0; printedColumnId < Rank; printedColumnId++)
			{
				resultFile << Matrix[printedRowId][printedColumnId] << " ";
			}
			// Закрытие строки
			resultFile << endl;
	}
	resultFile << endl;
	
	resultFile.close();
}


// Проверка совместимости главной диагонали с первой строкой
// diagonalPVersion - версия главной диагонали
int SquareFinder::CheckPrimaryDiagonalCompatibility(int diagonalPVersion)
{
	int duplicationDetected = 0;

	// Проверка корректности первой диагонали, соответствующей заданной версии
	if (VectorVersions[Rank*FixedLineVersion + 0] == VectorVersions[Rank*diagonalPVersion + 0])
	{
		// Проверка не совпадения всех элементов кроме 0-го
		for (int i = 1; i < Rank && !duplicationDetected; i++)
		{
			if (VectorVersions[Rank*FixedLineVersion + i] == VectorVersions[Rank*diagonalPVersion + i])
			{
				duplicationDetected = 1;
			}
		}
	}
	else
	{
		duplicationDetected = 1;
	}

	return !duplicationDetected;
}


// Проверка совместимости побочной диагонали с первой строкой и главной диагональю
// diagonalPVersion - версия главной диагонали
// diagonalSVersion - версия побочной диагонали
int SquareFinder::CheckSecondaryDiagonalCompatibility(int diagonalPVersion, int diagonalSVersion)
{
	int duplicationDetected = 0;

	// Проверка корректности главной и побочной диагонали, соответствующих заданным версиям - 
	// не совпадения всех элементов кроме 0-го для главной диагонали и первой строки и последнего - для побочной диагонали и первой строки
	// а также, в случае нечётного ранга квадрата - совпадения центров диагоналей
	if (VectorVersions[Rank*FixedLineVersion + 0] == VectorVersions[Rank*FixedLineVersion + 0] && VectorVersions[Rank*FixedLineVersion + Rank - 1] == VectorVersions[Rank*diagonalSVersion + Rank - 1])
	{
		// Проверка совпадения центров диагоналей для квадратов нечётного ранга
		if (Rank % 2 == 1 && VectorVersions[Rank*diagonalPVersion + (Rank - 1)/2] != VectorVersions[Rank*diagonalSVersion + (Rank - 1)/2])
		{
			duplicationDetected = 1;
		}

		// Проверка не сопадения остальных элементов
		for (int i = 0; i < Rank && !duplicationDetected; i++)
		{
			// Проверка корректности i-х компонентов диагоналей
				// Сравнение i-го компонента побочной диагонали и первой строки
				if (VectorVersions[Rank*FixedLineVersion + i] == VectorVersions[Rank*diagonalSVersion + i] && i != Rank - 1)
				{
					duplicationDetected = 1;
				}

				// Сравнение i-го компонента главной и побочной диагонали
				if (VectorVersions[Rank*diagonalPVersion + i] == VectorVersions[Rank*diagonalSVersion + i])
				{
					// Проверка попадания на середину квадрата нечётного ранга
					if (Rank % 2 == 0 || i*2 + 1 != Rank)
					{
						duplicationDetected = 1;
					}
				}

				// Сравнение i-го компонента первой диагонали и обратного - (squareRank - 1 - i)-го компонента второй диагонали (сравнение в рамках строки)
				if (VectorVersions[Rank*diagonalPVersion + i] == VectorVersions[Rank*diagonalSVersion + Rank - 1 - i])
				{
					// Проверка попадания на середину квадрата нечётного ранга
					if (Rank % 2 == 0 || i*2 + 1 != Rank)
					{
						duplicationDetected = 1;
					}
				}
		}
	}
	else
	{
		duplicationDetected = 1;
	}

	return !duplicationDetected;
}


// Проверка совместимости столбца с фиксированной строкой, диагоналями и квадратом в целом
// columnIndex - положение столбца, в который хотим записать версию columnVersion
// columnVersion - версия вектора, которую проверяем на возможность записи в столбец columnIndex
int SquareFinder::CheckColumnCompatibility(int columnIndex, int columnVersion)
{
	int duplicationDetected = 0;

	// Проверка совместимости столбца
	if(VectorVersions[columnVersion*Rank + 0] == Matrix[0][columnIndex]
		&& VectorVersions[columnVersion*Rank + columnIndex] == Matrix[columnIndex][columnIndex]
		&& VectorVersions[columnVersion*Rank + Rank - 1 - columnIndex] == Matrix[Rank - 1 - columnIndex][columnIndex])
	{
		// Проверка совместимости всех ячеек столбца с диагоналями
		for (int j = 0; j < Rank && !duplicationDetected; j++)
		{
			// Проверка на совпадение j-го элемента столбца и columnIndex-элемента первой строки
			if (VectorVersions[columnVersion*Rank + j] == Matrix[0][columnIndex] && j != 0)
			{
				duplicationDetected = 1;
			}

			// Проверка на совпадение j-го элемента столбца и j-го элемента главной диагонали
			if (VectorVersions[columnVersion*Rank + j] == Matrix[j][j] && j != columnIndex)
			{
				duplicationDetected = 1;
			}

			// Проверка совпадения j-го элемента столбца и (Rank - j)-го элемента побочной диагонали
			if (VectorVersions[columnVersion*Rank + j] == Matrix[j][Rank - j - 1] && j != Rank - columnIndex - 1)
			{
				duplicationDetected = 1;
			}
		}

		// Проверка совместимости столбца с остальными столбцами
		for (int i = 0; i < columnIndex  && !duplicationDetected; i++)
		{
			// Проверка совместимости заданного столбца с i-м столбцом
			for (int j = 0; j < Rank && !duplicationDetected; j++)
			{
				// Проверка совместимости элементов j заданного столбца и i-го столбца
				if (VectorVersions[columnVersion*Rank + j] == Matrix[j][i])
				{
					duplicationDetected = 1;
				}
			}
		}
	}
	else
	{
		duplicationDetected = 1;
	}

	return !duplicationDetected;
}


// Проверка квадрата на то, что он является латинским
// Примечание: так как квадрат формируется построчно из векторов,
// то проверка по внутри строк - необязательна, но она оставлена
// для того, чтобы функция была универсальнее, а для более быстрого
// отсеивания не латинских квадратов, сначала выполняется проверка
// внутри столбцов
int SquareFinder::IsSquareLatin()
{
	int duplicationDetected = 0;

	// Проверка столбцов квадрата
	for (int columnId = 0; columnId < Rank && !duplicationDetected; columnId++)
	{
		// Проверка корректности столбца columnId
		for (int rowId = 0; rowId < Rank && !duplicationDetected; rowId++)
		{
			// Проверка на совпадение элемента [rowId; columnId] со всеми остальными элементами столбца columndId
			for (int comparedRowId = rowId + 1; comparedRowId < Rank && !duplicationDetected; comparedRowId++)
			{
				if (Matrix[comparedRowId][columnId] == Matrix[rowId][columnId])
				{
					duplicationDetected = 1;
				}
			}
		}
	}

	// Проверка строк квадрата
	for (int rowId = 0; rowId < Rank && !duplicationDetected; rowId++)
	{
		// Проверка корректности строки rowId
		for (int columnId = 0; columnId < Rank && !duplicationDetected; columnId++)
		{
			// Проверка на совпадение элемента [rowId; columnId] со всеми остальными элементами строки rowId
			for (int comparedColumnId = columnId + 1; comparedColumnId < Rank && !duplicationDetected; comparedColumnId++)
			{
				if (Matrix[rowId][columnId] == Matrix[rowId][comparedColumnId])
				{
					duplicationDetected = 1;
				}
			}
		}
	}

	return !duplicationDetected;
}

// Проверка квадрата на то, что он является диагональным
int SquareFinder::IsSquareDiagonal()
{
	int duplicationDetected = 0;

	// Проверка первой диагонали - диагонали [0;0] - [rank;rank]
	for (int itemId = 0; itemId < Rank && !duplicationDetected; itemId++)
	{
		// Проверка на совпадение элемента [itemId; itemId] со всеми остальными элементами диагонали
		for (int comparedId = itemId + 1; comparedId < Rank && !duplicationDetected; comparedId++)
		{
			if (Matrix[itemId][itemId] == Matrix[comparedId][comparedId])
			{
				duplicationDetected = 1;
			}
		}
	}

	// Проверка второй диагонали - диагонали [rank - itemId - 1; itemId]
	for (int itemId = 0; itemId < Rank && !duplicationDetected; itemId ++)
	{
		// Проверка на совпадение элемента [rank - itemId - 1; itemId] со всеми остальными элементами диагонали
		for (int comparedId = itemId + 1; comparedId < Rank && !duplicationDetected; comparedId++)
		{
			if (Matrix[(Rank - itemId - 1)][itemId] == Matrix[(Rank - comparedId - 1)][comparedId])
			{
				duplicationDetected = 1;
			}
		}
	}

	return !duplicationDetected;
}


// Отображение информации и параметрах начала расчётов
void SquareFinder::ShowStartParameters()
{
	// Вывод сообщения о начале расчётов
	cout << "Starting computations with the following parameters:" << endl;
	cout << "Square rank: " << Rank << endl;
	cout << "Square:" << endl;

	for (int i = 0; i < Rank; i++)
	{
		for (int j = 0; j < Rank; j++)
		{
			cout << Matrix[i][j] << ' ';
		}

		cout << endl;
	}
}


int main(int argumentsCount, char* argumentsValues[])
{
	SquareFinder* finder;		// Поисковик квадратов

	// Создание и запуск поисковика
	finder = new SquareFinder();
	finder->Start();

	// Очистка памяти
	delete finder;

	// Завершение работы
	cout << "Press any key to continue..." << endl;
	cin.get();

	return 0;
}