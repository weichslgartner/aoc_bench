#include <fmt/core.h>
#include <deque>
#include <unordered_map>
#include <map>
#include "absl/container/flat_hash_map.h"
#include "absl/container/node_hash_map.h"
#include "folly/container/F14Map.h"
#include "boost/unordered_map.hpp"
#include <benchmark/benchmark.h>
#include <vector>

int bound = 30000000;
std::deque<int> init_numbers { 6, 13, 1, 15, 2, 0 };

template<class hash_map_type>
auto solve(hash_map_type &memory, std::deque<int> init_numbers, int const bound) {
	//hash_map memory { };
	//memory.reserve(bound/4);
	auto current_number { 0 };
	auto last_number { -1 };
	for (auto i { 1 }; i <= bound; ++i) {
		if (!init_numbers.empty()) {
			current_number = init_numbers.front();
			init_numbers.pop_front();
		} else {
			if (!memory.contains(last_number)) {
				current_number = 0;
			} else {
				current_number = i - 1 - memory[last_number];
			}
		}
		//fmt::print("{}, {}\n", i, current_number);
		if (memory.contains(last_number)) {
			memory.at(last_number) = i - 1;
		} else {
			memory.insert( { last_number, i - 1 });
		}
		last_number = current_number;
	}
	return current_number;
}

template<class hash_map_type>
auto solve_find(hash_map_type &memory, std::deque<int> init_numbers, int const bound) {
	//hash_map memory { };
	//memory.reserve(bound/4);
	auto current_number { 0 };
	auto last_number { -1 };
	for (auto i { 1 }; i <= bound; ++i) {
		if (!init_numbers.empty()) {
			current_number = init_numbers.front();
			init_numbers.pop_front();
		} else {
			if (memory.find(last_number) == memory.end()) {
				current_number = 0;
			} else {
				current_number = i - 1 - memory[last_number];
			}
		}
		//fmt::print("{}, {}\n", i, current_number);
		if (memory.find(last_number) != memory.end()) {
			memory.at(last_number) = i - 1;
		} else {
			memory.insert(std::pair { last_number, i - 1 });
		}
		last_number = current_number;
	}
	return current_number;
}

auto solve_vector(std::vector<int> &memory, std::deque<int> init_numbers, int const bound) {
	//hash_map memory { };
	//memory.reserve(bound/4);
	auto current_number { 0 };
	auto last_number { -1 };

	for (auto i { 1 }; i <= bound; ++i) {
		if (!init_numbers.empty()) {
			current_number = init_numbers.front();
			init_numbers.pop_front();
		} else {
			if (last_number >= memory.size() or memory.at(last_number) == 0) {
				current_number = 0;
			} else {
				current_number = i - 1 - memory[last_number];
			}
		}
		//fmt::print("{}, {}\n", i, current_number);
		if (last_number != -1) {
			if (last_number < memory.size()) {
				memory.at(last_number) = i - 1;
			} else {
				memory.resize(last_number + 1,0);

				memory.at(last_number) = i - 1;
			}
		}
		last_number = current_number;
	}
	return current_number;
}

static void BENCH_folly_F14NodeMap(benchmark::State &state) {
	for (auto _ : state) {
		folly::F14NodeMap<int, int> memory;
		auto res = solve(memory, init_numbers, bound);
		benchmark::DoNotOptimize(res);
	}
}

static void BENCH_folly_F14NodeMap_reserve(benchmark::State &state) {
	for (auto _ : state) {
		folly::F14NodeMap<int, int> memory;
		memory.reserve(bound / 4);
		auto res = solve(memory, init_numbers, bound);
		benchmark::DoNotOptimize(res);
	}
}

static void BENCH_folly_F14FastMap(benchmark::State &state) {
	for (auto _ : state) {
		folly::F14FastMap<int, int> memory;
		auto res = solve(memory, init_numbers, bound);
		benchmark::DoNotOptimize(res);
	}
}

static void BENCH_folly_F14FastMap_reserve(benchmark::State &state) {
	for (auto _ : state) {
		folly::F14FastMap<int, int> memory;
		memory.reserve(bound / 4);
		auto res = solve(memory, init_numbers, bound);
		benchmark::DoNotOptimize(res);
	}
}

static void BENCH_absl_flat_hash_map(benchmark::State &state) {
	for (auto _ : state) {
		absl::flat_hash_map<int, int> memory;
		auto res = solve(memory, init_numbers, bound);
	}
}

