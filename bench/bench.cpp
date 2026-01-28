#include "bench.h"

#include <algorithm>
#include <iostream>

namespace lvgl::bench {

Registry& Registry::get() {
  static Registry instance;
  return instance;
}

void Registry::register_benchmark(std::unique_ptr<Benchmark> bench) {
  benchmarks_.push_back(std::move(bench));
}

const std::vector<std::unique_ptr<Benchmark>>& Registry::get_benchmarks()
    const {
  return benchmarks_;
}

void Registry::run(const std::string& pattern) {
  // Logic for running specific benchmarks will be handled in main loop
  // based on the filtered list provided by get_benchmarks().
  // This method might be deprecated or used for direct execution API.
}

}  // namespace lvgl::bench
