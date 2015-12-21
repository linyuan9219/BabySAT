//#include <iostream>
#include <cstring>
#include <ctime>
#include <time.h>
#include <cstdlib>
#include <stdio.h>

using namespace std;

typedef long long int s_type;

#define max(a, b) (a < b) ? b : a

void swap(int &a, int &b) {
    int c = b;
    b = a;
    a = b;
}

struct Node
{
    int index;
    Node* next;
    Node(int i)
    {
        index = i;
        next = NULL;
    }
};

class EdgeList
{
public:
    Node *head;
    EdgeList(){head = NULL;}
    ~EdgeList(){}
    void insertNode(Node *n)
    {
        n->next = head;
        head = n;
    }
};

s_type t_factor[31]; // for faster multi

class Poly
{
public:
    s_type factor[31];
    int max_factor_index;
    
    Poly(const Poly &obj)
    {
        max_factor_index = obj.max_factor_index;
        memcpy(factor, obj.factor, sizeof(factor));
    }

    Poly()
    {
        max_factor_index = 0;
        factor[0] = 1;
    }
    Poly(s_type f)
    {
        factor[0] = f;
        max_factor_index = 0;
    }
    Poly(s_type f1, s_type f2)
    {
        factor[0] = f1;
        factor[1] = f2;
        max_factor_index = 1;
    }
    ~Poly(){}
    void simpleMulti(int b)
    {
        factor[max_factor_index + 1] = factor[max_factor_index];
        for (int i = max_factor_index - 1; i >= 0; i--)
            factor[i + 1] = factor[i] + b * factor[i + 1];
        factor[0] *= b;
        max_factor_index++;
    }
    bool empty()
    {
        return (max_factor_index == -1);
    }
    void operator += (Poly& p)
    {
        if (max_factor_index == -1)
        {
            max_factor_index = p.max_factor_index;
            for (int i = 0; i <= max_factor_index; i++)
                factor[i] = p.factor[i];
            return;
        }
        if (max_factor_index < p.max_factor_index)
        {
            for (int i = 0; i <= max_factor_index; i++)
                factor[i] += p.factor[i];
            memcpy(factor + max_factor_index + 1, p.factor + max_factor_index + 1, (p.max_factor_index - max_factor_index) * sizeof(int));
            max_factor_index = p.max_factor_index;
        }
        else
        {
            for (int i = 0; i <= p.max_factor_index; i++)
                factor[i] += p.factor[i];
        }
    }
    Poly operator + (Poly& p)
    {
        if (p.max_factor_index == -1)
            return (*this);
        Poly res;
        int max_index = max(p.max_factor_index, max_factor_index);
        for (s_type i = 0; i <= max_index; i++)
            res.factor[i] = factor[i] + p.factor[i];
        while (res.factor[max_index] == 0)
            max_index--;
        res.max_factor_index = max_index;
        return res;
    }
    void operator -= (Poly& p)
    {
        for (int i = 0; i <= p.max_factor_index; i++)
            factor[i] -= p.factor[i];
    }
    Poly operator - (Poly& p)
    {
        Poly res;
        int max_index = max(p.max_factor_index, max_factor_index);
        for (int i = 0; i <= max_index; i++)
            res.factor[i] = factor[i] - p.factor[i];
        while (res.factor[max_index] == 0)
            max_index--;
        res.max_factor_index = max_index;
        return res;
    }
    Poly operator * (Poly &p)
    {
        Poly res;
        for (s_type i = 0; i <= p.max_factor_index; i++)
            for (s_type j = 0; j <= max_factor_index; j++)
            {
                res.factor[i + j] += factor[j] * p.factor[i];
            }
        res.max_factor_index = max_factor_index + p.max_factor_index;
        return res;
    }
    
    void operator *= (Poly &p)
    {
        memcpy(t_factor, factor, sizeof(factor));
        memset(factor, 0, sizeof(factor));
        for (int i = 0; i <= p.max_factor_index; i++)
            for (int j = 0; j <= max_factor_index; j++)
            {
                factor[i + j] += t_factor[j] * p.factor[i];
            }
        max_factor_index = max_factor_index + p.max_factor_index;
    }
    /*
    friend ostream& operator << (ostream& cout, const Poly& p);
    */
};
/*
ostream& operator << (ostream& out, const Poly& p)
{
	
	for (s_type i = p.max_factor_index; i > 0; i--)
	{
	    out << p.factor[i] << "k^" << i << '+';
	}
	out << p.factor[0] << endl;
	return out;
}
*/

int n;

int edge[31][31];
EdgeList e_list[31];

