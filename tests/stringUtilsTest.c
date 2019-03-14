#include "stringUtilsTest.h"
#include "../src/string_utils.h"
#include "macros.h"

void setup() {
	return;
}

void teardown() {
	return;
}

void testReplaceInnerStr() {
	char* test = "Hello mr World!";
	char* replaced = su_replace_occurrencies_of(test, " ", "-@-");
	assert_eq_str("Hello-@-mr-@-World!", replaced);
}

void testEscapedString() {
	char* test = "Hi\\ leo";
	assert_eq_str(test, su_escape_spaces(test));
}
