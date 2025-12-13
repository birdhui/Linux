obj-m = hello.o

KDIR := /home/tpgml/WSL2-Linux-Kernel-linux-msft-wsl-6.6.87.2
PWD := $(shell pwd)

default:
	make -C $(KDIR) M=$(PWD) modules

clean:
	rm -rf *.kr *.mod *.cmd *.o *.ko
