// ����� ��������� ��������� ���������

# include <iostream>
# include <fstream>

using namespace std;

int main()
{
	const int rank = 10;
	const int height = 3;

	int columns[height][rank][rank];
	int rows[height][rank][rank];
	int square[height][rank][rank];
	int cellsHistory[height][rank][rank][rank];
	int pairsABDictionary[rank][rank];
	int pairsACDictionary[rank][rank];
	int pairsBCDictionary[rank][rank];

	int rowId;
	int columnId;
	int layerId;
	int isGet;
	int cellValue;
	int oldCellValue;

	fstream resultFile;
	char* resultFileName = "result.txt";

	int stop = 0;
	int squaresCount = 0;

	// ��������� ������� ����� �������������� �������� ��������������� �������
	for (int h = 0; h < height; h++)
	{
		for (int i = 0; i < rank; i++)
		{
			for (int j = 0; j < rank; j++)
			{
				columns[h][i][j] = 1;
				rows[h][i][j] = 1;
				square[h][i][j] = -1;
			}
		}
	}

	// ������� ������ ������� ������������� �������� � �������
	for (int h = 0; h < height; h++)
	{
		for (int i = 0; i < rank; i++)
		{
			for (int j = 0; j < rank; j++)
			{
				for (int k = 0; k < rank; k++)
				{
					cellsHistory[h][i][j][k] = 1;
				}
			}
		}
	}

	// ������� ������� �������������� ���������� ��� ��������� ���������
	for (int i = 0; i < rank; i++)
	{
		for (int j = 0; j < rank; j++)
		{
			pairsABDictionary[i][j] = 1;
			pairsACDictionary[i][j] = 1;
			pairsBCDictionary[i][j] = 1;
		}
	}

	rowId = 0;
	columnId = 0;
	layerId = 0;

	while (!stop)
	{
		// ���� �������� ��� ������ (layerId; rowId; columnId)
		isGet = 0;
		cellValue = -1;

		for (int i = 0; i < rank; i++)
		{
			// ��������� ����������� ������ �������� i � ������ (rowId, columnId) �������� ������ layerId
			if (columns[layerId][i][columnId] == 1 && rows[layerId][rowId][i] == 1 && cellsHistory[layerId][rowId][columnId][i] == 1)
			{
				// ��������� ���������� ��������
					// ��������� �������� ��� �������� ������� ����
					if (layerId == 0)
					{
						cellValue = i;
						isGet = 1;
					
						break;
					}
					// ��������� �������� ��� �������� ������� ����
					if (layerId == 1)
					{
						// �������� ������������ ���� A || B �� ������������� � ������� ������ ���� ���������
						if (pairsABDictionary[square[0][rowId][columnId]][i] == 1)
						{
							cellValue = i;
							isGet = 1;

							break;
						}
					}

					if (layerId == 2)
					{
						if (pairsACDictionary[square[0][rowId][columnId]][i] == 1 && pairsBCDictionary[square[1][rowId][columnId]][i] == 1)
						{
							cellValue = i;
							isGet = 1;

							break;
						}
					}

			}
		}

		// ��������� ���������� �������
		if (isGet)
		{
			// ��������� ���������� ������ ��������
				// ���������� �������� ��������
				oldCellValue = square[layerId][rowId][columnId];

				// ������ ������ ��������
					// ���������� �������� � �������
					square[layerId][rowId][columnId] = cellValue;
					// �������� �������� � ��������
					columns[layerId][cellValue][columnId] = 0;
					// �������� �������� � �������
					rows[layerId][rowId][cellValue] = 0;
					// �������� �������� � ������� �������� ������
					cellsHistory[layerId][rowId][columnId][cellValue] = 0;

				// ����������� ����������� �������� ��� �������� ������� (��� ��� �� �������� � ���� �������)
				if (oldCellValue != -1)
				{
					// ���������� �������� � �������
					columns[layerId][oldCellValue][columnId] = 1;
					// ���������� �������� � ������
					rows[layerId][rowId][oldCellValue] = 1;
				}

			// �������� ��������� ���� � ��������������
				// �������� ��������� ���� A || B
				if (layerId == 1)
				{
					pairsABDictionary[square[0][rowId][columnId]][square[1][rowId][columnId]] = 0;
				}
				// �������� ��������� ���� A || C � B || C
				if (layerId == 2)
				{
					pairsACDictionary[square[0][rowId][columnId]][square[2][rowId][columnId]] = 0;
					pairsBCDictionary[square[1][rowId][columnId]][square[2][rowId][columnId]] = 0;
				}

				// ���������� ������� �� ������������� ��� ��-�� ����� ��������
				if (oldCellValue != -1)
				{
					// "����������" ���� A || B
					if (layerId == 1)
					{
						pairsABDictionary[square[0][rowId][columnId]][oldCellValue] = 1;
					}
					
					// "����������" ���� A || C � B || C
					if (layerId == 2)
					{
						pairsACDictionary[square[0][rowId][columnId]][oldCellValue] = 1;
						pairsBCDictionary[square[1][rowId][columnId]][oldCellValue] = 1;
					}
				}

			// ������� � ��������� ������, ���� �� �������� ����� ��������
			if (rowId == rank - 1 && columnId == rank - 1 && layerId == height - 1)
			{
				// ��������� ���������� ��������� ��������
				squaresCount++;
				
					// ���� ����������� ������� ���������
					int squareLatin[height];
					int squareDiagonal[height];

					for (int h = 0; h < height; h++)
					{
						int isSquareLatin = 0;
						int isSquareDiagonal = 0;

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
										if (square[h][comparedRowId][columnId] == square[h][rowId][columnId])
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
										if (square[h][rowId][columnId] == square[h][rowId][comparedColumnId])
										{
											duplicationDetected = 1;
										}
									}
								}
							}

							isSquareLatin = !duplicationDetected;
						}

						{
							int duplicationDetected = 0;

							// �������� ������ ��������� - ��������� [0;0] - [rank;rank]
							for (int itemId = 0; itemId < rank && !duplicationDetected; itemId++)
							{
								// �������� �� ���������� �������� [itemId; itemId] �� ����� ���������� ���������� ���������
								for (int comparedId = itemId + 1; comparedId < rank && !duplicationDetected; comparedId++)
								{
									if (square[h][itemId][itemId] == square[h][comparedId][comparedId])
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
									if (square[h][(rank - itemId - 1)][itemId] == square[h][(rank - comparedId - 1)][comparedId])
									{
										duplicationDetected = 1;
									}
								}
							}

							isSquareDiagonal = !duplicationDetected;
						}

						squareLatin[h] = isSquareLatin;
						squareDiagonal[h] = isSquareDiagonal;
					}

				resultFile.open(resultFileName, std::ios_base::app);

				resultFile << "------------------------" << endl;
				resultFile << "Triple # " << squaresCount << ":" << endl;
				resultFile << "Squares {A; B; C}: {"
						<< (squareDiagonal[0] == 1 ? "Diagonal" : (squareLatin[0] == 1 ? "Latin" : "???" )) << "; "
						<< (squareDiagonal[1] == 1 ? "Diagonal" : (squareLatin[1] == 1 ? "Latin" : "???" )) << "; "
						<< (squareDiagonal[2] == 1 ? "Diagonal" : (squareLatin[2] == 1 ? "Latin" : "???" )) << "}" << endl;
				resultFile << "------------------------" << endl;

				cout << "------------------------" << endl;
				cout << "Triple # " << squaresCount << ":" << endl;
				cout << "Squares {A; B; C}: {"
						<< (squareDiagonal[0] == 1 ? "Diagonal" : (squareLatin[0] == 1 ? "Latin" : "???" )) << "; "
						<< (squareDiagonal[1] == 1 ? "Diagonal" : (squareLatin[1] == 1 ? "Latin" : "???" )) << "; "
						<< (squareDiagonal[2] == 1 ? "Diagonal" : (squareLatin[2] == 1 ? "Latin" : "???" )) << "}" << endl;
				cout << "------------------------" << endl;
				

				for (int h = 0; h < height; h++)
				{
					for (int i = 0; i < rank; i++)
					{
						for (int j = 0; j < rank; j++)
						{
							cout << square[h][i][j] << " ";
							resultFile << square[h][i][j] << " ";
						}
						cout << endl;
						resultFile << endl;
					}

					cout << endl;
					resultFile << endl;
				}

				resultFile.close();
			}
			else
			{
				// ����� ������
				if (layerId < height - 1)
				{
					// ������� �� ������� ���� � ������ ���������
					layerId++;
				}
				else
				{
					// ����� �������������� ���������
						// ������� �� ������� ������� ����� ��������� ��� �������� �����
						layerId = 0;
						// ��������� ���������
						if (columnId < rank - 1)
						{
							// ����� �������
							columnId++;
						}
						else
						{
							// ����� ������ � �������
							columnId = 0;
							rowId++;
						}
				}
			}
		}
		else
		{
			// ��������� ����� ������������ ������ �������� � ������ (rowId; columnId)
				// ��������� ������� ��������
				cellValue = square[layerId][rowId][columnId];

				// ���������� �������� � ��������� �������
				if (cellValue != -1)
				{
					// ���������� �������� � �������
					columns[layerId][cellValue][columnId] = 1;
					// ���������� �������� � ������
					rows[layerId][rowId][cellValue] = 1;
					// ���������� -1 � �������
					square[layerId][rowId][columnId] = -1;

					// �������� ������� ������ (layerId; rowId; columnId)
					for (int i = 0; i < rank; i++)
					{
						cellsHistory[layerId][rowId][columnId][i] = 1;
					}
				}

				if (cellValue != -1)
				{
					// "�����������" ���� A || B
					if (layerId == 1)
					{
						pairsABDictionary[square[0][rowId][columnId]][cellValue] = 1;
					}
					
					// "�����������" ���� A || C � B || C
					if (layerId == 2)
					{
						pairsACDictionary[square[0][rowId][columnId]][cellValue] = 1;
						pairsBCDictionary[square[1][rowId][columnId]][cellValue] = 1;
					}
				}

				// ����� ������
				if (layerId > 0)
				{
					// ����� ���� � ������ ���������
					layerId--;
				}
				else
				{
					// ����� �������������� ���������
						// ������� �� ������� ������� ��������� ����� �������� �����
						layerId = height - 1;
						// ����� ���������
						if (columnId > 0)
						{
							// ���������� ���������� �������� � ����������� �������
							columnId--;
						}
						else
						{
							// ������� � �������� ������ ���������� ������
							columnId = rank - 1;
							rowId--;

							// �������� ������ �� ���� ���������
							if (rowId < 0)
							{
								stop = 1;
							}
						}
				}
		}
	}

	cout << "------------------------" << endl;
	cout << "Total tripes found: " << squaresCount << endl;
	cout << "Press any key to exit ... " << endl;
	cin.get();

	return 0;
}