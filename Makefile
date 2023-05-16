##
## EPITECH PROJECT, 2023
## B-PSU-400-LYN-4-1-ftrace-adam.balfet
## File description:
## Makefile
##

SRC_PATH = src
BUILD_PATH = ./obj
COMPILER = g++
FILE_TYPE = .cpp
SECTION_NAME = ECS

SRC =\
	${SRC_PATH}/scene.cpp\

INCLUDE =\
	-I./include/\

INDEX = ${words ${shell a="${2}" ; echo $${a/${1}*/${1}} }}
SRC_COUNT = ${shell echo ${SRC} | wc -w}
OBJ = ${SRC:${SRC_PATH}/%${FILE_TYPE}=${BUILD_PATH}/%.o}
OBJ_FLAGS = -g3 -O3 -W -Wall -Wextra -Werror -std=c++20 ${INCLUDE}
BIN_FLAGS =
BIN_NAME = ecs.so

all: ${BIN_NAME}

${BUILD_PATH}/%.o: ${SRC_PATH}/%${FILE_TYPE}
	@mkdir -p ${dir $@}
	@echo "[${SECTION_NAME}] [${call INDEX, "$<", ${SRC}}/${SRC_COUNT}] $<"
	@${COMPILER} -MD -fPIC ${OBJ_FLAGS} -c $< -o $@

${BIN_NAME}: ${OBJ}
	@echo "[${SECTION_NAME}] final build: ${BIN_NAME}"
	@${COMPILER} -shared -o ${BIN_NAME} ${OBJ} ${BIN_FLAGS}

clean:
	@echo "[${SECTION_NAME}] remove: ${BUILD_PATH}"
	@rm -rf ${BUILD_PATH}

fclean: clean
	@echo "[${SECTION_NAME}] remove: ${BIN_NAME}"
	@rm -rf ${BIN_NAME}

re: fclean all

-include ${OBJ:%.o=%.d}

.PHONY: all clean fclean re
