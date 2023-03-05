## General resources for OS development

* [Programming a kernel in c++](https://wiki.osdev.org/C%2B%2B)
* [Code Tour: Serenity OS startup (from boot to GUI)](https://youtu.be/NpcGMuI7hxk)

# [KFS_1](https://cdn.intra.42.fr/pdf/pdf/60900/en.subject.pdf)

Check this link on how to boot a basic kernel [osdev wiki](https://wiki.osdev.org/Bare_Bones).

### Linking

For linking and writing ld scripts, checkout these resources:
* https://www.math.utah.edu/docs/info/ld_3.html
* https://youtu.be/B7oKdUvRhQQ Bare metal embedded lecture-4: Writing linker scripts and section placement
* https://youtu.be/dOfucXtyEsU ppCon 2018: Matt Godbolt “The Bits Between the Bits: How We Get to main()”

### Multiboot specification

[GNU Multiboot specification](https://www.gnu.org/software/grub/manual/multiboot/multiboot.html)

### VGA text mode

Write to screen using the VGA text mode:
* https://en.wikipedia.org/wiki/VGA_text_mode
* https://en.wikipedia.org/wiki/List_of_8-bit_computer_hardware_graphics#IBM_PC/XT_and_compatible_systems

# [KFS_2](https://cdn.intra.42.fr/pdf/pdf/64575/en.subject.pdf)
This module is about segmentation, which is obsolete, so we set up a flat-memory model and ignore it.

Too lazy to include resource for segmentation now, will come back later.

# [KFS_3](https://cdn.intra.42.fr/pdf/pdf/64576/en.subject.pdf)

kfs-3 is all about memory and memory managment, this module will introduce you to virtual memory and paging, keep in mind that our implementation of memory as of writing this is at the earliest stage and still need to be improved as the system grows, but for now it does the most basic thing which is to allocate memory, both virtual and physical.

### Detecting memory from GRUB
* https://wiki.osdev.org/Detecting_Memory_(x86)
* https://www.gnu.org/software/grub/manual/multiboot/multiboot.html

### Virtual memory and paging
* https://wiki.osdev.org/Paging
* Paging section in the [Intel® 64 and IA-32 Architectures Developer's Manual: Vol. 3A](https://www.intel.com/content/www/us/en/architecture-and-technology/64-ia-32-architectures-software-developer-vol-3a-part-1-manual.html)
* [Casey cole operating system playlist](https://www.youtube.com/playlist?list=PLWCT05ePsnGww5psXWHRLG7p30eKKt1Pd) first 8 lectures.
* [David Black-Schaffer Virtual Memory playlist](https://www.youtube.com/playlist?list=PLiwt1iVUib9s2Uo5BeYmwkDFUh70fJPxX)

### Additional resources
* [Code Tour: Kernel virtual memory system in SerenityOS](https://www.youtube.com/watch?v=3xgOybGlYes&t=2089s)
* https://wiki.osdev.org/Page_Frame_Allocation
