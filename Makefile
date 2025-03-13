# Files
TARGET=ft_ping
SRCDIR=./src
OBJDIR=./obj
INCDIR=./inc
SRCS=$(shell find $(SRCDIR) -type f -name "*.c")
OBJS=$(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRCS))

# Compiler
CC=cc
CFLAGS=-Wall -Wextra -Werror -g
CINC=-I $(INCDIR)

# Rules
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ -lm

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) $(CINC) -c $< -o $@

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -rf $(TARGET)

re: fclean all

.PHONY: all clean fclean re
