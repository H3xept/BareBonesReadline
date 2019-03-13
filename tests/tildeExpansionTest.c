#include "../src/tilde_expansion.h"
#include "macros.h"
#include "tildeExpansionTest.h"

void setup() {
	return;
}

void teardown() {
	return;
}

void testExpandTildeHome() {
	char* to_expand = "~/Leonardo";
	char expected[100] = {0x0};
	char* expanded = expand_tildes(to_expand);
	sprintf(expected, "%s/Leonardo", getenv("HOME"));
	assert_eq_str(expected, expanded);
}
