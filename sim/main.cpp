#include "driver/driver.hpp"
#include <CLI/CLI.hpp>

#include <timer.hpp>

int main(int argc, char **argv) {
  std::string path_to_exec{};
  std::string path_to_trace{};
  uint32_t size = 0;
  //
  CLI::App cli_app("Simulator 2022");

  cli_app.add_option("-p,--path", path_to_exec, "Path to executable file")
      ->required();
  cli_app.add_option("--trace", path_to_trace, "Path for trace dump");
  cli_app.add_option("--cacheSize", size, "Size of direct-mapped cache");

  CLI11_PARSE(cli_app, argc, argv);
  sim::Driver driver{path_to_exec, path_to_trace, size};

timer::Timer timer{};
  if ( driver.getBBCache() ) {
    driver.run();
  } else {
    driver.runCache();
  }
  double elapsed = timer.elapsed<timer::Timer::millisecs>();
  std::cout << elapsed << " millisecs\n";
  std::cout << driver.getInstCounter() << " instructions\n";
  std::cout << driver.getInstCounter() / elapsed * 1e+3 / 1e+6 << " MIPS"
            << std::endl;
  driver.printCoverageCache();
  return 0;
}
