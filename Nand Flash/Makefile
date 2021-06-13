objs := head.o init.o NandFlash.o main.o

NandFlash.bin : $(objs)
	arm-linux-ld -Tnand.lds -o NandFlash_elf $^
	arm-linux-objcopy -O binary -S NandFlash_elf $@
	arm-linux-objdump -D -m arm NandFlash_elf > NandFlash.dis
	
%.o : %.c
	arm-linux-gcc -Wall -c -O2 -o $@ $<
	
%.o : %.s
	arm-linux-gcc -Wall -c -O2 -o $@ $<
	
clean:
	rm -f NandFlash.dis NandFlash.bin NandFlash_elf *.o