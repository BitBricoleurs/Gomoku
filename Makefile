BUILD_DIR = build
BINARY_NAME = pbrain-gomoku-ai

all: $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake .. && make
	cp $(BUILD_DIR)/$(BINARY_NAME) .

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)
	rm ./$(BINARY_NAME)
