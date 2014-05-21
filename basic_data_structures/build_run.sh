g++ -I../common/ -Isrc/ src/main.cpp -O3 -o main.o -std=gnu++11 -c
g++ -I../common/ -Isrc/ ../common/lexico_tree/KeyManager.cpp -o KeyManager.o -O3 -std=gnu++11 -c
g++ -O3 -o tester *.o  -lboost_system -lpthread 
rm *.o

for filename in $@
do
     echo "processing $filename"
     ./tester $filename 0
     ./tester $filename 1
     ./tester $filename 2
     ./tester $filename 3
     ./tester $filename 4
     ./tester $filename 5
     ./tester $filename 6
     ./tester $filename 7
done
