#ifndef PPU_H
#define PPU_H

#include "./display.h"
#include "./interrupt.h"
#include "./memory.h"
#include "./ppu_bus.h"
#include "./rom.h"

// Rows and columns of the PPU render target
#define PPU_SCANLINES 262
#define PPU_LINEDOTS  341

// Maximum number of events per dot
#define PPU_EVENTS_PER_DOT 30

// PPU scanline segments
#define PPU_SCANLINE_VISIBLE   0
#define PPU_SCANLINE_IDLE      240
#define PPU_SCANLINE_VBLANK    241
#define PPU_SCANLINE_PRERENDER 261

// PPU control flags
#define PPU_CTRL_NAMETABLE_BASE       0b11
#define PPU_CTRL_VRAM_INC             (1 << 2)
#define PPU_CTRL_PATTERN_TABLE_SPRITE (1 << 3)
#define PPU_CTRL_PATTERN_TABLE_BG     (1 << 4)
#define PPU_CTRL_SPRITE_SIZE          (1 << 5)
#define PPU_CTRL_MASTER_SLAVE         (1 << 6)
#define PPU_CTRL_NMI                  (1 << 7)

// PPU mask flags
#define PPU_MASK_GREYSCALE         (1 << 0)
#define PPU_MASK_SHOW_BG_LEFT      (1 << 1)
#define PPU_MASK_SHOW_SPRITES_LEFT (1 << 2)
#define PPU_MASK_SHOW_BG           (1 << 3)
#define PPU_MASK_SHOW_SPRITES      (1 << 4)
#define PPU_MASK_EMPHASIZE_RED     (1 << 5)
#define PPU_MASK_EMPHASIZE_GREEN   (1 << 6)
#define PPU_MASK_EMPHASIZE_BLUE    (1 << 7)

// PPU status flags
#define PPU_STATUS_VBLANK     (1 << 7)
#define PPU_STATUS_S0_HIT     (1 << 6)
#define PPU_STATUS_S_OVERFLOW (1 << 5)

/**
 * @brief PPU events that occur per-dot.
 *
 */
typedef enum {
    PPU_EVENT_IDLE,
    PPU_EVENT_FETCH_NAME,
    PPU_EVENT_FETCH_ATTRIBUTE,
    PPU_EVENT_FETCH_PATTERN_LO,
    PPU_EVENT_FETCH_PATTERN_HI,
    PPU_EVENT_SHIFT_REGISTERS,
    PPU_EVENT_COPY_X,
    PPU_EVENT_COPY_Y,
    PPU_EVENT_INCREMENT_X,
    PPU_EVENT_INCREMENT_Y,
    PPU_EVENT_CLEAR_OAMADDR,
    PPU_EVENT_CLEAR_FLAGS,
    PPU_EVENT_SET_VBLANK,
    PPU_EVENT_SKIP_CYCLE,
} ppu_event_t;

typedef struct {
    /**
     * @brief PPUCTRL register.
     *
     */
    unsigned char ctrl;

    /**
     * @brief PPUMASK register.
     *
     */
    unsigned char mask;

    /**
     * @brief PPUSTATUS register.
     *
     */
    unsigned char status;

    /**
     * @brief OAMADDR register.
     *
     */
    unsigned char oam_addr;

    /**
     * @brief OAMDATA register.
     *
     */
    unsigned char oam_data;

    /**
     * @brief PPUSCROLL register.
     *
     */
    unsigned char scroll;

    /**
     * @brief PPUADDR register.
     *
     */
    unsigned char addr;

    /**
     * @brief PPUDATA register.
     *
     */
    unsigned char data;

    /**
     * @brief OAMDMA register.
     *
     */
    unsigned char oam_dma;

    /**
     * @brief Current VRAM address.
     *
     */
    address_t v;

    /**
     * @brief Temporary VRAM address.
     *
     */
    address_t t;

    /**
     * @brief Fine X scroll.
     *
     */
    unsigned char x;

    /**
     * @brief Write toggle.
     *
     * If true, writes to PPUADDR will set the high-byte of
     * the current VRAM address (v).
     */
    bool w;

    /**
     * @brief Nametable latch.
     *
     */
    unsigned char nt_latch;

    /**
     * @brief Palette attribute latch.
     *
     */
    unsigned char pa_latch;

    /**
     * @brief Pattern table latches.
     *
     */
    unsigned char pt_latches[2];

    /**
     * @brief Pattern table shift registers. The high 8 bits are the current
     * tile. Every 8 cycles, new data is loaded into the low 8 bits of the
     * register.
     *
     */
    unsigned short pt_shift[2];

    /**
     * @brief Primary object attribute memory.
     *
     */
    unsigned char primary_oam[256];

    /**
     * @brief Secondary object attribute memory.
     *
     */
    unsigned char secondary_oam[32];

    /**
     * @brief Data bus for communicating with the CPU.
     *
     */
    unsigned char io_databus;

    /**
     * @brief Toggle to suppress setting VBlank.
     *
     */
    bool suppress_vbl;

    /**
     * @brief Toggle to suppress enabling NMI.
     *
     */
    bool suppress_nmi;

    /**
     * @brief Number of cycles.
     *
     */
    unsigned long cycles;

    /**
     * @brief Current scanline (row).
     *
     */
    unsigned scanline;

    /**
     * @brief Current pixel (column).
     *
     */
    unsigned dot;

    /**
     * @brief Is in an odd frame?
     *
     */
    bool odd_frame;

    /**
     * @brief Output color buffer.
     *
     */
    color_t color_buffer[PPU_LINEDOTS * PPU_SCANLINES];

    /**
     * @brief Pointer to the PPU bus.
     *
     */
    ppu_bus_t *bus;

    /**
     * @brief Pointer to the interrupt state.
     *
     */
    interrupt_t *interrupt;

    /**
     * @brief Events in the render scanlines.
     *
     */
    ppu_event_t visible_events[PPU_LINEDOTS][PPU_EVENTS_PER_DOT];

    /**
     * @brief Events in the pre-render scanline.
     *
     */
    ppu_event_t prerender_events[PPU_LINEDOTS][PPU_EVENTS_PER_DOT];

    /**
     * @brief Events in the VBlank scanline.
     *
     */
    ppu_event_t vblank_events[PPU_LINEDOTS][PPU_EVENTS_PER_DOT];
} ppu_t;

/**
 * @brief Create the PPU.
 *
 * @param ppu
 * @param bus
 * @param interrupt
 */
void create_ppu(ppu_t *ppu, ppu_bus_t *bus, interrupt_t *interrupt);

/**
 * @brief Destroy the PPU.
 *
 * @param ppu
 */
void destroy_ppu(ppu_t *ppu);

/**
 * @brief Initialize the per-dot event tables.
 *
 * @param ppu
 */
void create_event_tables_ppu(ppu_t *ppu);

/**
 * @brief Read the current state of the CPU for debugging.
 *
 * @param ppu
 * @param buffer
 * @param buffer_size
 */
void read_state_ppu(ppu_t *ppu, char *buffer, unsigned buffer_size);

/**
 * @brief Check if the PPU is currently rendering.
 *
 * @param ppu
 * @return true
 * @return false
 */
bool is_rendering_ppu(ppu_t *ppu);

/**
 * @brief Update the PPU.
 *
 * @param ppu
 */
void update_ppu(ppu_t *ppu);

#endif