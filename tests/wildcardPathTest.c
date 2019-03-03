#include "wildcardPathTest.h"
#include "../src/globbing/wildcard_path.h"
#include "macros.h"

void setup() {
	return;
}

void teardown() {
	return;
}

void testCreateWildcardPath() {
	WildcardPath* test = wp_new("path0/path*1/path2*");
	assert_not_null(test);
	assert_not_null(test->next);
	assert_not_null(test->next->next);
}

void testCreateWildcardPathEndingSlash() {
	WildcardPath* test = wp_new("path0/path*1/path2*/");
	assert_not_null(test);
	assert_not_null(test->next);
	assert_not_null(test->next->next);
	assert_not_null(test->next->next->next);
	assert_null(test->next->next->next->next);
}

void testCreateOnlyWildcardsPath() {
	WildcardPath* test = wp_new("*/*");
	assert_not_null(test);
	assert_not_null(test->next);
}