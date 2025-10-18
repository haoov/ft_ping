TARGET:=ft_ping

SRC:=$(wildcard ./src/*.c)
INC:=$(wildcard ./inc/*.h)

all: ft_ping

ft_ping: $(SRC)
	gcc -Wall -Wextra -Werror -I ./inc $^ -o $(TARGET) -lm

clean:
	rm $(TARGET) 

fclean: clean

re: fclean all

.PHONY: all clean fclean re
