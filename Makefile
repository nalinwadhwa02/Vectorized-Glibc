CC = clang

CUSTOMCODEBLOCKS_FLAGS = -mavx2 -mlzcnt -O3
COMPILEROPTIMIZED_FLAGS = -O3 -mavx2
BENCHMARK_FLAGS = -g -mavx2

BENCHMARKFILES_DIR ?= benchmark-files
CUSTOMBUILDS_DIR ?= builds/custom
COMPILERBUILDS_DIR ?= builds/compiler
OBJ_DIR ?= objects
CUSTOMCODEBLOCKS_DIR ?= custom-codeblocks
COMPILEROPTIMIZEDBLOCKS_DIR ?= compiler-optimized

COMPILEROPTIMIZED_FILES := $(wildcard $(COMPILEROPTIMIZEDBLOCKS_DIR)/*.h)

CUSTOMCODEBLOCKS_FILES := $(wildcard $(CUSTOMCODEBLOCKS_DIR)/*.h)

CUSTOMCODEBLOCKS_BUILDS := $(patsubst $(CUSTOMCODEBLOCKS_DIR)/%.h,$(CUSTOMBUILDS_DIR)/%.o,$(CUSTOMCODEBLOCKS_FILES))

COMPILEROPTIMIZED_BUILDS := $(patsubst $(COMPILEROPTIMIZEDBLOCKS_DIR)/%.h,$(COMPILERBUILDS_DIR)/%.o,$(COMPILEROPTIMIZED_FILES))

buildcustom: $(CUSTOMCODEBLOCKS_BUILDS)

buildcompiler: $(COMPILEROPTIMIZED_BUILDS)

$(CUSTOMBUILDS_DIR)/%.o : $(CUSTOMCODEBLOCKS_DIR)/%.h
	$(CC) $(CUSTOMCODEBLOCKS_FLAGS) -c $< -o $@

$(COMPILERBUILDS_DIR)/%.o : $(COMPILEROPTIMIZEDBLOCKS_DIR)/%.h
	$(CC) $(COMPILEROPTIMIZED_FLAGS) -c $< -o $@

$(OBJ_DIR)/%.out : $(BENCHMARKFILES_DIR)/%.c
	$(CC) $(BENCHMARK_FLAGS) $(wildcard $(BUILDS_DIR)/*.o) $< -o $@

clean:
	rm -rf $(CUSTOMBUILDS_DIR)/*.o
	rm -rf $(COMPILERBUILDS_DIR)/*.o
	rm -rf $(OBJ_DIR)/*.out
