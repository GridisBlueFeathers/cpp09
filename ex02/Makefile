################################################################################
#
#	Project variables
#
################################################################################
NAME = PmergeMe

CXX = c++

CXXFLAGS = -Wall -Werror -Wextra -MMD -MP -g

INCLUDE = -I./include

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

FILES = PmergeMe \

OBJS = ${FILES:%=${OBJ_DIR}/%.o}
DEPS = ${FILES:%=${OBJ_DIR}/%.d}

TESTS = ${FILES:%=${BIN_DIR}/%}
TEST_OBJS = ${FILES:%=${OBJ_DIR}/test%.o}

OBJ_DIRS = ${sort ${dir ${OBJS}}}
TEST_DIRS = ${sort ${dir ${TESTS}}}

DEFS = -D DEBUG=0

################################################################################
#
#	Project targets
#
################################################################################

all: ${NAME}

${NAME}: ${OBJS} ${OBJ_DIR}/main.o
	${CXX} ${CXXFLAGS} $^ -o $@

${OBJ_DIR}/%.o: ${SRC_DIR}/%.cpp | ${OBJ_DIRS}
	mkdir -p obj
	bear --append -- ${CXX} ${CXXFLAGS} ${INCLUDE} ${DEFS} -c $< -o $@

${OBJ_DIRS}:
	mkdir -p $@

-include ${DEPS}

################################################################################
#
#	Tests targets
#
################################################################################

tests: bin/allTests
	valgrind ./bin/allTests

bin/allTests: ${TEST_OBJS} ${OBJS}
	mkdir -p bin
	${CXX} ${CXXFLAGS} $^ -o bin/allTests \
		-I/usr/src/googletest/googletest/include \
		-L/usr/lib \
		-lgtest -lgtest_main -pthread

test%: ${BIN_DIR}/%
	valgrind ./$<

${BIN_DIR}/%: ${OBJ_DIR}/test%.o ${OBJS} | ${TEST_DIRS}
	${CXX} ${CXXFLAGS} $^ -o $@ \
		-I/usr/src/googletest/googletest/include \
		-L/usr/lib \
		-lgtest -lgtest_main -pthread

${TEST_DIRS}:
	mkdir -p $@

################################################################################
#
#	Project rules
#
################################################################################

clean:
	rm -rf ${OBJ_DIR}
	rm -rf compile_commands.json
	rm -rf .cache

fclean: clean
	rm -rf ${NAME}
	rm -rf ${BIN_DIR}

re: fclean all

run: ARGS=9 3 5 6 10 5 8 7 1
run: ${NAME}
	./${NAME} ${ARGS}

valgrind: ARGS=9 3 5 6 10 5 8 7 1
valgrind: ${NAME}
	valgrind \
		--leak-check=full \
		--show-leak-kinds=all \
		./${NAME} ${ARGS}

.PHONY: all clean fclean re run valgrind
