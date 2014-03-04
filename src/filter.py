def filter():
    strFileName = ''
    for i in range(121):
        strFileName = 'out{}.txt'.format(i)
        print('Read File : {}'.format(strFileName))
        fin = open(strFileName, 'r')
        linearLines = fin.readlines()
        linearSet = set()
        for line in linearLines:
          linearSet.add(line)
        fin.close()

    fout = open('out_2807.txt', 'w');
    linearList = sorted(list(linearSet))
    for elem in linearList:
        fout.write(elem)
    fout.close

if __name__ == '__main__':
    filter()

