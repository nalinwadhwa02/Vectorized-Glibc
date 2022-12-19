CC = clang
CB_FLAGS = -mavx2 -mlzcnt -O3
TEST_FLAGS = -g -mavx2 -v -O3 -mlzcnt

CB_DIR ?= major-codeblocks
BLD_DIR ?= bld

CB_FILES := $(wildcard $(CB_DIR)/*.h)
CB_BLDS := $(patsubst $(CB_DIR)/%.h, $(BLD_DIR)/%.o, $(CB_FILES))

build: $(CB_BLDS)

$(BLD_DIR)/%.o : $(CB_DIR)/%.h
	$(CC) $(CB_FLAGS) -c $< -o $@

test: testmain.c
	$(CC) $(TEST_FLAGS) $(wildcard $(BLD_DIR)/*.o) $< -o test.out

clean:
	rm -rf $(CUSTOMBUILDS_DIR)/*.o
	rm -rf $(COMPILERBUILDS_DIR)/*.o
	rm -rf $(OBJ_DIR)/*.out

