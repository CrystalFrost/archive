// ����� ������������ ��������� ��������� ������� ��������� ��������� �������� ������ � ��������������� �������� ����������

# include "GetDiagSearch.h"

using namespace std;

// ����������� �� ��������� - ������������� �������� ������ ���������
GetDiagSearch::GetDiagSearch()
{
	InitializeSearch();
}

// ������������� �������� ����� ������� ��������
void GetDiagSearch::InitializeSearch()
{
	// ��������� ������� ���������� ������� ��������� ���������
	for (int i = 0; i < rank; i++)
	{
		primary[i] = 1;
		secondary[i] = 1;
	}

	// ��������� ������� �������� � ����� ������� ��������� ���������, � ������� - "-1"
	for (int i = 0; i < rank; i++)
	{
		for (int j = 0; j < rank; j++)
		{
			columns[i][j] = 1;
			rows[i][j] = 1;
			square[i][j] = -1;
		}
	}

	// ������� ������� ������������ �������� � ������� ���������
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

	// ��������� ������ ������ �������� � �������� ������������� ���� �������� 
	for (int i = 0; i < rank; i++)
	{
		square[0][i] = i;
		rows[0][i] = 0;
		columns[i][i] = 0;

		cellsHistory[0][i][i] = 0;
	}

	// �������� ������������� �������� "0" � ������ ������� � "9" - � ������ �������� ���������
	primary[0] = 0;
	secondary[rank - 1] = 0;

	// ���������� ������� ��������� � "0"
	squaresCount = 0;

	// ����� ��� ����� � ������������
	resultFileName = "result.txt";
}

// ����� ���������� ��������, ������� ����� ���� �� ��������� � ������ (rowId, columndId)
// ���������� ��������� ��������:
//		0 - �������� ����� �� �������, � ������� �������� "-1";
//		1 - �������� ������� � �������� � �������.

