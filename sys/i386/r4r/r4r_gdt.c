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
#include <sys/systm.h>

#include <machine/cpu.h>
#include <machine/cpufunc.h>
#include <machine/smp.h>
#include <machine/tss.h>

//#include <machine/intr_machdep.h>
//#include <machine/specialreg.h>

#include <i386/r4r/r4r_gdt.h>
/*
static struct i386tss ring0_tss;
static struct i386tss ring1_tss;
static struct i386tss ring2_tss;
static struct i386tss ring3_tss;
*/
/*
 * Note: FreeBSD historically names GDT indices as *_SEL.
 * R4R uses *_IDX to explicitly represent GDT indices,
 * and *_SEL only for final CPU segment selectors.
 */
struct soft_segment_descriptor r4r_gdt_segs[] = {
/* R0CODE_IDX	(NGDT + 0 = 19) Code Descriptor for ring 0 (kernel) */
{	0x0,			/* segment base address  */
	0xfffff,		/* length - all address space */
	SDT_MEMERA,		/* segment type */
	RPL_R0,			/* segment descriptor privilege level */
	1,			/* segment descriptor present */
	0, 0,
	1,			/* default 32 vs 16 bit size */
	1  			/* limit granularity (byte/page units)*/ },
/* R0DATA_IDX	(NGDT + 1 = 20) Data Descriptor for ring 0 (kernel) */
{	0x0,			/* segment base address  */
	0xfffff,		/* length - all address space */
	SDT_MEMRWA,		/* segment type */
	RPL_R0,			/* segment descriptor privilege level */
	1,			/* segment descriptor present */
	0, 0,
	1,			/* default 32 vs 16 bit size */
	1  			/* limit granularity (byte/page units)*/ },
/* R1CODE_IDX	(NGDT + 2 = 21) Code Descriptor for ring 1 */
{	0x0,			/* segment base address  */
	0xfffff,		/* length - all address space */
	SDT_MEMERA,		/* segment type */
	RPL_R1,			/* segment descriptor privilege level */
	1,			/* segment descriptor present */
	0, 0,
	1,			/* default 32 vs 16 bit size */
	1  			/* limit granularity (byte/page units)*/ },
/* R1DATA_IDX	(NGDT + 3 = 22) Data Descriptor for ring 1 */
{	0x0,			/* segment base address  */
	0xfffff,		/* length - all address space */
	SDT_MEMRWA,		/* segment type */
	RPL_R1,			/* segment descriptor privilege level */
	1,			/* segment descriptor present */
	0, 0,
	1,			/* default 32 vs 16 bit size */
	1  			/* limit granularity (byte/page units)*/ },
/* R2CODE_IDX	(NGDT + 4 = 23) Code Descriptor for ring 2 */
{	0x0,			/* segment base address  */
	0xfffff,		/* length - all address space */
	SDT_MEMERA,		/* segment type */
	RPL_R2,			/* segment descriptor privilege level */
	1,			/* segment descriptor present */
	0, 0,
	1,			/* default 32 vs 16 bit size */
	1  			/* limit granularity (byte/page units)*/ },
/* R2DATA_IDX	(NGDT + 5 = 24) Data Descriptor for ring 2 */
{	0x0,			/* segment base address  */
	0xfffff,		/* length - all address space */
	SDT_MEMRWA,		/* segment type */
	RPL_R2,			/* segment descriptor privilege level */
	1,			/* segment descriptor present */
	0, 0,
	1,			/* default 32 vs 16 bit size */
	1  			/* limit granularity (byte/page units)*/ },
/* R3CODE_IDX	(NGDT + 6 = 25) Code Descriptor for ring 3 */
{	0x0,			/* segment base address  */
	0xfffff,		/* length - all address space */
	SDT_MEMERA,		/* segment type */
	RPL_R3,			/* segment descriptor privilege level */
	1,			/* segment descriptor present */
	0, 0,
	1,			/* default 32 vs 16 bit size */
	1  			/* limit granularity (byte/page units)*/ },
/* R3DATA_IDX	(NGDT + 7 = 26) Data Descriptor for ring 3 */
{	0x0,			/* segment base address  */
	0xfffff,		/* length - all address space */
	SDT_MEMRWA,		/* segment type */
	RPL_R3,			/* segment descriptor privilege level */
	1,			/* segment descriptor present */
	0, 0,
	1,			/* default 32 vs 16 bit size */
	1  			/* limit granularity (byte/page units)*/ },
	/* ... */
};


