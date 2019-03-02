#include "wildcardPathTest.h"
#include "../src/wildcard_path.h"
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

