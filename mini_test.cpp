#include "./src/SATSolver.h"

int main(int argc, char *argv[])
{
    string filename = argv[1];

    SATSolver sat(filename);

    cout << sat.checkClauses(0) << endl;
        
}
