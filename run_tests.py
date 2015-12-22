import os

N = 30

count = 0
num_satcase = 0

os.system("rm failed_cases.txt")
os.system("g++ mini_test.cpp")

for i in range(N):
    os.system("python ./sample/testcase/caseGenerator.py")
    os.system("minisat testcase.txt > out1")
    os.system("./a.out testcase.txt > out2")
   
    with open('out1') as f1:
        for line in f1:
            tmp1 = line
    
    with open('out2') as f2:
        for line in f2:
            tmp2 = line
    
    
    if (tmp1 == 'SATISFIABLE\n' and tmp2 == '1\n') or (tmp1 == 'UNSATISFIABLE\n' and tmp2 == '0\n'):        
        num_satcase += 1
    else:
        os.system("cat testcase.txt >> failed_cases.txt")
    count += 1
   
print num_satcase, 'cases satisfy among', count, 'cases.'
