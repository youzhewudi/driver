obj-m+=proc_test.o proc_wr.o miscdev.o

all:
  make -C /lib/modules/$(shell uname -r) M=$(shell pwd) modules

clean:
  rm -rf .*.cmd *.mod.c *.o *.ko .tmp* Module.symvers modules.order

misc_test:
	gcc -o misc_test misc_test.c
