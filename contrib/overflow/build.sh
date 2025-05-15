make -j$(nproc) -f scripts/makefile.std
g++ -std=c99 -fpermissive contrib/overflow/read_to_gray.c -I. -o read_to_gray libpng.a -lz