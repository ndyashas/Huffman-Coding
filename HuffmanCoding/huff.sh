gcc createHuffTable.c;
./a.out $1;
gcc encode.c;
./a.out table.txt $1;
rm a.out table.txt;