/**
 * rot.c - rotate strings
 * 
 *  (C) 2021	James John <me@donjajo.com>
 * 
 * License: GPL3
 * 
*/
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>
#include <unistd.h>

#define ELCOUNT(b)	(b/sizeof(wchar_t))
#define	PROGNAME	"rot"

static void rotate_string(size_t n, wchar_t str[n], int nr, size_t start_index)
{
	int start = (start_index + nr) % (int) n;
	start = start < 0 ? n  + start : start;
	size_t j = n - start + 1;

	if (start > 0) {
		wchar_t buf[n];

		memcpy(buf, &str[start], sizeof(wchar_t[j]));
		memcpy(&buf[j - 1], &str[0], sizeof(wchar_t[start]));
		memcpy(str, buf, sizeof(wchar_t[n]));
	}
}

static void rotate_strings(size_t n, wchar_t *strs[n], int nr, size_t start_index)
{
	int j = (start_index + nr) % (int) n;
	j = j < 0 ? n  + j : j;
	size_t i;

	if (j > 0) {
		wchar_t *buf[n];
		for (i = 0; i < n; i++, j++) {
			if (j >= n)
				j = 0;
			buf[i] = strs[j];
		}

		for (int k = 0; k < n; k++) {
			strs[k] = buf[k];
		}
	}
}

static void showhelp(void)
{
	printf("Usage: %s [options]\n\n", PROGNAME);
	printf("Rotate string characters in n times. Index starts from 0\n\n");
	printf("Options:\n");
	printf("%3s=NO. OF ROTATIONS %25s\n", "-n", "Number of rotations");
	printf("%3s %37s\n", "-h", "Show help menu");
	printf("%3s=DELIMITER %30s\n", "-d", "Strings delimiter");
	printf("%3s=INDEX %63s\n", "-s", "Start index. i.e. Rotate starting from s index");
	printf("%3s=INDEX %42s\n", "-r", "Return only r index value");
	exit(EXIT_SUCCESS);
}

size_t to_wchar(char const* chars, wchar_t** buf) {
	size_t len = mbstowcs(0, chars, 0);
	if (len == (size_t) -1) {
		perror("mbstowcs");
		exit(EXIT_FAILURE);
	}

	if (len == 0)
		return 0;

	if (!*buf) {
		*buf = calloc(len+1, sizeof(wchar_t));
		if (!*buf) {
			perror("calloc");
			exit(EXIT_FAILURE);
		}
	}

	mbstowcs(*buf, chars, len);
	return len;
}

size_t get_split_count(size_t n, wchar_t str[n], size_t delim_n, const wchar_t delim[delim_n])
{
	size_t c = 0;
	for (size_t i = 0; (i + delim_n - 1) < n; i++) {
		if (wcsncmp(&str[i], delim, delim_n) == 0) {
			i += delim_n - 1;
			c++;
		}
	}
	return c;
}

void _alcp(wchar_t **buf, size_t i, size_t sz, wchar_t *str)
{
	buf[i] = calloc(sz+1, sizeof(wchar_t));
	if (!buf[i]) {
		perror("calloc");
		exit(EXIT_FAILURE);
	}

	memcpy(buf[i], str, sizeof(wchar_t[sz]));
}

wchar_t **split(size_t n, wchar_t str[n], size_t delim_n, const wchar_t delim[delim_n], size_t *ret_count)
{
	size_t split_count = n / delim_n; // get_split_count(n, str, delim_n, delim);
	if (!split_count)
		return 0;

	wchar_t **buf = calloc(split_count, sizeof(wchar_t*));
	size_t i = 0, j = 0, k = 0;

	if (!buf) {
		perror("calloc");
		exit(EXIT_FAILURE);
	}

	for (; i < n; i++) {
		size_t l = i - j;
		if (wcsncmp(&str[i], delim, delim_n) == 0) {
			if (l > 0) {
				// allocate and copy
				_alcp(buf, k, l, &str[j]);
				k++;
			}
			i += delim_n - 1;
			j = i+1;
		}
	}

	if (j < n) {
		_alcp(buf, k, n - j, &str[j]);
		k++;
	}

	if (!k) {
		free(buf);
		return 0;
	}

	*ret_count = k;
	return buf;
}

void print_r(size_t n, wchar_t **ret, wchar_t *delim)
{
	for (size_t i = 0; i < n; i++) {
		fputws(ret[i], stdout);
		if (i + 1 < n)
			fputws(delim, stdout);
	}
}

struct opts {
	int n;			// Number of rotations
	wchar_t	*delim;		// Delimiter of strings
	size_t	delim_n;	// Delimiter length
	size_t	s;		// Start index
	size_t	rn;		// Return the (rn)th value
};

int main(int argc, char *argv[argc])
{
	int opt_n;
	struct opts opts = {
		.n		= 0,
		.delim		= 0,
		.delim_n	= 0,
		.s		= 0,
		.rn		= -1
	};
	extern char *optarg;
	FILE *fp = stdin;
	wchar_t buf[BUFSIZ];
	size_t count = ELCOUNT(BUFSIZ);

	while ((opt_n = getopt(argc, argv, "n:hd:s:r:")) != -1) {
		switch (opt_n) {
			case 'n':
				opts.n = atoi(optarg);
			break;
			case 'h':
				showhelp();
			break;
			case 'd':
				opts.delim_n = to_wchar(optarg, &opts.delim);
			break;
			case 's':
				opts.s = atoi(optarg);
			break;
			case 'r':
				opts.rn = atoi(optarg);
			break;
			case ':':
				showhelp();
			break;
			default:
				showhelp();
			break;
		}
	}

	if (opts.n == 0) {
		showhelp();
	}

	while (fgetws(buf, count, fp)) {
		size_t len = wcsnlen(buf, count);
		_Bool has_newline = 0;
		size_t ret_count = 0;
		wchar_t **split_buf;

		if (len == 1)
			goto PRINT;

		if (buf[len-1] == '\n') {
			buf[len - 1] = 0;
			len--;
			has_newline = 1;
		}

		if (opts.delim_n == 0) {
			rotate_string(len, buf, opts.n, opts.s);
		} else {
			split_buf = split(len, buf, opts.delim_n, opts.delim, &ret_count);
			if (ret_count) {
				rotate_strings(ret_count, split_buf, opts.n, opts.s);
			}
		}

PRINT:
		if (ret_count) {
			if (opts.rn != (size_t) -1 && opts.rn < ret_count) {
				fputws(split_buf[opts.rn], stdout);
			} else {
				print_r(ret_count, split_buf, opts.delim);
			}
		} else {
			if (opts.rn != (size_t) -1 && opts.rn < len)
				fputwc(buf[opts.rn], stdout);
			else
				fputws(buf, stdout);
		}

		if (has_newline) fprintf(stdout, "\n");
		fflush(stdout);
	}
}