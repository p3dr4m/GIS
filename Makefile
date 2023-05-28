# Thanks to Job Vranish (https://spin.atomicobject.com/2016/08/26/makefile-c-projects/)

TARGET_EXEC := GIS

BUILD_DIR := ./build
SRC_DIRS := ./src
MAIN_CPP := ./GIS.cpp
DATA_DIR := ./data

# Find all the C and C++ files we want to compile
# Note the single quotes around the * expressions. The shell will incorrectly expand these otherwise, but we want to send the * directly to the find command.
SRCS := $(shell find $(SRC_DIRS) -name '*.cpp' -or -name '*.c' -or -name '*.s')
SRCS += $(MAIN_CPP)

# Prepends BUILD_DIR and appends .o to every src file
# As an example, ./your_dir/hello.cpp turns into ./build/./your_dir/hello.cpp.o
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
#OBJS := $(patsubst $(SRC_DIRS)/%,$(BUILD_DIR)/%,$(SRCS:%=%.o))
# String substitution (suffix version without %).
# As an example, ./build/hello.cpp.o turns into ./build/hello.cpp.d
DEPS := $(OBJS:.o=.d)

# Every folder in ./src will need to be passed to GCC so that it can find header files
INC_DIRS := $(shell find $(SRC_DIRS) -type d)
# Add a prefix to INC_DIRS. So moduleA would become -ImoduleA. GCC understands this -I flag
INC_FLAGS := $(addprefix -I,$(INC_DIRS)) -std=c++11 -pthread

# The -MMD and -MP flags together generate Makefiles for us!
# These files will have .d instead of .o as the output.
CPPFLAGS := $(INC_FLAGS) -MMD -MP



# The final build step.
$(BUILD_DIR)/$(TARGET_EXEC): clean $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

# Build step for C source
$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# Build step for C++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

# copy data files to build directory
$(BUILD_DIR)/data/%: data/%
	mkdir -p $(dir $@)
	cp $< $@

.PHONY: clean
clean:
	@if [ -d $(BUILD_DIR) ]; then \
		rm -rf $(BUILD_DIR); \
		echo "Cleaned $(BUILD_DIR)"; \
	else \
		echo "$(BUILD_DIR) does not exist"; \
	fi
# Include the .d makefiles. The - at the front suppresses the errors of missing
# Makefiles. Initially, all the .d files will be missing, and we don't want those
# errors to show up.
-include $(DEPS)

# Rule for copying data directory contents to the build directory
copy_data:
	cp -r $(DATA_DIR)/* $(BUILD_DIR)

# Add the copy_data rule as a dependency for the final build target
$(BUILD_DIR)/$(TARGET_EXEC): copy_data
