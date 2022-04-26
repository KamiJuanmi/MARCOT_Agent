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

SRC = array.c hashtable.c utilidades.c st_disp.c cliente.c 

OBJ = $(SRC:.c=.o)

all: cliente rmobj

cliente: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

.PHONY: clean

rmobj:
	rm $(OBJ)

clean:
	rm cliente 
