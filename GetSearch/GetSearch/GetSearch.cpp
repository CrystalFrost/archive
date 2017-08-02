// ����� ������������ ��������� ��������� ������� ��������� ��������� �������� �����

# include <iostream>

using namespace std;

int main()
{
	const int rank = 5;

	int primary[rank];
	int secondary[rank];
	int columns[rank][rank];
	int rows[rank][rank];
	int square[rank][rank];
	int cellsHistory[rank][rank][rank];

	int rowId;
	int columnId;
	int isGet;
	int cellValue;
	int oldCellValue;

	int stop = 0;
	int squaresCount = 0;

	for (int i = 0; i < rank; i++)
	{
		primary[i] = 1;
		secondary[i] = 1;
	}

	for (int i = 0; i < rank; i++)
	{
		for (int j = 0; j < rank; j++)
		{
			columns[i][j] = 1;
			rows[i][j] = 1;
			square[i][j] = -1;
		}
	}

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

	rowId = 0;
	columnId = 0;
	while (!stop)
	{
		// ����������� �������� ������ rowId, columnId
		isGet = 0;
		cellValue = -1;
		for (int i = 0; i < rank && !isGet; i++)
		{
			// ��������� ����������� ������ �������� i � ������ (rowId, columnId)
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
				
				// ������� � ��������� ������, ���� �� �������� ����� ��������
				if (rowId == rank - 1 && columnId == rank - 1)
				{
					squaresCount++;
					
					cout << "------------------------" << endl;
					for (int i = 0; i < rank; i++)
					{
						for (int j = 0; j < rank; j++)
						{
							cout << square[i][j] << " ";
						}
						cout << endl;
					}
				}
				else
				{
					if (columnId < rank - 1)
					{
						columnId++;
					}
					else
					{
						columnId = 0;
						rowId++;
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

				// ������������ � ���������� ������
				if (columnId > 0)
				{
					columnId--;
				}
				else
				{
					columnId = rank - 1;
					rowId--;

					if (rowId == 0)
					{
						stop = 1;
					}
				}
		}
	}

	cout << "------------------------" << endl;
	cout << "Total squares found: " << squaresCount << endl;
	cout << "Press any key to exit ... " << endl;
	cin.get();

	return 0;
}