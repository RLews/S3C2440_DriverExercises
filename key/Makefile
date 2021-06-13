key.bin : crt0.s key.c
	arm-linux-gcc -g -c -o crt0.o crt0.s
	arm-linux-gcc -g -c -o key.o key.c
	arm-linux-ld -Ttext 0x0000000 -g crt0.o key.o -o key_elf
	arm-linux-objcopy -O binary -S key_elf key.bin
	arm-linux-objdump -D -m arm key_elf > key.dis
clean:
	rm -f key.bin key.dis *.o key_elf