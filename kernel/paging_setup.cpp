#include "paging_setup.hpp"

/*
    Every paging structure is 4096 Bytes in size and comprises a number of individual entries. With 32-bit paging,
    each entry is 32 bits (4 bytes); there are thus 1024 entries in each structure.
    Note: page_directory_entry size is 4 bytes.
*/
paging_structure_entry page_directory[1024];
uint16_t index = 0;

/*
    This will enable 32-bit paging without PAE.
*/
void enable_paging()
{
    asm(
        "mov %cr0, %eax\n"
        "or $0x80000000, %eax\n"
        "mov %eax, %cr0"
    );
}

void load_page_directory(void *page_directory)
{
    asm volatile (
        "movl %0, %%eax\n"
        "movl %%eax, %%cr3\n"
        :: "r" (page_directory) : "%eax"
    );
}

paging_structure_entry create_page_directory_entry(bool present, bool read_write, bool u_s, bool pwt, bool cache_disbled, bool page_size,
                                                   uint32_t address)
{
    paging_structure_entry entry;

    entry.present = present;
    entry.read_write = read_write;
    entry.u_s = u_s;
    entry.pwt = pwt;
    entry.cache_disbled = cache_disbled;
    entry.bit_7 = page_size;

    // Address should be (4K bytes alligned) so we don't need the last 12 bits.
    address = (address >> 12);
    entry.page_table_address = address;
    return entry;
}

paging_structure_entry create_page_table_entry(bool present, bool read_write, bool u_s, bool pwt, bool cache_disbled, bool pat,
                                            bool global, uint32_t address)
{
    paging_structure_entry entry;

    entry.present = present;
    entry.read_write = read_write;
    entry.u_s = u_s;
    entry.pwt = pwt;
    entry.cache_disbled = cache_disbled;
    entry.bit_7 = pat;
    entry.global = global;

    // Address should be (4K bytes alligned) so we don't need the last 12 bits.
    address = (address >> 12);
    entry.page_table_address = address;
    return entry;
}

void insert_page_directory_entry(paging_structure_entry entry)
{
    page_directory[index] = entry;
    index++;
}
