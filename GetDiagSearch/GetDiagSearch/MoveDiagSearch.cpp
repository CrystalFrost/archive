// ����� ��� ������������ ��������� ��������� ������� ����������� �����

# include "MoveDiagSearch.h"

// ����������� �� ���������
MoveDiagSearch::MoveDiagSearch()
{
	totalProcessedSquaresSmall = 0;
	totalProcessedSquaresLarge = 0;
	resultFileName = "result.txt";

	InitializeMoveSearch();
}


// ������������� ������ ������������ �����
void MoveDiagSearch::InitializeMoveSearch()
{
	// �������� ������ ���������
	for (int i = 0; i < rank; i++)
	{
		for (int j = 0; j < rank; j++)
		{
			squareA[i][j] = -1;
			squareB[i][j] = -1;
			rowsHistory[i][j] = 1;
		}
	}

	// �������� ������� ������������� ����� � ��������� ������������
	// � ������� �����, �������������� ��� �������� ��������
	for (int i = 0; i < rank; i++)
	{
		rowsUsage[i] = 1;
		currentSquareRows[i] = -1;
	}

	// ���������� ������� ��������� � ������������� ���������
	squaresCount = 0;
}


// ������ ������
void MoveDiagSearch::StartMoveSearch()
{
	// ������������� �� ������� ���������� ���������� ���
	__hook(&GetDiagSearch::FoundTheSquare, &squareAGenerator, &MoveDiagSearch::OnFoundTheSquare);

	// ��������� ��������� ���
	squareAGenerator.Start();

	// ������������ �� ������� ���������� ���������� ���
	__unhook(&GetDiagSearch::FoundTheSquare, &squareAGenerator, &MoveDiagSearch::OnFoundTheSquare);
}


// ��������� ���������� ���������� ���
void MoveDiagSearch::OnFoundTheSquare(int newSquare[GetDiagSearch::rank][GetDiagSearch::rank])
{
	// ������ ������ ������ ��������� ������� ������������ �����
		// ������� �������� ����� ������� �����
		InitializeMoveSearch();

		// ����������� ���������� ��������
		for (int i = 0; i < rank; i++)
		{
			for (int j = 0; j < rank; j++)
			{
				squareA[i][j] = newSquare[i][j];
			}
		}

		// ��������� ���������� �� ������������ ���������
		totalProcessedSquaresSmall++;
		if (totalProcessedSquaresSmall > 0 && totalProcessedSquaresSmall % 1000000000 == 0)
		{
			totalProcessedSquaresLarge++;
			totalProcessedSquaresSmall = 0;
		}

		// ����� ���������� � ���� ��������� "� �����"
		if ((totalProcessedSquaresSmall - 1) % 100000 == 0)
		{
			cout << "------------------------" << endl;
			cout << "Processed " << totalProcessedSquaresLarge << " milliards and " << totalProcessedSquaresSmall << " squares." << endl;
			cout << "Now processed the square:" << endl;
			cout << endl;
			PrintSquare(squareA);
			cout << "------------------------" << endl;
		}

		// ������ �����������
		MoveRows();
}


