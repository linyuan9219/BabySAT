#include "./src/SATSolver.h"

int main()
{
	string filename = "./sample/aim-50-2_0-no-2.cnf";

	SATSolver sat(filename);

	cout << sat.checkClauses(0) << endl;
}