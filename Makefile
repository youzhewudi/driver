obj-m+=proc_test.o

all:
  make -C /lib/modules/$(shell uname -r) M=$(shell pwd) modules

clean:
  rm -rf .*.cmd *.mod.c *.o *.ko .tmp* Module.symvers modules.order
