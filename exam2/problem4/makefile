all: big_little_car.bin

CFLAGS+=-Wall -ggdb
LDLIBS+=-pthread

.PHONY:
	clean all

%.bin: %.o
	${CC} ${CFLAGS} ${LDLIBS} -o $@ $^

clean:
	rm -rf *.o *.bin files/*.txt
