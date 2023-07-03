CC = gcc
BUILD_DIR = build
SRC_DIR = src
INCLUDE_DIR = includes
EXECUTABLE_FILE = buyer
CFLAGS = -I$(INCLUDE_DIR)

OBJECTS = \
	$(BUILD_DIR)/main.o \
	$(BUILD_DIR)/utils.o \
	$(BUILD_DIR)/commands.o \
	$(BUILD_DIR)/dashboard.o \
	$(BUILD_DIR)/server.o

all: buyer

buyer: $(BUILD_DIR) $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(EXECUTABLE_FILE)

$(BUILD_DIR):
	mkdir build

$(BUILD_DIR)/main.o: $(SRC_DIR)/main.c $(INCLUDE_DIR)/server.h $(INCLUDE_DIR)/consts.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/main.c -o $(BUILD_DIR)/main.o

$(BUILD_DIR)/server.o: $(SRC_DIR)/server.c $(INCLUDE_DIR)/server.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/server.c -o $(BUILD_DIR)/server.o

$(BUILD_DIR)/utils.o: $(SRC_DIR)/utils.c $(INCLUDE_DIR)/utils.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/utils.c -o $(BUILD_DIR)/utils.o

$(BUILD_DIR)/commands.o: $(SRC_DIR)/commands.c $(INCLUDE_DIR)/commands.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/commands.c -o $(BUILD_DIR)/commands.o

$(BUILD_DIR)/dashboard.o: $(SRC_DIR)/dashboard.c $(INCLUDE_DIR)/dashboard.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/dashboard.c -o $(BUILD_DIR)/dashboard.o

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) *.o *.out
	rm -rf buyer