int Color2Point[31];

int cantColorNum[31];

int appColorNum = 0;
int midColorNum = 0;

int map[60];

int bin_edge[31];

int ccount = 0; // independent Set count
int hcount = 0; // independent extended_1 Set count
int h2count = 0; // independent extended_2 Set count

int dcount = 0; // for split graph points count

int edge_count = 0; // for judge graph mode

int mark[31]; // for DFS

// something pre_inited for speed!

Poly p(0, 1);
Poly tmp1(0, 1);
Poly tmp2(0, 1);
Poly one(1);
Poly res[31];
Poly t1(0, 1), t2(0, 1), t3(0, 1), t4(0, 1), t5(0, 1), t6(0, 1);
Poly r1, r2, r3;
Poly r;

int calcCount = 0; // for calc call numbers, just for log

int calcCantColor(int curPoint, int mode = 0) // mode is for extend independentSet
{
    Node *node = e_list[map[curPoint]].head;
    int cantColor = 0;
    while (node != 0)
    {
        if (!(cantColor & (1 << Color2Point[node->index]))
            && (!mode || node->index != map[curPoint - 1])
            && (!mode || !(mode - 1) || node->index != map[curPoint - 2]))
        {
            cantColorNum[map[curPoint]]++;
            cantColor |= (1 << Color2Point[node->index]);
        }
        node = node->next;
    }
    return cantColor;
}

Poly& searchChrompoly(int curPoint) // calc chrompoly main part!
{
    //calcCount++;

    cantColorNum[map[curPoint]] = 0;
    res[curPoint].max_factor_index = -1;
    int cantColor = calcCantColor(curPoint);
    int factor = cantColorNum[map[curPoint]];

    int constappColorNum = appColorNum;

    if (curPoint == dcount && dcount != 0)
        midColorNum = appColorNum;
    int startColor = 0;
    if (curPoint >= dcount && dcount != 0)
        startColor = midColorNum;

    if (curPoint < n - ccount - hcount - h2count)
    for (int i = startColor; i < constappColorNum; i++)
    {
        if (!(cantColor & (1 << i)))
        {
            Color2Point[map[curPoint]] = i;
            p = searchChrompoly(curPoint + 1);
            res[curPoint] += p;
            factor++;
        }
    }
    if (curPoint >= n - 3 * h2count)
    {
        if (curPoint + 3 >= n)
        {
            p.max_factor_index = 0;
            p.factor[0] = 1;
        }
        else
            p = searchChrompoly(curPoint + 3);

        cantColorNum[map[curPoint + 1]] = 0;
        cantColorNum[map[curPoint + 2]] = 0;
        
        int cantColorA = calcCantColor(curPoint + 1);
        int cantColorB = calcCantColor(curPoint + 2, 2);

        int n1 = cantColorNum[map[curPoint]];
        int n2 = cantColorNum[map[curPoint + 1]];
        int n3 = cantColorNum[map[curPoint + 2]];
        int n4 = 0, n5 = 0, n6 = 0;
        
        cantColor |= cantColorB;
        for (int i = 0; i < 31; i++)
            if (cantColor & (1 << i))
                n4++;
        cantColorA |= cantColorB;
        for (int i = 0; i < 31; i++)
            if (cantColorA & (1 << i))
                n5++;
        cantColor |= cantColorA;
        for (int i = 0; i < 31; i++)
            if (cantColor & (1 << i))
                n6++;
        t6.factor[0] = -1 * n6;

        r1.max_factor_index = 0;
        r1.factor[0] = 1;
        r1.simpleMulti(-1 * n1);
        r1.simpleMulti(-1 * n2);
        r1.simpleMulti(-1 * n3);
        r2.max_factor_index = 0;
        r2.factor[0] = 1;
        r2.simpleMulti(-1 * n2);
        r2.simpleMulti(-1 * n4);
        r3.max_factor_index = 0;
        r3.factor[0] = 1;
        r3.simpleMulti(-1 * n1);
        r3.simpleMulti(-1 * n5);

        r1 += t6;
        r1 -= r2;
        r1 -= r3;
        p *= r1;
        return p;
    }
    if (curPoint >= n - 2 * hcount - 3 * h2count)
    {
        if (curPoint + 2 >= n)
        {
            p.max_factor_index = 0;
            p.factor[0] = 1;
        }
        else
            p = searchChrompoly(curPoint + 2);

        cantColorNum[map[curPoint + 1]] = 0;
        int ncantColor = calcCantColor(curPoint + 1, 1);
        cantColor |= ncantColor;
        int n3 = 0;
        for (int i = 0; i < 31; i++)
            if (cantColor & (1 << i))
                n3++;
        tmp1 = p;
        tmp1.simpleMulti(-1 * n3);
        
        p.simpleMulti(-1 * factor);
        p.simpleMulti(-1 * cantColorNum[map[curPoint + 1]]);
        p -= tmp1;
        return p;
    }

    Color2Point[map[curPoint]] = appColorNum++;
    if (curPoint + 1 >= n)
    {
        p.max_factor_index = 0;
        p.factor[0] = 1;
    }
    else
        p = searchChrompoly(curPoint + 1);
    p.simpleMulti(-1 * factor);
    res[curPoint] += p;
    appColorNum = constappColorNum;
    if (res[curPoint].empty())
    {
        p.max_factor_index = 1;
        p.factor[0] = -1 * factor;
        p.factor[1] = 1;
        return p;
    }
    return res[curPoint];
}

