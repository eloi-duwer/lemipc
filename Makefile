SRC_NAME = main.c \
	helpers.c \
	print_helpers.c \
	shm_offsets.c \
	board_control.c \
	msq_listening.c \
	play_helpers.c \
	send_game_ended_msg.c

NAME = lemipc

CC = clang

CFLAGS = -Wall -Wextra -I./include -I./libft/include -g

SRCF = ./src/

OBJF = ./obj/

OBJS = $(addprefix $(OBJF), $(SRC_NAME:.c=.o))

LFT = libft/libft.a


all: $(NAME)

$(LFT):
	make -C ./libft

$(NAME): $(LFT) $(OBJS)
	$(CC) -o $(NAME) $(OBJS) -Wall -Wextra -L./libft -lft -lrt -pthread -g

$(OBJF)%.o: $(SRCF)%.c
	@mkdir -p $(@D)
	$(CC) -o $@ $(CFLAGS) -c $(addprefix $(SRCF), $*.c)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all
