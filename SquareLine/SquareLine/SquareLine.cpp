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
	/* ��������� - �������� ���������� �������, ������ �������� ������� ���������� - ������, � ����� �������
	return (base == 0 || base == 1) ? 1 : Factorial(base - 1)*base;
	*/

	int result = 0;
	int table[12] = { 1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880, 3628800, 39916800 };

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

/*// checkpointVersion - ������ �������� �� ����������� �����
// version - ��������� �� ������ � �������� �����
// squareRank - ���� ��������, ����� ������� � �������� �����

// ����������: ���� ������ �������� �� ����������� (�.�. ���� ������ � ������� -1)
// �� ��������� ������ ������ ��������� � ������� �� ����������� �����.
// ���� �� ������ �������� - �����������, �� ������������ ��������� ������ ��������
// ���� ������ �������� ����� �� ������������, �� ������ ������ ������ ������ 1
void CreateStartVersion(int* checkpointVersion, int* version, int squareRank)
{
	int rowId;									// ������������� ������ 
	int maxVersion = Factorial(squareRank) - 1;	// ����� ������������ ������

	// ��������� ����������� ������. ���� ������ �� ����������� - ��� ��������� ��� ���������
	for (int i = 0; i < squareRank; i++)
	{
		version[i] = checkpointVersion[i];
	}

	// ������������ ������ � ����������� �� ������������� ������ ��������
	if (checkpointVersion[squareRank - 1] != -1)
	{
		// ������� ������ = ������ �� ����������� ����� + 1
			rowId = squareRank - 1;

			// ���� ������, ������ ������� ��� ����� �����������
			while (version[rowId] + 1 > maxVersion)
			{
				// ���� ������ ����� �� ������������ - ����������� -1 � ������� � ����������� ������
				version[rowId] = -1;
				rowId--;
			}
			// ����������� �� ������� ������ ������, �� ������� ������������
			version[rowId]++;
	}
}

// ����������� ���������� � ���������� ������ ��������
// squareRank - ���� ��������
// version - ����� ������, � ������� ����� ������
void ShowStartParameters(int squareRank, int* version)
{
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
}

// �������� ����� �������� �� ���������������
// ��� ��� ��������� � ������ ������ ��������� - ����� ������ �� �����������
// �������������� �������� ���������� ��������� �������� ������ (checkedRowId) � ����������
// squareRank - ���� ��������
// version - ����������� ������
// rowVersions - �������� ��������� ������
// checkedRowId - ����������� ������
int CheckSquarePart(int squareRank, int* square, int checkedRowId)
{
	int duplicationDetected = 0;

	if (checkedRowId != 0)
	{
		// �������� ������������ ����� ��������
			// �������� ������ ��������� - ��������� [0; 0] - [maxRowId; maxRowId]
			for (int rowId = checkedRowId - 1; rowId >= 0  && duplicationDetected == 0; rowId--)
			{
				// �������� ���������� �������� [checkedRowId; checkedRowId] � �������� [rowId; rowId]
				if (square[checkedRowId*squareRank + checkedRowId] == square[rowId*squareRank + rowId])
				{
					duplicationDetected = 1;
				}
			}

			// �������� ������ ��������� - ��������� [0; squareRank - 1] - [checkedRowId; squareRank - checkedRowId - 1]
			for (int rowId = checkedRowId - 1; rowId >= 0 && duplicationDetected == 0; rowId--)
			{
				// �������� ���������� �������� [checkedRowId; squareRank - checkedRowId - 1] � ��������� [rowId; squareRank - rowId - 1]
				if (square[checkedRowId*squareRank + squareRank - checkedRowId - 1] == square[rowId*squareRank + squareRank - rowId - 1])
				{
					duplicationDetected = 1;
				}
			}

			// �������� �������� ��������
			for (int itemId = squareRank - 1; itemId >= 0 && duplicationDetected == 0; itemId--)
			{
				// �������� ������� itemId
				for (int rowId = checkedRowId - 1; rowId >= 0  && duplicationDetected == 0; rowId--)
				{
					// �������� �� ���������� �������� [checkedRowId; itemId] � [rowid; itemId]
					if (square[checkedRowId*squareRank + itemId] == square[rowId*squareRank + itemId])
					{
						duplicationDetected = 1;
					}
				}
			}
	}

	return !duplicationDetected;
}


// �������� �������� �� ��, ��� �� �������� ���������
// squareRank - ���� ��������
// version - ������ ��������
// rowVersions - ������ � �������� �� ������� � ���������� ���������� �����
// ����������: ��� ��� ������� ����������� ��������� �� ��������,
// �� �������� �� ������ ����� - �������������, �� ��� ���������
// ��� ����, ����� ������� ���� �������������, � ��� ����� ��������
// ���������� �� ��������� ���������, ������� ����������� ��������
// ������ ��������
int IsSquareLatin(int squareRank, int* square)
{
	int duplicationDetected = 0;

	// �������� �������� ��������
	for (int columnId = 0; columnId < squareRank && !duplicationDetected; columnId++)
	{
		// �������� ������������ ������� columnId
		for (int rowId = 0; rowId < squareRank && !duplicationDetected; rowId++)
		{
			// �������� �� ���������� �������� [rowId; columnId] �� ����� ���������� ���������� ������� columndId
			for (int comparedRowId = rowId + 1; comparedRowId < squareRank && !duplicationDetected; comparedRowId++)
			{
				if (square[comparedRowId*squareRank + columnId] == square[rowId*squareRank + columnId])
				{
					duplicationDetected = 1;
				}
			}
		}
	}

	// �������� ����� ��������
	for (int rowId = 0; rowId < squareRank && !duplicationDetected; rowId++)
	{
		// �������� ������������ ������ rowId
		for (int columnId = 0; columnId < squareRank && !duplicationDetected; columnId++)
		{
			// �������� �� ���������� �������� [rowId; columnId] �� ����� ���������� ���������� ������ rowId
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

// �������� �������� �� ��, ��� �� �������� ������������
// squareRank - ���� ��������
// version - ������ ��������
// rowVersions - ������ � �������� �� ������� � ���������� ���������� �����
int IsSquareDiagonal(int squareRank, int* square)
{
	int duplicationDetected = 0;

	// �������� ������ ��������� - ��������� [0;0] - [rank;rank]
	for (int itemId = 0; itemId < squareRank && !duplicationDetected; itemId++)
	{
		// �������� �� ���������� �������� [itemId; itemId] �� ����� ���������� ���������� ���������
		for (int comparedId = itemId + 1; comparedId < squareRank && !duplicationDetected; comparedId++)
		{
			if (square[itemId*squareRank + itemId] == square[comparedId*squareRank + comparedId])
			{
				duplicationDetected = 1;
			}
		}
	}

	// �������� ������ ��������� - ��������� [rank - itemId - 1; itemId]
	for (int itemId = 0; itemId < squareRank && !duplicationDetected; itemId ++)
	{
		// �������� �� ���������� �������� [rank - itemId - 1; itemId] �� ����� ���������� ���������� ���������
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

// ������������ ����������� �����
// completedCheckpointFileName - ��� ����� � ����������� ����������� ������
// newCheckpointFileName - ��� ����� � ����� ����������� ������
// squareRank - ���� ��������
// version - ������ ��������
void CreateCheckpoint(char* completedCheckpointFileName, char* newCheckpointFileName, int squareRank, int* version)
{
	fstream newCheckpointFile;

	// ������������ ����������� �����
		// �������� ������ ����� ����������� �����
		newCheckpointFile.open(newCheckpointFileName, std::ios_base::out);
		newCheckpointFile << squareRank;

		for (int i = 0; i < squareRank; i++)
		{
			newCheckpointFile << " " << version[i];
		}

		newCheckpointFile.close();

		// �������� ������� ����� ����������� �����
		remove(completedCheckpointFileName);
								
		// �������������� ������ ����� ����������� �����
		rename(newCheckpointFileName, completedCheckpointFileName);
}

// ������ ���������� �������� � ����
// resultFileName - ��� ����� � ������� ���������� ���������
// squareRank - ���� ��������
// version - ������ ��������
// rowVersions - ������ � �������� �� ������� � ���������� ���������� �����
void SaveSquare(char* resultFileName, int squareRank, int* version, int* square)
{
	fstream resultFile;

	// ����� �������� � ���� � ������������
	resultFile.open(resultFileName, std::ios_base::app);
	resultFile << "Version" << '\t' << "Square" << endl;

	// ����� ������ ����� � ��������� ����� ���������
	for (int printedRowId = 0; printedRowId < squareRank; printedRowId++)
	{
		// ����� ��������� ������
			// ����� ������ ������
			resultFile << version[printedRowId] << '\t';
			// ����� ��������� ������
			for (int printedColumnId = 0; printedColumnId < squareRank; printedColumnId++)
			{
				resultFile << square[printedRowId*squareRank + printedColumnId] << " ";
			}
			// �������� ������
			resultFile << endl;
	}
	resultFile << endl;
	
	resultFile.close();
}

// ������������ ������ ���������
// squareRank - ���� ��������
// version - ������, � ������� �������� ���������
// rowVersions - ������ ������, ������� ����� ���� � ������
void GenerateSquares(int squareRank, int* version, int* rowVersions, Symbol* dictionary, char* completedCheckpointFileName, char* newCheckpointFileName, char* resultFileName, int checkpointInterval)
{
	int rowId;									// �������������� ������
	int maxVersion = Factorial(squareRank) - 1;	// ���������� ����� ������
	int checkpointCounter = 0;					// ������� �������� ����� ������������ �������
	int* square;								// ������ - �������

	// �������� ������ ��� ������
	square = new int[squareRank*squareRank];
	// "��������" ������ � ���������
	for (int i = 0; i < squareRank*squareRank; i++)
	{
		square[i] = -1;
	}

	// ���������� ������� ������
	rowId = squareRank - 1;
	while (version[rowId] == -1)
	{
		rowId--;
	}
	
	// ��������� ��������� �������
	for (int i = 0; i < squareRank; i++)
	{
		// ��������� i-� ������ ��������
		CreateVectorVersion(square + i*squareRank, squareRank, version[i], dictionary);
	}

	// ���������� ��� ��������� �������� ��� 0-� ������ ������ ������ (� ������ - � �� ����)
	while (version[0] == 0)
	{
		// ������������ ���������� ��� ������ ���������
		if (rowId == squareRank -1)
		{
			// ������ � ��������� ������� ��������
				// �������� � ���������� ���������� ��������
				if (CheckSquarePart(squareRank, square, rowId))
				{
					// ��������� ���������� ��������
					if (IsSquareLatin(squareRank, square) && IsSquareDiagonal(squareRank, square))
					{
						// ��������� ���������� � ������������ ��������
							// ������ ���������� �������� � ����
							SaveSquare(resultFileName, squareRank, version, rowVersions);
							// �������� ����������� �����
							CreateCheckpoint(completedCheckpointFileName, newCheckpointFileName, squareRank, version);
					}
				}

				// ������� � ���������� ��������
					// ����������� ������ ������ rowId
						// ��������� ������������ ������
						while (version[rowId] + 1 > maxVersion)
						{
							// ���� ������ ����� �� ������������ - "���������" ������ � ��������, ����������� -1 � ������ � ������� � ����������� ������
							for (int i = 0; i < squareRank; i++)
							{
								square[rowId*squareRank + i] = -1;
							}
	
							version[rowId] = -1;
							rowId--;
						}

						// ������� � ��������� ������
							// ���������� ������ ������
							version[rowId]++;
							// ��������� ������ ��������
							CreateVectorVersion(square + rowId*squareRank, squareRank, version[rowId], dictionary);
		}
		else
		{
			// ������ � �������������� ��������
				// �������� �������������� ��������
				if (CheckSquarePart(squareRank, square, rowId))
				{
					// ����������� ������ � ������������� ��������
						// ��������� � ��������� ������ � ���������� � � ��������
						rowId++;
						version[rowId] = 0;
						CreateVectorVersion(square + rowId*squareRank, squareRank, version[rowId], dictionary);
				}
				else
				{
					// ������� � ��������� ������ � ��������� �� ����������� �� ������ ������ ����� (��� ���������� ���� ����)
						// ��������� ������������ ������
						while (version[rowId] + 1 > maxVersion)
						{
							// ���� ������ ����� �� ������������ - "���������" ������ � ��������, ����������� -1 � ������ � ������� � ����������� ������
							for (int i = 0; i < squareRank; i++)
							{
								square[rowId*squareRank + i] = -1;
							}
	
							version[rowId] = -1;
							rowId--;
						}
						
						// ������� � ��������� ������
							// ���������� ������ ������
							version[rowId]++;
							// ��������� ������ ��������
							CreateVectorVersion(square + rowId*squareRank, squareRank, version[rowId], dictionary);
				}
		}

		// ������������ ����������� �����
		checkpointCounter++;
		if (checkpointCounter % checkpointInterval == 0)
		{
			CreateCheckpoint(completedCheckpointFileName, newCheckpointFileName, squareRank, version);
		}
	}

	// �������� ���������� ������
	CreateCheckpoint(completedCheckpointFileName, newCheckpointFileName, squareRank, version);
}*/

