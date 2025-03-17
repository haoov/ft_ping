# Files
TARGET	:= ft_ping
SRCDIR	:= ./src
OBJDIR	:= ./obj
INCDIR	:= ./inc
ARGPDIR	:= ./argparser
SRCS	:= $(wildcard $(SRCDIR)/*.c)
OBJS	:= $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRCS))
ARGP	:= $(ARGPDIR)/bin/argparser.a

# Compiler
CC		:= cc
CFLAGS	:= -Wall -Wextra -Werror -g
CINC	:= -I $(INCDIR) -I $(ARGPDIR)/inc

# Rules
all: $(TARGET)

$(TARGET): $(ARGP) $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ -lm $(ARGP)

$(ARGP) :
	make -C $(ARGPDIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) $(CINC) -c $< -o $@

$(OBJDIR) :
	mkdir -p $@

clean:
	make clean -C $(ARGPDIR)
	rm -rf $(OBJDIR)

fclean: clean
	make fclean -C $(ARGPDIR)
	rm -rf $(TARGET)

re: fclean all

.PHONY: all clean fclean re
