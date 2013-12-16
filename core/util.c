#include "artemis_util.h"

LART_PRIVATE size_t
identity(size_t numel) {
	/* TODO(teh) optimize based on numel */
	return numel;
}

#include <assert.h>
#define CHECK(LV, FN, ...) { LV = FN(__VA_ARGS__); assert(LV); }

#include <libcalg/hash-table.h>

#include "artemis_wrap.h"

LART_PRIVATE Set *
singleton(Record *r, Entry *e)
{
	Set *s;
	int result;
	assert(r && r->table && e);
	/* Create a new singleton set, unique on path */
	CHECK(s, set_new, &wrap_hash_entry_path, &wrap_equals_entry_path);
	/* FIXME? set_register_free_function(s, &free_entry); */
	CHECK(result, set_insert, s, e);
	/* TODO(teh) make this all atomic / sychronize on r->table? */
	CHECK(result, hash_table_insert, r->table, e, s);
	/* FIXME should guarrantee s? */
	return s;
}
