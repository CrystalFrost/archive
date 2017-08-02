// ����� ��� ������������� ������������ ��������� ���������

# if !defined GetDiagPairSearch_h
# define GetDiagPairSearch_h

# include <iostream>
# include <fstream>
# include "GetDiagSearch.h"

using namespace std;

// ����� "�������� ��� ������������� ������������ ��������� ���������"
class GetDiagPairSearch : public GetDiagSearch
{
public:
	GetDiagPairSearch();								// ����������� �� ���������
	void StartPairSearch();								// ������ ������ ��������-����
	void CheckSeparateSquare(int square[rank][rank]);	// ���� ������������� ��������� � ��������� ��������

protected:
	virtual int GetCell(int rowId, int columnId);		// ��������� ���������� �������� ��� ������ (rowid, columnId) �������� ������� ��������
	void InitializePairSearch();						// ������������� �������� ����� ������� ������� ��������
	void OnFoundTheSquare(int newSquare[rank][rank]);	// ���������� ������� - ���������� ������� �������� ���� �������� squareAGenerator
	virtual void ProcessSquare();						// ����������� ��������� ���������� ������� ��������
	virtual void ProcessSquaresPair();					// ��������� ��������� ���� ���������

	GetDiagSearch squareAGenerator;			// ��������� ������ ��������� ����
	int squareA[rank][rank];				// ������� ������� �������� � ����
	int pairsDictionary[rank][rank];		// ������� �������������� ��� � ����������� ���� ���������
private:
	void PrintSearchFooter();				// ����� ���������� �� ������ ������
};

# endif