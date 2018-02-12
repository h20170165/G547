obj-m := prog.o
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
user:
	gcc prog_user.c -o prog_user
