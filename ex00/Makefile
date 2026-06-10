################################################################################
#
#	Project variables
#
################################################################################
NAME = btc

CXX = c++

CXXFLAGS = -Wall -Werror -Wextra -MMD -MP -g

INCLUDE = -I./include

SRC_DIR = src
OBJ_DIR = obj

FILES = BitcoinExchange \

OBJS = ${FILES:%=${OBJ_DIR}/%.o}
DEPS = ${FILES:%=${OBJ_DIR}/%.d}

OBJ_DIRS = ${sort ${dir ${OBJS}}}

DEFS = -D DEBUG=0

################################################################################
#
#	Project targets
#
################################################################################

all: ${NAME}

${NAME}: CXXFLAGS := ${CXXFLAGS} -std=c++98
${NAME}: ${OBJS} ${OBJ_DIR}/main.o
	${CXX} ${CXXFLAGS} $^ -o $@

${OBJ_DIR}/%.o: ${SRC_DIR}/%.cpp | ${OBJ_DIRS}
	mkdir -p obj
	${CXX} ${CXXFLAGS} ${INCLUDE} ${DEFS} -c $< -o $@

${OBJ_DIRS}:
	mkdir -p $@

-include ${DEPS}

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

run: ARGS = input.txt
run: ${NAME}
	./${NAME} ${ARGS}

valgrind: ARGS = input.txt
valgrind: ${NAME}
	valgrind \
		--leak-check=full \
		--show-leak-kinds=all \
		./${NAME} ${ARGS}

.PHONY: all clean fclean re run valgrind
