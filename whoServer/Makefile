INCLUDE_DIR = ./header
SOURCE_DIR = ./source
OBJECT_DIR = ./object

CC = g++
CFLAGS=-I$(INCLUDE_DIR) -g3 -fmax-errors=10 -pthread
OUT = whoServer

_DEPS = SocketOps.hpp ServerThread.hpp
#pattern matching from  _DEPS to include directory
DEPS = $(patsubst %,$(INCLUDE_DIR)/%,$(_DEPS))

_OBJ = main.o
#same pattern matching principe
OBJ = $(patsubst %,$(OBJECT_DIR)/%,$(_OBJ))

.PHONY: clean

############goals#######################

#default goal
$(OUT): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

#general rule for all object files
$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

count:
	wc $(SOURCE_DIR)/*.cpp $(DEPS)

clean:
	rm -f $(OBJECT_DIR)/*.o ./whoServer
