#include <unordered_map>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "Clause.h"

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
        getline(fin, line);
        while (line[0] == 'c')
            getline(fin, line);

        vector<string> str_list = split(line, ' ');

        literal_num = fromString(str_list[2]);
        cout << "initialize " << literal_num << " literals" << endl;

        int *tmp_int;
        for (int i = 0; i < literal_num; i++)
        {
            tmp_int = new int;
            *tmp_int = -1;
            unchosen_literal.push_back(tmp_int);
        }

        cout << "read and initialize clauses" << endl;
        while (!fin.eof())
        {
            getline(fin, line);
            if (line[0] == 'c')
                continue;
            clause_list.push_back(new Clause(line, unchosen_literal));
        }

        cout << "generated " << clause_list.size() << " clauses" << endl;

        cout << "build reverted map" << endl;
        for (int i = 0; i < unchosen_literal.size(); i++)
        {
            vector<Clause*> tmp_vec;
            int* key = unchosen_literal[i];
            literal2clauses[key] = tmp_vec;
        }

        vector<Clause*> tmp;
        for (int i = 0; i < clause_list.size(); i++)
            for (int j = 0; j < clause_list[i]->literal_list.size(); j++)
            {
                int* key = clause_list[i]->literal_list[j];
                if (literal2clauses.count(key))
                    literal2clauses[key].push_back(clause_list[i]);
                else
                {
                    tmp.push_back(clause_list[i]);
                    literal2clauses[key] = tmp;
                    tmp.clear();
                }
            }
        /*
        for (int i = 0; i < 100; i++)
        {
            cout << "# of clauses related to literal: " << *unchosen_literal[i] << " is: ";
            vector<Clause*> clauses = literal2clauses[unchosen_literal[i]];
            cout << clauses.size() << endl;
        }
        */
        fin.close();
    }

    // any pos returing a true need to return the literal list that makes CNF = true
    // parameter pos is just for debugging
    bool checkClauses(int pos) {
        //cout << "pos is " << pos << endl;

        if (unchosen_literal.empty())
            return false;

        int* xi = unchosen_literal[unchosen_literal.size() - 1];
        unchosen_literal.pop_back();
        chosen_literal.push_back(xi);
        *xi = 1;

        vector<Clause*> relevant_clauses = literal2clauses[xi];
        int relevant_clause_num = relevant_clauses.size();
        
        //cout << "relevant clause num is " << relevant_clause_num << endl;
 
        int status = -1;
        for (int i = 0; i < relevant_clause_num; i++) 
        {
            int r = relevant_clauses[i]->newLiteralSetted(xi);

            if ((r & 0x3) == 3)
                status &= -1;
            else if ((r & 0x3) == 0)
                status = 0;
            else if (r != 5)
                ++setted_clause_num;
        }

        //cout << "status is " << status << endl;
        //cout << "setted_clause_num = " << setted_clause_num << endl;

        if (setted_clause_num == clause_list.size())
            return true;
        else if (status == -1)
        {
            int backup = setted_clause_num;
            if (checkClauses(pos + 1))
                return true;
            setted_clause_num = backup;
        }

        //cout << "starting flip part" << endl;
        // Flip 
        *xi = 0;

        status = -1;
        for (int i = 0; i < relevant_clause_num; i++) 
        {
            int r = relevant_clauses[i]->update();

            if ((r & 0x3) == 3)
                status &= -1;
            else if ((r & 0x3) == 0)
                status = 0;
            else if (r != 5)
                ++setted_clause_num;

            if (r == 7 || r == 4)
                --setted_clause_num;
        }

        //cout << "status is " << status << endl;
        //cout << "setted_clause_num is " << setted_clause_num << endl;
        if (setted_clause_num == clause_list.size())
            return true;
        else if (status == -1)
        {
            int backup = setted_clause_num;
            if (checkClauses(pos + 1))
                return true;
            setted_clause_num = backup;
        }

        *xi = -1;
        unchosen_literal.push_back(xi);
        chosen_literal.pop_back();
        return false;
    }
};
