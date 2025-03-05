# Files
TARGET=ft_ping
SRCDIR=./src
OBJDIR=./obj
SRCS=$(shell find $(SRCDIR) -type f -name "*.c")
OBJS=$(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRCS))

# Compiler
CC=cc
CFLAGS=-Wall -Wextra -Werror -g

# Rules
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -rf $(TARGET)

re: fclean all

.PHONY: all clean fclean re
