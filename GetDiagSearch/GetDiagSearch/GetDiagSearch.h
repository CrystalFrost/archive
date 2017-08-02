// ����� ������������ ��������� ��������� ������� ��������� ��������� �������� ������ � ��������������� �������� ����������

# if !defined GetDiagSearch_h
# define GetDiagSearch_h

# include <iostream>
# include <fstream>

using namespace std;

// ����� "�������� ������������ ��������� ���������"
class GetDiagSearch
{
public:
	GetDiagSearch();						// ����������� �� ���������
	void Start();							// ������ ������

	static const int rank = 9;				// ���� ������� ���������

	__event void FoundTheSquare(int newSquare[rank][rank]);	// ������� ���������� ������ ������������� ��������
protected:
	void static StepForwardSDiag(int* sDiagIndex);			// ������� ��� ����� ����� �������� ���������
	void static StepBackwardSDiag(int* sDiagIndex);			// ������� ��� ����� ����� �������� ���������
	void static StepForward(int* rowId, int* columnId);		// ������� ��� ����� ����� ������
	void static StepBackward(int* rowId, int* columnId);	// ������� ��� ����� ����� ������

	virtual int GetCell(int rowId, int columnId);			// ��������� ���������� �������� ��� ������ (rowid, columnId)
	void InitializeSearch();								// ������� ������������� �������� ����� ������� ��������
	virtual void ProcessSquare();							// ������� ��������� ���������� ��������
	void PrintSquare(int printedSquare[rank][rank]);		// ������� ������ �������� � �������
	void SaveSquare(int printedSquare[rank][rank], fstream& resultFile);	// ������� ������ �������� � ����

	int square[rank][rank];					// ������� ��������
	int squaresCount;						// ����� ����������� ���������
	char* resultFileName;					// ��� ����� � ������������

	int primary[rank];						// ���������� ������� ���������
	int secondary[rank];					// ���������� �������� ���������
	int columns[rank][rank];				// ������� ��������, ���������������� � �������� - columns[��������][�������] = 0|1. 0 - �������� ������. 1 - ��������.
	int rows[rank][rank];					// ������� ��������, ���������������� � ������� - rows[������][��������] = 0|1
	int cellsHistory[rank][rank][rank];		// ��� ��������, ������� �������������� ��� ������������ ����������� ����� �������� - cellsHistory[������][�������][��������]

	int pDiagIndex;							// ����� �������������� ������ �� ������� ���������
	int sDiagIndex;							// ����� �������������� ������ �� �������� ���������
	int rowId;								// ����� ������ � �������������� ��������� rowId = 0 .. rank - 1
	int columnId;							// ����� ������� � �������������� ��������� columnId = 0 .. rank - 1
private:
};

# endif