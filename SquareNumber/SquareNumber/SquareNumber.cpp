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

// ���������� �������� ������ ������� �� ��� ����� � �������
// vector - ��������� �� ������ ������� vectorRank
// vectorRank - ������ �������
// version - ����� ������ �������
// dictionary - ������� �� �������� �������� ������
void CreateVectorVersion(int* vector, int vectorRank, int version, Symbol* dictionary)
{
	int componentNumber = -1;		// ����� ���������� ���������� �������, ������� ����������� � ������
	int subversion = version;		// ����� ��������� �������
	int subrank = vectorRank;		// ����� �������������� ����� �������
	int divider = -1;				// ��������, ������������ ��� ����������
	int dictionarySymbolIndex = -1;	// ������������� ������� � �������
	int freeSymbolNumber = -1;		// ����� ���������� �������� � ������� � ������ ����, ��� ��� ������� - ������������!

	// ���������� ������� �� ��� ������
	if (version >= 0)
	{
		// ���������� ������� �� �������� ������
			// ������� ��������� ������� � ������ ������������� ��������� �������
			for (int i = 0; i < vectorRank; i++)
			{
				dictionary[i].IsUsed = 0;
				vector[i] = -1;
			}

			// ���������� ������ �������
			for (int i = 0; i < vectorRank; i++)
			{
				// ����������� i-�� ���������� �������
					// ���������� ������ i-�� ���������� �������
					divider = Factorial(subrank - 1);
					componentNumber = subversion / divider;
					// ���������� ������ � ����� ��� ���������� ����
					subversion = subversion % divider;
					subrank--;

					// ����� � ������� ���������� ���������� � ����������� ������� - componentNumber
						// ������� �������������� ��������
						dictionarySymbolIndex = -1;
						freeSymbolNumber = -1;
						// ������� ��������� �������
						do
						{
							// ��������� ���������� �������� �������
								// ����������� �������� ���������
								dictionarySymbolIndex++;
								// ������� ��� ��������� ���������
								if (dictionary[dictionarySymbolIndex].IsUsed == 0)
								{
									freeSymbolNumber++;
								}
								// ���������� �������� ������� �������
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
		// ���������� ������� - "��������" ��� ������������� ������
		for (int i = 0; i < vectorRank; i++)
		{
			vector[i] = -1;
		}
	}
}

// ���������� ������ ��������
// square - ��������� �� ������, � ������� ����� ������������ �������� ��������
// rank - ������ ��������
// version - ��������� �� ������, ����������� ������ ��������
// dictionary - ��������� �� ������ �� �������
void CreateSquareVersion(int* square, int rank, int* version, Symbol* dictionary)
{
	int* row;		// ��������������� ������, ������������ ��� ������������ ������
	int rowVersion;	// ������ �������������� ������

	// ��������� ������
	row = new int[rank];
	
	// ������������ ����� �������� �������� ������
	for (int rowId = 0; rowId < rank; rowId++)
	{
		// ������������ ��������� ������ �� ������ � ������
			// ���������� ������ ����������� ������
			rowVersion = version[rowId];
			// �������� ������ ��� ������� �������� ������
			CreateVectorVersion(row, rank, rowVersion, dictionary);
			// ������ ���������� �������� � ������ ��������
			for (int componentId = 0; componentId < rank; componentId++)
			{
				square[rowId*rank + componentId] = row[componentId];
			}
	}

	// ������������� ������� ������
	delete [] row;
}

/*// ���������� ������ �������� �� 1
// version - ��������� �� ������ � �������� �����
// rank - ���� ��������, ����� ������� � �������� �����
// component - ����� ���������� �������, ������� ���� ���������
// ����������: ���� ������� ������ ������������ ������,
// �� ����� "������������" - ��� ������ ����� �������� �� 1
void IncrementSquareVersion(int* version, int rank, int component)
{
	int isIncremented = 0;
	int componentId = component;
	int maxVersion = Factorial(rank) - 1;

	// ����������� ������ ����� �� ����� �� 1
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

/*// �������� ����, �������� �� ������ �������� ������������� ��������
// version - ��������� �� ������ � �������� ����� ��������
// rank - ���� ��������, ����� ������� � �������� ����� ��������
int IsMaxSquareVersion(int* version, int rank)
{
	int isVersionMax = 1;				// ���� �������������� �������� ������ ��������
	int maxVersion = Factorial(rank) - 1;	// ����� ������������ ������, ������� ����� �������� ��������� ������� � ������ ��������

	// �������� ����������� ������� �� ��, ��� �������� � ������ �� ���� ������������� ������������ ������ �������
	for (int componentId = 0; componentId < rank && isVersionMax; componentId++)
	{
		// �������� ���������� ���������� �������
		if (version[componentId] != maxVersion)
		{
			isVersionMax = 0;
		}
	}

	return isVersionMax;
}*/

/*// �������� ����, ��� ������ ���� ����� � ������ �������� - ��������
// version - ��������� �� ������ � �������� ����� ��������
// rank - ���� ��������, ����� ������� � �������� ����� ��������
int IsRowVersionsDistinct(int* version, int rank)
{
	int duplicationDetected = 0;

	// �������� ������� ������ ����� �� ���������� ����� �����
	for (int componentId = 0; componentId < rank && !duplicationDetected; componentId++)
	{
		// �������� ������ ������ componentId �� ���������� � �����������
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

/*// �������� ������������ ������� ������ ��������, ���������������� �������� ������
// version - ��������� �� ������ � �������� ����� ��������
// rank - ���� ��������, ����� ������� � �������� ����� ��������
// rowA - ������ ������ �� ���� ������
// rowB - ������ ������ �� ���� ������
int IsFirstColumnCorrect(int* version, int rank, int* rowA, int* rowB)
{
	int* column = new int[rank];
	int divider = Factorial(rank - 1);
	int duplicationDetected = 0;

	// ����� �������� �������� ������������� �����
	*rowA = -1;
	*rowB = -1;

	// ������������ �������� ������� �������
	for (int columnId = 0; columnId < rank; columnId++)
	{
		column[columnId] = version[columnId] / divider;
	}

	// �������� ������������ �������� � ������ �������
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

// �������� �������� �� ��, ��� �� �������� ���������
// square - ��������� �� ������ � ���������� ��������
// rank - ���� ��������
// ����������: ��� ��� ������� ����������� ��������� �� ��������,
// �� �������� �� ������ ����� - �������������, �� ��� ���������
// ��� ����, ����� ������� ���� �������������, � ��� ����� ��������
// ���������� �� ��������� ���������, ������� ����������� ��������
// ������ ��������
int IsSquareLatin(int* square, int rank)
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
				if (square[rowId*rank + columnId] == square[comparedRowId*rank + columnId])
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
				if (square[rowId*rank + columnId] == square[rowId*rank + comparedColumnId])
				{
					duplicationDetected = 1;
				}
			}
		}
	}

	return !duplicationDetected;
}

