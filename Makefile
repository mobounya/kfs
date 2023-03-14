NAME = kfs.bin

ISO = kfs.iso

LINKER_SCRIPT = linker.ld

TARGET = i686-elf

LIBC = ./User/Libc/ft_libc.a

COMPILER_FLAGS = -I./ -I./User/Libc -O2 -ffreestanding -Wall -Wextra -fno-exceptions -fno-rtti -nostdlib -nodefaultlibs -fno-stack-protector -mgeneral-regs-only

LINKER_FLAGS = -ffreestanding -nostdlib

GDT_FILES = $(addprefix GDT/, GDT)
INTERUPTS_FILES = $(addprefix Interrupts/, IDT PIC)
MEMORY_FILES = $(addprefix Memory/, PhysicalMemoryManager PagingStructureEntry MemoryRegion PageDirectory PageTable MemoryPage VirtualMemoryManager KernelVirtualMemoryManager UserVirtualMemoryManager)
VGA_FILES = $(addprefix VGA/, VGA)
KERNEL_FILES = $(addprefix Kernel/, Kernel $(MEMORY_FILES) $(VGA_FILES) $(INTERUPTS_FILES) $(GDT_FILES))
KERNEL_SRC = $(addsuffix .cpp, $(KERNEL_FILES))
KERNEL_OBJ = $(addsuffix .o, $(KERNEL_FILES))

ASM_FILES = $(addprefix Kernel/, Boot)
ASM_SRC = $(addsuffix .s, $(ASM_FILES))
ASM_OBJ = $(addsuffix .o, $(ASM_FILES))

CPP_FILES = $(KERNEL_FILES)
CPP_SRC = $(KERNEL_SRC)
CPP_OBJ = $(KERNEL_OBJ)

RED = \033[1;31m
GREEN = \033[1;32m
NC = \033[1;0m

all : $(NAME)

$(NAME) : $(ASM_OBJ) $(CPP_OBJ)
	@echo "$(RED)Linking kernel and boot files...$(NC)"
	@make -C ./User/Libc
	@$(TARGET)-g++ -T $(LINKER_SCRIPT) $(LINKER_FLAGS) -o $@ $(ASM_OBJ) $(CPP_OBJ) $(LIBC) -lgcc
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

libc:
	make -C ./User/Libc

libc-fclean:
	make fclean -C ./User/Libc

libc-clean:
	make clean -C ./User/Libc

iso-clean:
	rm -rf isodir/
	rm -f $(ISO)

clean:
	rm -f $(CPP_OBJ) $(ASM_OBJ)

fclean: clean
	rm -f $(NAME)

.phony:
	clean fclean iso-clean iso
