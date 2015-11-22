#include <unordered_map>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

//INPUTS
vector<int*> unchosen_literal;
vector<int*> chosen_literal;
unordered_map<int*, type> clause_map;
int len = 0;
int SIZE = unchosen_literal.size();

bool checkClause(unchosen_literal, chosen_literal, clause_map, int flag) {

    if (len == SIZE) 
        return true;

    if (unchosen_literal.empty())
        return false;

    int* xi;
    
    if (flag == 1) {
        xi = unchosen_literal.pop_back();
        chosen_literal.push_back(xi);
    } else if (flag == 0) {
        int* tmp = chosen_literal.pop();
        unchosen_literal.push_back(tmp);
        xi = chosen_literal.back();
    }

    clause_list = clause_map[xi];
    int size = clause_list.size();
    int count = 0;
    
    for (int i = 0; i < size; i++) {
        
        int r = clause_list[i]->newLiteralSetted(xi);
        
        if (r == -1) {
            flag = 1;
            checkClause(unchosen_literal, chosen_literal, clause_map, flag);
        } else if (r == 0) {
            goto backtrack;
        } else {
            count += 1;
            flag = 1;
        }
    }

    if (count == size) {
        len += 1;
        checkClause(unchosen_literal, chosen_literal, clause_map, flag);
    }


    backtrack:
    *xi = *xi ^ 1; //??? flip, to be discussed
    for (int i = 0; i < size; i++) {

        int r = clause_list[i]->update();
        
        if (r == -1) {
            flag = 1;
            checkClause(unchosen_literal, chosen_literal, clause_map, flag);
        } else if (r == 0) {
            flag = 0;
            checkClause(unchosen_literal, chosen_literal, clause_map, flag);
        } else {
            count += 1;
            flag = 1;
        }
    }    
    
    if (count == size) {
        len += 1;
        checkClause(unchosen_literal, chosen_literal, clause_map, flag);
    }
}
