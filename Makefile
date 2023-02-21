NAME = kfs.bin

ISO = kfs.iso

LINKER_SCRIPT = linker.ld

TARGET = i686-elf

COMPILER_FLAGS = -I./ -ffreestanding -Wall -Wextra -fno-exceptions -fno-rtti -nostdlib -nodefaultlibs -fno-stack-protector

LINKER_FLAGS = -ffreestanding -nostdlib

LIBC_FILES = $(addprefix User/Libc/, Libc)
LIBC_SRC = $(addsuffix .cpp, $(LIBC_FILES))
LIBC_OBJ = $(addsuffix .o, $(LIBC_FILES))

KERNEL_FILES = $(addprefix Kernel/, Kernel VGA/VGA Memory/PhysicalMemoryManager Memory/PagingStructureEntry Memory/MemoryRegion Memory/PageDirectory Memory/PageTable Memory/MemoryPage Memory/VirtualMemoryManager)
KERNEL_SRC = $(addsuffix .cpp, $(KERNEL_FILES))
KERNEL_OBJ = $(addsuffix .o, $(KERNEL_FILES))

ASM_FILES = $(addprefix Kernel/, Boot)
ASM_SRC = $(addsuffix .s, $(ASM_FILES))
ASM_OBJ = $(addsuffix .o, $(ASM_FILES))

CPP_FILES = $(LIBC_FILES) $(KERNEL_FILES)
CPP_SRC = $(LIBC_SRC) $(KERNEL_SRC)
CPP_OBJ = $(LIBC_OBJ) $(KERNEL_OBJ)

RED = \033[1;31m
GREEN = \033[1;32m
NC = \033[1;0m

all : $(NAME)

$(NAME) : $(ASM_OBJ) $(CPP_OBJ)
	@echo "$(RED)Linking kernel and boot files...$(NC)"
	@$(TARGET)-g++ -T $(LINKER_SCRIPT) $(LINKER_FLAGS) -o $@ $(ASM_OBJ) $(CPP_OBJ) -lgcc
	@echo "$(GREEN)Done linking kernel and boot files...$(NC)"

$(ASM_OBJ) : $(ASM_SRC)
	@echo "$(RED)Making boot files...$(NC)"
	@$(TARGET)-as $*.s -o $@
	@echo "$(GREEN)Done making boot files$(NC)"

$(CPP_OBJ) : $(CPP_SRC)
	@echo "$(RED)Compiling kernel files...$(NC)"
	@$(TARGET)-g++ $(COMPILER_FLAGS) -c $*.cpp -o $@
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
	rm -f $(CPP_OBJ) $(ASM_OBJ)

fclean: clean
	rm -f $(NAME)

.phony:
	clean fclean iso-clean iso
