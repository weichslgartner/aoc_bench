# aoc_bench

benchmarking AdventofCode solutions


pip install conan
mkdir build && cd build
conan install .. --build=missing
cmake ..
./bin/day15 --benchmark_min_time=40
