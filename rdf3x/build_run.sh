#rdf3x
g++ -O3 -I../common -Isrc/gh-rdf3x  -Isrc/gh-rdf3x/include src/main_rdf3x.cpp -o main.o -std=gnu++11 -c
g++ -O3 -Isrc/gh-rdf3x/include src/gh-rdf3x/infra/osdep/*.cpp -std=gnu++11 -c
g++ -O3 -Isrc/gh-rdf3x/include src/gh-rdf3x/infra/util/*.cpp -std=gnu++11 -c
g++ -O3 -Isrc/gh-rdf3x/include src/gh-rdf3x/rts/buffer/*.cpp -std=gnu++11 -c
g++ -O3 -Isrc/gh-rdf3x/include src/gh-rdf3x/rts/database/*.cpp -std=gnu++11 -c
g++ -O3 -Isrc/gh-rdf3x/include src/gh-rdf3x/rts/operator/*.cpp -std=gnu++11 -c
g++ -O3 -Isrc/gh-rdf3x/include src/gh-rdf3x/rts/partition/*.cpp -std=gnu++11 -c
g++ -O3 -Isrc/gh-rdf3x/include src/gh-rdf3x/rts/runtime/*.cpp -std=gnu++11 -c
g++ -O3 -Isrc/gh-rdf3x/include src/gh-rdf3x/rts/segment/*.cpp -std=gnu++11 -c
g++ -O3 -Isrc/gh-rdf3x/include src/gh-rdf3x/rts/transaction/*.cpp -std=gnu++11 -c

g++ $DEBUG -O3 -o tester *.o  -lboost_system -lpthread 
rm *.o

for filename in $@
do
     echo "processing $filename"
     ./tester $filename
done
