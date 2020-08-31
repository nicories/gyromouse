cc = arm-none-eabi-gcc -gdwarf-4 -g3 -mcpu=cortex-m0 -mthumb -Wall  -g -I include
db = arm-none-eabi-gdb
default_file = level

FILES = src/head.o src/levelmap.o src/usart.o src/level.o src/i2c.o src/led.o src/buttons.o

.PHONY: all
all: $(default_file).bin


%.bin: %.elf
	arm-none-eabi-objcopy -O binary -S $< $@

%.elf: $(FILES)
	$(cc) -nostartfiles -Wl,-Ttext=0x8000000 -o $@ $(FILES)

src/head.o: src/head.S
	$(cc) -c -o $@ $<

%.o: %.c
	$(cc) -c -o $@ $<

upload: $(default_file).elf
	openocd -f nucleo-f0.cfg -c "init" -c "targets" -c "reset halt" -c "program level.elf reset exit"

.PHONY: clean
clean:
	rm *.bin *.elf levelmap_test

.PHONY: openocd
openocd: $(default_file).elf nucleo-f0.cfg
	openocd -f nucleo-f0.cfg

.PHONY: debug
debug: $(default_file).elf
	$(db)  -ex "target remote localhost:3333" \
	-ex "source ~/.local/lib/python3.8/site-packages/cmdebug/svd_gdb.py" \
	-ex "svd_load STM32F042x.svd" $<

.PHONY: test
test: test/levelmap_test.c src/levelmap.c 
	gcc $^ -o levelmap_test
	./levelmap_test
