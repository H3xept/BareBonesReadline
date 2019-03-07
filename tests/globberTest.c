#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include "globberTest.h"
#include "macros.h"
#include "../src/globber.h"

#define GLOBBER_TEST_FOLDER "/var/tmp/globberTest"
#define GLOBBER_TEST_SUBFOLDER "/var/tmp/globberTest/subfolder"
#define test_file "/test.txt"
#define test_file2 "/wow.c"

static int assert_contains_str(struct StringNode* head, char* str) {
	struct StringNode* curr = head;
	while(curr) {
		if (!strcmp(curr->data, str)) {
			return 1;
		}
		curr = curr->next;
	} return 0;
}

static FILE* create_file(char* fname) {
	FILE* f = fopen(fname, "w");
	if (!f) {
		printf("Error in setup %s\n",fname);
		return NULL;
	}
	return f;
}

static int number_of_strings(struct StringNode* node) {
	int ret = 0;
	struct StringNode* curr = node;
	while(curr) {
		ret++;
		curr = curr->next;
	}
	return ret;
}

static void write_and_close_to_file(FILE* f, char* format, ...) {
	assert(f);
	assert(format);

    va_list args;
    va_start(args, format);
    vfprintf(f, format, args);
    va_end(args);
    fclose(f);
}


void setup() {
	mkdir(GLOBBER_TEST_FOLDER, ACCESSPERMS);
	mkdir(GLOBBER_TEST_SUBFOLDER, ACCESSPERMS);

	FILE* f = create_file(GLOBBER_TEST_FOLDER test_file);
	write_and_close_to_file(f, "Hello world!\n");

	f = create_file(GLOBBER_TEST_SUBFOLDER test_file);
	write_and_close_to_file(f, "Hello world, but in subfolder!\n");

	f = create_file(GLOBBER_TEST_SUBFOLDER test_file2);
	write_and_close_to_file(f, "// TOTALLY A C FILE\n");
}

void testRootPathFromLocalString() {
	assert_null(last_determinisitc_path_for_string("lello"));
}

void testRootPathFromActualPath() {
	assert_eq_str(GLOBBER_TEST_FOLDER "/", last_determinisitc_path_for_string(GLOBBER_TEST_SUBFOLDER));
}

void testRootPathFromActualPathWithStarGlobbingSymbols() {
	assert_eq_str(GLOBBER_TEST_FOLDER"/", last_determinisitc_path_for_string(GLOBBER_TEST_FOLDER "/*/FOLDERINO"));
	assert_eq_str(GLOBBER_TEST_FOLDER"/", last_determinisitc_path_for_string(GLOBBER_TEST_FOLDER "/*"));
}

void testRootPathWithOnlyStarGlobbingSymbols() {
	assert_eq_str("/",last_determinisitc_path_for_string("/*/*"));
	assert_null(last_determinisitc_path_for_string("*/*"));
}

void testQuestionMarkGlobbingSymbol() {
	struct StringNode* nodes = NULL;
	nodes = expand_string(GLOBBER_TEST_SUBFOLDER"/*.?");
	assert_true(assert_contains_str(nodes, GLOBBER_TEST_SUBFOLDER test_file));
	assert_true(assert_contains_str(nodes, GLOBBER_TEST_SUBFOLDER test_file2));
	assert_eq_int(3, number_of_strings(nodes));
}

void testExpandString() {
	struct StringNode* node = NULL;
	node = expand_string(GLOBBER_TEST_FOLDER "/*");
	assert_true(assert_contains_str(node, GLOBBER_TEST_FOLDER test_file));
	node = expand_string(GLOBBER_TEST_FOLDER "/*/*");
	assert_true(assert_contains_str(node, GLOBBER_TEST_SUBFOLDER test_file));
}

void testExpandNonExpandableString() {
	struct StringNode* node = NULL;
	node = expand_string("not_existing_file*");
	assert_eq_str("not_existing_file*", node->data);
}

void testGlobLineSingleExpand() {
	char* globbed = glob_line("ls /var/tmp/g* not_expandable*");
	assert_eq_str("ls /var/tmp/globberTest not_expandable*", globbed);
}

void teardown() {

}
