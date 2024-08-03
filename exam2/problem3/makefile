all: two_person_jobs.bin

CFLAGS+=-Wall -ggdb
LDLIBS+=-pthread

.PHONY:
	clean all

%.bin: %.o
	${CC} ${CFLAGS} ${LDLIBS} -o $@ $^

clean:
	rm -rf *.o *.bin files/*.txt
