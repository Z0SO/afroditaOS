override KERNEL := kernel.elf
override ISO := limine-cd.iso
override IMG := limine-cd.img
override VDI := limine-cd.vdi
override IMG_RAW := raw.img
override LIMINECFG := limine.cfg
override GDBCFG := debug.gdb
override VMNAME := sxlenx

# It is highly recommended to use a custom built cross toolchain to build a kernel.
# We are only using "cc" as a placeholder here. It may work by using
# the host system's toolchain, but this is not guaranteed.

CC := /usr/bin/cc
LD := /usr/bin/ld
LMN := limine/limine-deploy.exe
BOOTEFI := ./limine/BOOTX64.EFI
ASMC := nasm

# This are specific to my setup, please modify them!!!!
#########################DESKTOP SETTINGS#################################
#QEMU := "/usr/bin/qemu-system-x86_64"
#GDB := "/usr/bin/gdb"
#########################LAPTOP SETTINGS###################################
QEMU := "/usr/bin/qemu-system-x86_64"
GDB := "/usr/bin/gdb"
###########################################################################
VBOXMANAGE := "/mnt/c/Program Files/Oracle/VirtualBox/VBoxManage.exe"
VBOXHEADLESS := "/mnt/c/Program Files/Oracle/VirtualBox/VBoxHeadless.exe"
VBOXCOM1PORT := 4441
VBOXCOM2PORT := 4442
VBOXCOM3PORT := 4443
VBOXCOM4PORT := 4444
CMDNEWSCREEN := cmd.exe /c start cmd /c wsl -e
MNTDIR := /mnt/bloodmoon

WSLHOSTIP := $(shell ipconfig.exe | grep 'vEthernet (WSL)' -a -A4 | tail -n1 | cut -d":" -f 2 | tail -n1 | sed -e 's/\s*//g')

KERNEL_ENTRY := _start

BLOCKSIZE := 1024
MEMSIZE := 1024
VMEMSIZE := 128
QFLAGS ?= -cpu qemu64 -d cpu_reset -machine q35 -m $(MEMSIZE) -boot d -serial stdio -serial telnet::4444,server,nowait -cdrom 
QFLAGSEXP ?= -cpu qemu64 -d cpu_reset -machine q35 -m $(MEMSIZE) -boot d -drive if=pflash,format=raw,unit=0,file=./OVMFbin/OVMF_CODE-pure-efi.fd,readonly=on -drive if=pflash,format=raw,unit=1,file=./OVMFbin/OVMF_VARS-pure-efi.fd -net none -serial stdio -drive file=

CFLAGS ?= -O2 -g -Wall -Wextra -pipe -std=c11
NASMFLAGS ?= -F dwarf -g
LDFLAGS ?=
CPPFLAGS ?=
GDBFLAGS ?=  --nx --command=$(GDBCFG)
GDBPORT ?= 1234

ABSDIR := $(shell pwd)
LMNDIR := limine
SRCDIR := src
BUILDHOME := build
BUILDDIR := build/bin
OBJDIR := build/lib
ISOBUILDDIR := build/iso_root
ISODIR := build/image
PROGSDIR := progs/sources
PROGSBUILDDIR := progs/export
PROGSDATADIR := progs/data

