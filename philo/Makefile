
CC = cc

CFLAGS = -Wall -Wextra -Werror -g

# LIBFT_DIR = ./libft
# LIBFT_LIB = $(LIBFT_DIR)/libft.a

NAME = philo

SRC = main.c args.c args_utils.c init.c output.c time.c actions.c life_cycle.c cleanup_error.c helper.c # Add here other source files

OBJ_DIR = ./obj
OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))

all: $(NAME)

# Link object files and libft to create the executable
$(NAME): $(OBJ) # $(LIBFT_LIB)
	$(CC) $(CFLAGS) $(OBJ) -o $@
#	$(CC) $(CFLAGS) $(OBJ) -o $@ -L$(LIBFT_DIR) -lft

# Rule to build libft if it doesn't exist
# $(LIBFT_LIB): $(LIBFT_DIR)/Makefile
#	$(MAKE) -C $(LIBFT_DIR)

#The obj dir will be created if it doesn't exist
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
#	$(MAKE) -C $(LIBFT_DIR) clean
	rm -rf $(OBJ_DIR)

fclean: clean
#	$(MAKE) -C $(LIBFT_DIR) fclean
	rm -f $(NAME)
	rm -rf $(OBJ_DIR)

re: fclean all

.PHONY: all clean fclean re

