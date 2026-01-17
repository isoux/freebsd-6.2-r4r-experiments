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

int  r4r_gdt_alloc(void);
void r4r_gdt_free(int sel);
void r4r_gdt_set(int sel, struct segment_descriptor *);
int  r4r_gdt_free_count(void);
int  r4r_gdt_used_count(void);

void r4r_gdt_cpu_init(int cpu);

#endif /* _R4R_GDT_H_ */
