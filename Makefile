DEBUG_BUILD = -g
INDIGO_ROOT = ..
BUILD_ROOT = $(INDIGO_ROOT)/build
BUILD_LIB = $(BUILD_ROOT)/lib

DEBUG_ENABLED = false

ifeq ($(OS),Windows_NT)
	OS_DETECTED = Windows
else
	OS_DETECTED = $(shell uname -s)
	ifeq ($(OS_DETECTED),Darwin)
		ifeq (DEBUG_ENABLED,true)
			CFLAGS = $(DEBUG_BUILD) -O3 -I$(INDIGO_ROOT)/indigo_libs -I$(INDIGO_ROOT)/indigo_libs -std=gnu11 -DINDIGO_MACOS
		else
			CFLAGS = -O3 -I$(INDIGO_ROOT)/indigo_libs -I$(INDIGO_ROOT)/indigo_libs -std=gnu11 -DINDIGO_MACOS
		endif
		LDFLAGS = -L$(BUILD_LIB) -lindigo
	endif
	ifeq ($(OS_DETECTED),Linux)
		ifeq (DEBUG_ENABLED,true)
			CFLAGS = $(DEBUG_BUILD) -O3 -I$(INDIGO_ROOT)/indigo_libs -I$(INDIGO_ROOT)/indigo_libs -std=gnu11 -DINDIGO_LINUX
		else
			CFLAGS = -O3 -I$(INDIGO_ROOT)/indigo_libs -I$(INDIGO_ROOT)/indigo_libs -std=gnu11 -DINDIGO_LINUX
		endif
		LDFLAGS = -L$(BUILD_LIB) -lindigo
	endif
endif

CFLAGS += $(shell pkg-config --cflags json-c)
LDFLAGS += $(shell pkg-config --libs json-c)

# https://stackoverflow.com/questions/30573481/how-to-write-a-makefile-with-separate-source-and-header-directories

SRC_DIR := ./src
OBJ_DIR := ./obj
BIN_DIR := ./bin

SRC := $(filter-out $(SRC_DIR)/agent_main.c, $(wildcard $(SRC_DIR)/*.c))
OBJ := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC))
EXE := $(BIN_DIR)/cliente

SRC_2 := $(filter-out $(SRC_DIR)/client_main.c, $(wildcard $(SRC_DIR)/*.c))
OBJ_2 := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_2))
EXE_2 := $(BIN_DIR)/agent

# A la hora de lanzar el agente se debe de lanzar con la orden indigo_server -p 8008 -i ./bin/agent
agent: $(EXE_2)
# A la hora de ejecutar el cliente se ha de lanzar ./bin/cliente
cliente: $(EXE)

$(EXE): $(OBJ) | $(BIN_DIR)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

$(EXE_2): $(OBJ_2) | $(BIN_DIR)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $@

$(BIN_DIR):
	mkdir -p $@

clean:
	@$(RM) -rv $(BIN_DIR) $(OBJ_DIR)

.PHONY: cliente agent clean