struct r4r_gdt_cpu {
	uint8_t	 bitmap[R4R_GDT_COUNT / 8];
	int	 used;		/* used R4R GDT descriptors */
	int	 free;		/* free R4R GDT descriptors */
	int	 last_idx;	/* last allocated GDT descriptor */
	int	 cpu;		/* cpu id */
};

static struct r4r_gdt_cpu r4r_gdt_cpu[MAXCPU];

/*
 * R4R currently operates in uniprocessor mode only.
 * SMP support will be introduced in a later development phase.
 */
void
r4r_gdt_cpu_init(union descriptor * gdt)
{
	struct r4r_gdt_cpu *gc;
	int i;

	KASSERT(MAXCPU == 1,
	    ("R4R: SMP is not supported yet"));

	/* UP kernel always runs on cpu 0 */
	gc = &r4r_gdt_cpu[0];

	/*
	 * R4R: initialize the remaining GDT entries with null descriptors.
	 *
	 * The original FreeBSD descriptors occupy [0 .. NGDT-1].
	 * The rest of the GDT space up to the architectural limit
	 * (8192 descriptors) is reserved for experimental R4R use and
	 * is explicitly filled with zeroed (null) descriptors.
	 *
	 * No semantic changes are introduced for the standard FreeBSD
	 * segment layout.
	 */
	for (i = R4R_GDT_START; i < GDT_MAX_DESCRIPTORS; i++)
		ssdtosd(&gdt_segs[GNULL_SEL], &gdt[i].sd);

	/*
	 * Initialize bitmap: all bits clear.
	 */
	bzero(gc->bitmap, sizeof(gc->bitmap));

	gc->used = 0;		/* R4R allocated*/
	gc->free = R4R_GDT_COUNT;	/* All R4R slots are free initially */
	gc->last_idx = R4R_GDT_START - 1;
	gc->cpu = 0;

	/*
	 * Mark original FreeBSD GDT entries as permanently used.
	 */
	for (i = 0; i < R4R_GDT_START; i++) {
		int byte = i >> 3;
		int bit  = i & 7;
		gc->bitmap[byte] |= (1 << bit);
	}

	/*
	 * Create initial R4R descriptors.
	 */
	r4r_make_gdt_segs(gdt);

}

/*
 * Make gdt memory segments.  All segments cover the full 4GB
 * of address space and permissions are enforced at page level.
 */
void
r4r_make_gdt_segs(union descriptor * gdt)
{
	struct r4r_gdt_cpu *gc;
	int cnt;
	int x;
	int gdt_idx;

	gc = &r4r_gdt_cpu[0];

	r4r_gdt_segs[R0CODE_IDX].ssd_limit = atop(0 - 1);
	r4r_gdt_segs[R0DATA_IDX].ssd_limit = atop(0 - 1);
	r4r_gdt_segs[R1CODE_IDX].ssd_limit = atop(0 - 1);
	r4r_gdt_segs[R1DATA_IDX].ssd_limit = atop(0 - 1);
	r4r_gdt_segs[R2CODE_IDX].ssd_limit = atop(0 - 1);
	r4r_gdt_segs[R2DATA_IDX].ssd_limit = atop(0 - 1);
	r4r_gdt_segs[R3CODE_IDX].ssd_limit = atop(0 - 1);
	r4r_gdt_segs[R3DATA_IDX].ssd_limit = atop(0 - 1);

	cnt = sizeof(r4r_gdt_segs) / sizeof(r4r_gdt_segs[0]);

	/*
	 * Install descriptors into GDT and mark bitmap used.
	 */
	for (x = 0; x < cnt; x++) {

		gdt_idx = gc->last_idx + (x + 1);

		ssdtosd(&r4r_gdt_segs[x], &gdt[gdt_idx].sd);

		gc->bitmap[gdt_idx >> 3] |= (1 << (gdt_idx & 7));
	}

	/*
	 * Update accounting.
	 */
	gc->last_idx += cnt;
	gc->free -= cnt;
	gc->used += cnt;
}

void
r4r_print_status(void)
{
	struct r4r_gdt_cpu *gc;

	gc = &r4r_gdt_cpu[0];

	printf("R4R: experimental ring framework enabled\n");
	printf("R4R GDT: cpu=%d used=%d free=%d last_idx=%d\n",
	    gc->cpu, gc->used, gc->free, gc->last_idx);
}