// �������� �������� �� ��, ��� �� �������� ������������
// square - ��������� �� ������ � ���������� ��������
// rank - ���� ��������
int IsSquareDiagonal(int* square, int rank)
{
	int duplicationDetected = 0;

	// �������� ������ ��������� - ��������� [0;0] - [rank;rank]
	for (int itemId = 0; itemId < rank && !duplicationDetected; itemId++)
	{
		// �������� �� ���������� �������� [itemId; itemId] �� ����� ���������� ���������� ���������
		for (int comparedId = itemId + 1; comparedId < rank && !duplicationDetected; comparedId++)
		{
			if (square[itemId*rank + itemId] == square[comparedId*rank + comparedId])
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
			if (square[(rank - itemId - 1)*rank + itemId] == square[(rank - comparedId - 1)*rank + comparedId])
			{
				duplicationDetected = 1;
			}
		}
	}

	return !duplicationDetected;
}

// �������� ������������ ����� ������ �������� �� 0-� ������ �� maxRowId � ������ row
// ��������������, ��� ������ row ������ ������ ������ maxRowId + 1
// square - ��������� �� ������ � ���������� ��������
// rank - ���� ��������
// row - ��������� �� ������ � ����� �������
// maxRowId - ����� ������ �� ������� �������� �������
int CheckSquarePart(int* square, int rank, int* row, int maxRowId)
{
	int duplicationDetected = 0;

	// �������� ������ ��������� - ��������� [0;0] - [maxRowId;maxRowId]
	for (int itemId = 0; itemId <= maxRowId && !duplicationDetected; itemId++)
	{
		// �������� �� ���������� �������� [itemId; itemId] �� ����� ���������� ���������� ���������
		for (int comparedId = itemId + 1; comparedId <= maxRowId && !duplicationDetected; comparedId++)
		{
			if (square[itemId*rank + itemId] == square[comparedId*rank + comparedId])
			{
				duplicationDetected = 1;
			}
		}
	}

	// �������� ������ ��������� - ��������� [maxRowId - itemId - 1; itemId]
	for (int itemId = 0; itemId <= maxRowId && !duplicationDetected; itemId ++)
	{
		// �������� �� ���������� �������� [maxRowId - itemId - 1; itemId] �� ����� ���������� ���������� ���������
		for (int comparedId = itemId + 1; comparedId <= maxRowId && !duplicationDetected; comparedId++)
		{
			if (square[itemId*rank + (rank - 1) - itemId] == square[comparedId*rank + (rank - 1) - comparedId])
			{
				duplicationDetected = 1;
			}
		}
	}

	// �������� �������� ��������
	for (int columnId = 0; columnId < rank && !duplicationDetected; columnId++)
	{
		// �������� ������������ ������� columnId
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
		// �������� ������������ ������� columnId
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

// ��������� ����������� ������� �� ������ rowId
// square - ��������� �� ������ � ���������� ��������
// rank - ���� ��������
// version - ������-������ ��������
// dictionary - ������� ��������
// rowId - ������ � ������� �������� �������������� ����������
// row - ��������� �� ������ � ����� �������
// checkpointCounter - ������� ��� ����������� ����� ������������ ������ �� ��������� ����������� �����
// completedCheckpointFileName - �������� ����� � ����������� ����������� ������
// newCheckpointFileName - �������� ����� � ����� ����������� ������
// checkpointInterval - �������� (� ���� ����� ������) ����� ������� ���� ������ ����������� �����
// resultFileName - �������� ����� � ������� ���� �������� ��������� ��������
void GenerateSubsquare(int* square, int rank, int* version, Symbol* dictionary, int rowId, int* row,
					   int* checkpointCounter, int checkpointInterval, char* completedCheckpointFileName, char* newCheckpointFileName, char* resultFileName)
{
	int maxRowVersion = Factorial(rank);
	int rowVersion;
	int startRowVersion = 0;

	fstream newCheckpointFile;
	fstream resultFile;

	// ���������� ��������� ������ ������������
	if (rowId == 0)
	{
		// ������ ��������� ������� � ��������� ������ (������ ������ �� �� ������, ������ �������� ��������� ���������)
			// ���������� ������ ��������
			CreateSquareVersion(square, rank, version, dictionary);

			// ��������� ����������� �����
			GenerateSubsquare(square, rank, version, dictionary, rowId + 1, row, checkpointCounter, checkpointInterval, completedCheckpointFileName, newCheckpointFileName, resultFileName);
	}
	else
	{
		// ��������� ������� � �������� ������ ��������
			// ������� ��������� ������ ������ �� ���������� ������
			if (rowId < rank - 1)
			{
				startRowVersion = version[rowId] == -1 ? 0 : version[rowId];
			}
			else
			{
				startRowVersion = version[rowId] + 1;
			}

			// ��������� ����� ��������
			for (rowVersion = startRowVersion; rowVersion < maxRowVersion; rowVersion++)
			{
				// ��������� ��������� ������ ������ � � �����������
				if (rowId + 1 < rank && version[rowId + 1] != -1)
				{
					// ������� � ����������� ��������� ����������� �� ��������� �������
					GenerateSubsquare(square, rank, version, dictionary, rowId + 1, row, checkpointCounter, checkpointInterval, completedCheckpointFileName, newCheckpointFileName, resultFileName);
				}
				else
				{
					// ��������� ������ ������� ������
						// �������� ������, ��������������� ����� ������
						CreateVectorVersion(row, rank, rowVersion, dictionary);
						// �������� ������������� ��������
						if (CheckSquarePart(square, rank, row, rowId - 1))
						{
							if (rowId + 1 == rank)
							{
								// �������������� �������� �������� �� ��, ��� �� ��������� � ������������
									// ������ ������� ������ �������
									for (int i = 0; i < rank; i++)
									{
										square[rowId*rank + i] = row[i];
									}
							
									// ������ ������� ������ ������ � ������ ������ ��������
									version[rowId] = rowVersion;

									// ���������� ��������
									if (IsSquareLatin(square, rank) && IsSquareDiagonal(square, rank))
									{
										// ����� ��������, ���������� ��������, � ���� � ������������
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

									/*// ������������ ����������� ����� ��� �������������
									(*checkpointCounter)++;
									if (*checkpointCounter == checkpointInterval)
									{
										// ������������ ����������� �����
											// �������� ������ ����� ����������� �����
											newCheckpointFile.open("checkpoint_new.txt", std::ios_base::out);
											newCheckpointFile << rank;

											for (int i = 0; i < rank; i++)
											{
												newCheckpointFile << " " << version[i];
											}

											newCheckpointFile.close();

											// �������� ������� ����� ����������� �����
											remove("checkpoint.txt");

											// �������������� ������ ����� ����������� �����
											rename("checkpoint_new.txt", "checkpoint.txt");

											// ����� ����� ������������� ������������ ����������� �����
											*checkpointCounter = 0;
									}*/
							}
							else
							{
								// ��������� ��������� �������� ��������
									// ������ ������� ������ �������
									for (int i = 0; i < rank; i++)
									{
										square[rowId*rank + i] = row[i];
									}
									// ������ ������� ������
									version[rowId] = rowVersion;
									// ������ ����������� �����
									GenerateSubsquare(square, rank, version, dictionary, rowId + 1, row, checkpointCounter, checkpointInterval, completedCheckpointFileName, newCheckpointFileName, resultFileName);
							}
						}

						// ������������ ����������� ����� ��� �������������
						(*checkpointCounter)++;
						if (*checkpointCounter == checkpointInterval)
						{
							// ������������ ����������� �����
								// �������� ������ ����� ����������� �����
								newCheckpointFile.open("checkpoint_new.txt", std::ios_base::out);
								newCheckpointFile << rank;

								for (int i = 0; i < rank; i++)
								{
									newCheckpointFile << " " << version[i];
								}

								newCheckpointFile.close();

								// �������� ������� ����� ����������� �����
								remove("checkpoint.txt");
								
								// �������������� ������ ����� ����������� �����
								rename("checkpoint_new.txt", "checkpoint.txt");

								// ����� ����� ������������� ������������ ����������� �����
								*checkpointCounter = 0;
						}
				}
			}
			// ����� ������ ������������ ������
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

	// ���������� ������� ���������� �� �����
	checkpointFile.open(completedCheckpointFileName, std::fstream::in);
	
	if (checkpointFile.is_open())
	{
		// ���������� ������� ���������� �� ����� ����������� �����
			// ���������� ����� ��������
			checkpointFile >> squareRank;

			// ������������ ��������, ��������� �� ����� ��������
			square = new int[squareRank*squareRank];
			version = new int[squareRank];
			row = new int[squareRank];
			dictionary = new Symbol[squareRank];

			// ������������ �������
			for (int i = 0; i < squareRank; i++)
			{
				dictionary[i].Value = i;
				dictionary[i].IsUsed = 0;
			}

			// ���������� ������ ��������
			for (int i = 0; i < squareRank; i++)
			{
				checkpointFile >> version[i];
			}

			// �������� ����� ����������� �����
			checkpointFile.close();

			// ����������� ����� ����������� ����������
			parametersIsRead = 1;
	}

	// ���������� ���������� �������� ���� ����������� ��� ������
	if (parametersIsRead)
	{
		// ���������� ���������� �������� �� ��������� ����������
			// ����� ��������� � ������ ��������
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

			// ���������� ���������� �������� �� �������
			GenerateSubsquare(square, squareRank, version, dictionary, 0, row, &checkpointCounter, checkpointInterval, completedCheckpointFileName, newCheckpointFileName, resultFileName);

			// ������������ ���������� ������
			delete [] square;
			delete [] version;
			delete [] row;
			delete [] dictionary;
	}

	cout << "Computations finished! Press eny key to exit..." << endl;
	cin.get();

	return 0;
}