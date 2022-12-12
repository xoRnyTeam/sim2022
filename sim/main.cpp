#include "driver/driver.hpp"
#include <CLI/CLI.hpp>

int main(int argc, char **argv) {
  std::string path_to_exec{};
  //
  CLI::App cli_app("Simulator 2022");

  cli_app.add_option("-p,--path", path_to_exec, "Path to executable file")
      ->required();
  CLI11_PARSE(cli_app, argc, argv);
  sim::Driver driver{path_to_exec};
  driver.run();

  return 0;
}