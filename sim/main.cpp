#include "driver/driver.hpp"
#include <CLI/CLI.hpp>

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
  driver.run();

  return 0;
}