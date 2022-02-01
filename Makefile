# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: phnguyen <phnguyen@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/12/14 01:20:15 by phnguyen          #+#    #+#              #
#    Updated: 2022/02/01 14:38:02 by phnguyen         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := gbmu

CC := clang++

CFLAGS += -Wall -Werror -Wextra
CFLAGS += -g
#CFLAGS += -fsanitize=address
#CFLAGS += -O2

SRCDIR = src/
OBJDIR = obj/
INCL = includes/

C_FILE =	main \
			Gbmu \
			Rom \
			Cpu \
			Cpu_instruc

SRC = $(addprefix $(SRCDIR), $(addsuffix .cpp, $(C_FILE)))
OBJ = $(addprefix $(OBJDIR), $(addsuffix .o, $(C_FILE)))

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

$(OBJDIR)%.o: $(SRCDIR)%.cpp $(INCL)/*
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -I$(INCL)

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY = all clean fclean re