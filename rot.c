#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>
#include <unistd.h>

#define ELCOUNT(b)	(b/sizeof(wchar_t))
#define	PROGNAME	"rot"

static void rotate(size_t n, wchar_t str[n], int nr)
{
	int start = nr % (int) n;
	start = start < 0 ? n  + start : start;
	size_t j = n - start + 1;

	if (start > 0) {
		wchar_t buf[n]; // = malloc(sizeof(wchar_t[n]));
		// if (!buf) {
		// 	perror("malloc");
		// 	exit(EXIT_FAILURE);
		// }

		memcpy(buf, &str[start], sizeof(wchar_t[j]));
		memcpy(&buf[j - 1], &str[0], sizeof(wchar_t[start]));
		memcpy(str, buf, sizeof(wchar_t[n]));
		// free(buf);
	}
}

static void showhelp(void)
{
	printf("Usage: %s [options]\n\n", PROGNAME);
	printf("Rotate string characters\n\n");
	printf("Options:\n");
	printf("%3s %30s\n", "-n", "Number of rotations");
	exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[argc])
{
	int opt_n, n = 0;
	extern char *optarg;
	FILE *fp = stdin;
	wchar_t buf[BUFSIZ];
	size_t count = ELCOUNT(BUFSIZ);

	while ((opt_n = getopt(argc, argv, "n:h")) != -1) {
		switch (opt_n) {
			case 'n':
				n = atoi(optarg);
			break;
			case 'h':
				showhelp();
			break;
			default:
			break;
		}
	}

	if (n == 0) {
		showhelp();
	}

	while (fgetws(buf, count, fp)) {
		size_t len = wcsnlen(buf, count);
		_Bool has_newline = 0;

		if (len == 1)
			goto PRINT;

		if (buf[len-1] == '\n') {
			buf[len - 1] = 0;
			len--;
			has_newline = 1;
		}
		
		rotate(len, buf, n);

PRINT:
		fprintf(stdout, "%ls", buf);
		if (has_newline) fprintf(stdout, "\n");
		fflush(stdout);
	}
}