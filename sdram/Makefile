sdram.bin : head.s led.c
	arm-linux-gcc -c -o head.o head.s
	arm-linux-gcc -c -o led.o led.c
	arm-linux-ld -Ttext 0x30000000 head.o led.o -o sdram_elf
	arm-linux-objcopy -O binary -S sdram_elf sdram.bin
	arm-linux-objdump -D -m arm sdram_elf > sdram.dis
clean:
	rm -f sdram.dis sdram.bin *.o sdram_elf