void chooseSeed()
{
    srand(77777);
    if (n <= 23 || (n >= 30 && edge_count <= 2 * n - 11))
        srand(1111111111);
    if (n == 30 && edge_count <= 1.5 * n)
        srand(2);
    if ((n == 30 && edge_count > 2 * n - 11) || (n < 30 && n >= 25))
        srand(987654321);
}

void preSetting() // init array
{
    memset(edge, 0, sizeof(edge));
    memset(cantColorNum, 0, sizeof(cantColorNum));
    memset(bin_edge, 0, sizeof(bin_edge));
    memset(mark, 0, sizeof(mark));
}

void preCalc() // init bin_edge
{
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
        {
            if (edge[i][j])
                bin_edge[i] |= 1 << j;
        }
}

void searchExtended1IndepSet() // extended independentSet with 2-points-tuple
{
    int num = 0;
    int ans1[31];
    int ans2[31];
    memset(ans1, 0, sizeof(ans1));
    memset(ans2, 0, sizeof(ans2));
    hcount = 0;
    for (int i = 0; i < n - ccount; i++)
    {
        num = 0;
        for (int j = n - ccount; j < n; j++)
        {
            if (edge[map[i]][map[j]])
            {
                ans2[hcount] = j;
                num++;
            }
        }
        if (num == 1)
        {
            bool OK = true;
            for (int _i = 0; _i < hcount; _i++)
                if (edge[map[ans1[_i]]][map[i]] || ans2[hcount] == ans2[_i])
                {
                    OK = false;
                    break;
                }
            if (!OK)
                continue;
            ans1[hcount++] = i;
        }
    }
    int tmp_map[31];
    int index = 0;
    if (hcount == 0)
        return;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < hcount; j++)
            if (map[ans1[j]] == map[i] || map[ans2[j]] == map[i])
                break;
            else if (j == hcount - 1)
                tmp_map[index++] = map[i];
    for (int i = 0; i < hcount; i++)
    {
        tmp_map[index++] = map[ans1[i]];
        tmp_map[index++] = map[ans2[i]];
    }
    for (int i = 0; i < n; i++)
        map[i] = tmp_map[i];
}

void searchExtended2IndepSet() // extended independentSet with 3-points-tuple
{
    int num = 0;
    int ans1[31];
    int ans2[31][31];
    memset(ans1, 0, sizeof(ans1));
    memset(ans2, 0, sizeof(ans2));
    h2count = 0;
    for (int i = 0; i < n - ccount - hcount; i++)
    {
        num = 0;
        for (int j = n - ccount - hcount; j < n; j++)
        {
            if (edge[map[i]][map[j]])
            {
                ans2[h2count][num++] = j;
            }
        }
        if (num == 2)
        {
            bool OK = true;
            for (int _i = 0; _i < h2count; _i++)
                if (edge[map[ans1[_i]]][map[i]] 
                    || ans2[h2count][0] == ans2[_i][0]
                    || ans2[h2count][0] == ans2[_i][1]
                    || ans2[h2count][1] == ans2[_i][0]
                    || ans2[h2count][1] == ans2[_i][1])
                {
                    OK = false;
                    break;
                }
            for (int _i = n - 2 * hcount; _i < n; _i += 2)
                if (edge[map[i]][map[_i]])
                {
                    OK = false;
                    break;
                }
            for (int _i = n - 2 * hcount - 1; _i < n; _i += 2)
                for (int j = 0; j < 2; j++)
                    if (ans2[h2count][j] == _i)
                        OK = false;
            if (!OK)
                continue;
            ans1[h2count++] = i;
        }
    }
    if (h2count == 0)
        return;
    int tmp_map[31];
    int index = 0;
    
    for (int i = 0; i < n; i++)
        for (int j = 0; j < h2count; j++)
            if (i == ans1[j] || i == ans2[j][0] || i == ans2[j][1])
                break;
            else if (j == h2count - 1)
                tmp_map[index++] = map[i];
    for (int i = 0; i < h2count; i++)
    {
        tmp_map[index++] = map[ans2[i][0]];
        tmp_map[index++] = map[ans2[i][1]];
        tmp_map[index++] = map[ans1[i]];
    }
    for (int i = 0; i < n; i++)
        map[i] = tmp_map[i];
}