PROGS := $(wildcard $(PROGSDIR)/*)

SYSDIR := $(MNTDIR)/system
DATDIR := $(MNTDIR)/data

LMNREPO := https://github.com/limine-bootloader/limine.git
LMNBRCH := v3.0-branch-binary

DIRS := $(wildcard $(SRCDIR)/*)

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

# Check that given variables are set and all have non-empty values,
# die with an error otherwise.
#
# Params:
#   1. Variable name(s) to test.
#   2. (optional) Error message to print.
check_defined = \
    $(strip $(foreach 1,$1, \
        $(call __check_defined,$1,$(strip $(value 2)))))
__check_defined = \
    $(if $(value $1),, \
      $(error Undefined $1$(if $2, ($2))))

override CFLAGS +=       \
    -I.                  \
    -std=c11             \
    -ffreestanding       \
    -fno-stack-protector \
    -fno-stack-check     \
    -fno-pie             \
    -fno-pic             \
    -m64                 \
    -march=x86-64        \
    -mabi=sysv           \
    -mno-80387           \
    -mno-red-zone        \
    -mcmodel=kernel      \
    -MMD
override CFLAGS += $(CEXTRA)

override LDFLAGS +=         \
    -nostdlib               \
    -static                 \
    -z max-page-size=0x1000 \
    -T linker.ld

override NASMFLAGS += \
    -f elf64

override CFILES :=$(call rwildcard,$(SRCDIR),*.c)        
override ASFILES := $(call rwildcard,$(SRCDIR),*.S)
override NASMFILES := $(call rwildcard,$(SRCDIR),*.asm)
override OBJS := $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(CFILES))
override OBJS += $(patsubst $(SRCDIR)/%.S, $(OBJDIR)/%_s.o, $(ASFILES))
override OBJS += $(patsubst $(SRCDIR)/%.asm, $(OBJDIR)/%_asm.o, $(NASMFILES))
override HEADER_DEPS := $(CFILES:.c=.d) $(ASFILES:.S=.d)

# You may comment @make run in this target if you intend
# to use virtualbox or real hardware.
all:
	@echo "Building kernel..."
	@make kernel
	@echo "Building ISO..."
	@make buildimg
	@echo "Running QEMU..."
	@make run
	@echo "Done!"

vbon:
	@make clean
	@make kernel
	@echo "Creating iso"
	@cp $(ISODIR)/$(IMG_RAW) $(ISODIR)/$(IMG)
	@sudo losetup -f $(ISODIR)/$(IMG)
	@make buildimggpt
	@echo "translating img to vdi"
	@$(VBOXMANAGE) convertfromraw $(ISODIR)/$(IMG) $(ISODIR)/$(VDI) --format VDI
	@echo "Creating virtual machine..."
	@$(VBOXMANAGE) createvm --name $(VMNAME) --ostype "Other_64" --register
	@echo "Setting up virtual machine..."
	@$(VBOXMANAGE) modifyvm $(VMNAME) --memory $(MEMSIZE) --vram $(VMEMSIZE) --chipset ich9 --firmware efi
	@$(VBOXMANAGE) modifyvm $(VMNAME) --uart1 0x3F8 1 --uartmode1 tcpserver $(VBOXCOM1PORT) --uarttype1 16450
	@$(VBOXMANAGE) modifyvm $(VMNAME) --nic1 nat
	@$(VBOXMANAGE) storagectl $(VMNAME) --name "SATA Controller" --add sata --controller IntelAhci
	@$(VBOXMANAGE) storageattach $(VMNAME) --storagectl "SATA Controller" --port 0 --device 0 --type hdd --medium $(ISODIR)/$(VDI)
	@$(VBOXMANAGE) modifyvm $(VMNAME) --boot1 disk --boot2 none --boot3 none --boot4 none
	@echo "Starting virtual machine..."
	@$(VBOXMANAGE) startvm $(VMNAME)
	@echo "Done!"

vboff:
	@echo "Stopping virtual machine..."

	@if [ -z "$(shell $(VBOXMANAGE) list runningvms | grep $(VMNAME))" ]; then \
		echo "Virtual machine is not running!"; \
	else \
		$(VBOXMANAGE) controlvm $(VMNAME) poweroff; \
	fi
	@sleep 3
	@echo "Deleting virtual machine..."
	@$(VBOXMANAGE) unregistervm $(VMNAME) --delete
	@rm -f $(ISODIR)/$(VDI)

kernel: $(OBJS) link

$(OBJDIR)/io/interrupts.o: $(SRCDIR)/io/interrupts.c
#	@ echo !==== COMPILING $^
	@ mkdir -p $(@D)
	@$(CC) $(CFLAGS) -mgeneral-regs-only -c $^ -o $@

$(OBJDIR)/drivers/serial/serial.o: $(SRCDIR)/drivers/serial/serial.c
#	@ echo !==== COMPILING $^ with -mgeneral-regs-only
	@ mkdir -p $(@D)
	@$(CC) $(CFLAGS) -mgeneral-regs-only -c $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
#	@ echo !==== COMPILING $^
	@ mkdir -p $(@D)
	@$(CC) $(CFLAGS) -c $^ -o $@

$(OBJDIR)/%_asm.o: $(SRCDIR)/%.asm
#	@ echo !==== COMPILING $^
	@ mkdir -p $(@D)
	@$(ASMC) $^ -felf64 -o $@

$(OBJDIR)/%_s.o: $(SRCDIR)/%.S
#	@ echo !==== COMPILING $^
	@ mkdir -p $(@D)
	@$(ASMC) $(NASMFLAGS) $^ -f elf64 -o $@

link: 
#	@ echo !==== LINKING $^
	@$(LD) $(LDFLAGS) -o $(BUILDDIR)/kernel.elf $(OBJS)

clean:
	@rm -rf $(OBJDIR)/*
	@rm -rf $(OBJS)
	@rm -f $(ISOBUILDDIR)/$(KERNEL)
	@rm -f $(ISOBUILDDIR)/$(LIMINECFG)
	@rm -f $(BUILDDIR)/$(KERNEL)
	@rm -f $(ISODIR)/$(IMG)
	@rm -f $(ISODIR)/$(ISO)
	@rm -f $(ISODIR)/$(VDI)

setup:
	@mkdir -p $(BUILDDIR)
	@mkdir -p $(OBJDIR)
	@mkdir -p $(ISOBUILDDIR)
	@mkdir -p $(PROGSBUILDDIR)
	@mkdir -p $(ISODIR)
	@dd if=/dev/zero of=$(ISODIR)/$(IMG_RAW) bs=4096 count=102400
	@git clone $(LMNREPO) --branch=$(LMNBRCH) --depth=1
	@cp -v $(LMNDIR)/limine.sys $(LMNDIR)/limine-cd.bin $(LMNDIR)/limine-cd-efi.bin $(ISOBUILDDIR)
	@echo file $(ABSDIR)/$(BUILDDIR)/$(KERNEL) > debug.gdb
	@echo target remote $(WSLHOSTIP):$(GDBPORT) >> debug.gdb
	@echo set disassembly-flavor intel >> debug.gdb
	@echo b $(KERNEL_ENTRY) >> debug.gdb
	@echo c >> debug.gdb

cleansetup:
	@rm -rf $(BUILDHOME)
	@rm -f debug.gdb
	@rm -rf $(LMNDIR)
	@rm -rf $(PROGSBUILDDIR)

buildimg:
	@cp -v limine.cfg $(BUILDDIR)/$(KERNEL) $(ISOBUILDDIR)
	@xorriso -as mkisofs -b limine-cd.bin \
        -no-emul-boot -boot-load-size 4 -boot-info-table \
        --efi-boot limine-cd-efi.bin \
        -efi-boot-part --efi-boot-image --protective-msdos-label \
        $(ISOBUILDDIR) -o $(ISODIR)/$(ISO)
	$(LMN) $(ISODIR)/$(ISO)

buildimgexp:
	@cp $(ISODIR)/$(IMG_RAW) $(ISODIR)/$(IMG)
	@mkfs.fat -F32 $(ISODIR)/$(IMG)
	@mmd -i $(ISODIR)/$(IMG) ::/EFI
	@mmd -i $(ISODIR)/$(IMG) ::/EFI/BOOT
	@mcopy -i $(ISODIR)/$(IMG) $(BOOTEFI) ::/EFI/BOOT
	@mcopy -i $(ISODIR)/$(IMG) ./startup.nsh ::
	@mcopy -i $(ISODIR)/$(IMG) $(BUILDDIR)/$(KERNEL) ::
	@mcopy -i $(ISODIR)/$(IMG) ./limine.cfg ::
#	@mcopy -i $(ISODIR)/$(IMG) ./test/test.fake ::/TEST

buildimggpt:

	$(eval LOOP_DEV_PATH := $(shell losetup -a | grep $(IMG) | cut -d: -f1))
	@echo Loop device path: $(LOOP_DEV_PATH)
	
	@sudo sgdisk --new 1:0:+50M --typecode 1:ef00 $(LOOP_DEV_PATH)
	@sudo sgdisk --new 2:0:+100M -t 2:8300 $(LOOP_DEV_PATH)
	@sudo sgdisk --new 3:0:0  -t 3:8300 $(LOOP_DEV_PATH)
	@sudo sgdisk -p $(LOOP_DEV_PATH)
	@sudo partprobe $(LOOP_DEV_PATH)
	@sudo fdisk -l $(LOOP_DEV_PATH)
	@sudo mkfs.fat -n PATATA -F32 $(LOOP_DEV_PATH)p1
	@sudo mkfs.fat -n FDATA -F32 $(LOOP_DEV_PATH)p2
	@sudo mkfs.ext2 -L FDATA2 $(LOOP_DEV_PATH)p3 -b $(BLOCKSIZE)
	@sudo dumpe2fs $(LOOP_DEV_PATH)p3 | grep -i superblock
	@sudo mkdir $(MNTDIR)
	@sudo mkdir $(SYSDIR)
	@sudo mkdir $(DATDIR)

	@sudo mount $(LOOP_DEV_PATH)p1 $(SYSDIR)

	@echo Disk mounted, creating system files
	@sudo mkdir -p $(SYSDIR)/efi/boot
	@sudo mkdir -p $(SYSDIR)/test

	@sudo cp $(BOOTEFI) $(SYSDIR)/efi/boot
	@sudo cp ./startup.nsh $(SYSDIR)
	@sudo cp $(BUILDDIR)/$(KERNEL) $(SYSDIR)
	@sudo cp ./limine.cfg $(SYSDIR)
	@sudo cp ./test/test.fake $(SYSDIR)/test
	@sudo umount $(SYSDIR)

	@echo System files created, creating f32 data files
	@sudo mount $(LOOP_DEV_PATH)p2 $(DATDIR)
	@sudo cp -r $(PROGSBUILDDIR) $(DATDIR)
	@sudo cp -r $(PROGSDATADIR) $(DATDIR)
	@sudo tree $(DATDIR)
	@sudo umount $(DATDIR)

	@echo F32 data files created, creating ext2 data files
	@sudo mount $(LOOP_DEV_PATH)p3 $(DATDIR)
	@sudo cp -r $(PROGSBUILDDIR) $(DATDIR)
	@sudo cp -r $(PROGSDATADIR) $(DATDIR)
	@sudo tree $(DATDIR)
	@sudo umount $(DATDIR)

	@sudo rm -rf $(MNTDIR)
	@sudo losetup -d $(LOOP_DEV_PATH)

run:
	$(QEMU) $(QFLAGS) $(ISODIR)/$(ISO)

run_exp:
	$(QEMU) $(QFLAGSEXP)$(ISODIR)/$(IMG)

gpt:
	@make kernel
	@echo "Building GPT..."
#This is required to be before buildimggpt!
	@cp $(ISODIR)/$(IMG_RAW) $(ISODIR)/$(IMG)
	@sudo losetup -f $(ISODIR)/$(IMG)
# Due to eval weird behaviour
	@make buildimggpt
	@echo "Running GPT QEMU..."
	@make run_exp
	@echo "Done!"

exp:
	@make kernel
	@echo "Building experimental ISO..."
	@make buildimgexp
	@echo "Running experimental QEMU..."
	@make run_exp
	@echo "Done!"

progs:
	$(foreach prog,$(PROGS),$(MAKE) -C $(prog); rsync -a $(prog)/build/bin/ $(PROGSBUILDDIR);)


cprogs:
	$(foreach prog,$(PROGS),$(MAKE) -C $(prog) clean;)
	@rm -rf $(PROGSBUILDDIR)/*

.PHONY: progs

debugpt:
	@make kernel
	@echo "Building GPT..."
#This is required to be before buildimggpt!
	@cp $(ISODIR)/$(IMG_RAW) $(ISODIR)/$(IMG)
	@sudo losetup -f $(ISODIR)/$(IMG)
# Due to eval weird behaviour
	@make buildimggpt
	@echo "Running GPT QEMU..."
	$(CMDNEWSCREEN) $(GDB) $(GDBFLAGS) &
#	$(QEMU) $(QFLAGSEXP) -drive format=raw $(ISODIR)/$(IMG)
	$(QEMU) -S -s $(QFLAGSEXP) -drive format=raw $(ISODIR)/$(IMG)

debuge:
	@make kernel
	@echo "Building experimental ISO..."
	@make buildimgexp
	@echo "Running experimental QEMU..."
	$(QEMU) -S -s $(QFLAGSEXP)$(ISODIR)/$(IMG)

debug:
	@make CEXTRA=-O0 kernel
	@make buildimg
	$(CMDNEWSCREEN) $(GDB) $(GDBFLAGS) &
	$(QEMU) -S -s $(QFLAGS) $(ISODIR)/$(ISO)