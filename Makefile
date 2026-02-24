CXX = g++
CXXFLAGS = -g -std=c++20 -Wall -Iinclude

SRC_PATH = src
INC_PATH = include
BUILD_DIR = build
BIN_DIR = bin
BIN_PATH = $(BUILD_DIR)/$(BIN_DIR)

#Source files
#SRC = $(SRC_PATH)/
MAIN_SRC = $(SRC_PATH)/main.cpp

#Include files
INC = $(INC_PATH)/color_formatter.hpp $(INC_PATH)/duration_formatter.hpp $(INC_PATH)/vec_formatter.hpp

#Targets
TGT_MAIN = formatter

# Default target
all: build $(TGT_MAIN)

# Create build directories
build:
	@echo "Generating build directory '$(BIN_PATH)'..."
	@mkdir -p $(BIN_PATH)

# Link both .cpp files into one executable
$(TGT_MAIN): $(MAIN_SRC) $(SRC) $(INC)
	@echo "Building main program"
	$(CXX) $(CXXFLAGS) -I$(INC_PATH) $(MAIN_SRC) $(SRC) -o $(BIN_PATH)/$@
	@echo "Built $(BIN_PATH)/$@"

#Run formatter
run: $(TGT_MAIN)
	@echo " "
	@echo "Runnning formatter.."
	@$(BIN_PATH)/$(TGT_MAIN)

# Clean up
clean:
	@echo "Cleaning build directory"
	rm -rf $(BUILD_DIR)
	@echo "Clean complete"
