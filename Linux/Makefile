CC = gcc
CFLAGS = -fPIC -Wall -O2
LDFLAGS = -shared
BUILD_DIR = build
SRC_DIR = src

# Source files
SOURCES = $(SRC_DIR)/MemoryMonitorLinux.c $(SRC_DIR)/KeyChainLinux.c

# Output .so files
TARGETS = $(BUILD_DIR)/MemoryMonitorLinux.so $(BUILD_DIR)/KeyChainLinux.so

all: $(BUILD_DIR) $(TARGETS)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/MemoryMonitorLinux.so: $(SRC_DIR)/MemoryMonitorLinux.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

$(BUILD_DIR)/KeyChainLinux.so: $(SRC_DIR)/KeyChainLinux.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

clean:
	rm -rf $(BUILD_DIR)
