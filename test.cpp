#include "./src/SATSolver.h"

int main()
{
	string filename = "./sample/0.txt";

	SATSolver sat(filename);

	cout << sat.checkClauses() << endl;
}