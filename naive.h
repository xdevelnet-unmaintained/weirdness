// naive implementation of well-know functions from libc

void *naive_memmove(void *dest, void *src, size_t n) {
	if (dest == NULL or src == NULL or dest == src or n == 0) return dest;

	char *writepoint = dest;
	char *readpoint = src;
	size_t ne = 0;

	if (src < dest) while (n--) {
		writepoint[n] = readpoint[n];
	}
	else while (ne != n) {
		writepoint[ne] = readpoint[ne];
		ne++;
	}
	return dest;
}
