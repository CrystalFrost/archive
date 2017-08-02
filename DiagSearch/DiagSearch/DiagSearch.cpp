# include <iostream>
# include <fstream>

using namespace std;

// ��������� - ������ �������
struct Symbol
{
	int Value;
	int IsUsed;
};

// ����� - �������� ���������
class SquareFinder
{
public:
	SquareFinder();		// ����������� �� ���������, ��������� ������ �� checkpoint-�
	~SquareFinder();	// ����������
	void Start();		// ������ �������� ����������

private:
	static int Factorial(int base);																	// ���������� ����������
	static void CreateVectorVersion(int* vector, int vectorRank, int version, Symbol* dictionary);	// �������� ������� �� ������ ������������ ��� �����������
	void Commit();					// ������������ ����������� �����
	void Restore();					// �������������� �������� ������� �� ����������� �����
	void SaveSquare();				// ���������� ���������� �������� � ���� �����������
	void ShowStartParameters();		// ����� � ������� ���������� �������
	int CheckPrimaryDiagonalCompatibility(int diagonalPVersion);							// �������� ������������ ������� ��������� ������������� ������
	int CheckSecondaryDiagonalCompatibility(int diagonalPVersion, int diagonalSVersion);	// �������� ������������ �������� ��������� ������� � ������������� ������
	int CheckColumnCompatibility(int ColumnIndex, int ColumnVersion);						// �������� ������������� ������� ������ ��� ��������� ������� � ���������� ���������� ��������
	int IsSquareLatin();			// �������� ����, ��� ������� - ���������
	int IsSquareDiagonal();			// �������� �������������� ��������

	int Rank;						// ���� ��������
	int FixedLineVersion;			// ������ ������������� ������
	int DiagonalPVersion;			// ������ ������� ���������
	int DiagonalSVersion;			// ������ �������� ���������
	int* ColumnsVersions;			// ������ � �������� ��������

	int ColumnIndex;				// ����� �������, ��������������� � ������ ������
	int ColumnVersion;				// ����� ������ �������, �������������� � ������ ������

	int* FixedLine;					// ������ � ���������� ������������� ������
	int* DiagonalP;					// ������ � ���������� ������� ���������
	int* DiagonalS;					// ������ � ���������� �������� ���������
	int** Matrix;					// ������ � ����������� �� ������� - ������ ������� ��������

	int* VectorVersions;			// ������ � �������� ������ ��������

	char* ResultFileName;			// ������ � ��������� ����� � ������������
	char* NewCheckpointFileName;	// ������ � ��������� ����� ����� ����������� �����
	char* CheckpointFileName;		// ������ � ��������� ����� ����������� ����������� �����

	Symbol* Dictionary;				// ������ �� �������

	int CheckpointInterval;			// �������� �������� ����������� ����� � ����� ��������
};


// ����������� �� ���������
SquareFinder::SquareFinder()
{
	// ����� �������� ������ � ����������� ������ � ������������ �������
	ResultFileName = "result.txt";
	NewCheckpointFileName = "checkpoint_new.txt";
	CheckpointFileName = "checkpoint.txt";

	// ����� �������� �������� ����������� �����
	CheckpointInterval = 1000000000;

	// ��������������� ��������� ������
	Restore();
}


// ����������
SquareFinder::~SquareFinder()
{
	// ������������� ������� ������
		// �������� �������
		delete [] Dictionary;

		// �������� ������ ��������
		delete [] VectorVersions;

		// �������� ������������� ������, ������� � �������� ����������
		delete [] FixedLine;
		delete [] DiagonalP;
		delete [] DiagonalS;

		// �������� ������� ��������
		for (int i = 0; i < Rank; i++)
		{
			delete [] Matrix[i];
		}
		
		delete [] Matrix;
}


// ���������� ������� ����������
int SquareFinder::Factorial(int base)
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
void SquareFinder::CreateVectorVersion(int* vector, int vectorRank, int version, Symbol* dictionary)
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


