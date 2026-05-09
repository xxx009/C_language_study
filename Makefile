CC := gcc
CFLAGS := -Iinclude -Wall -Wextra -Werror -std=c11
TARGET := subway_planner

SRC := \
	src/subway/main.c \
	src/subway/subway_core.c \
	src/subway/subway_operations.c \
	src/subway/subway_transfer_budget.c \
	src/subway/subway_routing.c \
	src/subway/subway_io_ui.c \
	src/subway/subway_economics.c

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
