from decode_tree import DecodeTree
from decode_tree import Decoder
#
from parser import Instruction
from ruamel.yaml import YAML
from pathlib import PosixPath, Path

import textwrap

class CodeGen():
    def __init__(self, instructions : list, project_path : PosixPath) -> None:
    #
        self._instructions = instructions
        self._tree = DecodeTree(self._instructions)
        self._data = dict()
        #
        self.project_path = project_path 
        self.decoder_src = project_path.joinpath(Path("src", "decoder"))
        #
    #
    @property
    def decode_tree(self):
        return self._tree._decode_tree
        #
    #
    def gen(self):
    #
        self.yaml_gen(self.decoder_src.joinpath(Path("codegen", "risc-v_tree.yaml")))
        self.decoder_gen(self.decoder_src.joinpath(Path("decoder.cpp")))
        self.isa_gen(self.project_path.joinpath(Path("include", "isa", "isa.hpp")))

        # NOTE: it should be commented due handwrited exec_{mn} funcs
        # self.exec_gen(self.project_path.joinpath(Path("src", "executor", "executor.cpp")))
        #
    #
    def exec_gen(self, path : Path):
    #
        with open(path, "w") as cpp:
        #
            print(include("executor/executor"), file=cpp, end='')
            print(include("isa/instr"), file=cpp, end='')
            #
            print(open_scope("namespace sim"), file=cpp, end='')
            #
            for it in self._instructions:
                print(open_scope(f"void exec_{it.mn}(Instruction& instr, Hart& hart)"), file=cpp)
                # NOTE: execution realization
                print(close_scope(), file=cpp)
            #
            print(f"Executor::Executor() : ", file=cpp, end='')
            print(f"m_exec_instr{{", file=cpp)
            #
            for it in self._instructions:
                print(f"{{InstrId::{it.mn}, exec_{it.mn}}},", file=cpp)
            #
            print(f"}} {{}}", file=cpp)
            #
            print(close_scope("namespace sim"), file=cpp)
        #
    #
    def isa_gen(self, path : Path):
    #
        with open(path, "w") as hpp:
            print(open_guard("ISA"), file=hpp)

            print(include("string", sys=True), file=hpp, end='')
            print(include("unordered_map", sys=True), file=hpp, end='')

            print(open_scope("namespace sim"), file=hpp, end='')
            print(open_scope("enum class InstrId : uint16_t"), file=hpp)

            print("    NONE = 0,", file=hpp)
            #
            for it in self._instructions:
                print(f"    {it.mn},", file=hpp)
                #
            #
            print(close_scope("enum class InstrId", semicolon=True), file=hpp)
            #
            print(open_scope("inline std::unordered_map<InstrId, std::string> InstrId2str"), file=hpp)

            for it in self._instructions:
                print(f"    {{InstrId::{it.mn}, \"{it.mn}\"}},", file=hpp)
            print(close_scope(semicolon=True), file=hpp)
            #
            print(close_scope("namespace sim"), file=hpp)
            print(close_guard("ISA"), file=hpp)

        #
    #
    def yaml_gen(self, path : Path):
        yaml = YAML()
        yaml.register_class(Instruction)
        yaml.indent = 4
        #
        self._data['decode_tree'] = self.decode_tree
        #
        with open(path, "w+") as file:
            yaml.dump(self._data, file)
        #
    #
    def decoder_gen(self, path : Path):
    #
        decoder = Decoder(self.decode_tree)
        decoder.dump()

        with open(path, "w") as cpp:
            print(include("isa/instr"), file=cpp)
            print(include("decoder/decoder"), file=cpp)
            print(include("bit", sys=True), file=cpp, end='')
            print(include("bitset", sys=True), file=cpp, end='')

            print(open_scope("namespace sim"), file=cpp)
            print(open_scope("Instruction Decoder::decode(word_t word)"), file=cpp)
            print("  Instruction instr{};", file=cpp)

            for str in decoder._decoder_code:
                print(str, file=cpp)

            print(close_scope(), file=cpp)
            print(close_scope(), file=cpp)
    #
#
def open_guard(name : str):
    return textwrap.dedent(
        f"""
         #ifndef {name}_HPP
         #define {name}_HPP
         """
    )

def close_guard(name : str):
    return textwrap.dedent(
        f"""
         #endif //! {name}_HPP
         """
    )

def open_scope(name : str):
    return textwrap.dedent(
        f"""
         {name} {{
         """
    )
    #
#
def close_scope(name : str = str(), semicolon=False):
    if semicolon == True:
        return textwrap.dedent(
            f"""
             }}; //! {name}
             """
        )
    else:
        return textwrap.dedent(
            f"""
             }} //! {name}
             """
        )
    #
#
def include(name : str, sys = False):
    if sys == True:
        return textwrap.dedent(
            f"""
             #include <{name}>
             """
        )
    else:
        return textwrap.dedent(
            f"""
             #include "{name}.hpp"
             """
        )