// ������ ������
void SquareFinder::Start()
{
	int maxVersion = Factorial(Rank) - 1;		// ������������ ����� ������
	int checkpointCounter = 0;					// ������� ������ ��� �������� ����������� �����

	// ����� ���������� � ����� ������ ������
	ShowStartParameters();

	// ������ ������ �� ������ �� ����������� �����
	while (VectorVersions[DiagonalPVersion*Rank] == 0)
	{
		// �������� ���������� �������� ������� ���������
		if (CheckPrimaryDiagonalCompatibility(DiagonalPVersion))
		{
			// ��������� ����������� �������� ������� ���������
				// ������ ��������� � ������� � ���������� ������
				for (int i = 0; i < Rank; i++)
				{
					DiagonalP[i] = VectorVersions[DiagonalPVersion*Rank + i];
					Matrix[i][i] = VectorVersions[DiagonalPVersion*Rank + i];
				}

				// ������� ��������� �������� ���������
				while (DiagonalSVersion < maxVersion)
				{
					// �������� ���������� �������� �������� ���������
					if (CheckSecondaryDiagonalCompatibility(DiagonalPVersion, DiagonalSVersion))
					{
						// ��������� ����������� �������� �������� ���������
							// ������ ��������� � ������� � ���������� ������
							for (int i = 0; i < Rank; i++)
							{
								DiagonalS[i] = VectorVersions[DiagonalSVersion*Rank + i];
								Matrix[Rank - 1 - i][i] = VectorVersions[DiagonalSVersion*Rank + i];
							}

							// ������ �������� ��� ���������� {"������ ������������� ������"; "������� ���������"; "�������� ���������"}
							// ����������: � ��������� ���� ��������� ��������� ������ �� ���������:
							// ColumnsVersions - ������ � �������� ��� ����������� ������. �� ���� ���� � 10-�� ������� � ������� - �� ����������, �� ����������� �����
							// ColumnIndex � ColumnVersion - ������� ����������� �������� ������� � ��� ������. ColumnVersion �� ����������� ������ ���� � ColumnsVersion
							do
							{
								// �������� ��������� ������ ���������� �������
									// �������� ������� ������
									if (CheckColumnCompatibility(ColumnIndex, ColumnVersion))
									{
										// ������ ������� � �������, �������� ��������
											// ������ ������ � �������
											ColumnsVersions[ColumnIndex] = ColumnVersion;

											// ������ ������� � �������
											for (int i = 0; i < Rank; i++)
											{
												Matrix[i][ColumnIndex] = VectorVersions[ColumnVersion*Rank + i];
											}

											// �������������� �������� ��������, ���� �� �������� �������
											if (ColumnIndex == Rank - 1 && IsSquareLatin() && IsSquareDiagonal())
											{
												SaveSquare();
											}
											else
											{
												// ������� � ���������� ������� (������)
												ColumnIndex++;
												// ����������� ������� - ColumnVersion = -1; - ������ �� ������� ������������ ������ � �������� ����� �������� �����,
												// ��� ���, ������ �������� ������ ��������������� ColumnIndex-�������� ������ ������.
												// �� ���� ��� �� ������� ������, �� ��� �� ����� �������� ���������!
												// ����������� ����������� � ����� "������� � ��������� ������"
												ColumnVersion = ColumnIndex * (maxVersion + 1) / Rank - 1 >= 0 ? ColumnIndex * (maxVersion + 1) / Rank - 1 : 0;
											}
									}

									// ������� � ��������� ������
									// �����������, ������ � "��������� � ���������� ������� (������)"
									// �������������� ������� - if (ColumnVersion < maxVersion)
									if (ColumnVersion < maxVersion && ColumnVersion < (ColumnIndex + 1) * (maxVersion + 1) / Rank + 1)
									{
										// ����������� ������
										ColumnVersion++;
									}
									else
									{
										// �������� �������� �������, ������� � �����������
											// �������� ������ ������� � ������� ������
											ColumnsVersions[ColumnIndex] = 0;
											// �������� ������� (����� ������ ������ � ����������) � �������
											for (int i = 0; i < Rank; i++)
											{
												if (i != 0 && i != ColumnIndex && i != Rank - 1 - ColumnIndex)
												{
													Matrix[i][ColumnIndex] = 0;
												}
											}
											// ��������� � ����������� �������, �������� ��� ������ � ��������� �
											ColumnIndex--;
											ColumnVersion = ColumnsVersions[ColumnIndex] + 1;
									}

									// �������� ����������� �����
									if (++checkpointCounter == CheckpointInterval)
									{
										checkpointCounter = 0;
										Commit();
									}
							}
							while (ColumnIndex != 0 || VectorVersions[ColumnVersion*Rank + 0] == 0);

							// �������� ������� ������� ��� ����� ������ ����������
							for (int i = 0; i < Rank; i++)
							{
								if (i != 0 && i != ColumnIndex && i != Rank - 1 - ColumnIndex)
								{
									Matrix[i][ColumnIndex] = 0;
								}
							}
							
							ColumnVersion = 0;
					}

					// ������� � ��������� ������
					DiagonalSVersion++;
				}

				// ����� ������ ������ �������� ��������� ��� ������ �����
				DiagonalSVersion = 0;

				// ������� � ��������� ������ ������� ���������
				DiagonalPVersion++;
		}
		else
		{
			// ������� � ��������� ������
			DiagonalPVersion++;
		}
	}
}

