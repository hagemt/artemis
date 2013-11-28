#include <assert.h>
#include <dlfcn.h>

#include "log.h"
#include "module.h"

static int
check_file(char *path);

LOG_INTERNAL int
log_module_alpha(Module *module)
{
	assert(module);
	if (!module->key) {
		assert(module->count == 0);
		if (check_file(module->name)) {
			return 1;
		}
		/* Load via dynamic link */
		module->key = dlopen(module->name, RTLD_LAZY | RTLD_LOCAL);
		/* FIXME(teh) ugh... too late... dlerror() */
		if (!module->key) {
			LOGE("%s: %s", "dlerror", dlerror());
			return 1;
		}
	}
	/* Load the symbol */
	module->value = dlsym(module->key, module->name);
	if (!module->value) {
		LOGE("%s: %s", "dlerror", dlerror());
		return 1;
	}
	/* TODO(teh) shouldn't increment if already loaded symbol? */
	++module->count;
	return 0;
}

LOG_INTERNAL int
log_module_omega(Module *module)
{
	assert(module);
	/* FIXME(teh) need to invalidate symbols? */
	if (--module->count == 0) {
		return dlclose(module->key);
	}
	return 0;
}

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

static int
check_file(char *path)
{
	struct stat info;
	return lstat(path, &info) || !S_ISREG(info.st_mode);
}
