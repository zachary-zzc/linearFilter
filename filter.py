def shelter():
    fin = open('record_array.csv', 'r');
    linearLines = fin.readlines()
    linearSet = set()
    for line in linearLines:
        linearSet.add(line)
    fin.close

    fout = open('record.csv', 'w');
    linearList = sorted(list(linearSet))
    for elem in linearList:
        fout.write(elem)
    fout.close

if __name__ == '__main__':
    shelter()

