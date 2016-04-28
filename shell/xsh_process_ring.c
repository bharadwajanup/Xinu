/*#include <xinu.h>
#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_RING_SIZE 4

static volatile int32 count;

void ring_element(int32 id);

shellcmd xsh_process_ring(int32 argc, char** argv) {
	if (argc < 2) {
		printf("Too few arguments.\n");
		printf("Usage: process_ring count [ring_size]\n");
		return 0;

	} else if (argc > 3) {
		printf("Too many arguments.\n");
		printf("Usage: process_ring count [ring_size]\n");
		return 0;
	}

	count = atoi(argv[1]);
	uint16 ring_size = (argc == 3) ? atoi(argv[2]) : DEFAULT_RING_SIZE;

	for (int16 id = ring_size; --id >= 0;) {
		resume(create(ring_element, 1024, 20, "ring_element", 1, id));
	}

	// FIXME: The function should wait until count is zero, print "ZERO!\n", and then return.

	return 0;
}

void ring_element(int32 id) {
	while (count != 0) {
		printf("Ring element %d -- Value %d\n", id, count--);
	}
}
*/