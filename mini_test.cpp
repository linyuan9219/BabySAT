#include "./src/SATSolver.h"

int main(int argc, char *argv[])
{
    string filename = argv[1];

    SATSolver sat(filename);

    ofstream file;
    file.open("output.txt");

    if (sat.checkClauses(0))
    {    
        file << "SAT" << endl;
        
        if (sat.unchosen_literal.empty())
        {   
            for (int i = sat.chosen_literal.size() - 1; i >= 0; i--)
            {
                int label = (*(sat.chosen_literal[i]) > 0) ? 1 : -1;
                int chosen_literal_size = sat.chosen_literal.size();
                file << label * (chosen_literal_size - i) << " ";
            }
            file << 0;
        }
        else
        {   
            for (int i = 0; i < sat.unchosen_literal.size(); i++)
            {
                file << i+1 << " ";
            }
            for (int i = sat.chosen_literal.size() - 1; i >= 0; i--)
            {
                int label = (*sat.chosen_literal[i] > 0) ? 1 : -1;
                int chosen_literal_size = sat.chosen_literal.size();
                int unchosen_literal_size = sat.unchosen_literal.size();
                file << label * (chosen_literal_size - i + unchosen_literal_size) << " ";
            }
            file << 0;
        }
    }

    else
    {
        file << "UNSAT" << endl;
    }
}