// ������������ ����������� �����
void SquareFinder::Commit()
{
	fstream checkpointFile;	// ����� ��� ������ � ������ ����������� �����

	// ������������ ����������� �����
		// �������� �����
		checkpointFile.open(NewCheckpointFileName, std::ios_base::out);

		// ������ ��������� �������
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

		// �������� �����
		checkpointFile.close();

		// �������� ������� ����� ����������� �����
		remove(CheckpointFileName);
								
		// �������������� ������ ����� ����������� �����
		rename(NewCheckpointFileName, CheckpointFileName);
}


// �������������� ��������� ������ �� ����� ����������� �����
void SquareFinder::Restore()
{
	fstream checkpointFile;				// ����� ��� ������ � ������ ����������� �����
	int parametersIsRead = 0;			// ���� ���������� ������� ����������
	int versionsCount = 0;				// ����������� ��������� ����� ������ ��������

	// C��������� ���������� �� �����
		// �������� ����� c ������� ����������� �����
		checkpointFile.open(CheckpointFileName, std::fstream::in);
		// ���������� ������� ����������
		if (checkpointFile.is_open())
		{
			// ���������� ������� ���������� �� ��������� ����� ����������� �����
				// ���������� ����� ��������
				checkpointFile >> Rank;

				// ���������� ������ ������ ������������� ������
				checkpointFile >> FixedLineVersion;

				// ���������� ������ ����������
				checkpointFile >> DiagonalPVersion;
				checkpointFile >> DiagonalSVersion;

				// ���������� ������ ��������
				ColumnsVersions = new int[Rank];
				for (int i = 0; i < Rank; i++)
				{
					checkpointFile >> ColumnsVersions[i];
				}

				// ���������� ������ ��������������� �������
				checkpointFile >> ColumnIndex;

				// ���������� ��������� ������������ ������ �������� �������
				checkpointFile >> ColumnVersion;

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
			Dictionary = new Symbol[Rank];
			for (int i = 0; i < Rank; i++)
			{
				Dictionary[i].Value = i;
				Dictionary[i].IsUsed = 0;
			}

			// ������������ ������ ��������
				// ������������ ������� c ��������
				versionsCount = Factorial(Rank);
				VectorVersions = new int [versionsCount*Rank];

				// ������������ ������
				for (int i = 0; i < versionsCount; i++)
				{
					// ������������ i-� ������
					CreateVectorVersion(VectorVersions + Rank*i, Rank, i, Dictionary);
				}

			// ������������ ������������� ������, ������� � �������� ����������
			FixedLine = new int[Rank];
			DiagonalP = new int[Rank];
			DiagonalS = new int[Rank];

			for (int i = 0; i < Rank; i++)
			{
				FixedLine[i] = VectorVersions[FixedLineVersion*Rank + i];
				DiagonalP[i] = VectorVersions[DiagonalPVersion*Rank + i];
				DiagonalS[i] = VectorVersions[DiagonalSVersion*Rank + i];
			}

			// ��������� ������ ��� ������� ��������
			Matrix = new int*[Rank];
			for (int i = 0; i < Rank; i++)
			{
				Matrix[i] = new int[Rank];
			}

			// ���������� ������ ������ �������
			for (int i = 0; i < Rank; i++)
			{
				Matrix[0][i] = FixedLine[i];
			}

			// ���������� �������� �������
			for (int j = 0; j < Rank; j++)
			{
				// ��������� ��������� ����� j-�� �������, ����� 1-� (��� - �����������)
				for (int i = 1; i < Rank; i++)
				{
					// ���������� �������� i-� ������ � j-�� �������
					Matrix[i][j] = VectorVersions[ColumnsVersions[j]*Rank + i];
				}
			}
	}
}


