#include <CLI/CLI.hpp>

int main(int argc, char** argv) {
    int flag = 0;

    CLI::App cli_app("Simulator 2022");

    cli_app.add_option("-f,--flag", flag, "Flag of condition")->required();
    CLI11_PARSE(cli_app, argc, argv);

    return 0;
}