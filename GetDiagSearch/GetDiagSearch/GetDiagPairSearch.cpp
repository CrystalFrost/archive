// ����� ��� ������������� ������������ ��������� ���������

# include "GetDiagPairSearch.h"

using namespace std;

// ����������� �� ��������� - ������������� �������� ������ ���������
GetDiagPairSearch::GetDiagPairSearch() : GetDiagSearch()
{
	InitializePairSearch();
}


// ������������� �������� ����� ������� ������� ��������
void GetDiagPairSearch::InitializePairSearch()
{
	InitializeSearch();

	// ��������� ������� ��� ������ ������� �������� ���� - squareA
	// � ������� ��������������� ��� � �����-��������� ��������
	for (int i = 0; i < rank; i++)
	{
		for (int j = 0; j < rank; j++)
		{
			squareA[i][j] = -1;
			pairsDictionary[i][j] = 1;
		}
	}
}


// ����� ���������� ��������, ������� ����� ���� �� ��������� � ������ (rowId, columndId) �������� ������� ��������
// ���������� ��������� ��������:
//		0 - �������� ����� �� �������, � ������� �������� "-1";
//		1 - �������� ������� � �������� � �������.

int GetDiagPairSearch::GetCell(int rowId, int columnId)
{
	int isGet;			// ���� ���������� ����������� ��������
	int cellValue;		// ��������� �������� ��� ������ � ������ (rowId, columnId)
	int oldCellValue;	// ���������� ��������, �������� � ���� ������

	// ����� ������ �������� ��� ������ (rowId, columnId)
	isGet = 0;
	cellValue = -1;
	for (int i = 0; i < rank && !isGet; i++)
	{
		// �������� i �� ����������� ������ � ������ (rowId; columnId) ��������-����
		if (columns[i][columnId] == 1 && rows[rowId][i] == 1 && cellsHistory[rowId][columnId][i] == 1)
		{
			// �������� �� ������ � �������� � �������, �� ���� ��� ��������� ���������
				// ���������� ����, �������, ��������, ����� ������� ������������ ��������� ��� ��������� ���
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

				// ��������� ������������ ���� �� � �������������� ������� ��������
				// ������� �������� - squareA � ������������ �������� ��� ������� - i
				if (pairsDictionary[squareA[rowId][columnId]][i] == 0)
				{
					isGet = 0;
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

			// �������� �������� ��������� ���� � ��������������
				// �������� �������������� ����� ����
				pairsDictionary[squareA[rowId][columnId]][square[rowId][columnId]] = 0;

				// ������� ������� � ������������� ��������� ����
				if (oldCellValue != -1)
				{
					pairsDictionary[squareA[rowId][columnId]][oldCellValue] = 1;
				}
	}
	else
	{
		// ��������� ����� ������������ ������ �������� � ������ (rowId; columnId)
			// ���������� ������� �������� �� �������� � �������
				// ��������� ������� ��������
				cellValue = square[rowId][columnId];

				// ���������� ��������� �������� � ��������� �������
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

					// �������� ������������ �������� � �������������� �����
					pairsDictionary[squareA[rowId][columnId]][cellValue] = 1;
				}
	}

	return isGet;
}


// ���������� ������� - ���������� ������� �������� ���� �������� squareAGenerator
void GetDiagPairSearch::OnFoundTheSquare(int newSquare[rank][rank])
{
	fstream resultFile;		// ����� ��� I/O � ���� � ������������

	// ��������� ��������� � ������� ������ ������� ��������,
	// ������� ��� ���������� � ��������
	InitializePairSearch();

	// ���������� ��������� �������
	for (int i = 0; i < rank; i++)
	{
		for (int j = 0; j < rank; j++)
		{
			squareA[i][j] = newSquare[i][j];
		}
	}

	// ��������� ���������� ������� �������� ����
		// ����� ���������� � �������
		cout << "------------------------" << endl;
		cout << "Starting analyze a square: " << endl;
		PrintSquare(squareA);
		cout << "------------------------" << endl;

	// ��������� ����� ���� � ����
	Start();

	// ������� ��������� ������
	if (squaresCount > 0)
	{
		PrintSearchFooter();
	}
}


// ����������� ��������� ��������� ���������� �������� � ���� � �������� A - squareA
void GetDiagPairSearch::ProcessSquare()
{
	// ������������ ��������� ����
	ProcessSquaresPair();
}


// ��������� ���������� ���� ������������ ��������� ���������
void GetDiagPairSearch::ProcessSquaresPair()
{
	fstream resultFile;		// ����� ��� I/O � ���� � ������������

	// ����� ���������� � ��������� ����
		// ����� ���������
		if (squaresCount == 1)
		{
			// ����� ���������� � ������ �������� ���� � ���� ���������
			cout << "------------------------" << endl;
			cout << "Detected pair for the square: " << endl;
			cout << "------------------------" << endl;
			PrintSquare(squareA);
			cout << "------------------------" << endl;

			// ����� ���������� � ����
			resultFile.open(resultFileName, std::ios_base::app);
			resultFile << "------------------------" << endl;
			resultFile << "Detected pair for the square: " << endl;
			resultFile << "------------------------" << endl;
			SaveSquare(squareA, resultFile);
			resultFile.close();
		}

		// ����� ���������� � �������
		cout << "------------------------" << endl;
		cout << "Pair # " << squaresCount << endl;
		cout << "------------------------" << endl;

		PrintSquare(squareA);
		PrintSquare(square);

		// ����� ���������� � ����
		resultFile.open(resultFileName, std::ios_base::app);
		resultFile << "------------------------" << endl;
		resultFile << "Pair # " << squaresCount << endl;
		resultFile << "------------------------" << endl;
		SaveSquare(squareA, resultFile);
		SaveSquare(square, resultFile);
		resultFile.close();

		/* ��������� �����
		PrintSquare(square);

		resultFile.open(resultFileName, std::ios_base::app);
		SaveSquare(square, resultFile);
		resultFile.close();*/
}


// ����� ���������� �� ������ ������
void GetDiagPairSearch::PrintSearchFooter()
{
	fstream resultFile;

	// ����� ������ � �������
	cout << "------------------------" << endl;
	cout << "Total pairs found: " << squaresCount << endl;
	cout << "------------------------" << endl;

	// ����� ������ � ����
	resultFile.open(resultFileName, std::ios_base::app);
	resultFile << "------------------------" << endl;
	resultFile << "Total pairs found: " << squaresCount << endl;
	resultFile << "------------------------" << endl;
	resultFile.close();
}


// ������ ������ ��� ������������ ��������� ���������
void GetDiagPairSearch::StartPairSearch()
{
	// ������������� �� ������� ���������� ���
	__hook(&GetDiagSearch::FoundTheSquare, &squareAGenerator, &GetDiagPairSearch::OnFoundTheSquare);

	// ��������� ����� ������������ ��������� ���������
	squareAGenerator.Start();

	// ������������ �� ������� ���������� ���
	__unhook(&GetDiagSearch::FoundTheSquare, &squareAGenerator, &GetDiagPairSearch::OnFoundTheSquare);
}


// ���� ������������� ��������� � ��������� ��������
void GetDiagPairSearch::CheckSeparateSquare(int square[rank][rank])
{
	// ��������� ��������� � ������� ������ ������� ��������,
	// ������� ��� ���������� � ��������
	InitializePairSearch();

	// ���������� ��������� �������
	for (int i = 0; i < rank; i++)
	{
		for (int j = 0; j < rank; j++)
		{
			squareA[i][j] = square[i][j];
		}
	}

	// ��������� ���������� ������� �������� ����
		// ����� ���������� � �������
		cout << "------------------------" << endl;
		cout << "Starting analyze a square: " << endl;
		PrintSquare(squareA);
		cout << "------------------------" << endl;

	// ��������� ����� ���� � ����
	Start();

	// ������� ��������� ������
	if (squaresCount > 0)
	{
		PrintSearchFooter();
	}
}