// ������ ���������� �������� � ����
void SquareFinder::SaveSquare()
{
	fstream resultFile;					// ����� ��� ������ � ������ �����������

	// ����� �������� � ���� � ������������
	resultFile.open(ResultFileName, std::ios_base::app);

	// ����� ������ ����������
	resultFile << "Diagonals: " << DiagonalPVersion << " " << DiagonalSVersion << "; ";

	// ����� ������ ��������
	resultFile << "Columns: ";
	for (int i = 0; i < Rank; i++)
	{
		resultFile << ColumnsVersions[i] << " ";
	}
	resultFile << endl;

	// ����� ������� ��������
	for (int printedRowId = 0; printedRowId < Rank; printedRowId++)
	{
		// ����� ��������� ������
			// ����� ��������� ������
			for (int printedColumnId = 0; printedColumnId < Rank; printedColumnId++)
			{
				resultFile << Matrix[printedRowId][printedColumnId] << " ";
			}
			// �������� ������
			resultFile << endl;
	}
	resultFile << endl;
	
	resultFile.close();
}


// �������� ������������� ������� ��������� � ������ �������
// diagonalPVersion - ������ ������� ���������
int SquareFinder::CheckPrimaryDiagonalCompatibility(int diagonalPVersion)
{
	int duplicationDetected = 0;

	// �������� ������������ ������ ���������, ��������������� �������� ������
	if (VectorVersions[Rank*FixedLineVersion + 0] == VectorVersions[Rank*diagonalPVersion + 0])
	{
		// �������� �� ���������� ���� ��������� ����� 0-��
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


// �������� ������������� �������� ��������� � ������ ������� � ������� ����������
// diagonalPVersion - ������ ������� ���������
// diagonalSVersion - ������ �������� ���������
int SquareFinder::CheckSecondaryDiagonalCompatibility(int diagonalPVersion, int diagonalSVersion)
{
	int duplicationDetected = 0;

	// �������� ������������ ������� � �������� ���������, ��������������� �������� ������� - 
	// �� ���������� ���� ��������� ����� 0-�� ��� ������� ��������� � ������ ������ � ���������� - ��� �������� ��������� � ������ ������
	// � �����, � ������ ��������� ����� �������� - ���������� ������� ����������
	if (VectorVersions[Rank*FixedLineVersion + 0] == VectorVersions[Rank*FixedLineVersion + 0] && VectorVersions[Rank*FixedLineVersion + Rank - 1] == VectorVersions[Rank*diagonalSVersion + Rank - 1])
	{
		// �������� ���������� ������� ���������� ��� ��������� ��������� �����
		if (Rank % 2 == 1 && VectorVersions[Rank*diagonalPVersion + (Rank - 1)/2] != VectorVersions[Rank*diagonalSVersion + (Rank - 1)/2])
		{
			duplicationDetected = 1;
		}

		// �������� �� ��������� ��������� ���������
		for (int i = 0; i < Rank && !duplicationDetected; i++)
		{
			// �������� ������������ i-� ����������� ����������
				// ��������� i-�� ���������� �������� ��������� � ������ ������
				if (VectorVersions[Rank*FixedLineVersion + i] == VectorVersions[Rank*diagonalSVersion + i] && i != Rank - 1)
				{
					duplicationDetected = 1;
				}

				// ��������� i-�� ���������� ������� � �������� ���������
				if (VectorVersions[Rank*diagonalPVersion + i] == VectorVersions[Rank*diagonalSVersion + i])
				{
					// �������� ��������� �� �������� �������� ��������� �����
					if (Rank % 2 == 0 || i*2 + 1 != Rank)
					{
						duplicationDetected = 1;
					}
				}

				// ��������� i-�� ���������� ������ ��������� � ��������� - (squareRank - 1 - i)-�� ���������� ������ ��������� (��������� � ������ ������)
				if (VectorVersions[Rank*diagonalPVersion + i] == VectorVersions[Rank*diagonalSVersion + Rank - 1 - i])
				{
					// �������� ��������� �� �������� �������� ��������� �����
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


// �������� ������������� ������� � ������������� �������, ����������� � ��������� � �����
// columnIndex - ��������� �������, � ������� ����� �������� ������ columnVersion
// columnVersion - ������ �������, ������� ��������� �� ����������� ������ � ������� columnIndex
int SquareFinder::CheckColumnCompatibility(int columnIndex, int columnVersion)
{
	int duplicationDetected = 0;

	// �������� ������������� �������
	if(VectorVersions[columnVersion*Rank + 0] == Matrix[0][columnIndex]
		&& VectorVersions[columnVersion*Rank + columnIndex] == Matrix[columnIndex][columnIndex]
		&& VectorVersions[columnVersion*Rank + Rank - 1 - columnIndex] == Matrix[Rank - 1 - columnIndex][columnIndex])
	{
		// �������� ������������� ���� ����� ������� � �����������
		for (int j = 0; j < Rank && !duplicationDetected; j++)
		{
			// �������� �� ���������� j-�� �������� ������� � columnIndex-�������� ������ ������
			if (VectorVersions[columnVersion*Rank + j] == Matrix[0][columnIndex] && j != 0)
			{
				duplicationDetected = 1;
			}

			// �������� �� ���������� j-�� �������� ������� � j-�� �������� ������� ���������
			if (VectorVersions[columnVersion*Rank + j] == Matrix[j][j] && j != columnIndex)
			{
				duplicationDetected = 1;
			}

			// �������� ���������� j-�� �������� ������� � (Rank - j)-�� �������� �������� ���������
			if (VectorVersions[columnVersion*Rank + j] == Matrix[j][Rank - j - 1] && j != Rank - columnIndex - 1)
			{
				duplicationDetected = 1;
			}
		}

		// �������� ������������� ������� � ���������� ���������
		for (int i = 0; i < columnIndex  && !duplicationDetected; i++)
		{
			// �������� ������������� ��������� ������� � i-� ��������
			for (int j = 0; j < Rank && !duplicationDetected; j++)
			{
				// �������� ������������� ��������� j ��������� ������� � i-�� �������
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


// �������� �������� �� ��, ��� �� �������� ���������
// ����������: ��� ��� ������� ����������� ��������� �� ��������,
// �� �������� �� ������ ����� - �������������, �� ��� ���������
// ��� ����, ����� ������� ���� �������������, � ��� ����� ��������
// ���������� �� ��������� ���������, ������� ����������� ��������
// ������ ��������
int SquareFinder::IsSquareLatin()
{
	int duplicationDetected = 0;

	// �������� �������� ��������
	for (int columnId = 0; columnId < Rank && !duplicationDetected; columnId++)
	{
		// �������� ������������ ������� columnId
		for (int rowId = 0; rowId < Rank && !duplicationDetected; rowId++)
		{
			// �������� �� ���������� �������� [rowId; columnId] �� ����� ���������� ���������� ������� columndId
			for (int comparedRowId = rowId + 1; comparedRowId < Rank && !duplicationDetected; comparedRowId++)
			{
				if (Matrix[comparedRowId][columnId] == Matrix[rowId][columnId])
				{
					duplicationDetected = 1;
				}
			}
		}
	}

	// �������� ����� ��������
	for (int rowId = 0; rowId < Rank && !duplicationDetected; rowId++)
	{
		// �������� ������������ ������ rowId
		for (int columnId = 0; columnId < Rank && !duplicationDetected; columnId++)
		{
			// �������� �� ���������� �������� [rowId; columnId] �� ����� ���������� ���������� ������ rowId
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

// �������� �������� �� ��, ��� �� �������� ������������
int SquareFinder::IsSquareDiagonal()
{
	int duplicationDetected = 0;

	// �������� ������ ��������� - ��������� [0;0] - [rank;rank]
	for (int itemId = 0; itemId < Rank && !duplicationDetected; itemId++)
	{
		// �������� �� ���������� �������� [itemId; itemId] �� ����� ���������� ���������� ���������
		for (int comparedId = itemId + 1; comparedId < Rank && !duplicationDetected; comparedId++)
		{
			if (Matrix[itemId][itemId] == Matrix[comparedId][comparedId])
			{
				duplicationDetected = 1;
			}
		}
	}

	// �������� ������ ��������� - ��������� [rank - itemId - 1; itemId]
	for (int itemId = 0; itemId < Rank && !duplicationDetected; itemId ++)
	{
		// �������� �� ���������� �������� [rank - itemId - 1; itemId] �� ����� ���������� ���������� ���������
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


// ����������� ���������� � ���������� ������ ��������
void SquareFinder::ShowStartParameters()
{
	// ����� ��������� � ������ ��������
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
	SquareFinder* finder;		// ��������� ���������

	// �������� � ������ ����������
	finder = new SquareFinder();
	finder->Start();

	// ������� ������
	delete finder;

	// ���������� ������
	cout << "Press any key to continue..." << endl;
	cin.get();

	return 0;
}