int GetDiagSearch::GetCell(int rowId, int columnId)
{
	int isGet;			// ���� ���������� ����������� ��������
	int cellValue;		// ��������� �������� ��� ������ � ������ (rowId, columnId)
	int oldCellValue;	// ���������� ��������, �������� � ���� ������

	// ����� ������ �������� ��� ������ (rowId, columnId)
	isGet = 0;
	cellValue = -1;
	for (int i = 0; i < rank && !isGet; i++)
	{
		// �������� i �� ����������� ������ � ������ (rowId; columnId)
		if (columns[i][columnId] == 1 && rows[rowId][i] == 1 && cellsHistory[rowId][columnId][i] == 1)
		{
			// �������� �� ������ � �������� � �������, �� ���� ��� ��������� ���������
				// ���������� ����, �������, ��������, ����� ������� ������������ ���������
				isGet = 1;
				// ��������� �������� - �� ���������� �� ��� �� ����������
					// �������� ������ ���������
					if(columnId == rowId)
					{
						if (primary[i] != 1)
						{
							isGet = 0;
						}
					}

					// �������� ������ ���������
					if (rowId == rank - 1 - columnId)
					{
						if (secondary[i] != 1)
						{
							isGet = 0;
						}
					}
		}

		// ����������� ��������, ���������� � �����
		if (isGet)
		{
			cellValue = i;
		}
	}

	// ��������� ���������� �������
	if (isGet)
	{
		// ��������� ���������� ������ ��������
			// ���������� �������� ��������
			oldCellValue = square[rowId][columnId];
			// ������ ������ ��������
				// ���������� �������� � �������
				square[rowId][columnId] = cellValue;
				// �������� �������� � ��������
				columns[cellValue][columnId] = 0;
				// �������� �������� � �������
				rows[rowId][cellValue] = 0;
				// �������� �������� � ����������
				if (rowId == columnId)
				{
					primary[cellValue] = 0;
				}
				if (rowId == rank - 1 - columnId)
				{
					secondary[cellValue] = 0;
				}
				// �������� �������� � ������� �������� ������
				cellsHistory[rowId][columnId][cellValue] = 0;

			// ����������� ����������� �������� ��� �������� ������� (��� ��� �� �������� � ���� �������)
			if (oldCellValue != -1)
			{
				// ���������� �������� � �������
				columns[oldCellValue][columnId] = 1;
				// ���������� �������� � ������
				rows[rowId][oldCellValue] = 1;
				// ���������� �������� � ���������
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
		// ��������� ����� ������������ ������ �������� � ������ (rowId; columnId)
			// ���������� ������� �������� �� �������� � �������
				// ��������� ������� ��������
				cellValue = square[rowId][columnId];
				// ���������� �������� � ��������� �������
				if (cellValue != -1)
				{
					// ���������� �������� � �������
					columns[cellValue][columnId] = 1;
					// ���������� �������� � ������
					rows[rowId][cellValue] = 1;
					// ���������� �������� � ���������
					if (rowId == columnId)
					{
						primary[cellValue] = 1;
					}
					if (rowId == rank - 1 - columnId)
					{
						secondary[cellValue] = 1;
					}
					// ���������� -1 � �������
					square[rowId][columnId] = -1;
					// �������� ������� ������ (rowId; columnId)
					for (int i = 0; i < rank; i++)
					{
						cellsHistory[rowId][columnId][i] = 1;
					}
				}
	}

	return isGet;
}


// ��� ����� ����� �������� ��������� � ������� ������� *sDiagIndex
void GetDiagSearch::StepForwardSDiag(int* sDiagIndex)
{
	// ������������ �� ��������� ��������� �������, �� ���������� �� ������� ���������
	do
	{
		(*sDiagIndex)++;
	}
	while (rank%2 == 1 && (*sDiagIndex)*2 + 1 == rank);
}


// ��� ����� ����� �������� ��������� � ������� ������� *sDiagIndex
void GetDiagSearch::StepBackwardSDiag(int* sDiagIndex)
{
	// ������������ �� ��������� ���������� �������, �� ���������� �� ������� ���������
	do
	{
		(*sDiagIndex)--;
	}
	while (rank%2 == 1 && (*sDiagIndex)*2 + 1 == rank);
}


// ��� ����� ����� ������ c ������� ������� (*rowId, *columnId)
void GetDiagSearch::StepForward(int* rowId, int* columnId)
{
	// ������������ �� ��������� ��������� �������, �� ������������� ����������,
	// ��� ������������� ��������� �� ������ ����.
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


// ��� ����� ����� ������ c ������� ������� (*rowId, *columnId)
void GetDiagSearch::StepBackward(int* rowId, int* columnId)
{
	// ������������ �� ��������� ���������� �������, �� ������������� ����������,
	// ��� ������������� ���������� �� ������ ����
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


// ��������� ���������� ��������
void GetDiagSearch::ProcessSquare()
{
	// ���������� ������� ���������� �������� 
	__raise FoundTheSquare(square);
}


// ������� ������ �������� � �������
void GetDiagSearch::PrintSquare(int printedSquare[rank][rank])
{
	// ����� �������� � �������
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


// ������� ������ �������� � ����
void GetDiagSearch::SaveSquare(int printedSquare[rank][rank], fstream& resultFile)
{
	// ������ �������� �������� �����
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


// ����� ������������ ��������� ���������
void GetDiagSearch::Start()
{
	int stop = 0;			// ���� ��������� ������ � �����
	int sDiagStop = 0;		// ���� ��������� ������ ����� �������� ���������
	int cellsStop = 0;		// ���� ��������� ������ �� �������

	int isPDiag = 0;		// ���� ���������� �������� ������� ���������
	int isSDiag = 0;		// ���� ���������� �������� �������� ���������

	pDiagIndex = 1;			// ����� �������������� ������ �� ������� ���������
	sDiagIndex = 0;			// ����� �������������� ������ �� �������� ���������

	rowId = 0;				// ����� ������ � �������������� �������
	columnId = 0;			// ����� ������� � �������������� �������

	// ����� ��������� �� �������
	while (!stop)
	{
		// ����� ��������� �� ������� � �������� ��������� � �� �������
			// ������ ������� ���������
			isPDiag = 0;

			while (!isPDiag && !stop)
			{
				// ����� �������� ��� �������� pDiagIndex ������� ���������
				if (GetCell(pDiagIndex, pDiagIndex))
				{
					// ��������� ���������� ��������
					if (pDiagIndex == rank - 1)
					{
						// ����������� ����� ��������� ������� ���������
						isPDiag = 1;
					}
					else
					{
						// ������� � ���������� ��������
						pDiagIndex++;
					}
				}
				else
				{
					// ��������� ���������� ���������� ��������
					if (pDiagIndex == 1)
					{
						// ��������� �����, ���� ��������� � ����� ������ ������
						stop = 1;
					}
					else
					{
						// ��������� � ����������� ��������
						pDiagIndex--;
					}
				}
			}

			// ������ ��������� ����� ��������
			sDiagStop = 0;
			while (isPDiag && !sDiagStop && !stop)
			{
				// ������ �������� ��������� � ���������� ������ �������� � ��������� ������� ���������
					// ������ �������� ���������
					isSDiag = 0;

					while (isPDiag && !isSDiag && !sDiagStop && !stop)
					{
						// ����� �������� ��� �������� sDiagIndex �������� ���������
						if (GetCell(rank - sDiagIndex - 1, sDiagIndex))
						{
							// ��������� ���������� ��������
							if (sDiagIndex == rank - 2)
							{
								// ��������� ���������� �������������� �������� (��������� - � ��������������� ������)
								isSDiag = 1;
							}
							else
							{
								// ������������ ����� �������� ���������
								StepForwardSDiag(&sDiagIndex);
							}
						}
						else
						{
							// ��������� ���������� ���������� ��������
							if (sDiagIndex == 0)
							{
								// �� ����������� � ������ ��������� - ���������� ���� ���������� ���������� �������� ���������
								sDiagStop = 1;
							}
							else
							{
								// ��������� � ����������� ��������
								StepBackwardSDiag(&sDiagIndex);
							}
						}
					}

					// ������ �������� ��� ���������� ������ ��������
						// ������� �������������� ��������
						rowId = 1;
						columnId = 0;
						cellsStop = 0;

						while (isPDiag && isSDiag && !stop && !cellsStop)
						{
							// ����� ������ �������� ��� ������ (rowId, columnId)
							if (GetCell(rowId, columnId))
							{
								// ��������� ���������� ��������
								if (rowId == rank - 1 && columnId == rank - 2)
								{
									// ��������� ����������� ������������� ���������� �������� �� ���������� ������ (rank - 1, rank - 2).
									// "-2" - ������, ��� ����� ��������� ������ �������� - ��� ��������� ������� ���������
									squaresCount++;
									ProcessSquare();
								}
								else
								{
									// ������ ��������� ���
									StepForward(&rowId, &columnId);
								}
							}
							else
							{
								// ��������� ���������� ��������
									// ������ ��� �����
									StepBackward(&rowId, &columnId);
									// ��������� ������� � ������������� ������
									if (rowId == 0)
									{
										// ���������� ���� ���������� ������ �� �������������� �������
										cellsStop = 1;
									}
							}
						}
			}
	}
}