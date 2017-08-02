# include <iostream>
# include <fstream>
# include <string>

# include "GreekSquare.h"
# include "SquareCell.h"
# include "CellAddress.h"

using namespace std;

int main()
{
	GreekSquare finder;

	finder.Initialize();
	finder.StartSearch();

	cout << "Press any key to continue..." << endl;
	cin.get();

	return 0;
}