// ������������ ����� ��������
// ������ ������ � ������ ������������ �� �������������, 
// �.�. �� ���� ��� � ������������ �����
void MoveDiagSearch::MoveRows()
{
	int currentRowId = 1;
	int isRowGet = 0;
	int gettingRowId = -1;
	int oldRowId = -1;

	int diagonalValues[rank];
	int duplicationDetected = 0;

	// ���������� ������ ������ �������� A � B � ����� ������ ��������������� ���������
	for (int j = 0; j < rank; j++)
	{
		squareB[0][j] = squareA[0][j];
	}

	// �������� �������������� ������ ������, �.�. ��� �������������
	rowsUsage[0] = 0;
	rowsHistory[0][0] = 0;
	currentSquareRows[0] = 0;

	while (currentRowId > 0)
	{
		// ��������� ������ �� ��������� �������� �� ������� currentRowId ������������ ��������
		isRowGet = 0;
		gettingRowId = -1;
		for (int i = 0; i < rank; i++)
		{
			// ��������� i-� ������ ��������� ��������
			if (rowsUsage[i] && rowsHistory[currentRowId][i])
			{
				isRowGet = 1;
				gettingRowId = i;

				break;
			}
		}

		// ������������ ��������� ������
		if (isRowGet)
		{
			// ������������ ���������� ����� ������
				// ������� � ������� ����� ������
					// ��������� ����� ������, ������� ������ ����� � ��������
					oldRowId = currentSquareRows[currentRowId];
					// ���������� ����� ������ � �������, ������ ������ �������������� �����, � ������� �������������� ����� � ������ ������� ����� ��������
						// ���������� ����� ������ � �������
						for (int j = 0; j < rank; j++)
						{
							squareB[currentRowId][j] = squareA[gettingRowId][j];
						}
						// �������� ������ � ������ ������������ �����
						rowsUsage[gettingRowId] = 0;
						// �������� ������ � ������� ������������� ������
						rowsHistory[currentRowId][gettingRowId] = 0;
						// ���������� ������ � ������ ������� ����� ��������
						currentSquareRows[currentRowId] = gettingRowId;

				// ������� ��� ���������� ������ ����� �������������
					// ������� ������� � ������� ������������ �����
					if (oldRowId != -1)
					{
						rowsUsage[oldRowId] = 1;
					}

				// ��������� �������������� ������������ ����� ��������
					// ���������� ���� ��������������� � ���������� �� ����������
					duplicationDetected = 0;
					// �������� ������� ���������
						// ���������� ����� �������������� �����
						for (int i = 0; i < rank; i++)
						{
							diagonalValues[i] = 1;
						}
						// �������� �������� ������� ���������
						for (int i = 0; i <= currentRowId; i++)
						{
							// �������� i-�� �������� ������� ��������� - ������ (i, i)
							if (diagonalValues[squareB[i][i]])
							{
								diagonalValues[squareB[i][i]] = 0;
							}
							else
							{
								duplicationDetected = 1;
								break;
							}
						}
					// �������� �������� ���������, ���� ��� ����� �����
					if (!duplicationDetected)
					{
						// �������� �������� ���������
							// ���������� ����� �������������� �����
							for (int i = 0; i < rank; i++)
							{
								diagonalValues[i] = 1;
							}
							// �������� �������� �������� ��������� ������� � "� ������"
							for (int i = 0; i <= currentRowId; i++)
							{
								// �������� i-�� �������� �������� ��������� - �������� (i, rank - 1 - i)
								if (diagonalValues[squareB[i][rank - 1 - i]])
								{
									diagonalValues[squareB[i][rank - 1 - i]] = 0;
								}
								else
								{
									duplicationDetected = 1;
									break;
								}
							}
					}

				// ��������� ������ �������� �������������� ��������
				if (!duplicationDetected)
				{
					// ������ ��������� ��� ����� � ����������� �� �������� ���������
					if (currentRowId == rank - 1)
					{
						// ������������ ��������� �������
						ProcessSquare();
					}
					else
					{
						// ������ ��� �����
						currentRowId++;
					}
				}
		}
		else
		{
			// ������������ ������������ ����� ������ - ������ ��� �����, ������� ����� ��������������, 
			// ������� �������������, �������� ������� ����� �������� � ������� ��� �������
				// ��������� ����� ������� ������
				oldRowId = currentSquareRows[currentRowId];
				// �������� ������� ������ � ��������
				for (int j = 0; j < rank; j++)
				{
					squareB[currentRowId][j] = -1;
				}
				// �������� ������� ������ ��������
				currentSquareRows[currentRowId] = -1;
				// �������� ���� ���������� ��������������
				rowsUsage[oldRowId] = 1;
				// �������� ������� ������ � ���� �������
				for (int i = 0; i < rank; i++)
				{
					rowsHistory[currentRowId][i] = 1;
				}
				// ������ ��� �����
				currentRowId--;
		}
	}
}


// ��������� ���������� ��������
void MoveDiagSearch::ProcessSquare()
{
	int isDifferent = 0;			// ����� ������� � ������� �� ��������� �������� (��� ������ ������������ ��� �����)
	fstream resultFile;				// ����� ��� I/O � ���� � ������������

	// ��������� ��� �� ��, ��� �� ����� ���������
	isDifferent = 0;
	
	for (int i = 0; i < rank; i++)
	{
		if (currentSquareRows[i] != i)
		{
			isDifferent = 1;
			break;
		}
	}

	// ��������� ���������� ��������
	if (isDifferent && IsSquareLatin() && IsSquareDiagonal() && IsSquareOrthogonal(squareA, squareB))
	{
		// ������ ���������� � ��������� ��������

		// ���������� �������� ���������
		squaresCount++;

		// ����������� �������� ��������
		if (squaresCount == 1)
		{
			for (int i = 0; i < rank; i++)
			{
				for (int j = 0; j < rank; j++)
				{
					orthogonalSquares[squaresCount - 1][i][j] = squareA[i][j];
				}
			}
		}

		// ����������� �������� - ����
		for (int i = 0; i < rank; i++)
		{
			for (int j = 0; j < rank; j++)
			{
				orthogonalSquares[squaresCount][i][j] = squareB[i][j];
			}
		}

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

		// ����� ���������� � ��������� ����
			// ����� ���������� � �������
			cout << "------------------------" << endl;
			cout << "Pair # " << squaresCount << endl;
			cout << "------------------------" << endl;

			PrintSquare(squareA);
			PrintSquare(squareB);

			// ����� ���������� � ����
			resultFile.open(resultFileName, std::ios_base::app);
			resultFile << "------------------------" << endl;
			resultFile << "Pair # " << squaresCount << endl;
			resultFile << "------------------------" << endl;
			SaveSquare(squareA, resultFile);
			SaveSquare(squareB, resultFile);

			// �������� �������� ��������������� ������ ���������
			for (int i = 0; i < squaresCount; i++)
			{
				for (int j = i + 1; j <= squaresCount; j++)
				{
					if (IsSquareOrthogonal(orthogonalSquares[i], orthogonalSquares[j]))
					{
						cout << "Square " << i << " # " << j << endl;
						resultFile << "Square " << i << " # " << j << endl;
					}
				}
			}

			resultFile.close();
	}
}

