# G547 Assignments 
This readme contains instructions to compile & execute User mode & Kernel mode C Codes.

### Assignment1
* **Compile & Insert Kernel Module**: 
```shell
$ make -C /lib/modules/$(uname -r)/build M=$PWD modules
$ sudo insmod assign1.ko
```

### Assignment1_ADC
* **Compile & Insert Kernel Module**: 
```shell
$ make all
$ sudo insmod adc.ko
```
* **Compile User Code**: 
```shell
$ gcc adc_user.c -o adc_user
```
* **Read ADC Value**: 
```shell
$ sudo ./adc_user
```
* **Change ADC Channel**: 
```shell
$ sudo ./adc_user 3
```

### Assignment2_TIMER
* **Compile & Insert Kernel Module**: 
```shell
$ make all
$ sudo insmod prog.ko
```
* **Compile User Code**: 
```shell
$ make all
```
* **Read Timer Value**: 
```shell
$ sudo ./prog_user READ
```
* **Set Timer Mode (Write to 0x43 port)**: 
```shell
$ sudo ./prog_user SETMODE
SETMODE $182
```
* **Set Timer Sleep (in milliseconds)**: 
```shell
$ sudo ./prog_user SLEEP
SLEEP $3000
```

## License
* This project is licensed under the GNU License - see the LICENSE file for details.

