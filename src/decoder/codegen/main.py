from pathlib import Path
import argparse

from parser import Parser
from codegen import CodeGen
from ruamel.yaml import YAML

def main():
    parser = argparse.ArgumentParser(description="RISC-V ISA parser")
    parser.add_argument('-dir', '--directory', type=str, help="Path to yaml sources directory")
    parser.add_argument('--iset', type=str, help="Base instruction set type")
    #
    args = parser.parse_args()
    parser = Parser(args)
    parser.parse()

    exec_path = Path.resolve(Path(__file__)).parent
    project_root = exec_path.parent.parent.parent
    print(project_root)
    
    driver = CodeGen(parser.instructions, project_root)
    driver.gen()
    #
#
if __name__ == "__main__":
    main()