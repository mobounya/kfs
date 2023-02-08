#pragma once

#include <stdint.h>


typedef struct __attribute__((__packed__)) page_directory_entry
{
    /*
        P, or 'Present'. If the bit is set, the page is actually in physical memory at the moment. For example, 
        when a page is swapped out, it is not in physical memory and therefore not 'Present'. If a page is called, but not present, 
        a page fault will occur, and the OS should handle it.
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
        1 bit ignored
    */
    uint32_t ignored_1 : 1;

    /*
        PS, or 'Page Size' stores the page size for that specific entry. If the bit is set, then the PDE maps to a page that is 4 MiB in size.
        Otherwise, it maps to a 4 KiB page table. Please note that 4-MiB pages require PSE to be enabled.
    */
    uint32_t page_size : 1;

    /*
        4 bits ignored
    */
    uint32_t ignored_4 : 4;

    /*
        Physical address of the page table (4K bytes alligned).
    */
    uint32_t page_table_address : 20;
} PDE;

extern page_directory_entry page_directory[1024];

void enable_paging();
page_directory_entry create_page_directory_entry(bool present, bool read_write, bool u_s, bool pwt, bool cache_disbled, bool page_size,
                                                   uint32_t address);
void insert_page_directory_entry(page_directory_entry entry);
