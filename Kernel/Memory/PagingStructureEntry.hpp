#pragma once

#include <stdint.h>

namespace Memory
{
    typedef struct __attribute__((__packed__)) paging_structure_entry
    {
        /*
            Present; must be 1 to reference a page table
        */
        uint32_t present : 1;

        /*
            'Read/Write' permissions flag. If the bit is set, the page is read/write. Otherwise when it is not set, the page is read-only. 
            The WP bit in CR0 determines if this is only applied to userland, always giving the kernel write access (the default) or both userland and the kernel.
        */
        uint32_t read_write : 1;

        /*
            'User/Supervisor' bit, controls access to the page based on privilege level. If the bit is set, then the page may be accessed by all; if the bit is not set, however, 
            only the supervisor can access it. For a page directory entry, the user bit controls access to all the pages referenced by the page directory entry.
            Therefore if you wish to make a page a user page, you must set the user bit in the relevant page directory entry as well as the page table entry
        */
        uint32_t u_s : 1;

        /*
            PWT, controls Write-Through' abilities of the page. If the bit is set, write-through caching is enabled. If not, then write-back is enabled instead.
        */
        uint32_t pwt : 1;

        /*
            PCD, is the 'Cache Disable' bit. If the bit is set, the page will not be cached. Otherwise, it will be.
        */
        uint32_t cache_disbled : 1;

        /*
        'Accessed' is used to discover whether a PDE or PTE was read during virtual address translation. If it has, then the bit is set, otherwise, it is not. Note that,
        this bit will not be cleared by the CPU, so that burden falls on the OS (if it needs this bit at all).
        */
        uint32_t accessed : 1;

        /*
            Dirty; indicates whether software has written to the 4-KByte page referenced by this entry.
            Should be ignored if this entry is a page directory entry.
        */
        uint32_t dirty : 1;

        /*
            if this entry is a page directory:
            - PS, or 'Page Size' stores the page size for that specific entry. If the bit is set, then the PDE maps to a page that is 4 MiB in size.
            Otherwise, it maps to a 4 KiB page table. Please note that 4-MiB pages require PSE to be enabled.
            
            else of this entry is a page table:
            - If the PAT is supported, indirectly determines the memory type used to access the 4-KByte page referenced by this
            entry ; otherwise, reserved (must be 0)
        */
        uint32_t bit_7 : 1;

        /*
            Global; if CR4.PGE = 1, determines whether the translation is global (see Section 4.10); ignored otherwise
            Should be ignored if this entry is a page directory entry.
        */
        uint32_t global : 1;

        /*
            3 bits ignored.
        */
        uint32_t ignored : 3;

        /*
            Physical address of the page table (4K bytes alligned).
        */
        uint32_t page_table_address : 20;

        paging_structure_entry *set_present(void);
        paging_structure_entry *set_read_write(void);
        paging_structure_entry *set_u_s(void);
        paging_structure_entry *set_pwt(void);
        paging_structure_entry *set_cache_disbled(void);
        paging_structure_entry *set_page_size(void);
        paging_structure_entry *set_pat(void);
        paging_structure_entry *set_global(void);
        paging_structure_entry *set_physical_address(uint32_t physical_address);
    } PSE;

    /*
        Use the corresponding type when makking a new entry, they're both the same,
        it is just for readability purposes.
    */
    typedef PSE page_directory_entry;
    typedef PSE page_table_entry;
}