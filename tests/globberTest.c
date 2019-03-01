#include "globberTest.h"
#include "macros.h"
#include "../src/globber.h"

void setup() {

}

void testRootPathFromLocalString() {
	assert_null(last_determinisitc_path_for_string("lello"));
}

void testRootPathFromActualPath() {
	assert_eq_str("/Users/h3xept/CS210/ACE4/", last_determinisitc_path_for_string("/Users/h3xept/CS210/ACE4/BareBonesReadline"));
}

void testRootPathFromActualPathWithGlobbingSymbols() {
	assert_eq_str("/Users/h3xept/CS210/", last_determinisitc_path_for_string("/Users/h3xept/CS210/*/BareBonesReadline"));
	assert_eq_str("/Users/h3xept/CS210/", last_determinisitc_path_for_string("/Users/h3xept/CS210/*"));
}

void testExpandString() {
	struct StringNode* node = expand_string("/Users/h3xept/Desktop/Uni_Drive/Year 2/Sem_2/CS210/ACE4/Ba*");

	while(node) {
		printf("%s\n",node->data);
		node = node->next;
	}

}

void testStringComponents() {
	struct StringComponents* components = components_with_globbing_symbol_index("leo*isglobbing",3);
	assert_eq_str("leo", components->before_gs);
	assert_eq_str("isglobbing", components->after_gs);
}


void teardown() {

}