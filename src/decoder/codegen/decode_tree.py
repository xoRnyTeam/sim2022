from collections import defaultdict
from parser import Instruction

from typing import List

INDENT = "    "

class DecodeTree():
    def __init__(self, instructions) -> None:
        self.instructions = instructions
        self._decode_tree = dict()
        #
        self.make_head()
        #
    #
    def make_head(self):
    #
        maj_range = get_maj_range(get_lead_bits(self.instructions))
        msb, lsb = maj_range[-1], maj_range[0]
        width = msb - lsb + 1
        #
        node = 0
        #
        self._decode_tree["range"] = get_range(msb, lsb)
        self._decode_tree["nodes"] = dict()
        #
        for _ in range(1 << width):
        #
            shifted_node = node >> lsb
            self._decode_tree["nodes"][shifted_node] = dict()
            is_visited, instrs = self.make_child(node, ones(width) << lsb, self.instructions, self._decode_tree["nodes"][shifted_node])
            #
            if is_visited == True:
                self._decode_tree["nodes"][shifted_node] = instrs
            #
            if self._decode_tree["nodes"][shifted_node] == dict():
                self._decode_tree["nodes"].pop(shifted_node)
            #
            node += 1 << lsb
          #
    #
    def make_child(self, node : int, separ_mask : int, instr_list : list, subtree : dict):
        sublist = make_sublist(instr_list, node, separ_mask)
        lead_bits = get_lead_bits(sublist, separ_mask)
        sublist_size = len(sublist)
        #
        if sublist_size == 0:
            return None, None
        #
        elif sublist_size == 1:
            return True, [sublist[0]]
        #
        elif (sublist_size > 1 and lead_bits == 0):
            print("Warning : collisions was detected")
            inst_list = list([it for it in sublist])
            return True, inst_list
        else:
            maj_range = get_maj_range(lead_bits)
            msb, lsb = maj_range[-1], maj_range[0]
            width = msb - lsb + 1
            instr_num = 0
            #
            cur_node = 0 | node
            node_bits = ones(width) << lsb
            next_separ_mask = separ_mask | node_bits
            # 
            subtree["range"] = get_range(msb, lsb)
            subtree["nodes"] = dict()
            #
            for _ in range(1 << width):
                #
                if instr_num == sublist_size:
                    break
                #
                masked_node = (cur_node & node_bits) >> lsb
                subtree["nodes"][masked_node] = dict()
                #
                is_visited, instrs = self.make_child(cur_node, next_separ_mask, sublist, subtree["nodes"][masked_node])
                #
                if is_visited == True:
                    if instrs:
                        subtree["nodes"][masked_node] = instrs
                        instr_num += len(instrs)
                #
                if subtree["nodes"][masked_node] == dict():
                    subtree["nodes"].pop(masked_node)
                #
                cur_node += 1 << lsb
            
            if subtree["nodes"] == dict():
                subtree.pop("nodes")

            return None, None
            #
    #
#
def check_all_nodes(nodes):
    for node in nodes.values():
        if isinstance(node, dict):
            return False
        #
        if not isinstance(node[0], Instruction):
            return False
        #
    return True
    #
#
def make_sublist(instr_list : list, node : int, separ_mask : int) -> list:
#    
    sublist = list()
    #
    for instr in instr_list:
        cur_enc = instr.encoding
        cur_enc = int(cur_enc.replace('x', '0'), 2)
        if cur_enc & separ_mask == node:
            sublist.append(instr)
    #
    return sublist   
    #
#
def get_maj_range(mask):
#
    positions = list()
    bits_ranges = list(list())

    for i in range(32):
        cur_bit = get_bit(mask, i)

        if cur_bit != 0:
            positions.append(i)
        else:
            if positions != list():
                bits_ranges.append(list(positions))
                positions.clear()
        #
        if positions != list():
            bits_ranges.append(list(positions))
    #
    return bits_ranges[-1]
    #
#
def get_lead_bits(instructions : list, separ_mask : int = 0) -> int:
#
    lead_bits = ((1 << 32) - 1) ^ separ_mask
    #
    for instr in instructions:
        lead_bits &= int(instr.mask, 16)
    #
    return lead_bits
    #
#
class Decoder():
    def __init__(self, tree : dict) -> None:
        self._tree = tree
        self._decode_map = defaultdict(dict)
        self._decoder_code = list()
        #
        self.make_decoder(self._tree)
        #
    #
    def make_decoder(self, tree : dict, prev_mask : int = 0, prev_node : int = 0):
    #
        cur_range = tree["range"]
        lsb, msb = cur_range['lsb'], cur_range['msb']
        cur_mask = prev_mask | get_mask(lsb, msb)
        #
        for node in tree["nodes"]:
            cur_node = prev_node | node << lsb
            #
            if isinstance(tree["nodes"][node], dict):
                self.make_decoder(tree["nodes"][node], cur_mask, cur_node)
            #
            elif isinstance(tree["nodes"][node][0], Instruction):
                self._decode_map[cur_mask][cur_node] = tree["nodes"][node][0]
            #
    #
    def dump(self):
    #
        for (mask, decode_nodes) in self._decode_map.items():
            self._decoder_code.append(get_switch(mask))
            self._decoder_code.append(f"{{")
            for (node, instr) in decode_nodes.items(): 
                self._decoder_code.append(get_case(node))
                self._decoder_code.append(f"{{")
                self._decoder_code += instr.get_decode()
                self._decoder_code.append(f"}}")
            self._decoder_code.append(f"}}")
            #
    #
#
#
def get_bit(num, ind) -> int:
    return num & (1 << ind)
    #
#
def get_range(msb : int, lsb : int) -> dict:
    return dict({"msb" : msb, "lsb" : lsb})
    #
#
def ones(num) -> int:
    return (1 << num) - 1
    #
#
def get_lsb(num) -> int:
    lsb = 0

    while (num & 1) != 1:
        num = num >> 1
        lsb += 1
    #
    return lsb
    #
#
def get_switch(mask : int = 0, lsb : int = 0) -> str:
    return f"switch ((word >> {lsb}) & {bin(mask)})"
    #
#
def get_case(node : int) -> str:
    return f"case {bin(node)}:"
    #
#
def get_mask(lsb, msb) ->int:
    return ones(msb - lsb + 1) << lsb


# def get_decoder(self, tree : dict, prev_mask : int = 0) -> list:
# #
#     add_fields = True   
#     cur_range = tree["range"]
#     lsb, msb = cur_range['lsb'], cur_range['msb']
#     #
#     mask = get_mask(lsb, msb)
#     # cur_switch = get_switch(lsb, mask)
#     #
#     # if check_all_nodes(tree["nodes"]):
#     #     self._decode_code += tree["nodes"][0][0].get_fields()
#     #     add_fields = False
#     #
#     # self._decode_code.append(cur_switch)
#     #
#     self._decode_code.append(f"{{")
#     #
#     for node in tree["nodes"]:
#         cur_case = get_case(node)
#         # self._decode_code.append(cur_case)
#         # self._decode_code.append(f"{{")
#         #
#         if isinstance(tree["nodes"][node], dict):
#             self.get_decoder(tree["nodes"][node], prev_mask | mask)
#         #
#         elif isinstance(tree["nodes"][node][0], Instruction):
#             self._decode_code += tree["nodes"][node][0].get_decode(add_fields)
#         #
#         self._decode_code.append(f"}}")
#     #
#     self._decode_code.append(f"}}")