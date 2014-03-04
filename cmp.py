
def cmp():
    strFileName1 = 'record.csv'
    strFileName2 = 'record_array.csv'

    fin1 = open(strFileName1, 'r')
    l1 = fin1.readlines()
    s1 = set()
    for line in l1:
        s1.add(line)
    l1 = list(s1)
    fin1.close

    fin2 = open(strFileName2, 'r')
    l2 = fin2.readlines()
    s2 = set()
    for line in l2:
        s2.add(line)
    l2 = list(s2)
    fin2.close

    fout = open('csvcmp.txt', 'w')
    fout.write('in src but not in src_array : \n')
    for elem in l1:
        if elem not in s2:
            fout.write(elem)

    fout.write('in src_array but not in src : \n')
    for elem in l2:
        if elem not in s1:
            fout.write(elem)

    fout.close()

if __name__ == '__main__':
    cmp()


