# include <iostream>
# include <fstream>

# include "GreekSquare.h"

using namespace std;

// Конструктор по умолчанию
GreekSquare::GreekSquare()
{
	isInitialized = 0;
}


// Инициализация поиска
void GreekSquare::Initialize()
{
	string pathFileName = "path.txt";								// Путь к файлу с путём заполнения греко-латинского квадрата
	fstream pathFile;												// Поток обращения к файлу

	// Сбрасываем флаг иницилизированности поиска
	isInitialized = 0;

	// Заполняем словарь
		// Заполняем значениями клеток квадратов A и B
		for (int i = 0; i < Rank; i++)
		{
			for (int j = 0; j < Rank; j++)
			{
				dictionary[i*Rank + j].DictionaryId = i*Rank + j;
				dictionary[i*Rank + j].CellAValue = i;
				dictionary[i*Rank + j].CellBValue = j;
			}
		}

		// Выставление ссылок и флагов внутри словаря, организованного как список
		for (int i = 0; i < Rank*Rank; i++)
		{
			// Обработка i-го элемента
				// Выставление ссылок на предыдущий элемент
				if (i > 0)
				{
					dictionary[i].Previous = &dictionary[i - 1];
				}

				// Выставление ссылок на следующий элемент
				if (i < Rank*Rank - 1)
				{
					dictionary[i].Next = &dictionary[i + 1];
				}

				// Выставление флагов первого и последнего элемента в списке
				if (i == 0)
				{
					dictionary[i].IsFirst = 1;
				}
				else
				{
					dictionary[i].IsFirst = 0;
				}

				if (i == Rank*Rank - 1)
				{
					dictionary[i].IsLast = 1;
				}
				else
				{
					dictionary[i].IsLast = 0;
				}
		}

		// Запоминаем голову и хвост списка
		firstFreeItem = &dictionary[0];
		lastFreeItem = &dictionary[Rank*Rank - 1];


	// Считываем пусть обхода квадрата
	pathFile.open(pathFileName.c_str(), std::ios_base::in);

	if (pathFile.is_open())
	{
		for (int i = 0; i < Rank*Rank; i++)
		{
			pathFile >> path[i].RowId;
			pathFile >> path[i].ColumnId;
		}
	}

	pathFile.close();

	// Сброс значений матриц квадратов
	for (int i = 0; i < Rank; i++)
	{
		for (int j = 0; j < Rank; j++)
		{
			square[i][j].DictionaryId = -1;
			square[i][j].CellAValue = -1;
			square[i][j].CellBValue = -1;

			squareA[i][j] = -1;
			squareB[i][j] = -1;
		}
	}

	// Сброс флагов использования значений элементов словаря
	for (int i = 0; i < Rank*Rank; i++)
	{
		dictionaryItemUsed[i] = 0;
	}

	// Сброс номеров последних использованых значений из словаря для клеток матрицы квадрата
	for (int i = 0; i < Rank; i++)
	{
		for (int j = 0; j < Rank; j++)
		{
			squareCellHistory[i][j] = -1;
		}
	}

	// Сброс флагов использования диагоналей
	for (int i = 0; i < Rank; i++)
	{
		pDiagonalA[i] = 0;
		sDiagonalA[i] = 0;
		pDiagonalB[i] = 0;
		sDiagonalB[i] = 0;
	}

	// Сброс флагов использования строк и столбцов
	for (int i = 0; i < Rank; i++)
	{
		for (int j = 0; j < Rank; j++)
		{
			rowValuesA[i][j] = 0;
			rowValuesB[i][j] = 0;
			columnValuesA[i][j] = 0;
			columnValuesB[i][j] = 0;
		}
	}

	// Выставляем флаг инициализированности
	isInitialized = 1;
}


