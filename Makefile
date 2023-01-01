NAME = kfs.bin

ISO = kfs.iso

LINKER_SCRIPT = linker.ld

TARGET = i686-elf

COMPILER_FLAGS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra

LINKER_FLAGS = -ffreestanding -O2 -nostdlib

KERNEL_FILES = kernel

KERNEL_OBJECTS = $(addsuffix .o, $(KERNEL_FILES))

BOOT_FILES = boot

BOOT_OBJECTS = $(addsuffix .o, $(BOOT_FILES))

RED = \033[1;31m
GREEN = \033[1;32m
NC = \033[1;0m

all : $(NAME)

$(NAME) : $(BOOT_FILES) $(KERNEL_FILES)
	@echo "$(RED)Linking kernel and boot files...$(NC)"
	@$(TARGET)-gcc -T $(LINKER_SCRIPT) -o $@ $(LINKER_FLAGS) $(BOOT_OBJECTS) $(KERNEL_OBJECTS) -lgcc
	@echo "$(GREEN)Done linking kernel and boot files...$(NC)"

$(BOOT_FILES) : % : %.s
	@echo "$(RED)Making boot files...$(NC)"
	@$(TARGET)-as $< -o $@.o
	@echo "$(GREEN)Done making boot files$(NC)"

$(KERNEL_FILES) : % : %.c
	@echo "$(RED)Compiling kernel files...$(NC)"
	@$(TARGET)-gcc -c $< -o $@.o $(COMPILER_FLAGS)
	@echo "$(GREEN)Done compiling kernel files$(NC)"

iso: grub.cfg all
	@echo "$(RED)Creating bootable iso file...$(NC)"
	@mkdir -p isodir/boot/grub
	@cp $(NAME) isodir/boot/$(NAME)
	@cp grub.cfg isodir/boot/grub/grub.cfg
	@grub-mkrescue --compress=xz -o $(ISO) isodir
	@echo "$(GREEN)Done creating bootable iso file$(NC)"

iso-clean:
	rm -rf isodir/
	rm -f $(ISO)

clean:
	rm -f $(KERNEL_OBJECTS) $(BOOT_OBJECTS)

fclean: clean
	rm -f $(NAME)

.phony:
	clean fclean
