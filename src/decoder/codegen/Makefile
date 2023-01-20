.PHONY: default clean

SIM_DIR=$(dir $(abspath $(dir $(abspath $(dir $(PWD))))))

RISC_V_DIR=${SIM_DIR}3rd_party/riscv-opcodes

.PRECIOUS: yaml_gen

yaml_gen:
	cd ${RISC_V_DIR} && make EXTENSIONS='rv_i rv_m rv_zicsr rv32_i'

all: yaml_gen
	python3 main.py --dir ${RISC_V_DIR}