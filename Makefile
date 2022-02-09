# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: phnguyen <phnguyen@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/12/14 01:20:15 by phnguyen          #+#    #+#              #
#    Updated: 2022/02/03 13:05:10 by phnguyen         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := gbmu

CC := clang++
GTKLIB := gtk-build/

CFLAGS += -Wall -Werror -Wextra
#CFLAGS += -g
CFLAGS += -fsanitize=address
#CFLAGS += -O2

SRCDIR = src/
OBJDIR = obj/
INCL = includes/

C_FILE =	Rom \
			Timer \
			Cpu_instruc Cpu_fetch Cpu_utils Cpu \
			Ppu \
			Lcd \
			Bus \
			Gbmu \
			Ui \
			main

SRC = $(addprefix $(SRCDIR), $(addsuffix .cpp, $(C_FILE)))
OBJ = $(addprefix $(OBJDIR), $(addsuffix .o, $(C_FILE)))

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) -lsfml-graphics -lsfml-window -lsfml-system

$(OBJDIR)%.o: $(SRCDIR)%.cpp $(INCL)/*
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -I$(INCL)

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY = all clean fclean re