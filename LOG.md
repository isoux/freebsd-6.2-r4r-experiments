# 📝 Change Log

## 2025-12-31

- 📌 Description: In the initial phase, we made only basic changes: we added an R4R option to the kernel options, modified the segments.h files for both amd64 and i386, and just set up the basic macros without any functional changes. That was our starting point.
- 🔗 Commit links: [commit 1](https://github.com/isoux/freebsd-6.2-r4r-experiments/commit/c92c17a482fe15f3fdd77a4e79fab5db837a0243), [commit 2](https://github.com/isoux/freebsd-6.2-r4r-experiments/commit/424caf3b3fcc0ebc621f24225ba12cc5c97d46a7)

## 2026-01-14

- 📌 Description: Emit a boot-time message when R4R support is compiled in.
- 🔗 Commit links: [commit 1](https://github.com/isoux/freebsd-6.2-r4r-experiments/commit/6cfc7304e988bc75659ad2d71066d7c08164102c), [commit 2](https://github.com/isoux/freebsd-6.2-r4r-experiments/commit/0c77eb9e04944fb0b177a9063489526e524efafb)

## 2026-01-17

- 📌 Description:  
Extended the i386 and amd64 GDT layouts under the R4R kernel option to reserve the full architectural GDT range (8192 descriptors). The original FreeBSD descriptors remain unchanged in the range [0..NGDT-1]. All remaining entries are explicitly initialized as null descriptors during early boot. R4R-specific descriptor initialization is deferred to a dedicated GDT management subsystem in a separate source file. No semantic changes are introduced to the standard FreeBSD segment layout.
- 🔗 Commit links: [commit 1](https://github.com/isoux/freebsd-6.2-r4r-experiments/commit/5c6802a41a98f080aea84e04f110812bc6e5aad6), [commit 2](https://github.com/isoux/freebsd-6.2-r4r-experiments/commit/333bedbc0ee6a134351753d9a1834d5464315835)

- 📌 Description (continued):  
Added the initial R4R GDT manager subsystem skeleton for i386. Introduced a dedicated r4r_gdt.h public interface and r4r_gdt.c implementation with per-CPU bitmap tracking for GDT descriptor ownership. The subsystem provides a clean API for future experimental descriptor allocation while explicitly reserving the standard FreeBSD GDT region. The R4R GDT manager is conditionally integrated into the kernel build system via files.i386 under the R4R option. At this stage, only per-CPU initialization logic is implemented; no functional descriptor allocation is performed yet.
- 🔗 Commit links: [commit 3](https://github.com/isoux/freebsd-6.2-r4r-experiments/commit/67b4cf5537cfc8a4308a49cd107d340637c89f65)

## 2026-01-18

- 📌 Description:  
Fixed and stabilized the initial R4R GDT subsystem integration. The R4R source directory was renamed from an incorrect legacy name to the final [`r4r`](https://github.com/isoux/freebsd-6.2-r4r-experiments/commit/0c03fcbab1d58e71af2bb2399670beab7eaee0cb) form to reflect the intended project naming. Header include paths were corrected to follow the FreeBSD kernel include hierarchy, resolving build-time dependency and mkdep failures. Missing kernel headers were added to ensure proper visibility of KASSERT(), bzero(), and CPU helper macros.  
A dedicated R4R kernel configuration file was introduced to support clean kernel builds with the R4R option enabled. The kernel now builds and installs successfully with the R4R GDT subsystem enabled.  
No functional behavior of the GDT manager was changed; this commit strictly focuses on build system correctness, directory structure consistency, and integration hygiene.

- 🔗 Commit links: [commit 1](https://github.com/isoux/freebsd-6.2-r4r-experiments/commit/328e6e365a22bc7aed80b19df47cf98215d794e7), [commit 2](https://github.com/isoux/freebsd-6.2-r4r-experiments/commit/cda189ff742e7d2da8410360561a1b7cffb8291c)