static void BENCH_absl_flat_hash_map_reserve(benchmark::State &state) {
	for (auto _ : state) {
		absl::flat_hash_map<int, int> memory;
		memory.reserve(bound / 4);
		auto res = solve(memory, init_numbers, bound);
	}
}

static void BENCH_absl_node_map(benchmark::State &state) {
	for (auto _ : state) {
		absl::node_hash_map<int, int> memory;
		auto res = solve(memory, init_numbers, bound);
	}
}

static void BENCH_absl_node_map_reserve(benchmark::State &state) {
	for (auto _ : state) {
		absl::node_hash_map<int, int> memory;
		memory.reserve(bound / 4);
		auto res = solve(memory, init_numbers, bound);
	}
}

static void BENCH_stl_unordered_map(benchmark::State &state) {
	for (auto _ : state) {
		std::unordered_map<int, int> memory;
		auto res = solve(memory, init_numbers, bound);
	}
}

static void BENCH_stl_unordered_map_reserve(benchmark::State &state) {
	for (auto _ : state) {
		std::unordered_map<int, int> memory;
		memory.reserve(bound / 4);
		auto res = solve(memory, init_numbers, bound);
	}
}

static void BENCH_stl_ordered_map(benchmark::State &state) {
	for (auto _ : state) {
		std::map<int, int> memory;
		auto res = solve(memory, init_numbers, bound);
	}
}

static void BENCH_boost_unordered_map(benchmark::State &state) {
	for (auto _ : state) {
		boost::unordered_map<int, int> memory;
		auto res = solve_find(memory, init_numbers, bound);
	}
}

static void BENCH_boost_unordered_map_reserve(benchmark::State &state) {
	for (auto _ : state) {
		boost::unordered_map<int, int> memory;
		memory.reserve(bound / 4);
		auto res = solve_find(memory, init_numbers, bound);
	}
}

static void BENCH_vector(benchmark::State &state) {
	for (auto _ : state) {
		std::vector<int> memory { };
		auto res = solve_vector(memory, init_numbers, bound);
	//	fmt::print("{}", res);
	}
}

static void BENCH_vector_resize(benchmark::State &state) {
	for (auto _ : state) {
		std::vector<int> memory { };
		memory.resize(bound / 8);
		auto res = solve_vector(memory, init_numbers, bound);
	//	fmt::print("{}", res);
	}
}

static void BENCH_vector_reserve(benchmark::State &state) {
	for (auto _ : state) {
		std::vector<int> memory { };
		memory.reserve(bound / 8);
		auto res = solve_vector(memory, init_numbers, bound);
	//	fmt::print("{}", res);
	}
}

BENCHMARK(BENCH_vector)->Unit(benchmark::kMillisecond);
BENCHMARK(BENCH_vector_resize)->Unit(benchmark::kMillisecond);
BENCHMARK(BENCH_vector_reserve)->Unit(benchmark::kMillisecond);
BENCHMARK(BENCH_stl_ordered_map)->Unit(benchmark::kMillisecond);
BENCHMARK(BENCH_stl_unordered_map)->Unit(benchmark::kMillisecond);
BENCHMARK(BENCH_stl_unordered_map_reserve)->Unit(benchmark::kMillisecond);
BENCHMARK(BENCH_boost_unordered_map)->Unit(benchmark::kMillisecond);
BENCHMARK(BENCH_boost_unordered_map_reserve)->Unit(benchmark::kMillisecond);
BENCHMARK(BENCH_folly_F14FastMap)->Unit(benchmark::kMillisecond);
BENCHMARK(BENCH_folly_F14FastMap_reserve)->Unit(benchmark::kMillisecond);
BENCHMARK(BENCH_folly_F14NodeMap)->Unit(benchmark::kMillisecond);
BENCHMARK(BENCH_folly_F14NodeMap_reserve)->Unit(benchmark::kMillisecond);
BENCHMARK(BENCH_absl_flat_hash_map)->Unit(benchmark::kMillisecond);
BENCHMARK(BENCH_absl_flat_hash_map_reserve)->Unit(benchmark::kMillisecond);
BENCHMARK(BENCH_absl_node_map)->Unit(benchmark::kMillisecond);
BENCHMARK(BENCH_absl_node_map_reserve)->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();

