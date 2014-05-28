export HDT_INCLUDE="-I ../common/ -Isrc -Isrc/hdt-lib-rc1-src/deps/libcds-v1.0.12/includes -Isrc/hdt-lib-rc1-src/src/ -Isrc/hdt-lib-rc1-src/src/dictionary/ -Isrc/hdt-lib-rc1-src/include"
g++ -O3 $HDT_INCLUDE src/main_hdt.cpp -o main.o -std=gnu++11 -c
g++ -O3 $HDT_INCLUDE src/hdt-lib-rc1-src/src/libdcs/VByte.cpp -std=gnu++11 -c
g++ -O3 $HDT_INCLUDE src/hdt-lib-rc1-src/src/libdcs/CSD_PFC.cpp  -std=gnu++11 -c
g++ -O3 $HDT_INCLUDE src/hdt-lib-rc1-src/src/libdcs/CSD.cpp  -std=gnu++11 -c
g++ -O3 $HDT_INCLUDE src/hdt-lib-rc1-src/src/sequence/LogSequence2.cpp -std=gnu++11 -c
g++ -O3 $HDT_INCLUDE src/hdt-lib-rc1-src/src/util/crc8.cpp -std=gnu++11 -c
g++ -O3 $HDT_INCLUDE src/hdt-lib-rc1-src/src/util/crc32.cpp -std=gnu++11 -c

g++ -o tester *.o 
rm *.o

for filename in $@
do
     echo "processing $filename"
     ./tester $filename
done
