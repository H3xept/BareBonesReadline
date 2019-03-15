#include "../src/history_parser.h"
#include "macros.h"
#include <stdlib.h>
#include <BareBonesHistory/history.h>

void setup() {
	init_history();
	add_history_entry("Entry 3");
	add_history_entry("Entry 2");
	add_history_entry("Entry 1");
}

void teardown() {
	return;
}

void testLastInvocation() {
	char* test = "!!";
	assert_eq_str("Entry 1", ht_parse(test));
}

void testPositiveNumberInvocation() {
	char* test = "!1";
	assert_eq_str("Entry 3", ht_parse(test));	
}

void testNegativeNumberInvocation() {
	char* test = "!-1";
	assert_eq_str("Entry 1", ht_parse(test));	
}

void testNonExistingInvocation() {
	assert_null(ht_parse("!345676543"));
}

void testSingleBang() {
	assert_null(ht_parse("!"));
}
