CC      := gcc
CFLAGS  := -std=c11 -O2 -g -Wall -Wextra -Wpedantic
LDLIBS  := -lraylib -lopengl32 -lgdi32 -lwinmm

BUILD_DIR := build

all: $(BUILD_DIR)/nbody.exe

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Main nbody target
$(BUILD_DIR)/nbody.exe: src/main.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@ $(LDLIBS)

# 2D experiment target
2d: $(BUILD_DIR)/2d.exe
	./$(BUILD_DIR)/2d.exe

$(BUILD_DIR)/2d.exe: render/2d.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@ $(LDLIBS)

# 3D experiment target
3d: $(BUILD_DIR)/3d.exe
	./$(BUILD_DIR)/3d.exe

$(BUILD_DIR)/3d.exe: render/3d.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@ $(LDLIBS)

run: all
	./$(BUILD_DIR)/nbody.exe

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all 2d 3d run clean
