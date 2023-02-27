NAME = ft_libc.a

TARGET = i686-elf

COMPILER_FLAGS = -I./ -Wall -Wextra -Werror -ffreestanding -nostdlib -nodefaultlibs

C_SOURCES = itoa.c memcpy.c strcmp.c strlen.c

C_OBJ = $(C_SOURCES:.c=.o)

HEADERS = string.h

all: $(NAME)

$(NAME): $(C_OBJ) $(HEADERS)
	ar rc $(NAME) $(C_OBJ)

%.o : %.c $(HEADERS)
	$(TARGET)-gcc $(COMPILER_FLAGS) -c $< -o $@

clean:
	rm -f $(C_OBJ)

fclean: clean
	rm -f $(NAME)

.phony:
	clean fclean
