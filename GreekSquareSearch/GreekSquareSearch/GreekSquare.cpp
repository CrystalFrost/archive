# include <iostream>
# include <fstream>

# include "GreekSquare.h"

using namespace std;

// ����������� �� ���������
GreekSquare::GreekSquare()
{
	isInitialized = 0;
}


// ������������� ������
void GreekSquare::Initialize()
{
	string pathFileName = "path.txt";								// ���� � ����� � ���� ���������� �����-���������� ��������
	fstream pathFile;												// ����� ��������� � �����

	// ���������� ���� ������������������� ������
	isInitialized = 0;

	// ��������� �������
		// ��������� ���������� ������ ��������� A � B
		for (int i = 0; i < Rank; i++)
		{
			for (int j = 0; j < Rank; j++)
			{
				dictionary[i*Rank + j].DictionaryId = i*Rank + j;
				dictionary[i*Rank + j].CellAValue = i;
				dictionary[i*Rank + j].CellBValue = j;
			}
		}

		// ����������� ������ � ������ ������ �������, ��������������� ��� ������
		for (int i = 0; i < Rank*Rank; i++)
		{
			// ��������� i-�� ��������
				// ����������� ������ �� ���������� �������
				if (i > 0)
				{
					dictionary[i].Previous = &dictionary[i - 1];
				}

				// ����������� ������ �� ��������� �������
				if (i < Rank*Rank - 1)
				{
					dictionary[i].Next = &dictionary[i + 1];
				}

				// ����������� ������ ������� � ���������� �������� � ������
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

		// ���������� ������ � ����� ������
		firstFreeItem = &dictionary[0];
		lastFreeItem = &dictionary[Rank*Rank - 1];


	// ��������� ����� ������ ��������
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

	// ����� �������� ������ ���������
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

	// ����� ������ ������������� �������� ��������� �������
	for (int i = 0; i < Rank*Rank; i++)
	{
		dictionaryItemUsed[i] = 0;
	}

	// ����� ������� ��������� ������������� �������� �� ������� ��� ������ ������� ��������
	for (int i = 0; i < Rank; i++)
	{
		for (int j = 0; j < Rank; j++)
		{
			squareCellHistory[i][j] = -1;
		}
	}

	// ����� ������ ������������� ����������
	for (int i = 0; i < Rank; i++)
	{
		pDiagonalA[i] = 0;
		sDiagonalA[i] = 0;
		pDiagonalB[i] = 0;
		sDiagonalB[i] = 0;
	}

	// ����� ������ ������������� ����� � ��������
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

	// ���������� ���� ��������������������
	isInitialized = 1;
}


// ������ ������
void GreekSquare::StartSearch()
{
	int squareItemId = 0;	// ������������� ��������������� �������� �������� �� ���� �� ��� ������ - path[squareItemId]
	int isGet = 0;			// ���� ���������� ��������

	int firstLinePassed = 0;	// ���� ������������ �������� ������� ������ ������ �������� (���� �� ����� � ���)
	int pDiagonalsPassed = 0;	// ���� ������������ �������� ������� ������� ���������� (���� �� ����� �� ���)
	int sDiagonalsPassed = 0;	// ���� ������������ �������� ������� �������� ���������� (���� �� ����� �� ���)
	int rowsPassed = 0;			// ���� ������������ �������� ������� ������� ��������� ���������
	int columnsPassed = 0;		// ���� ������������ �������� ������� �������� ��������� ���������

	SquareCell currentCell;		// �������������� ������ ��������
	SquareCell* oldValue;		// ���������� �������� ������ �����-���������� ��������
	SquareCell* newValue;		// ����� �������� ������ �����-���������� ��������
	int newValueId;				// ������������� (� �������) ������ �������� ��� ������ ��������
	int rowId;					// ����� ������ � �������������� ���������
	int columnId;				// ����� ������� � �������������� ���������

	int dictionaryAValue;		// �������� ������ �������� A �� �������� �������
	int dictionaryBValue;		// �������� ������ �������� B �� �������� �������
	int cellAValue;				// �������� �������������� ������ �������� �
	int cellBValue;				// �������� �������������� ������ �������� B

	int oldAValue;
	int oldBValue;
	int newAValue;
	int newBValue;

	/*int startIndex;*/
	SquareCell* dictionaryItem;
	int itemLocation;

	int counter = 0;			// ������� ������

	// ����� �����-��������� ���������
	while (squareItemId >= 0)
	{
		// ��������� ������� ������� ��������
			// ����� ����������� �������� �������
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
				// ��������� i-�� �������� �������
				/*if (dictionaryItemUsed[i] == 0)*/
				if (dictionaryItemUsed[dictionaryItem->DictionaryId] == 0)
				{
					// ��������� ����������, i-�� �������� �������.
						// ����� ������, ���������� ��������
						isGet = 1;
						/*dictionaryAValue = dictionary[i].CellAValue;
						dictionaryBValue = dictionary[i].CellBValue;*/
						dictionaryAValue = dictionaryItem->CellAValue;
						dictionaryBValue = dictionaryItem->CellBValue;

						// �������� �������� �� ������������ ������ ������
						/*if (rowId == 0 && (columnId != dictionary[i].CellAValue || columnId != dictionary[i].CellBValue))*/
						if (rowId == 0 && (columnId != dictionaryAValue || columnId != dictionaryBValue))
						{
							isGet = 0;
						}

						// �������� �������� �� ������������ ����������
							// �������� �� ������������ �������� ����������
							// ������� ���� dictionaryAValue != cellAValue ������� ���, ��� � �������� ��� �������� ����� ���� ����������� ��������
							// �������� ��� �������� �� �������� 30 � 31 - � �������� A 3 ��� ����, ��� � ��������. ����� - ����� �������� ��� ��
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

							// �������� �� ������������ �������� ����������
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

						// �������� �������� �� ������������ �������
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

						// �������� �������� �� ������������ ��������
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

						// ���������� ������ ��������
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

			// ��������� ���������� �������� �������
			if (isGet)
			{
				// ������ ������ �������� � ������ ���������
					// ���������� ������ � ����������� ��������
					oldValue = &square[rowId][columnId];
					newValue = &dictionary[newValueId];
					oldAValue = oldValue->CellAValue;
					oldBValue = oldValue->CellBValue;
					newAValue = newValue->CellAValue;
					newBValue = newValue->CellBValue;

					// "�������������" ����������� �������� � �������� ������
					if (oldValue->DictionaryId >= 0)
					{
						// ������� ������ ��� ����������� ��������
							// �������� ����� � ������� ������������ ������ ��������� �������
							dictionaryItemUsed[oldValue->DictionaryId] = 0;

							// �������� ������ � �������� ��������� ����������
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

							// �������� ������ � �������� ��������� �����
							/*rowValuesA[rowId][oldValue.CellAValue] = 0;
							rowValuesB[rowId][oldValue.CellBValue] = 0;*/
							rowValuesA[rowId][oldAValue] = 0;
							rowValuesB[rowId][oldBValue] = 0;

							// �������� ������ � �������� ��������� ��������
							/*columnValuesA[columnId][oldValue.CellAValue] = 0;
							columnValuesB[columnId][oldValue.CellBValue] = 0;*/
							columnValuesA[columnId][oldAValue] = 0;
							columnValuesB[columnId][oldBValue] = 0;

							// ����������� �������� ������� � ��������� ������
								// ����������� �������������� ��������� ��������
								if (firstFreeItem == 0 && lastFreeItem == 0)
								{
									// ������ ���� - ������� ����� ������
									itemLocation = 1;
								}

								if (firstFreeItem != 0 && oldValue->DictionaryId < firstFreeItem->DictionaryId)
								{
									// ������� ����� �������� � ������ ������
									itemLocation = 2;
								}

								if (lastFreeItem != 0 && oldValue->DictionaryId > lastFreeItem->DictionaryId)
								{
									// ������� ����� �������� � ����� ������
									itemLocation = 3;
								}

								if (firstFreeItem != 0 && lastFreeItem != 0 && firstFreeItem->DictionaryId < oldValue->DictionaryId && oldValue->DictionaryId < lastFreeItem->DictionaryId)
								{
									// ������� ����� �������� � �������� ������
									itemLocation = 4;
								}

								// ����������� �������� � ��������������� ��������� � �����
								switch (itemLocation)
								{
									case 1:
									// ����������� �������� � ������ ������
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
									// ����������� �������� � ������ ������
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
									// ����������� �������� � ����� ������
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
									// ����������� �������� � �������� ������
									/*if (firstFreeItem != 0 && lastFreeItem != 0 && oldValue->DictionaryId < firstFreeItem->DictionaryId && oldValue->DictionaryId < lastFreeItem->DictionaryId)
									{*/
										// ����� ����������� � ������������ ��������
											// ����� ����������� ��������
											for (int i = oldValue->DictionaryId - 1; i >= 0; i--)
											{
												// �������� i-�� �������� �� ��, ��� �� �������� ���������� ��������� ��� ������������� oldValue
												if (dictionaryItemUsed[i] == 0)
												{
													dictionary[i].Next = oldValue;
													oldValue->Previous = &dictionary[i];
													dictionary[i].IsLast = 0;
													oldValue->IsFirst = 0;
												}
											}

											// ����� ���������� ��������
											for (int i = oldValue->DictionaryId + 1; i < Rank*Rank; i++)
											{
												// �������� i-�� �������� �� ��, ��� �� �������� ��������� ��������� ��� ������������� oldValue
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

					// ������ ������ ��������
						// ������ �������� � ��������
						square[rowId][columnId] = *newValue;
						/*squareA[rowId][columnId]= newValue.CellAValue;
						squareB[rowId][columnId]= newValue.CellBValue;*/
						squareA[rowId][columnId]= newAValue;
						squareB[rowId][columnId]= newBValue;

						// ��������� �������� � �������� ������
							// � ������� ������ �������������� ��������
							dictionaryItemUsed[newValue->DictionaryId] = 1;

							// � ������� �������� ������ �����-���������� ��������
							squareCellHistory[rowId][columnId] = newValue->DictionaryId;

							// � ������� ��������� ������� ����������
							if (rowId == columnId)
							{
								/*pDiagonalA[newValue.CellAValue] = 1;
								pDiagonalB[newValue.CellBValue] = 1;*/
								pDiagonalA[newAValue] = 1;
								pDiagonalB[newBValue] = 1;
							}

							// � ������� ��������� �������� ����������
							if (Rank - rowId - 1 == columnId)
							{
								/*sDiagonalA[newValue.CellAValue] = 1;
								sDiagonalB[newValue.CellBValue] = 1;*/
								sDiagonalA[newAValue] = 1;
								sDiagonalB[newBValue] = 1;
							}

							// � ������� ��������� �����
							/*rowValuesA[rowId][newValue.CellAValue] = 1;
							rowValuesB[rowId][newValue.CellBValue] = 1;*/
							rowValuesA[rowId][newAValue] = 1;
							rowValuesB[rowId][newBValue] = 1;

							// � ������� ��������� ��������
							/*columnValuesA[columnId][newValue.CellAValue] = 1;
							columnValuesB[columnId][newValue.CellBValue] = 1;*/
							columnValuesA[columnId][newAValue] = 1;
							columnValuesB[columnId][newBValue] = 1;

						// �������� ������ �������� �� ������ ���������
							// ����������� �������������� ��������� ��������
							if (newValue->IsFirst && newValue->IsLast)
							{
								// ������� ��������� ������� ������
								itemLocation = 1;
							}

							if (newValue->IsFirst && !newValue->IsLast)
							{
								// ������� ������� �� ������ ������
								itemLocation = 2;
							}

							if (!newValue->IsFirst && newValue->IsLast)
							{
								// ������� ������� �� ����� ������
								itemLocation = 3;
							}

							if (firstFreeItem != 0 && lastFreeItem != 0 && firstFreeItem->DictionaryId < newValue->DictionaryId && newValue->DictionaryId < lastFreeItem->DictionaryId)
							{
								// ������� �������� �� �������� ������
								itemLocation = 4;
							}

							// ���������� ��������� ����������
							switch(itemLocation)
							{
								case 1:
								// �������� ���������� �������� ������
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
								// �������� �������� �� ������ ������
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
								// �������� �������� �� ������ ������
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
								// �������� �������� �� �������� ������
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

			// ������� � ��������� ������ ��������
			if (isGet)
			{
				// ������� � ��������� ������
				if (squareItemId < Rank*Rank - 1)
				{
					squareItemId++;
				}
				else
				{
					// ��������� ���������� �����-���������� ��������
					SaveSquare();
				}
			}
			else
			{
				// ������� � ���������� ������
					// ���������� �������� ��������
					oldValue = &square[rowId][columnId];
					oldAValue = oldValue->CellAValue;
					oldBValue = oldValue->CellBValue;

					// ��������� �������� ��������
					if (oldValue->DictionaryId >= 0)
					{
						// ������� ��������, ��������� � ���������� ���������
							// ������� �������� ������� ������
							square[rowId][columnId].DictionaryId = -1;
							square[rowId][columnId].CellAValue = -1;
							square[rowId][columnId].CellBValue = -1;

							squareA[rowId][columnId] = -1;
							squareB[rowId][columnId] = -1;

							// ������� ������� �������� ������ �����-���������� ��������
							/*for (int i = 0; i < Rank*Rank; i++)
							{*/
								squareCellHistory[rowId][columnId] = -1;
							/*}*/

							// ������� ������
								// �������� ����� � ������� ������������ ������ ��������� �������
								dictionaryItemUsed[oldValue->DictionaryId] = 0;

								// �������� ������ � �������� ��������� ����������
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

								// �������� ������ � �������� ��������� �����
								/*rowValuesA[rowId][oldValue.CellAValue] = 0;
								rowValuesB[rowId][oldValue.CellBValue] = 0;*/
								rowValuesA[rowId][oldAValue] = 0;
								rowValuesB[rowId][oldBValue] = 0;

								// �������� ������ � �������� ��������� ��������
								/*columnValuesA[columnId][oldValue.CellAValue] = 0;
								columnValuesB[columnId][oldValue.CellBValue] = 0;*/
								columnValuesA[columnId][oldAValue] = 0;
								columnValuesB[columnId][oldBValue] = 0;

								// ����������� �������� ������� � ��������� ������ ��������� ���������
									// ����������� �������������� ��������� ��������
									if (firstFreeItem == 0 && lastFreeItem == 0)
									{
										// ������ ���� - ������� ����� ������
										itemLocation = 1;
									}

									if (firstFreeItem != 0 && oldValue->DictionaryId < firstFreeItem->DictionaryId)
									{
										// ������� ����� �������� � ������ ������
										itemLocation = 2;
									}

									if (lastFreeItem != 0 && oldValue->DictionaryId > lastFreeItem->DictionaryId)
									{
										// ������� ����� �������� � ����� ������
										itemLocation = 3;
									}

									if (firstFreeItem != 0 && lastFreeItem != 0 && firstFreeItem->DictionaryId < oldValue->DictionaryId && oldValue->DictionaryId < lastFreeItem->DictionaryId)
									{
										// ������� ����� �������� � �������� ������
										itemLocation = 4;
									}

									// ����������� ��������� ����������
									switch (itemLocation)
									{
										case 1:
											// ����������� �������� � ������ ������
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
											// ����������� �������� � ������ ������
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
											// ����������� �������� � ����� ������
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
											// ����������� �������� � �������� ������
										/*if (firstFreeItem != 0 && lastFreeItem != 0 && firstFreeItem->DictionaryId < oldValue->DictionaryId && oldValue->DictionaryId < lastFreeItem->DictionaryId)
										{*/
											// ����� ����������� � ������������ ��������
												// ����� ����������� ��������
												for (int i = oldValue->DictionaryId - 1; i >= 0; i--)
												{
													// �������� i-�� �������� �� ��, ��� �� �������� ���������� ��������� ��� ������������� oldValue
													if (dictionaryItemUsed[i] == 0)
													{
														dictionary[i].Next = oldValue;
														oldValue->Previous = &dictionary[i];
														dictionary[i].IsLast = 0;
														oldValue->IsFirst = 0;
													}
												}

												// ����� ���������� ��������
												for (int i = oldValue->DictionaryId + 1; i < Rank*Rank; i++)
												{
													// �������� i-�� �������� �� ��, ��� �� �������� ��������� ��������� ��� ������������� oldValue
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

					// ������� � ����������� ��������
					squareItemId--;
			}

			// ��������� ��������
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


// ������ ���������� �����-���������� �������� � ����
void GreekSquare::SaveSquare()
{
	string fileName = "result.txt";
	fstream resultFile;

	resultFile.open(fileName.c_str(), std::ios_base::app);

	if (resultFile.is_open())
	{
		resultFile << "------------------------" << endl;
		resultFile << "Detected orthogonal square(s) for the square:" << endl;

		// ����� ���������� �����-���������� ��������
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

		// ����� �������� A
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

		// ����� �������� B
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