// ����� ��� ������������ ��������� ��������� ������� ����������� �����

# if !defined MoveDiagSearch_h
# define MoveDiagSearch_h

# include <iostream>
# include "GetDiagSearch.h"

using namespace std;

class MoveDiagSearch
{
public:
	static const int rank = GetDiagSearch::rank;

	MoveDiagSearch();
	void InitializeMoveSearch();
	void StartMoveSearch();
	void OnFoundTheSquare(int newSquare[GetDiagSearch::rank][GetDiagSearch::rank]);
private:
	void MoveRows();
	int IsSquareLatin();
	int IsSquareDiagonal();
	int IsSquareOrthogonal(int a[rank][rank], int b[rank][rank]);

	void PrintHeader();
	void PrintSquare(int printedSquare[rank][rank]);
	void SaveSquare(int writedSquare[rank][rank], fstream& resultFile);
	void ProcessSquare();

	GetDiagSearch squareAGenerator;
	int squareA[rank][rank];		// �������� ���, ������ � ������� ����� ��������������
	int squareB[rank][rank];		// ����������� ���, ������ ������ �������� � ��������������
	int rowsUsage[rank];			// ������ ������ �������� �������������� ����� �� ������ ������ - rowsUsage[����� ������] = 0 | 1, ��� 0 - ������ ��� ������������, 1 - ���.
	int rowsHistory[rank][rank];	// ������ ������� ���������� ������������� ����� - rowsHistory[����� ������][��������] = 0 | 1, 
									// ��� 0 - ������ � ������� "��������" �������������� ��� ������ "����� ������" ������������ ��������. 1 - ������ ��� ����� ������������
	int currentSquareRows[rank];	// ������ � �������� ������� �����, �������������� � ��������. �� i-� ������� - ����� ������������ ������
	int squaresCount;				// ����� ������������ ������������ ��������� � ������������� ����� �� ����������� squareA
	int totalProcessedSquaresSmall;	// ����� ������������ ���, ����������� �� ���������� � �������� �� ���������
	int totalProcessedSquaresLarge;	// ����� ������������ ���, ����������� �� ���������� - � ����������

	char* resultFileName;						// ��� ����� � ������������
	int orthogonalSquares[rank - 1][rank][rank];// ������ ��� ������ ������ ������������� ���������
};

# endif