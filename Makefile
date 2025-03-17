##
## EPITECH PROJECT, 2025
## B-OOP-400-TLS-4-1-tekspice-loris.gode
## File description:
## Makefile
##

SRC = main.cpp \
	$(wildcard src/*.cpp) \
	$(wildcard src/commands/*.cpp) \

OBJ = $(SRC:.cpp=.o)

NAME = myftp

all: $(NAME)

$(NAME): $(OBJ)
		g++ -o $(NAME) $(OBJ) -std=c++20 -Wall -Wextra -Werror -g3

clean:
		rm -f $(OBJ)

fclean: clean
		rm -f $(NAME)

re: fclean all
