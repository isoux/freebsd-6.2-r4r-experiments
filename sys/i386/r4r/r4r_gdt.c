/*
 * SPDX-License-Identifier: MIT
 *
 * src/sys/i386/r4r/r4r_gdt.c
 *
 * (C) Copyright 2026 Isa <isa@isoux.org>
 */

/*
 * R4R GDT Manager
 *
 * Experimental per-CPU GDT descriptor management layer used by the
 * R4R (Rusted 4 Rings) research framework. This subsystem reserves
 * the standard FreeBSD GDT region and manages the remaining
 * architectural GDT space for experimental use.
 *
 * No semantic changes are introduced to the standard FreeBSD
 * segment layout.
 */

#include <sys/cdefs.h>
__FBSDID("$R4R: r4r_gdt.c MIT licensed experimental GDT manager $");

#include <sys/param.h>
#include <machine/cpu.h>
#include <machine/smp.h>

#include "r4r_gdt.h"

struct r4r_gdt_cpu {
	uint8_t bitmap[R4R_GDT_COUNT / 8];
	int used;
};

static struct r4r_gdt_cpu r4r_gdt_cpu[MAXCPU];

void
r4r_gdt_cpu_init(int cpu)
{
	struct r4r_gdt_cpu *gc;
	int i;

	KASSERT(cpu >= 0 && cpu < MAXCPU,
	    ("r4r_gdt_cpu_init: invalid cpu %d", cpu));

	gc = &r4r_gdt_cpu[cpu];

	/* Clear bitmap and usage counter */
	bzero(gc->bitmap, sizeof(gc->bitmap));
	gc->used = 0;

	/*
	 * Mark original FreeBSD GDT entries as used.
	 * Slots [0 .. NGDT-1] are reserved and must never
	 * be allocated by R4R.
	 */
	for (i = 0; i < R4R_GDT_START; i++) {
		int byte = i / 8;
		int bit  = i % 8;

		gc->bitmap[byte] |= (1 << bit);
		gc->used++;
	}
}
