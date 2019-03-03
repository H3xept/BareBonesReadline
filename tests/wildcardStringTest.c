#include "wildcardStringTest.h"
#include "../src/globbing/wildcard_string.h"
#include "macros.h"

void setup() {
	return;
}

void teardown() {
	return;
}

void testCreateWildcardString() {
	WildcardString* string = ws_new("leo*po?popopo",0);
	assert_eq_str("leo",string->before_gs);
	assert_eq_int(0, string->index);
	assert_eq_str("*",string->gs);
	assert_eq_str("po",string->next->before_gs);
	assert_eq_int(-4, string->next->index);
	assert_eq_str("?",string->next->gs);
	assert_eq_str("popopo",string->next->next->before_gs);
	assert_eq_int(7, string->next->next->index);
	assert_null(string->next->next->next);
}

void testCreateWildcardStringEndingWild() {
	WildcardString* string = ws_new("leo*",0);
	assert_eq_str("leo",string->before_gs);
	assert_eq_int(0, string->index);
	assert_eq_str("*",string->gs);
	assert_null(string->next);
}

void testCreateWildcardStringJustWild() {
	WildcardString* string = ws_new("*",0);
	assert_null(string->before_gs);
	assert_eq_int(0, string->index);
	assert_eq_str("*",string->gs);
	assert_null(string->next);
}