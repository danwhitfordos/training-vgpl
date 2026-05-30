CFLAGS += -Wall -Wpedantic -Werror -Og -MMD

TEST_SRCS := $(wildcard *_test.c)
TEST_BINS := $(TEST_SRCS:.c=)

parser_test: lexer.o
compiler_test: lexer.o parser.o

%_test: %_test.c %.o
	$(CC) $(CFLAGS) $^ -o $@

test: $(TEST_BINS)
	@echo "--- Running tests ---"
	@for bin in $(TEST_BINS); do \
		echo "$$bin Running"; \
		./$$bin || exit 1; \
		echo "$$bin OK"; \
	done
	@echo "--- All tests OK---"


memtest: $(TEST_BINS)
	@echo "--- Running tests with valgrind ---"
	@for bin in $(TEST_BINS); do \
		echo "$$bin Running"; \
		valgrind --leak-check=full --error-exitcode=1 ./$$bin || exit 1; \
		echo "$$bin OK"; \
	done
	@echo "--- All tests OK---"
