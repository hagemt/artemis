#include <assert.h>

/* TODO(teh) make sure this works to same spec */
#ifdef WINDOWS
#warning "modules are not yet supported on Windows"

#include <windows.h>

LOG_INTERNAL int
log_module_alpha(Module *module)
{
	assert(module);
	if (!module->key) {
		assert(module->count == 0);
		module->key = LoadLibrary(module->name);
		if (!module->key) return 1;
	}
	module->value = GetProcAddress(module->key, module->name);
	if (!module->value) return 1;
	++module->count;
	return 0;
}

LOG_INTERNAL int
log_module_omega(Module *module)
{
	assert(module);
	if (--module->count == 0) {
		return FreeLibrary(name);
	}
	return 0;
}

#endif /* WINDOWS */
