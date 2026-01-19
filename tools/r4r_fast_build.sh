#!/bin/csh

# R4R kernel build script (no modules, restore acpi.ko only)

umask 022

set SRCROOT=/usr/src
set OLDDIR=`pwd`

cd ${SRCROOT} || exit 1

set KERNCONF=R4R
set ACPIKO=/usr/obj/usr/src/sys/${KERNCONF}/modules/usr/src/sys/modules/acpi/acpi/acpi.ko

echo "===> R4R kernel build started"

if ( -d /boot/kernel ) then
    echo "===> Removing existing /boot/kernel directory"
    rm -rf /boot/kernel
endif

# Build kernel only, no modules
echo "===> Building kernel"
make -j2 -DNO_CLEAN buildkernel KERNCONF=${KERNCONF} NO_MODULES=yes
if ( $status != 0 ) then
    echo "!!! buildkernel failed"
    exit 1
endif

# Install kernel only, no modules
echo "===> Installing kernel"
make installkernel KERNCONF=${KERNCONF} NO_MODULES=yes
if ( $status != 0 ) then
    echo "!!! installkernel failed"
    exit 1
endif

# Restore only ACPI module
if ( -f ${ACPIKO} ) then
    echo "===> Installing ACPI module only"
    mkdir -p /boot/kernel
    cp ${ACPIKO} /boot/kernel/acpi.ko
else
    echo "!!! ERROR: ACPI module not found:"
    echo "    ${ACPIKO}"
    exit 1
endif

echo "=== DONE ==="
echo "Kernel: ${KERNCONF}"
echo "ACPI module restored."
echo "You can reboot now."

cd ${OLDDIR}

