.PHONY: all clean permissions

USER := $(shell whoami)

all: part1 part2 part3 part4 part5 part6

CC := gcc
CFLAGS  := -ggdb -m32 -static -U_FORTIFY_SOURCE -fomit-frame-pointer -DCOOKIE=$(shell cat cookie)

part1: CFLAGS += -fno-stack-protector -z execstack
part2: CFLAGS += -fno-stack-protector -z execstack
part3: CFLAGS += -fno-stack-protector -z execstack
part4: CFLAGS += -fno-stack-protector -z execstack
part5: CFLAGS += -fno-stack-protector
part6: CFLAGS += -fno-stack-protector -z execstack

part1: part1.c

part2: part2.c

part3: part3.c helper.c

part4: part4.c helper.c

part5: part5.c helper.c

part6: part6.c helper.c

clean:
	rm -f part1 part2 part3 part4 part5 part6

