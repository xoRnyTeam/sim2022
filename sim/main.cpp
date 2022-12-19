#include "driver/driver.hpp"
#include <CLI/CLI.hpp>

#include <timer.hpp>

int main(int argc, char **argv) {
  std::string path_to_exec{};
  std::string path_to_trace{};
  //
  CLI::App cli_app("Simulator 2022");

  cli_app.add_option("-p,--path", path_to_exec, "Path to executable file")
      ->required();
  cli_app.add_option("--trace", path_to_trace, "Path for trace dump");

  CLI11_PARSE(cli_app, argc, argv);
  sim::Driver driver{path_to_exec, path_to_trace};

  timer::Timer timer{};
  driver.run();
  double elapsed = timer.elapsed<timer::Timer::millisecs>();
  std::cout << elapsed << " millisecs\n";
  std::cout << driver.getInstCounter() << " instractions\n";
  std::cout << driver.getInstCounter() / elapsed * 1e+3 << std::endl;

  return 0;
}