// Запуск поиска
void GreekSquare::StartSearch()
{
	int squareItemId = 0;	// Идентификатор обрабатываемого элемента квадрата из пути по его обходу - path[squareItemId]
	int isGet = 0;			// Флаг нахождения элемента

	int firstLinePassed = 0;	// Флаг соответствия элемента словаря первой строке квадрата (если он стоит в ней)
	int pDiagonalsPassed = 0;	// Флаг соответствия элемента словаря главным диагоналям (если он стоит на них)
	int sDiagonalsPassed = 0;	// Флаг соответствия элемента словаря побочным диагоналям (если он стоит на них)
	int rowsPassed = 0;			// Флаг соответствия элемента словаря строкам латинских квадратов
	int columnsPassed = 0;		// Флаг соответствия элемента словаря столбцам латинских квадратов

	SquareCell currentCell;		// Обрабатываемая клетка квадрата
	SquareCell* oldValue;		// Заменяемое значение клетки греко-латинского квадрата
	SquareCell* newValue;		// Новое значение клетки греко-латинского квадрата
	int newValueId;				// Идентификатор (в словаре) нового значения для клетки квадрата
	int rowId;					// Номер строки с обрабатываемым элементом
	int columnId;				// Номер столбца с обрабатываемым элементом

	int dictionaryAValue;		// Значение клетки квадрата A из элемента словаря
	int dictionaryBValue;		// Значение клетки квадрата B из элемента словаря
	int cellAValue;				// Значение обрабатываемой клетки квадрата А
	int cellBValue;				// Значение обрабатываемой клетки квадрата B

	int oldAValue;
	int oldBValue;
	int newAValue;
	int newBValue;

	/*int startIndex;*/
	SquareCell* dictionaryItem;
	int itemLocation;

	int counter = 0;			// Счётчик циклов

	// Поиск греко-латинских квадратов
	while (squareItemId >= 0)
	{
		// Заполняем текущий элемент квадрата
			// Поиск подходящего элемента словаря
			rowId = path[squareItemId].RowId;
			columnId = path[squareItemId].ColumnId;
			/*currentCell = square[path[squareItemId].RowId][path[squareItemId].ColumnId];*/
			currentCell = square[rowId][columnId];
			cellAValue = currentCell.CellAValue;
			cellBValue = currentCell.CellBValue;

			isGet = 0;
			newValueId = -1;

			/* startIndex = squareCellHistory[rowId][columnId] + 1; */
			/*startIndex = firstFreeItem->DictionaryId;*/

			/*for (int i = startIndex; i < Rank*Rank && !isGet; i++)*/
			dictionaryItem = firstFreeItem;
			while (!isGet && !dictionaryItem->IsLast)
			{
				// Обработка i-го элемента словаря
				/*if (dictionaryItemUsed[i] == 0)*/
				if (dictionaryItemUsed[dictionaryItem->DictionaryId] == 0)
				{
					// Обработка свободного, i-го элемента словаря.
						// Сброс флагов, считывание значений
						isGet = 1;
						/*dictionaryAValue = dictionary[i].CellAValue;
						dictionaryBValue = dictionary[i].CellBValue;*/
						dictionaryAValue = dictionaryItem->CellAValue;
						dictionaryBValue = dictionaryItem->CellBValue;

						// Проверка элемента на соответствие первой строке
						/*if (rowId == 0 && (columnId != dictionary[i].CellAValue || columnId != dictionary[i].CellBValue))*/
						if (rowId == 0 && (columnId != dictionaryAValue || columnId != dictionaryBValue))
						{
							isGet = 0;
						}

						// Проверка элемента на соответствие диагоналям
							// Проверка на соответствие основным диагоналям
							// Условие вида dictionaryAValue != cellAValue вызвано тем, что в квадрате уже легально может быть проверяемое значение
							// Например при переходе от значения 30 к 31 - в квадрате A 3 как было, так и осталось. Далее - смысл проверки тот же
							if (isGet)
							{
								/*if (rowId == columnId &&
									((pDiagonalA[dictionary[i].CellAValue] != 0 && dictionary[i].CellAValue != currentCell.CellAValue)
										||
									(pDiagonalB[dictionary[i].CellBValue] != 0 && dictionary[i].CellBValue != currentCell.CellBValue)))*/
								if (rowId == columnId &&
									((pDiagonalA[dictionaryAValue] != 0 && dictionaryAValue != cellAValue)
										||
									(pDiagonalB[dictionaryBValue] != 0 && dictionaryBValue != cellBValue)))
								{
									isGet = 0;
								}
							}

							// Проверка на соответствие побочным диагоналям
							if (isGet)
							{
								/*if (Rank - rowId - 1 == columnId &&
									((sDiagonalA[dictionary[i].CellAValue] != 0 && dictionary[i].CellAValue != currentCell.CellAValue)
										||
									(sDiagonalB[dictionary[i].CellBValue] != 0 && dictionary[i].CellBValue != currentCell.CellBValue)))*/
								if (Rank - rowId - 1 == columnId &&
									((sDiagonalA[dictionaryAValue] != 0 && dictionaryAValue != cellAValue)
										||
									(sDiagonalB[dictionaryBValue] != 0 && dictionaryBValue != cellBValue)))
								{
									isGet = 0;
								}
							}

						// Проверка элемента на соответствие строкам
						if (isGet)
						{
							/*if ((rowValuesA[rowId][dictionary[i].CellAValue] == 1 && dictionary[i].CellAValue != currentCell.CellAValue)
								|| (rowValuesB[rowId][dictionary[i].CellBValue] == 1 && dictionary[i].CellBValue != currentCell.CellBValue))*/
							if ((rowValuesA[rowId][dictionaryAValue] == 1 && dictionaryAValue != cellAValue)
								|| (rowValuesB[rowId][dictionaryBValue] == 1 && dictionaryBValue != cellBValue))
							{
								isGet = 0;
							}
						}

						// Проверка элемента на соответствие столбцам
						if (isGet)
						{
							/*if ((columnValuesA[columnId][dictionary[i].CellAValue] == 1 && dictionary[i].CellAValue != currentCell.CellAValue)
								|| (columnValuesB[columnId][dictionary[i].CellBValue] == 1 && dictionary[i].CellBValue != currentCell.CellBValue))*/
							if ((columnValuesA[columnId][dictionaryAValue] == 1 && dictionaryAValue != cellAValue)
								|| (columnValuesB[columnId][dictionaryBValue] == 1 && dictionaryBValue != cellBValue))
							{
								isGet = 0;
							}
						}

						// Подведение итогов проверки
						if (isGet)
						{
							newValueId = dictionaryItem->DictionaryId;
						}
						else
						{
							dictionaryItem = dictionaryItem->Next;
						}
				}
				else
				{
					dictionaryItem = dictionaryItem->Next;
				}
			}

			// Обработка найденного элемента словаря
			if (isGet)
			{
				// Запись нового значения в клетку квадратов
					// Считывание нового и заменяемого значения
					oldValue = &square[rowId][columnId];
					newValue = &dictionary[newValueId];
					oldAValue = oldValue->CellAValue;
					oldBValue = oldValue->CellBValue;
					newAValue = newValue->CellAValue;
					newBValue = newValue->CellBValue;

					// "Высвобождение" заменяемого значения в массивах флагов
					if (oldValue->DictionaryId >= 0)
					{
						// Очистка флагов для заменяемого элемента
							// Убирание флага в массиве используемых сейчас элементов словаря
							dictionaryItemUsed[oldValue->DictionaryId] = 0;

							// Убирание флагов в массивах элементов диагоналей
							if (rowId == columnId)
							{
								/*pDiagonalA[oldValue.CellAValue] = 0;
								pDiagonalB[oldValue.CellBValue] = 0;*/
								pDiagonalA[oldAValue] = 0;
								pDiagonalB[oldBValue] = 0;
							}

							if (Rank - rowId - 1 == columnId)
							{
								/*sDiagonalA[oldValue.CellAValue] = 0;
								sDiagonalB[oldValue.CellBValue] = 0;*/
								sDiagonalA[oldAValue] = 0;
								sDiagonalB[oldBValue] = 0;
							}

							// Убирание флагов в массивах элементов строк
							/*rowValuesA[rowId][oldValue.CellAValue] = 0;
							rowValuesB[rowId][oldValue.CellBValue] = 0;*/
							rowValuesA[rowId][oldAValue] = 0;
							rowValuesB[rowId][oldBValue] = 0;

							// Убирание флагов в массивах элементов столбцов
							/*columnValuesA[columnId][oldValue.CellAValue] = 0;
							columnValuesB[columnId][oldValue.CellBValue] = 0;*/
							columnValuesA[columnId][oldAValue] = 0;
							columnValuesB[columnId][oldBValue] = 0;

							// Возвращение элемента словаря в связанный список
								// Определение относительного положения элемента
								if (firstFreeItem == 0 && lastFreeItem == 0)
								{
									// Список пуст - элемент будет первым
									itemLocation = 1;
								}

								if (firstFreeItem != 0 && oldValue->DictionaryId < firstFreeItem->DictionaryId)
								{
									// Элемент будет добавлен в начало списка
									itemLocation = 2;
								}

								if (lastFreeItem != 0 && oldValue->DictionaryId > lastFreeItem->DictionaryId)
								{
									// Элемент будет добавлен в конец списка
									itemLocation = 3;
								}

								if (firstFreeItem != 0 && lastFreeItem != 0 && firstFreeItem->DictionaryId < oldValue->DictionaryId && oldValue->DictionaryId < lastFreeItem->DictionaryId)
								{
									// Элемент будет добавлен в середину списка
									itemLocation = 4;
								}

								// Возвращение элемента в соответствующее положение в спике
								switch (itemLocation)
								{
									case 1:
									// Возвращение элемента в пустой список
									/*if (firstFreeItem == 0 && lastFreeItem == 0)
									{*/
										oldValue->IsFirst = 1;
										oldValue->IsLast = 1;
										oldValue->Previous = 0;
										oldValue->Next = 0;

										firstFreeItem = oldValue;
										lastFreeItem = oldValue;
									/*}*/

									break;

									case 2:
									// Возвращение элемента в начало списка
									/*if (firstFreeItem != 0 && oldValue->DictionaryId < firstFreeItem->DictionaryId)
									{*/
										oldValue->IsFirst = 1;
										oldValue->IsLast = 0;
										oldValue->Previous = 0;
										oldValue->Next = firstFreeItem;

										firstFreeItem = oldValue;
									/*}*/

									break;

									case 3:
									// Возвращение элемента в конец списка
									/*if (lastFreeItem != 0 && oldValue->DictionaryId > lastFreeItem->DictionaryId)
									{*/
										oldValue->IsFirst = 0;
										oldValue->IsLast = 1;
										oldValue->Previous = lastFreeItem;
										oldValue->Next = 0;

										lastFreeItem = oldValue;
									/*}*/
									break;

									case 4:
									// Возвращение элемента в середину списка
									/*if (firstFreeItem != 0 && lastFreeItem != 0 && oldValue->DictionaryId < firstFreeItem->DictionaryId && oldValue->DictionaryId < lastFreeItem->DictionaryId)
									{*/
										// Поиск предыдущего и последующего элемента
											// Поиск предыдущего элемента
											for (int i = oldValue->DictionaryId - 1; i >= 0; i--)
											{
												// Проверка i-го элемента на то, что он является предыдущим элементом для возвращаемого oldValue
												if (dictionaryItemUsed[i] == 0)
												{
													dictionary[i].Next = oldValue;
													oldValue->Previous = &dictionary[i];
													dictionary[i].IsLast = 0;
													oldValue->IsFirst = 0;
												}
											}

											// Поиск следующего элемента
											for (int i = oldValue->DictionaryId + 1; i < Rank*Rank; i++)
											{
												// Проверка i-го элемента на то, что он является следующим элементом для возвращаемого oldValue
												if (dictionaryItemUsed[i] == 0)
												{
													dictionary[i].Previous = oldValue;
													oldValue->Next = &dictionary[i];
													dictionary[i].IsFirst = 0;
													oldValue->IsLast = 0;
												}
											}
									/*}*/
									break;
								}
					}

					// Запись нового значения
						// Запись значения в квадраты
						square[rowId][columnId] = *newValue;
						/*squareA[rowId][columnId]= newValue.CellAValue;
						squareB[rowId][columnId]= newValue.CellBValue;*/
						squareA[rowId][columnId]= newAValue;
						squareB[rowId][columnId]= newBValue;

						// Отмечание значений в массивах флагов
							// В массиве флагов использованных значений
							dictionaryItemUsed[newValue->DictionaryId] = 1;

							// В истории значений клеток греко-латинского квадрата
							squareCellHistory[rowId][columnId] = newValue->DictionaryId;

							// В текущих значениях главных диагоналей
							if (rowId == columnId)
							{
								/*pDiagonalA[newValue.CellAValue] = 1;
								pDiagonalB[newValue.CellBValue] = 1;*/
								pDiagonalA[newAValue] = 1;
								pDiagonalB[newBValue] = 1;
							}

							// В текущих значениях побочных диагоналей
							if (Rank - rowId - 1 == columnId)
							{
								/*sDiagonalA[newValue.CellAValue] = 1;
								sDiagonalB[newValue.CellBValue] = 1;*/
								sDiagonalA[newAValue] = 1;
								sDiagonalB[newBValue] = 1;
							}

							// В текущих значениях строк
							/*rowValuesA[rowId][newValue.CellAValue] = 1;
							rowValuesB[rowId][newValue.CellBValue] = 1;*/
							rowValuesA[rowId][newAValue] = 1;
							rowValuesB[rowId][newBValue] = 1;

							// В текущих значениях столбцов
							/*columnValuesA[columnId][newValue.CellAValue] = 1;
							columnValuesB[columnId][newValue.CellBValue] = 1;*/
							columnValuesA[columnId][newAValue] = 1;
							columnValuesB[columnId][newBValue] = 1;

						// Убирание нового значения из списка свободных
							// Определение относительного положения элемента
							if (newValue->IsFirst && newValue->IsLast)
							{
								// Убираем последний элемент списка
								itemLocation = 1;
							}

							if (newValue->IsFirst && !newValue->IsLast)
							{
								// Убираем элемент из головы списка
								itemLocation = 2;
							}

							if (!newValue->IsFirst && newValue->IsLast)
							{
								// Убираем элемент их конца списка
								itemLocation = 3;
							}

							if (firstFreeItem != 0 && lastFreeItem != 0 && firstFreeItem->DictionaryId < newValue->DictionaryId && newValue->DictionaryId < lastFreeItem->DictionaryId)
							{
								// Убираем элемента из середины списка
								itemLocation = 4;
							}

							// Фактическо изменение указателей
							switch(itemLocation)
							{
								case 1:
								// Убирание последнего элемента списка
								/*if (newValue->IsFirst && newValue->IsLast)
								{*/
									firstFreeItem = 0;
									lastFreeItem = 0;
								
									firstFreeItem = 0;
									lastFreeItem = 0;
									newValue->Previous = 0;
									newValue->Next = 0;
								/*}*/

								break;

								case 2:
								// Убирание элемента из головы списка
								/*if (newValue->IsFirst && !newValue->IsLast)
								{*/
									newValue->Next->IsFirst = 1;
									newValue->Next->Previous = 0;
								
									firstFreeItem = newValue->Next;
									newValue->Previous = 0;
									newValue->Next = 0;
								/*}*/
								break;

								case 3:
								// Убирание элемента из хвоста списка
								/*if (!newValue->IsFirst && newValue->IsLast)
								{*/
									newValue->Previous->IsLast = 1;
									newValue->Previous->Next = 0;

									lastFreeItem = newValue->Previous;
									newValue->Previous = 0;
									newValue->Next = 0;
								/*}*/
								break;

								case 4:
								// Убирание элемента из середины списка
								/*if (!newValue->IsFirst && !newValue->IsLast)
								{*/
									newValue->Previous->Next = newValue->Next;
									newValue->Next->Previous = newValue->Previous;

									newValue->Previous = 0;
									newValue->Next = 0;
								/*}*/
								break;
							}

			}

			// Переход к следующей клетке квадрата
			if (isGet)
			{
				// Переход к следующей клетке
				if (squareItemId < Rank*Rank - 1)
				{
					squareItemId++;
				}
				else
				{
					// Обработка найденного греко-латинского квадрата
					SaveSquare();
				}
			}
			else
			{
				// Переход к предыдущей клетке
					// Считывание текущего значения
					oldValue = &square[rowId][columnId];
					oldAValue = oldValue->CellAValue;
					oldBValue = oldValue->CellBValue;

					// Обработка текущего значения
					if (oldValue->DictionaryId >= 0)
					{
						// Очистка значений, связанных с предыдущим значением
							// Очистка значения текущей клетки
							square[rowId][columnId].DictionaryId = -1;
							square[rowId][columnId].CellAValue = -1;
							square[rowId][columnId].CellBValue = -1;

							squareA[rowId][columnId] = -1;
							squareB[rowId][columnId] = -1;

							// Очистка истории значений клетки греко-латинского квадрата
							/*for (int i = 0; i < Rank*Rank; i++)
							{*/
								squareCellHistory[rowId][columnId] = -1;
							/*}*/

							// Очистка флагов
								// Убирание флага в массиве используемых сейчас элементов словаря
								dictionaryItemUsed[oldValue->DictionaryId] = 0;

								// Убирание флагов в массивах элементов диагоналей
								if (rowId == columnId)
								{
									/*pDiagonalA[oldValue.CellAValue] = 0;
									pDiagonalB[oldValue.CellBValue] = 0;*/
									pDiagonalA[oldAValue] = 0;
									pDiagonalB[oldBValue] = 0;
								}

								if (Rank - rowId - 1 == columnId)
								{
									/*sDiagonalA[oldValue.CellAValue] = 0;
									sDiagonalB[oldValue.CellBValue] = 0;*/
									sDiagonalA[oldAValue] = 0;
									sDiagonalB[oldBValue] = 0;
								}

								// Убирание флагов в массивах элементов строк
								/*rowValuesA[rowId][oldValue.CellAValue] = 0;
								rowValuesB[rowId][oldValue.CellBValue] = 0;*/
								rowValuesA[rowId][oldAValue] = 0;
								rowValuesB[rowId][oldBValue] = 0;

								// Убирание флагов в массивах элементов столбцов
								/*columnValuesA[columnId][oldValue.CellAValue] = 0;
								columnValuesB[columnId][oldValue.CellBValue] = 0;*/
								columnValuesA[columnId][oldAValue] = 0;
								columnValuesB[columnId][oldBValue] = 0;

								// Возвращение элемента словаря в связанный список свободных элементов
									// Определение относительного положения элемента
									if (firstFreeItem == 0 && lastFreeItem == 0)
									{
										// Список пуст - элемент будет первым
										itemLocation = 1;
									}

									if (firstFreeItem != 0 && oldValue->DictionaryId < firstFreeItem->DictionaryId)
									{
										// Элемент будет добавлен в начало списка
										itemLocation = 2;
									}

									if (lastFreeItem != 0 && oldValue->DictionaryId > lastFreeItem->DictionaryId)
									{
										// Элемент будет добавлен в конец списка
										itemLocation = 3;
									}

									if (firstFreeItem != 0 && lastFreeItem != 0 && firstFreeItem->DictionaryId < oldValue->DictionaryId && oldValue->DictionaryId < lastFreeItem->DictionaryId)
									{
										// Элемент будет добавлен в середину списка
										itemLocation = 4;
									}

									// Фактическое изменение указателей
									switch (itemLocation)
									{
										case 1:
											// Возвращение элемента в пустой список
										/*if (firstFreeItem == 0 && lastFreeItem == 0)
										{*/
											oldValue->IsFirst = 1;
											oldValue->IsLast = 1;
											oldValue->Previous = 0;
											oldValue->Next = 0;
	
											firstFreeItem = oldValue;
											lastFreeItem = oldValue;
										/*}*/
										break;

										case 2:
											// Возвращение элемента в начало списка
										/*if (firstFreeItem != 0 && oldValue->DictionaryId < firstFreeItem->DictionaryId)
										{*/
											oldValue->IsFirst = 1;
											oldValue->IsLast = 0;
											oldValue->Previous = 0;
											oldValue->Next = firstFreeItem;

											firstFreeItem = oldValue;
										/*}*/
										break;

										case 3:
											// Возвращение элемента в конец списка
										/*if (lastFreeItem != 0 && oldValue->DictionaryId > lastFreeItem->DictionaryId)
										{*/
											oldValue->IsFirst = 0;
											oldValue->IsLast = 1;
											oldValue->Previous = lastFreeItem;
											oldValue->Next = 0;

											lastFreeItem = oldValue;
										/*}*/
										break;

										case 4:
											// Возвращение элемента в середину списка
										/*if (firstFreeItem != 0 && lastFreeItem != 0 && firstFreeItem->DictionaryId < oldValue->DictionaryId && oldValue->DictionaryId < lastFreeItem->DictionaryId)
										{*/
											// Поиск предыдущего и последующего элемента
												// Поиск предыдущего элемента
												for (int i = oldValue->DictionaryId - 1; i >= 0; i--)
												{
													// Проверка i-го элемента на то, что он является предыдущим элементом для возвращаемого oldValue
													if (dictionaryItemUsed[i] == 0)
													{
														dictionary[i].Next = oldValue;
														oldValue->Previous = &dictionary[i];
														dictionary[i].IsLast = 0;
														oldValue->IsFirst = 0;
													}
												}

												// Поиск следующего элемента
												for (int i = oldValue->DictionaryId + 1; i < Rank*Rank; i++)
												{
													// Проверка i-го элемента на то, что он является следующим элементом для возвращаемого oldValue
													if (dictionaryItemUsed[i] == 0)
													{
														dictionary[i].Previous = oldValue;
														oldValue->Next = &dictionary[i];
														dictionary[i].IsFirst = 0;
														oldValue->IsLast = 0;
													}
												}
										/*}*/
										break;
									}
					}

					// Переход к предыдущему элементу
					squareItemId--;
			}

			// Обработка счётчика
			counter++;
			if (counter % 10000000 == 0)
			{
				counter = 0;
				cout << "-------------------------" << endl;
				cout << "Now processing the square:" << endl;
				for (int i = 0; i < Rank; i++)
				{
					for (int j = 0; j < Rank; j++)
					{
						cout << square[i][j].CellAValue << square[i][j].CellBValue << " ";
					}
					cout << endl;
					}
				cout << endl;
			}
	}

}


