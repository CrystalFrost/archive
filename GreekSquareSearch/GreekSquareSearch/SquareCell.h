// Структура "Клетка квадрата"

# if !defined SquareCell_h
# define SquareCell_h

struct SquareCell
{
	int DictionaryId;
	int CellAValue;
	int CellBValue;
	int IsFirst;
	int IsLast;
	SquareCell* Previous;
	SquareCell* Next;
};

# endif