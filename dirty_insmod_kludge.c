// should be used when some build scripts requires "insmod loop",
// but your system is modern enough, so that call is redundant will return non-zero value.

#include <unistd.h>
#include <string.h>
#include <iso646.h>

int main(int argc, char **argv) {
	if (argc == 2 and strcmp(argv[1], "loop") == 0) return 0;
	execv("/sbin/insmod_real", argv); // real insmod should me moved to insmod_real.
	return 0;
}

// c99 dirty_insmod_kludge.c -o insmod -Wall -Wextra -Werror -s
