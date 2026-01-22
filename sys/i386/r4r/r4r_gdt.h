/*
 * SPDX-License-Identifier: MIT
 *
 * src/sys/i386/r4r/r4r_gdt.h
 *
 * (C) Copyright 2026 Isa <isa@isoux.org>
 */

#ifndef _R4R_GDT_H_
#define _R4R_GDT_H_

#include <machine/segments.h>
/*
 * Entries in the R4R Global Descriptor Table (GDT)
 * Slots [NGDT .. GDT_MAX_DESCRIPTORS -1]
 */

/* General Code / Data descriptors for all rings
 *
 * R4R GDT indices start at NGDT, which represents the first free
 * descriptor slot in the original FreeBSD GDT layout.
 *
 * By using NGDT as the base, R4R extends the existing GDT without
 * modifying or overlapping standard FreeBSD descriptors.
 */
#define	R0CODE_IDX	(NGDT + 0)	/* Ring 0 Code Descriptor */
#define	R0DATA_IDX	(NGDT + 1)	/* Ring 0 Data Descriptor */
#define	R1CODE_IDX	(NGDT + 2)	/* Ring 1 Code Descriptor */
#define	R1DATA_IDX	(NGDT + 3)	/* Ring 1 Data Descriptor */
#define	R2CODE_IDX	(NGDT + 4)	/* Ring 2 Code Descriptor */
#define	R2DATA_IDX	(NGDT + 5)	/* Ring 2 Data Descriptor */
#define	R3CODE_IDX	(NGDT + 6)	/* Ring 3 Code Descriptor */
#define	R3DATA_IDX	(NGDT + 7)	/* Ring 3 Data Descriptor */

/* General TSS / LDT Descritor for All Rings */
#define	R0TSS_IDX	(NGDT + 8)	/* Ring 0 TSS Descriptor */
#define	R0LDT_IDX	(NGDT + 9)	/* Ring 0 LDT Descriptor */
#define	R1TSS_IDX	(NGDT + 10)	/* Ring 1 TSS Descriptor */
#define	R1LDT_IDX	(NGDT + 11)	/* Ring 1 LDT Descriptor */
#define	R2TSS_IDX	(NGDT + 12)	/* Ring 2 TSS Descriptor */
#define	R2LDT_IDX	(NGDT + 13)	/* Ring 2 LDT Descriptor */
#define	R3TSS_IDX	(NGDT + 14)	/* Ring 3 TSS Descriptor */
#define	R3LDT_IDX	(NGDT + 15)	/* Ring 3 LDT Descriptor */

/*
 * R?CODE_IDX macros represent GDT indices.
 * R?CODE_SEL macros represent final segment selectors.
 */

//#define SEL(idx, rpl)   (((idx) << 3) | (rpl))

#define RPL_R0	0
#define RPL_R1	1
#define RPL_R2	2
#define RPL_R3	3

#define R0CODE_SEL	GSEL(R0CODE_IDX, RPL_R0)
#define R0DATA_SEL	GSEL(R0DATA_IDX, RPL_R0)
#define R1CODE_SEL	GSEL(R1CODE_IDX, RPL_R1)
#define R1DATA_SEL	GSEL(R1DATA_IDX, RPL_R1)
#define R2CODE_SEL	GSEL(R2CODE_IDX, RPL_R2)
#define R2DATA_SEL	GSEL(R2DATA_IDX, RPL_R2)
#define R3CODE_SEL	GSEL(R3CODE_IDX, RPL_R3)
#define R3DATA_SEL	GSEL(R3DATA_IDX, RPL_R3)


int  r4r_gdt_alloc(void);
void r4r_gdt_free(int sel);
void r4r_gdt_set(int sel, struct segment_descriptor *);
int  r4r_gdt_free_count(void);
int  r4r_gdt_used_count(void);
void r4r_print_status(void);
void r4r_gdt_cpu_init(union descriptor * gdt);
void r4r_make_gdt_segs(union descriptor * gdt);

#endif /* _R4R_GDT_H_ */