// Запись найденного греко-латинского квадрата в файл
void GreekSquare::SaveSquare()
{
	string fileName = "result.txt";
	fstream resultFile;

	resultFile.open(fileName.c_str(), std::ios_base::app);

	if (resultFile.is_open())
	{
		resultFile << "------------------------" << endl;
		resultFile << "Detected orthogonal square(s) for the square:" << endl;

		// Вывод найденного греко-латинского квадрата
		if (WriteGreekSquare)
		{
			for (int i = 0; i < Rank; i++)
			{
				for (int j = 0; j < Rank; j++)
				{
					cout << square[i][j].CellAValue << square[i][j].CellBValue << " ";
				}
				cout << endl;
			}
			cout << endl;
		}

		// Вывод квадрата A
		for (int i = 0; i < Rank; i++)
		{
			for (int j = 0; j < Rank; j++)
			{
				resultFile << square[i][j].CellAValue << " ";
			}
			resultFile << endl;
		}
		resultFile << endl;

		resultFile << "------------------------" << endl;

		// Вывод квадрата B
		for (int i = 0; i < Rank; i++)
		{
			for (int j = 0; j < Rank; j++)
			{
				resultFile << square[i][j].CellBValue << " ";
			}
			resultFile << endl;
		}
		resultFile << endl;
	}

	resultFile.close();
}