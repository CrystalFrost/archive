// ��������� ������������ ��������� ���������

# if !defined Generator_h
# define Generator_h

# include <iostream>
# include <fstream>
# include "Square.h"

using namespace std;

class Generator
{
public:
	__event void SquareGenerated(Square generatedSquare);	// ������� ������������� ������ ������������� ��������

	Generator();							// ����������� �� ���������
	Generator(Generator& source);			// ����������� ����������
	void Start();							// ������ ��������� ���������
	void Reset();							// ����� ���� �������� ���������� ��������
	void SetFileNames(char* start, char* result, char* checkpoint, char* temp);	// �������� ���� ������ ���������� � ����������� �����
	void Initialize(char* start, char* result, char* checkpoint, char* temp);	// ������������� ������

	Generator& operator = (Generator&  value);									// �������� �����������
	friend std::ostream& operator << (std::ostream& os, Generator& value);		// �������� ������ ��������� ����������
	friend std::istream& operator >> (std::istream& is, Generator& value);		// �������� ���������� ��������� ����������

protected:
	static const int Rank = Square::Rank;	// ���� �������� (��� ��������)
	static const int Free = 1;				// ���� ���������� ��� ������������� ��������
	static const int Used = 0;				// ���� �������������� �������� � �����-���� ������ ��������� | ������ | �������
	static const int CellsInPath = (Rank - 1)*Rank;	// ����� �������������� ������
	static const int Yes = 1;				// ���� "��"
	static const int No = 0;				// ���� "���"

	void CopyState(Generator& source);		// ����������� ���������
	void Read(std::istream& is);			// ���������� ��������� ���������� �� ������
	void Write(std::ostream& os);			// ������ ��������� ���������� � �����

	Square newSquare;						// ������������ �������

	int path[CellsInPath][2];				// ���� ���������� ������� �������� - path[i][0] - ������ �� ���� i, path[i][1] - �������
	int keyRowId;							// ������������� ������ �������� ������ - �� �������� ������� ������ ����� ���������������
	int keyColumnId;						// ������������� ������� �������� ������
	int keyValue;							// �������� �������� ������, �� ���������� �������� ������ ����� ���������������

	int primary[Rank];						// ���������� ������� ���������
	int secondary[Rank];					// ���������� �������� ���������
	int columns[Rank][Rank];				// ������� ��������, ���������������� � �������� - columns[��������][�������] = 0|1. 0 - �������� ������. 1 - ��������.
	int rows[Rank][Rank];					// ������� ��������, ���������������� � ������� - rows[������][��������] = 0|1
	int cellsHistory[Rank][Rank][Rank];		// ��� ��������, ������� �������������� ��� ������������ ����������� ����� �������� - cellsHistory[������][�������][��������]

	char* startParametersFileName;			// �������� ����� � ����������� ������� �������
	char* resultFileName;					// �������� ����� � ������������
	char* checkpointFileName;				// �������� ����� ����������� �����
	char* tempCheckpointFileName;			// ��������� �������� ����� ����� ����������� �����

	int isInitialized;						// ���� �������� ������������� ������
	int squaresCount;						// ����� ������������ ���
	int rowId;								// ������������� ������ ������������ ������
	int columnId;							// ������������� ������� �������������� ������
	int cellId;								// ������������� ������ � ������� ����� ������ ��������

private:
	void CreateCheckpoint();				// �������� ����������� �����
	void ProcessSquare();					// ��������� ���������� ��������

	char* generatorStateHeader;				// ���������, ����� �������� � ����� ���������� ��� ����������� ����� ��� ��������� ���������� ������������ ���������
};

# endif