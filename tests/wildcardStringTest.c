#include "wildcardStringTest.h"
#include "../src/wildcard_string.h"
#include "macros.h"

void setup() {
	return;
}

void teardown() {
	return;
}

void testCreateWildcardString() {
	WildcardString* string = ws_new("leo*po?popopo");
	assert_eq_str("leo",string->before_gs);
	assert_eq_str("*",string->gs);
	assert_eq_str("po",string->next->before_gs);
	assert_eq_str("?",string->next->gs);
	assert_eq_str("popopo",string->next->next->before_gs);
	assert_null(string->next->next->before_gs);
}