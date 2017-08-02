// ����� ��� ������������ ��������� ��������� ������� ����������� �����

# if !defined MovePairSearch_h
# define MovePairSearch_h

# include <iostream>
# include "Generator.h"

using namespace std;

class MovePairSearch
{
public:
	static const int Rank = Square::Rank;

	MovePairSearch();								// ����������� �� ���������
	void Reset();									// ����� �������� ������
	void ClearBeforeNextSearch();					// ������� ����������� ���������� ����� ��������� �������
	void InitializeMoveSearch(char* start, char* result, char* checkpoint, char* temp);	// ������������� ������
	void StartMoveSearch();							// ������ ������ ������������� ��������� ������� ������������ �����
	void OnSquareGenerated(Square newSquare);		// ���������� ������� ���������� ���, ����������� ����� � ���� ����
private:
	static const int CheckpointInterval = 100000;	// �������� �������� ����������� �����
	static const int OrhoSquaresCacheSize = 32;		// ������ ���� ��� �������� ���������, ������������� ���������������

	void MoveRows();								// ����������� ����� ��������� ��� � ������ ���� � ����
	void ProcessOrthoSquare();						// ��������� ���������� �������������� ��������
	void CheckMutualOrthogonality();				// �������� �������� ��������������� ���������
	void CreateCheckpoint();						// �������� ����������� �����
	void Read(std::istream& is);					// ������ ��������� ������ �� ������
	void Write(std::ostream& os);					// ������ ��������� ������ � �����
	void ShowSearchTotals();						// ����������� ����� ������ ������

	Generator squareAGenerator;						// ��������� ���
	int squareA[Rank][Rank];						// �������� ���, ������ � ������� ����� ��������������
	int squareB[Rank][Rank];						// ����������� ���, ������ ������ �������� � ��������������
	int rowsUsage[Rank];							// ������ ������ �������� �������������� ����� �� ������ ������ - rowsUsage[����� ������] = 0 | 1, ��� 0 - ������ ��� ������������, 1 - ���.
	int rowsHistory[Rank][Rank];					// ������ ������� ���������� ������������� ����� - rowsHistory[����� ������][��������] = 0 | 1, 
													// ��� 0 - ������ � ������� "��������" �������������� ��� ������ "����� ������" ������������ ��������. 1 - ������ ��� ����� ������������
	int currentSquareRows[Rank];					// ������ � �������� ������� �����, �������������� � ��������. �� i-� ������� - ����� ������������ ������
	int pairsCount;									// ����� ������������ ������������ ��������� � ������������� ����� �� ����������� squareA
	int totalPairsCount;							// ����� ����� ������������ ������������ ��������� - � ������ ����� ������
	int totalSquaresWithPairs;						// ����� ����� ���������, � ������� ������ ���� �� ���� �������������
	int totalProcessedSquaresLarge;					// ����� ������������ ���, ����������� �� ���������� - � ����������
	int totalProcessedSquaresSmall;					// ����� ������������ ���, ����������� �� ���������� � �������� �� ���������

	Square orthoSquares[OrhoSquaresCacheSize];		// ��� ��� �������� ���������, ������������� ���������������

	char* startParametersFileName;					// �������� ����� � ����������� ������� �������
	char* resultFileName;							// �������� ����� � ������������
	char* checkpointFileName;						// �������� ����� ����������� �����
	char* tempCheckpointFileName;					// ��������� �������� ����� ����� ����������� �����

	int isInitialized;								// ���� �������������������� ������
	int isStartFromCheckpoint;						// ���� ������� � ����������� �����

	char* moveSearchGlobalHeader;					// ���������, ����� �������� � ����� ���� ������ � ��������� ������ ������� ����������� �����
	char* moveSearchComponentHeader;				// ���������, ����� �������� � ����� ���� ������ � ��������� ���������� ����������� �����
};

# endif