from pathlib import Path
import yaml
# from ruamel.yaml import YAML

ISET_EXTENSIONS = dict({"RV32I" : list([["rv32_i"], ["rv_i"]])})

def get_range(msb, lsb) -> dict:
    return dict({"msb" : msb, "lsb" : lsb})

def get_slice(range : dict) -> str:
    msb, lsb = range["msb"], range["lsb"]  
    return f"slice<{msb}, {lsb}>(word);"

REG_DICT = dict({
    "rm": [get_range(14, 12)],
    "rd": [get_range(11, 7)],
    "csr": [get_range(31, 20)],
    "rs2": [get_range(24, 20)],
    "rs1": [get_range(19, 15)],
    "rs3": [get_range(31, 27)],
})

IMM_DICT = dict({
    "imm20": [get_range(31, 12)],
    "jimm20": [
        get_range(31, 31),
        get_range(30, 21),
        get_range(20, 20),
        get_range(19, 12),
    ],
    "succ": [get_range(23, 20)],
    "pred": [get_range(27, 24)],
    "fm": [get_range(31, 28)],
    "imm12": [get_range(31, 20)],
    "bimm12hi": [get_range(31, 31), get_range(30, 25)],
    "bimm12lo": [get_range(11, 8), get_range(7, 7)],
    "imm12hi": [get_range(31, 25)],
    "imm12lo": [get_range(11, 7)],
})


class Instruction():
    def __init__(self, mn : str, instr_data : dict):
        self.mn = mn.upper()
        self.encoding = instr_data["encoding"]
        self.encoding = self.encoding.replace('-', 'x')
        self.mask = instr_data["mask"]
        #
        self.fields = instr_data["variable_fields"]
        #
    #
    def get_decode(self) -> list:
    #
        decode = list()
        decode.append(f"//! {self.mn}")
        decode.append(f"//! {self.encoding}")
        decode.append(f"instr.id = InstrId::{self.mn};")

        for field in self.fields:
            reg_ranges = REG_DICT.get(field)
            imm_ranges = IMM_DICT.get(field)
            #
            if reg_ranges != None:
                for it in reg_ranges:
                    decode.append(f"instr.{field} = {get_slice(it)}")
            #
            elif imm_ranges != None:
                for it in imm_ranges:
                    decode.append(f"instr.imm |= {get_slice(it)}")
        #
        decode.append("return instr;")
        #
        return decode
    #       
#
class Parser():
#
    def __init__(self, args) -> None:
        self.iset = args.iset
        self.path = args.directory + "/instr_dict.yaml"
        print(self.path)
        self.iset = args.iset
        #
        self.data = None
        self.cur_extensions = ISET_EXTENSIONS.get(self.iset)
        #
        # Read YAML file
        with open(self.path, 'r') as file:
            self.data = yaml.safe_load(file)
        #
        self.instructions = list()
        self.fields = list()
        #
    #
    def parse(self):
    #
        for it in self.data.items():
            mn, instr_data = it[0], it[1]
            if instr_data["extension"] in self.cur_extensions:
                self.instructions.append(Instruction(mn, instr_data))