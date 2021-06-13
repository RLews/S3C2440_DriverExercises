objs := head.o init.o uart.o main.o

uart.bin : $(objs)
	arm-linux-ld -Tuart.lds -o uart_elf $^
	arm-linux-objcopy -O binary -S uart_elf $@
	arm-linux-objdump -D -m arm uart_elf > uart.dis
	
%.o : %.c
	arm-linux-gcc -Wall -O2 -c -o $@ $<
	
%.o : %.s
	arm-linux-gcc -Wall -O2 -c -o $@ $<
	
clean:
	rm -f uart.bin uart.dis uart_elf *.o
