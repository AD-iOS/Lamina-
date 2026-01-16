#include "file_run.hpp"

#include <string>
#include <fstream>

#include "../compiler/lexer.hpp"
#include "../compiler/parser.hpp"
#include "../compiler/generator/generator.hpp"
#include "../runtime/vm.hpp"

static std::string read_file(const std::string& file_name) {
    std::ifstream file(file_name);
    return std::string(std::istreambuf_iterator(file),std::istreambuf_iterator<char>());
}

int file_run(const std::string& file_name) {
    auto src = read_file(file_name);
    lmx::Lexer lexer(src);
    auto ts = lexer.tokenize(src);
    lmx::Parser parser(ts);
    lmx::Generator gener;
    auto node = parser.parse_program();
    if (!node || parser.error()) return -1;
    [[maybe_unused]] auto _1 = node->gen(gener);
    gener.ops.emplace_back(lmx::runtime::Opcode::HALT);
    lmx::runtime::VirtualCore vm;
    vm.set_program(&gener.ops);

    vm.run();
    return 0;
}
