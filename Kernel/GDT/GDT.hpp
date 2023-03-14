#pragma once

#include <stddef.h>
#include <stdint.h>

#define GDT_SIZE 8192

// For the access byte in the segment descriptor.
#define PRESENT_SEGMENT         (0b1 << 7)
#define DPL_USER                (0b11 << 5)
#define CODE_DATA_SEGMENT       (0b1 << 4)
#define EXECUTABLE_SEGMENT      (0b1 << 3)

#define DC_GROW_DOWN            (0b1 << 2)
#define DC_CONFORMING_SEGMENT   (0b1 << 2)

#define ALLOW_READ_ACCESS       (0b1 << 1)
#define ALLOW_WRITE_ACCESS       (0b1 << 1)

// For the flags in the segment descriptor.
#define GRANUALITY_FLAG         (1 << 3)
#define SEGMENT_32BIT_FLAG      (1 << 2)
#define LONG_MODE_CODE_FLAG     (1 << 1)

namespace Kernel
{
    struct GDT_Segment_Descriptor
    {
        private:
            uint64_t limit_1 : 16;
            uint64_t base_1 : 24;
            uint64_t access_byte : 8;
            uint64_t limit_2 : 4;
            uint64_t flags : 4;
            uint64_t base_2 : 8;

        public:
            void        set_limit(uint32_t limit);
            uint32_t    get_limit(void) const;
            void        set_base(uint32_t base);
            uint32_t    get_base(void) const;
            void        set_access_byte(uint8_t access_byte);
            uint8_t     get_access_byte(void) const;
            void        set_flags(uint8_t flags);
            uint8_t     get_flags(void) const;
    };

    struct GDT_Table
    {
        private:
            GDT_Segment_Descriptor entries[GDT_SIZE];
        public:
            GDT_Segment_Descriptor          *insert_new_entry(const GDT_Segment_Descriptor &entry, size_t index);
            const GDT_Segment_Descriptor    *get_entry(size_t index) const;
    };

    struct GDT_Descriptor
    {
        private:
            uint16_t size;
            uint32_t offset;
        public:
            void            set_size(uint16_t size);
            uint16_t        get_size(void) const;
            void            set_offset(uint32_t offset);
            uint32_t        get_offset(void) const;
    };

    class GDT
    {
        public:
            static GDT_Table        *gdt_ptr;
            static GDT_Descriptor   *gdt_descriptor_ptr;

        public:
            static GDT_Segment_Descriptor   create_GDT_Descriptor(uint32_t base, uint32_t limit, uint8_t access_byte, uint8_t flags);
            static const GDT_Table          *get_gdt_ptr(void);
            static const GDT_Descriptor     *get_gdt_descriptor_ptr(void);
    };
}
