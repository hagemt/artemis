#include "log.h"

#include <assert.h>

#ifdef IS_NOT_POSIX /* FIXME */

#include <windows.h>

static FARPROC
__load_dll(char *name)
{
	HMODULE module;
	FARPROC fp = NULL;
	if ((module = LoadLibrary(name))) {
		fp = GetProcAddress(module, name);
	}
	return fp;
}

log_function_t
load(char *name)
{
	log_function_t fp;
	assert(name);
	fp = (log_function_t) __load_dll(name);
	/* TODO FreeLibrary (later) mechanism */
	return fp;
}

#else /* POSIX */

#include <dlfcn.h>

#define MAX_NAME 4096

/*
#include <stdio.h>

log_function_t
load(char *name)
{
	void *module;
	log_function_t fp;
	char fullname[MAX_NAME];
	snprintf(fullname, "lib%s.so", name);
	if ((module = dlopen(fullname, RTLD_LAZY | RTLD_LOCAL))) {
		fp = (log_function_t) dlsym(module, name);
		if (dlclose(module)) {
			fprintf(stderr, "%s\n", dlerror());
		}
	}
	return fp;
}
*/

#include <string.h>

log_function_t
load(char *name)
{
	log_function_t fp;
	void *module, *symbol;
	if ((module = dlopen(name, RTLD_LAZY | RTLD_LOCAL))) {
		symbol = dlsym(module, name);
		memcpy(&fp, &symbol, sizeof(fp));
		if (dlclose(module)) {
			/* FIXME dlclose failed ? log dlerror() */
		}
	}
	return fp;
}

#endif /* POSIX */
