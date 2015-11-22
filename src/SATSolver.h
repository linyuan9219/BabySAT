#include <unordered_map>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "Clause.h"
#include "ultility.h"

using namespace std;

class SATSolver
{
public:
    vector<int*> unchosen_literal;
    vector<int*> chosen_literal;

    vector<Clause*> clause_list;

    unordered_map< int*, vector<Clause*> > literal2clauses;

    int literal_num;

    int setted_clause_num;

    SATSolver() {
        setted_clause_num = 0;
    }

    SATSolver(string filename) {
        setted_clause_num = 0;

        ifstream fin(filename.c_str());

        string line;
        fin >> line;
        vector<string> str_list = split(line, ' ');

        literal_num = fromString(str_list[2]);
        for (int i = 0; i < literal_num; i++)
            unchosen_literal.push_back(new int);

        while (!fin.eof())
        {
            fin >> line;
            clause_list.push_back(new Clause(line, unchosen_literal));
        }

        vector<Clause*> tmp;
        for (int i = 0; i < clause_list.size(); i++)
            for (int j = 0; j < clause_list[i].literal_list.size(); j++)
            {
                int* key = clause_list[i].literal_list[j]
                if (literal2clauses.count(key))
                    literal2clauses[key] = clause_list[i];
                else
                {
                    tmp.push_back(clause_list[i]);
                    literal2clauses[key] = tmp;
                }
            }
    }

    // any pos returing a true need to return the literal list that makes CNF = true
    bool checkClauses() {
        if (unchosen_literal.empty())
            return false;

        int* xi = unchosen_literal.pop_back();
        chosen_literal.push_back(xi);
        *xi = 1;

        clause_list = clause_map[xi];
        vector<Clause>* relevant_clauses = &literal2clauses[xi];
        int relevant_clause_num = relevant_clauses.size();
        
        int status = -1;
        for (int i = 0; i < relevant_clause_num; i++) 
        {
            int r = relevant_clauses[i]->newLiteralSetted(xi);
            
            if (r & 0x3 == 3)
                status &= -1;
            else if (r & 0x3 == 0)
                status = 0;
            else
                ++setted_clause_num;
        }

        if (setted_clause_num == size)
            return true;
        else if (status == -1)
            if (checkClauses())
                return true;

        // Flip 
        *xi = *xi ^ 1;

        int status = -1;
        for (int i = 0; i < relevant_clause_num; i++) 
        {
            int r = relevant_clauses[i]->update();
            
            if (r & 0x3 == 3)
                status &= -1;
            else if (r & 0x3 == 0)
                status = 0;
            else
                ++setted_clause_num;

            if (r == 7 || r == 4)
                --setted_clause_num;
        }

        if (setted_clause_num == size)
            return true;
        else if (status == -1)
            if (chekcClauses())
                return true;

        return false;
    }
};
