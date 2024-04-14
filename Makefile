# write by lnfromnorth

SRC_DIR=./src
BUILD_DIR=./build
LIB_DIR=./lib
BIN_DIR=./bin
TEST_DIR=./tests
EXAMPLES_DIR=./examples
INCLUDE_DIR=./include

SRCS := $(shell find $(SRC_DIR) -name '*.cc')
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

TESTS := $(shell find $(TEST_DIR) -name '*.cc')
TEST_EXE := $(TESTS:%=$(BIN_DIR)/%.out)

EXAMPLES := $(shell find $(EXAMPLES_DIR) -name '*.cc')
EXAMPLES_EXE := $(EXAMPLES:%=$(BIN_DIR)/%.out)

CPPFLAGS := -g -Wall -I $(INCLUDE_DIR)

test: $(TEST_EXE) $(EXAMPLES_EXE)
	@echo "===== compile success ====="

$(BIN_DIR)/%.cc.out: %.cc $(OBJS)
	@echo "===== CXX BIN COAP ====="
	$(CXX) $(CPPFLAGS) $(OBJS) $< -o $@

$(BUILD_DIR)/%.cc.o: %.cc
	@echo "===== CXX LIB COAP ====="
	$(CXX) $(CPPFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -rf $(BIN_DIR)/tests/*
	rm -rf $(BIN_DIR)/examples/*
	rm ./logfile.txt