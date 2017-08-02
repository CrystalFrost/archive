# include <iostream>
# include <fstream>

/*# include "GetDiagSearch.h"
# include "GetDiagPairSearch.h"
# include "MoveDiagSearch.h"*/

/*# include "Square.h"*/
/*# include "Generator.h"*/
/*# include "PairSearch.h"*/
# include "MovePairSearch.h"

using namespace std;

int main()
{
	/*
	// Полный поиск ОДЛК
	GetDiagPairSearch finder;
	finder.StartPairSearch();
	*/

	/*// Проверка отдельного квадрата (rank и фактический размер массива должны соответствать)
	int square[GetDiagPairSearch::rank][GetDiagPairSearch::rank] = 
	{
		{0, 1, 2, 3, 4, 5, 6, 7},
		{4, 2, 6, 0, 7, 1, 5, 3},
		{3, 5, 1, 7, 0, 6, 2, 4},
		{7, 6, 5, 4, 3, 2, 1, 0},
		{6, 0, 4, 2, 5, 3, 7, 1},
		{2, 3, 0, 1, 6, 7, 4, 5},
		{5, 4, 7, 6, 1, 0, 3, 2},
		{1, 7, 3, 5, 2, 4, 0, 6}
	};

	GetDiagPairSearch finder;
	finder.CheckSeparateSquare(square);*/

	// Быстрый поиск перестановкой строк
	/*MoveDiagSearch finder;
	finder.StartMoveSearch();*/

	/*int squareMatrix[Square::Rank][Square::Rank] = 
	{
		{0, 1, 2, 3, 4, 5, 6, 7, 8, 9},
		{2, 4, 9, 7, 0, 3, 5, 1, 6, 8},
		{9, 7, 1, 5, 6, 4, 8, 3, 0, 2},
		{1, 0, 8, 6, 9, 2, 7, 4, 5, 3},
		{8, 6, 5, 0, 7, 1, 3, 2, 9, 4},
		{6, 3, 4, 9, 2, 8, 0, 5, 1, 7},
		{5, 9, 7, 8, 3, 0, 2, 6, 4, 1},
		{3, 8, 0, 4, 5, 7, 1, 9, 2, 6},
		{7, 5, 6, 2, 1, 9, 4, 8, 3, 0},
		{4, 2, 3, 1, 8, 6, 9, 0, 7, 5}
	};*/

	/*Square mySquare;

	parametersFile.open(parametersFileName, std::ios_base::in);

	if (parametersFile.is_open())
	{
		parametersFile >> mySquare;
		parametersFile.close();
	}

	cout << mySquare;*/

	char* startFileName = "start_parameters.txt";
	char* resultFileName = "result.txt";
	char* checkpointFileName = "checkpoint.txt";
	char* tempCheckpointFileName = "checkpoint_new.txt";

	/*Generator generator;
	generator.Initialize(startFileName,  resultFileName, checkpointFileName, tempCheckpointFileName);
	generator.Start();*/

	/*PairSearch search;
	search.Initialize(startFileName, resultFileName, checkpointFileName, tempCheckpointFileName);

	search.StartPairSearch();*/

	MovePairSearch search;
	search.InitializeMoveSearch(startFileName, resultFileName, checkpointFileName, tempCheckpointFileName);
	search.StartMoveSearch();

	/*const int squaresCount = 108;
	Square list[squaresCount];
	int maxTotalDegree = 0;
	int totalDegree = 0;
	int degreeAB = 0;
	int degreeAC = 0;
	int degreeBC = 0;
	int triplesCount = 0;
	int level = 175;

	char* squareFileName = "square.txt";
	fstream squareFile;
	
	squareFile.open(squareFileName, std::ios_base::in);
	if (squareFile.is_open())
	{
		for (int i = 0; i < squaresCount; i++)
		{
			squareFile >> list[i];
		}
	}

	// Перебор всех троек
	for (int i = 0; i < squaresCount; i++)
	{
		for (int j = i + 1; j < squaresCount; j++)
		{
			for (int k = j + 1; k < squaresCount; k++)
			{
				degreeAB = Square::OrthoDegree(list[i],list[j]);
				degreeAC = Square::OrthoDegree(list[i],list[k]);
				degreeBC = Square::OrthoDegree(list[j],list[k]);
				
				totalDegree = degreeAB + degreeAC + degreeBC;

				if (totalDegree >= 250)
				{
					if (totalDegree > maxTotalDegree)
					{
						maxTotalDegree = totalDegree;
					}

					cout << "Interesting triple: " << totalDegree << endl;
					cout << "{" << i << ", " << j << ", " << k << "} - {" << degreeAB << ", " << degreeAC << ", " << degreeBC<< "}" << endl;
					triplesCount++;
				}

				if (degreeAB + degreeAC >= level || degreeAB + degreeBC == level || degreeAC + degreeBC == level)
				{
					cout << "Interesting triple: " << totalDegree << endl;
					cout << "{" << i << ", " << j << ", " << k << "} - {" << degreeAB << ", " << degreeAC << ", " << degreeBC<< "}" << endl;
					triplesCount++;
				}
			}
		}
	}

	cout << endl;
	cout << "Total triples: " << triplesCount << endl;
	cout << "Maximum degree of orthogonality: " << maxTotalDegree << endl << endl;*/
	cout << "Press any key to exit ... " << endl;
	cin.get();

	return 0;
}