import random

def generator():
    
    num_var = random.randint(1, 10)
    num_clause = random.randint(1, 2*(num_var))

    outfile = open('testcase.txt', 'w')
    outfile.write('p' +' cnf' + ' ' + str(num_var) + ' ' + str(num_clause) + '\n')
    dic = {i: -1 for i in range(1, num_var+1)}
   
    mem = []
    ind = 0
    while ind < num_clause:
        line = ''
        k = random.randint(1, 2*num_var)
        j = 0
        l = []
        pre = '#'
        while j < k:
            tmp = random.randint(-num_var, num_var)
            if tmp == 0:
                j -= 1
            else:
                dic[abs(tmp)] = 1
                if (abs(tmp) not in l) and pre != '#': 
                    line += ' ' + str(tmp)
                    l.append(abs(tmp))
                elif (abs(tmp) not in l) and pre == '#':
                    line += str(tmp)
                    l.append(abs(tmp))
                    pre = '*'
            
            j +=1
        for i in dic.keys():
            if dic[i] == -1:
                line += ' ' + str(i)

        l1 = line.split(' ')
        l2 = map(int, l1)
        l2 = sorted(l2, key=abs)
        line = ' '.join(map(str, l2))

        if line in mem:
            ind -= 1
        else:
            outfile.write(line + ' ' + '0' + '\n')
            mem.append(line)
        ind += 1
    outfile.close()

if __name__ == "__main__":
    generator()
