NAME_SOLVE   := solve.exe
NAME_MAKE_EX := make_ex.exe

CC           := gcc
CFLAGS       := -I./include

SRCS_DIR     := srcs
OBJ_DIR      := objs
LIB_DIR      := lib

SRCS_FILES   := $(wildcard $(SRCS_DIR)/*.c)
LIB_FILES    := $(wildcard $(LIB_DIR)/*.c)

OBJS_COMMON  := $(addprefix $(OBJ_DIR)/, $(notdir $(SRCS_FILES:.c=.o)))
OBJS_COMMON  += $(addprefix $(OBJ_DIR)/, $(notdir $(LIB_FILES:.c=.o)))
OBJ_SOLVE    := $(OBJ_DIR)/main.o
OBJ_MAKE_EX  := $(OBJ_DIR)/main_make_ex.o

all: $(NAME_SOLVE) $(NAME_MAKE_EX)

$(NAME_SOLVE): $(OBJS_COMMON) $(OBJ_SOLVE)
	$(CC) $(CFLAGS) $^ -o $@

$(NAME_MAKE_EX): $(OBJS_COMMON) $(OBJ_MAKE_EX)
	$(CC) $(CFLAGS) $^ -o $@

vpath %.c $(SRCS_DIR) $(LIB_DIR) .

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)
	rm -f log

fclean: clean
	rm -f $(NAME_SOLVE) $(NAME_MAKE_EX)

re: fclean all

.PHONY: all clean fclean re
