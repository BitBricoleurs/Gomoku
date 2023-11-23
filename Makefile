BUILD_DIR = build
BINARY_NAME = pbrain-gomoku-ai
TEST_BINARY_NAME = gomoku_tests

all: $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake -DBUILD_TESTING=OFF .. && make
	cp $(BUILD_DIR)/$(BINARY_NAME) .

release: $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=OFF .. && make
	cp $(BUILD_DIR)/$(BINARY_NAME) .

test: $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake -DBUILD_TESTING=ON .. && make $(TEST_BINARY_NAME)
	cd $(BUILD_DIR) && ctest -V

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)
	rm -f ./$(BINARY_NAME)
	rm -f ./$(TEST_BINARY_NAME)
