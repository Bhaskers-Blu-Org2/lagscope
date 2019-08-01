#include "common.h"

#ifdef _WIN32
char* optarg = NULL;

int getopt(int argc, char *const argv[], const char *optstr)
{
	static int optind = 1;

	if ((optind >= argc) || (argv[optind][0] != '-') || (argv[optind][0] == 0))
		return -1;

	int opt = argv[optind][1];
	const char *p = strchr(optstr, opt);

	if (p == NULL)
		return '?';

	if (p[1] == ':') {
		optarg = &argv[optind][2];
		if (optarg[0] == 0)
			optarg = NULL;
	}

	optind++;
	return opt;
}

int gettimeofday(struct timeval * tp, struct timezone * tzp)
{
	static const uint64_t EPOCH = ((uint64_t) 116444736000000000ULL);

	SYSTEMTIME  system_time;
	FILETIME    file_time;
	uint64_t    time;

	GetSystemTime( &system_time );
	SystemTimeToFileTime( &system_time, &file_time );
	time =  ((uint64_t)file_time.dwLowDateTime );
	time += ((uint64_t)file_time.dwHighDateTime) << 32;

	tp->tv_sec  = (long) ((time - EPOCH) / 10000000L);
	tp->tv_usec = (long) (system_time.wMilliseconds * 1000);
        return 0;
}

static int vasprintf(char **strp, const char *format, va_list ap)
{
	int len = _vscprintf(format, ap);
	if (len == -1)
		return -1;
	char *str = (char*)malloc((size_t) len + 1);
	if (!str)
		return -1;
	int rv = vsnprintf(str, len + 1, format, ap);
	if (rv == -1) {
		free(str);
		return -1;
	}
	*strp = str;
	return rv;
}

int asprintf(char **strp, const char *format, ...)
{
	va_list ap;
	va_start(ap, format);
	int rv = vasprintf(strp, format, ap);
	va_end(ap);
	return rv;
}

#endif
