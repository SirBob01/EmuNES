#include "./ppu_bus.h"
#include "./cpu_bus.h"

void create_ppu_bus(ppu_bus_t *bus, rom_t *rom, mapper_t *mapper) {
    bus->rom = rom;
    bus->mapper = mapper;
    memset(bus->memory, 0, CPU_RAM_SIZE);
}

address_t mirror_address_ppu_bus(address_t address, rom_mirroring_t mirroring) {
    if (address < PPU_MAP_NAMETABLE_MIRROR && address >= PPU_MAP_NAMETABLE_0) {
        // Mirrored nametable regions
        switch (mirroring) {
        case MIRROR_VERTICAL:
            return PPU_MAP_NAMETABLE_0 +
                   ((address - PPU_MAP_NAMETABLE_0) % 0x800);
        case MIRROR_HORIZONTAL: {
            if (address < PPU_MAP_NAMETABLE_2) {
                return PPU_MAP_NAMETABLE_0 +
                       ((address - PPU_MAP_NAMETABLE_0) % 0x400);
            } else {
                return PPU_MAP_NAMETABLE_2 +
                       ((address - PPU_MAP_NAMETABLE_2) % 0x400);
            }
        }
        }
    } else if (address >= PPU_MAP_PALETTE) {
        // Mirrored palette table regions
        address_t norm = PPU_MAP_PALETTE + ((address - PPU_MAP_PALETTE) % 0x20);
        switch (norm) {
        case 0x3F10:
        case 0x3F14:
        case 0x3F18:
        case 0x3F1C:
            return norm - 0x10;
        default:
            return norm;
        }
    }
    return address;
}

unsigned char *get_memory_ppu_bus(ppu_bus_t *bus, address_t address) {
    rom_mirroring_t mirroring = bus->rom->header.mirroring;
    return bus->memory + mirror_address_ppu_bus(address, mirroring);
}

unsigned char read_ppu_bus(ppu_bus_t *bus, address_t address) {
    if (address >= PPU_MAP_NAMETABLE_0) {
        return *get_memory_ppu_bus(bus, address);
    } else {
        return read_ppu_mapper(bus->mapper, address);
    }
}

void write_ppu_bus(ppu_bus_t *bus, address_t address, unsigned char value) {
    if (address >= PPU_MAP_NAMETABLE_0) {
        *get_memory_ppu_bus(bus, address) = value;
    } else {
        write_ppu_mapper(bus->mapper, address, value);
    }
}