#include "common/file_run.hpp"
#include "common/repl.hpp"
#include <string>

int main(int argc, char* argv[]) {
    if (argc < 2)
        return run_repl();
    std::string filename = argv[1];
    return file_run(filename);
}