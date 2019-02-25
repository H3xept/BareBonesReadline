#include "keymapperTest.h"
#include "../src/keymapper.h"
#include "macros.h"
#include <stdlib.h>

struct KeyMap* head;

static int function1() {
	return 1;
}

static int function2() {
	return 2;
}

static int function3() {
	return 3;
}

void setup() {
	head = km_new(1, function1);
}

void teardown() {

}

void testReadHead() {
	assert_eq_int(head->function(), 1);
}

void testInsert() {
	struct KeyMap* new_node = km_new(2,function2);
	km_add(head, new_node);
	assert_eq_int(head->next->function(), 2);
}

void testMiddleInsert() {
	struct KeyMap* new_node = km_new(2,function2);
	km_add(head, new_node);
	new_node = km_new(3,function2);
	km_add(head->next, new_node);
	new_node = km_new(4,function3);
	km_add(head, new_node);
	assert_eq_int(head->function(), 1);
	assert_eq_int(head->next->function(), 3);
	assert_eq_int(head->next->next->function(), 2);
	assert_eq_int(head->next->next->next->function(), 2);
}

void testSearch() {
	struct KeyMap* new_node = km_new(2,function2);
	km_add(head, new_node);
	new_node = km_new(3,function2);
	km_add(head->next, new_node);
	new_node = km_new(4,function3);
	km_add(head, new_node);
	assert_eq_int(km_search(head, 4)->function(), 3);
}