int CheckLinesCompatibility(int squareRank, int versionA, int versionB, int* versions)
{
	int duplicationDetected = 0;

	for (int i = 0; i < squareRank && !duplicationDetected; i++)
	{
		if (versions[squareRank*versionA + i] == versions[squareRank*versionB + i])
		{
			duplicationDetected = 1;
		}
	}

	return !duplicationDetected;
}

int CheckFirstDiagonalCompatibility(int squareRank, int versionA, int versionB, int* versions)
{
	int duplicationDetected = 0;

	// �������� ������������ ������ ���������, ��������������� �������� ������
	if (versions[squareRank*versionA + 0] == versions[squareRank*versionB + 0])
	{
		// �������� �� ���������� ���� ��������� ����� 0-��
		for (int i = 1; i < squareRank && !duplicationDetected; i++)
		{
			if (versions[squareRank*versionA + i] == versions[squareRank*versionB + i])
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

int CheckDiagonalsCompatibility(int squareRank, int versionA, int versionB, int versionC, int* versions)
{
	int duplicationDetected = 0;

	// �������� ������������ ������ � ������ ���������, ��������������� �������� ������� - 
	// �� ���������� ���� ��������� ����� 0-�� ��� ������ ��������� � ������ ������ � ���������� - ��� ������ ��������� � ������ ������
	if (versions[squareRank*versionA + 0] == versions[squareRank*versionB + 0] && versions[squareRank*versionA + squareRank - 1] == versions[squareRank*versionC + squareRank - 1])
	{
		for (int i = 0; i < squareRank && !duplicationDetected; i++)
		{
			// �������� ������������ i-� ����������� ����������
				// ��������� i-�� ���������� ������ ��������� � ������ ������
				if (versions[squareRank*versionA + i] == versions[squareRank*versionB + i] && i != 0)
				{
					duplicationDetected = 1;
				}

				// ��������� i-�� ���������� ������ � ������ ���������
				if (versions[squareRank*versionB + i] == versions[squareRank*versionC + i])
				{
					duplicationDetected = 1;
				}

				// ��������� i-�� ���������� ������ ��������� � ������ ������
				if (versions[squareRank*versionA + i] == versions[squareRank*versionC + i] && i != squareRank - 1)
				{
					duplicationDetected = 1;
				}

				// ��������� i-�� ���������� ������ ��������� � ��������� - (squareRank - 1 - i)-�� ���������� ������ ���������
				if (versions[squareRank*versionB + i] == versions[squareRank*versionC + squareRank - 1 - i])
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

int main(int argumentsCount, char* argumentsValues[])
{
/*	// ���������� ������� ����������
	int parametersIsRead = 0;									// ���� ���������� ������� ����������
	int checkpointInterval = 10000000;							// ����� �������� ����� ������������ �������
	char* completedCheckpointFileName = "checkpoint.txt\0";		// ���� ����������� ����������� �����
	char* newCheckpointFileName = "checkpoint_new.txt\0";		// ���� ����������� ����������� �����
	char* resultFileName = "result.txt";						// ���� � ������������ ��������
	fstream checkpointFile;										// ����� ��� ������ � ������ ����������� �����*/

	// ��������� ������ ������ ���������
	int squareRank;				// ���� ��������
	Symbol* dictionary;			// ������ �� �������
	int* rowVersions;			// ������ ���������� �� ��� ��������� ����������, ������� ����� ���������� ������
	int rowCombinations;		// ����� ��������� ����������
	int compatibleRows;			// ����� ����������� ���������� �����, ����������
	long long compatibleDiagonals;
	int counter;				// ������� ��� ������ �������������� ����������
	int firstCompatibleVersion;
	int lastCompatibleVersion;
	int isCompatible;

	/*int* versionFromCheckpoint;	// ������ ��������, ��������� �� ����� ����������� �����
	int* version;				// ������� �������������� ������*/

	/*// C��������� ���������� �� �����
		// �������� ����� c ������� ����������� �����
		checkpointFile.open(completedCheckpointFileName, std::fstream::in);
		// ���������� ������� ����������
		if (checkpointFile.is_open())
		{
			// ���������� ������� ���������� �� ��������� ����� ����������� �����
				// ���������� ����� ��������
				checkpointFile >> squareRank;

				// ���������� ������ ��������
				versionFromCheckpoint = new int[squareRank];
				for (int i = 0; i < squareRank; i++)
				{
					checkpointFile >> versionFromCheckpoint[i];
				}

				// �������� ����� ����������� �����
				checkpointFile.close();

				// ����������� ����� ����������� ����������
				parametersIsRead = 1;
	}

	// ������������ ��������, ��������� �� ���������� �� ����� 
	if (parametersIsRead)
	{
		// ������������ �������� �� ��������� ����������
			// ������������ �������
			dictionary = new Symbol[squareRank];
			for (int i = 0; i < squareRank; i++)
			{
				dictionary[i].Value = i;
				dictionary[i].IsUsed = 0;
			}

			// ���������� ���� ��������� ������ �����
				// ������������ ������� ���������� �� ������
				rowCombinations = Factorial(squareRank);
				rowVersions = new int [rowCombinations*squareRank];

				// ������������ ������
				for (int i = 0; i < rowCombinations; i++)
				{
					// ������������ i-� ������
					CreateVectorVersion(rowVersions + squareRank*i, squareRank, i, dictionary);
				}

			// ������������ ������� ������� ������
			version = new int[squareRank];
			CreateStartVersion(versionFromCheckpoint, version, squareRank);
		}

	// �������� ���� ������ ��������� - ����� ���������
	if (parametersIsRead)
	{
		// ����� ���������� � ������ �������
		ShowStartParameters(squareRank, versionFromCheckpoint);
		
		// ������������ ���������
		GenerateSquares(squareRank, version, rowVersions, dictionary, completedCheckpointFileName, newCheckpointFileName, resultFileName, checkpointInterval);
	}

	// ������������� ������
	if (parametersIsRead)
	{
		// �������� �������� - ������ �� ����� ����������� ����� � ������� ������
		delete [] versionFromCheckpoint;
		delete [] version;

		// �������� ������� � �������� ������
		delete [] rowVersions;
		// �������� �������
		delete [] dictionary;
	}*/

	// ������� ����� ��������
	squareRank = 10;
	compatibleRows = 0;
	compatibleDiagonals = 0;
	counter = 0;
	firstCompatibleVersion = -1;
	lastCompatibleVersion = -1;
	isCompatible = 0;

	// ������������ �������� �� ��������� ����������
		// ������������ �������
		dictionary = new Symbol[squareRank];
		for (int i = 0; i < squareRank; i++)
		{
			dictionary[i].Value = i;
			dictionary[i].IsUsed = 0;
		}

		// ���������� ���� ��������� ������ �����
			// ������������ ������� ���������� �� ������
			rowCombinations = Factorial(squareRank);
			rowVersions = new int [rowCombinations*squareRank];
			
			// ������������ ������
			for (int i = 0; i < rowCombinations; i++)
			{
				// ������������ i-� ������
				CreateVectorVersion(rowVersions + squareRank*i, squareRank, i, dictionary);
			}

	// �������� ����� ����������� �����
	for (int i = 0; i < rowCombinations; i++)
	{
		//compatibleRows += CheckLinesCompatibility(squareRank, 0, i, rowVersions);
		//compatibleRows += CheckFirstDiagonalCompatibility(squareRank, 0, i, rowVersions);
		isCompatible = CheckFirstDiagonalCompatibility(squareRank, 0, i, rowVersions);
		if (isCompatible)
		{
			compatibleRows++;
			lastCompatibleVersion = i;
			if (compatibleRows == 1)
			{
				firstCompatibleVersion = i;
			}
		}
	}

	/*for (int i = 41067; i <= 41067; i++)*/
	/*for (int i = 41067; i <= 362855; i++)*/
	for (int i = firstCompatibleVersion; i <= lastCompatibleVersion; i++)
	{
		for (int j = 0; j < rowCombinations; j++)
		{
			compatibleDiagonals += CheckDiagonalsCompatibility(squareRank, 0, i, j, rowVersions);

			if (counter++ == 100000000)
			{
				cout << "i = " << i << ";\t j = " << j << ";\t Diagonals = " << compatibleDiagonals << endl;
				counter = 0;
			}
		}
	}

	// ������������� ������
		// �������� ������� � �������� ������
		delete [] rowVersions;
		// �������� �������
		delete [] dictionary;

	// ���������� ������
	cout << "Compatible rows: " << compatibleRows << endl;
	cout << "First version: " << firstCompatibleVersion << endl;
	cout << "Last version: " << lastCompatibleVersion << endl;
	cout << "Compatible diagonals: " << compatibleDiagonals << endl;
	cout << "Press any key to continue..." << endl;
	cin.get();

	return 0;
}