// ������ ��������� � ���������� ��� �� ������������ �����
void MoveDiagSearch::PrintHeader()
{
	cout << "------------------------" << endl;
	cout << "Detected a pair for the square: " << endl;
	cout << "------------------------" << endl;
	for (int i = 0; i < rank; i++)
	{
		for (int j = 0; j < rank; j++)
		{
			cout << squareA[i][j] << " ";
		}
		cout << endl;
	}
	cout << "------------------------" << endl;
}


// ������� ������ �������� � �������
void MoveDiagSearch::PrintSquare(int printedSquare[rank][rank])
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
void MoveDiagSearch::SaveSquare(int writedSquare[rank][rank], fstream& resultFile)
{
	// ������ �������� �������� �����
	for (int i = 0; i < rank; i++)
	{
		for (int j = 0; j < rank; j++)
		{
			resultFile << writedSquare[i][j] << " ";
		}
		resultFile << endl;
	}
	resultFile << endl;
}


// �������� �������� �� ��, ��� �� �������� ���������
// ����������: ��� ��� ������� ����������� ��������� �� ��������,
// �� �������� �� ������ ����� - �������������, �� ��� ���������
// ��� ����, ����� ������� ���� �������������, � ��� ����� ��������
// ���������� �� ��������� ���������, ������� ����������� ��������
// ������ ��������
int MoveDiagSearch::IsSquareLatin()
{
	int duplicationDetected = 0;

	// �������� �������� ��������
	for (int columnId = 0; columnId < rank && !duplicationDetected; columnId++)
	{
		// �������� ������������ ������� columnId
		for (int rowId = 0; rowId < rank && !duplicationDetected; rowId++)
		{
			// �������� �� ���������� �������� [rowId; columnId] �� ����� ���������� ���������� ������� columndId
			for (int comparedRowId = rowId + 1; comparedRowId < rank && !duplicationDetected; comparedRowId++)
			{
				if (squareB[comparedRowId][columnId] == squareB[rowId][columnId])
				{
					duplicationDetected = 1;
				}
			}
		}
	}

	// �������� ����� ��������
	for (int rowId = 0; rowId < rank && !duplicationDetected; rowId++)
	{
		// �������� ������������ ������ rowId
		for (int columnId = 0; columnId < rank && !duplicationDetected; columnId++)
		{
			// �������� �� ���������� �������� [rowId; columnId] �� ����� ���������� ���������� ������ rowId
			for (int comparedColumnId = columnId + 1; comparedColumnId < rank && !duplicationDetected; comparedColumnId++)
			{
				if (squareB[rowId][columnId] == squareB[rowId][comparedColumnId])
				{
					duplicationDetected = 1;
				}
			}
		}
	}

	return !duplicationDetected;
}


// �������� �������� �� ��, ��� �� �������� ������������
int MoveDiagSearch::IsSquareDiagonal()
{
	int duplicationDetected = 0;

	// �������� ������ ��������� - ��������� [0;0] - [rank;rank]
	for (int itemId = 0; itemId < rank && !duplicationDetected; itemId++)
	{
		// �������� �� ���������� �������� [itemId; itemId] �� ����� ���������� ���������� ���������
		for (int comparedId = itemId + 1; comparedId < rank && !duplicationDetected; comparedId++)
		{
			if (squareB[itemId][itemId] == squareB[comparedId][comparedId])
			{
				duplicationDetected = 1;
			}
		}
	}

	// �������� ������ ��������� - ��������� [rank - itemId - 1; itemId]
	for (int itemId = 0; itemId < rank && !duplicationDetected; itemId ++)
	{
		// �������� �� ���������� �������� [rank - itemId - 1; itemId] �� ����� ���������� ���������� ���������
		for (int comparedId = itemId + 1; comparedId < rank && !duplicationDetected; comparedId++)
		{
			if (squareB[(rank - itemId - 1)][itemId] == squareB[(rank - comparedId - 1)][comparedId])
			{
				duplicationDetected = 1;
			}
		}
	}

	return !duplicationDetected;
}


// �������� ��������� squareA � squareB �� ���������������
int MoveDiagSearch::IsSquareOrthogonal(int a[rank][rank], int b[rank][rank])
{
	int isOrthogonal = 1;
	int pair[rank][rank];

	for (int i = 0; i < rank; i++)
	{
		for (int j = 0; j < rank; j++)
		{
			pair[i][j] = 1;
		}
	}

	for (int i = 0; i < rank && isOrthogonal; i++)
	{
		for (int j = 0; j < rank && isOrthogonal; j++)
		{
			if (pair[a[i][j]][b[i][j]])
			{
				pair[a[i][j]][b[i][j]] = 0;
			}
			else
			{
				isOrthogonal = 0;
			}
		}
	}

	return isOrthogonal;
}