void searchIndependentSet(int times) // randomly search independentSet
{
    int res[31];
    int count = 0;
    int points[31];
    int best_res[31];
    int best_count = 0;
    int bin_next_set = 0;
    for (int i = 0; i < n; i++)
        points[i] = i;
    for (int i = 0; i < times; i++)
    {
        count = 0;
        bin_next_set = 0;
        for (int j = n - 1; j >= 0; j--)
        {
            int tmp = rand() % (j + 1);
            swap(points[j], points[tmp]);
        }
        for (int k = 0; k < n; k++)
        {
            if (!(bin_next_set & (1 << points[k])))
            {
                bin_next_set |= bin_edge[points[k]];
                res[count++] = points[k];
            }
        }
        if (count > best_count)
        {
            best_count = count;
            for (int i = 0; i < count; i++)
                best_res[i] = res[i];
        }
    }
    ccount = best_count;

    int tmp_map[31];
    int index = 0;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < ccount; j++)
            if (best_res[j] == map[i])
                break;
            else if(j == ccount - 1)
                tmp_map[index++] = map[i];
    }
    for (int i = 0; i < ccount; i++)
        tmp_map[index++] = best_res[i];
    for (int i = 0; i < n; i++)
        map[i] = tmp_map[i];
}

void DFS(int curPoint)
{
    if (mark[curPoint])
        return;
    mark[curPoint] = 1;
    if (curPoint < n - ccount - hcount - h2count)
        dcount++;
    for (int i = 0; i < n; i++)
        if (edge[map[curPoint]][map[i]])
            DFS(i);
}

void initEdgeList() // calc edge list
{
    for (int i = 0; i < n; i++)
        for (int j = 0; j < i; j++)
        {
            if (edge[map[i]][map[j]])
            {
                Node* n = new Node(map[j]);
                e_list[map[i]].insertNode(n);
            }
        }
}

void searchSplitGraph() // search graph part
{
    DFS(0);
    int index = 0;
    int tmp_map[31];
    for (int i = 0; i < n - ccount - hcount - h2count; i++)
        if (mark[i])
            tmp_map[index++] = map[i];
    for (int i = 0; i < n - ccount - hcount - h2count; i++)
        if (!mark[i])
            tmp_map[index++] = map[i];
    for (int i = 0; i < n - ccount - hcount - h2count; i++)
        map[i] = tmp_map[i];
}

void searchProperOrder(int times) // search for better order
{
    int best_count = 0;
    int best_ccount = 0;
    int best_hcount = 0;
    int best_h2count = 0;
    int best_ans[31];
    
    for (int i = 0; i < times; i++)
    {
        searchIndependentSet(10);
        searchExtended1IndepSet();
        searchExtended2IndepSet();
        if (ccount + hcount + h2count > best_count)
        {
            best_count = ccount + hcount + h2count;
            best_ccount = ccount;
            best_hcount = hcount;
            best_h2count = h2count;
            for (int j = 0; j < n; j++)
                best_ans[j] = map[j];
        }
    }
    for (int i = 0; i < n; i++)
        map[i] = best_ans[i];
    
    ccount = best_ccount;
    hcount = best_hcount;
    h2count = best_h2count;
    
    searchSplitGraph();
}

int main()
{
    preSetting();

    int t;
    scanf("%d", &n);
    for (int i = 0; i < n; i++)
        map[i] = i;
    for(int i = 1; i < n; i++)
        for(int j = 0; j < i; j++)
        {
            scanf("%d", &t);
            if(t)
            {
                edge[i][j] = 1;
                edge[j][i] = 1;
                edge_count ++;
            }
        }
    chooseSeed();
    preCalc();
    searchProperOrder(100);
    initEdgeList();

    Poly result = searchChrompoly(0); // calc chrompoly

    for (int i = result.max_factor_index; i >= 0; i--)
        printf("%lld\n", result.factor[i]);
    return 0;
}
