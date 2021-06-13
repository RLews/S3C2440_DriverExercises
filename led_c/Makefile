led.bin : crt0.s led.c
	arm-linux-gcc -g -c -o crt0.o crt0.s
	arm-linux-gcc -g -c -o led.o led.c
	arm-linux-ld -Ttext 0x0000000 -g crt0.o led.o -o led_elf
	arm-linux-objcopy -O binary -S led_elf led.bin
	arm-linux-objdump -D -m arm led_elf > led.dis
clean:
	rm -f led.dis led.bin led_elf *.o