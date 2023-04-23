NAME = kfs.bin

ISO = kfs.iso

LINKER_SCRIPT = linker.ld

TARGET = i686-elf

COMPILER_FLAGS = -I./ -I./Kernel/Klibcpp -I./Kernel/Klibc -O2 -ffreestanding -Wall -Wextra -fno-exceptions -fno-rtti -nostdlib -nodefaultlibs -fno-stack-protector

LINKER_FLAGS = -ffreestanding -nostdlib

SIGNALS_FILES = $(addprefix Signals/, Signals)
KLIBCPP = $(addprefix Klibcpp/, tohex)
DEBUG_FILES = $(addprefix Debug/, Debug)
DEVICES_FILES = $(addprefix Devices/, Keyboard) 
KLIBC = $(addprefix Klibc/, isalpha islower isupper tolower toupper strcpy itoa memcpy strcmp strlen memset strndup strcat strdup)
CPU_FILES = $(addprefix CPU/, CPU)
GDT_FILES = $(addprefix GDT/, GDT TSS)
INTERUPTS_FILES = $(addprefix Interrupts/, IDT PIC InterruptHandlers)
MEMORY_FILES = $(addprefix Memory/, QuickDirtyMalloc PhysicalMemoryManager PagingStructureEntry MemoryRegion PageDirectory PageTable MemoryPage VirtualMemoryManager KernelVirtualMemoryManager UserVirtualMemoryManager Address VirtualAddress PhysicalAddress)
VGA_FILES = $(addprefix Display/VGA/, VGA)
SCREEN_FILES = $(addprefix Display/, Screen)
KERNEL_FILES = $(addprefix Kernel/, Kernel $(MEMORY_FILES) $(VGA_FILES) $(INTERUPTS_FILES) $(GDT_FILES) $(CPU_FILES) $(KLIBC) $(DEVICES_FILES) $(SCREEN_FILES) $(DEBUG_FILES) $(KLIBCPP) $(SIGNALS_FILES))

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
