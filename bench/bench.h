#ifndef LVGL_CPP_BENCH_BENCH_H_
#define LVGL_CPP_BENCH_BENCH_H_

#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace lvgl::bench {

/**
 * @brief Benchmark execution state and configuration.
 */
struct State {
  int iterations = 100;
  // identifying args could go here later
};

/**
 * @brief Abstract base class for all benchmarks.
 */
class Benchmark {
 public:
  virtual ~Benchmark() = default;
  virtual const char* name() const = 0;
  virtual void run(State& state) = 0;
};

/**
 * @brief Singleton registry for discovering benchmarks.
 */
class Registry {
 public:
  static Registry& get();

  /**
   * @brief Register a benchmark instance.
   * Transfer ownership to the registry.
   */
  void register_benchmark(std::unique_ptr<Benchmark> bench);

  /**
   * @brief Get the list of all registered benchmarks.
   */
  const std::vector<std::unique_ptr<Benchmark>>& get_benchmarks() const;

  /**
   * @brief Run benchmarks matching the pattern.
   * @param pattern Substring to match against benchmark name.
   */
  void run(const std::string& pattern);

 private:
  Registry() = default;
  std::vector<std::unique_ptr<Benchmark>> benchmarks_;
};

/**
 * @brief Helper class for auto-registration.
 */
template <typename T>
struct AutoRegister {
  AutoRegister() { Registry::get().register_benchmark(std::make_unique<T>()); }
};

}  // namespace lvgl::bench

/**
 * @brief Macro to define a new benchmark.
 *
 * Usage:
 * LVGL_BENCHMARK(MyWidgetCreate) {
 *   for(int i=0; i<state.iterations; i++) { ... }
 * }
 */
#define LVGL_BENCHMARK(Name)                                      \
  class Bench_##Name : public lvgl::bench::Benchmark {            \
   public:                                                        \
    const char* name() const override { return #Name; }           \
    void run(lvgl::bench::State& state) override;                 \
  };                                                              \
  static lvgl::bench::AutoRegister<Bench_##Name> register_##Name; \
  void Bench_##Name::run(lvgl::bench::State& state)

#endif  // LVGL_CPP_BENCH_BENCH_H_
