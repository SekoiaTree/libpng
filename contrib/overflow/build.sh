autoupdate
autoreconf -f -i
./configure
make -j$(nproc) clean
make -j$(nproc) libpng15.la
g++ -std=c99 -fpermissive ../read_to_gray.c -I. -o read_to_gray .libs/libpng15.a -lz