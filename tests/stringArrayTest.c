#include "stringArrayTest.h"
#include "../src/string_array.h"
#include "macros.h"

struct StringNode* head;

void setup() {
	head = sa_new("Hello");
}

void teardown() {
	return;
}

void testReadHead() {
	assert_eq_str("Hello",head->data);
}

void testInsert() {
	struct StringNode* new_node = sa_new("world");
	sa_add(head, new_node);
	assert_eq_str("world",head->next->data);
}

void testMiddleInsert() {
	// Hello mighty world nice
	struct StringNode* new_node = sa_new("world");
	sa_add(head, new_node);
	new_node = sa_new("nice");
	sa_add(head->next, new_node);
	new_node = sa_new("mighty");
	sa_add(head, new_node);

	assert_eq_str(head->next->data, "mighty");
	assert_eq_str(head->next->next->data, "world");
	assert_eq_str(head->next->next->next->data, "nice");
}

void testSearch() {
	struct StringNode* new_node = sa_new("world");
	sa_add(head, new_node);
	assert_eq_str(sa_search(head, "world")->data, "world");
}

void testedit() {
	struct StringNode* new_node = sa_new("world");
	sa_add(head, new_node);
	sa_edit(head, "world","moon");
	assert_eq_str(sa_search(head, "moon")->data, "moon");
}

void testRemove() {
	struct StringNode* new_node = sa_new("mighty");
	sa_add(head, new_node);
	new_node = sa_new("world");
	sa_add(head, new_node);
	sa_remove(&head, "mighty");
	assert_eq_str(head->data, "Hello");
	assert_eq_str(head->next->data, "world");
	sa_remove(&head, "Hello");
	assert_eq_str("world", head->data);
}

void testPrependAll() {
	struct StringNode* new_node = sa_new("mighty");
	sa_add(head, new_node);
	sa_edit_prepend_all(head, "path/");
	assert_eq_str(head->data, "path/Hello");
	assert_eq_str(head->next->data, "path/mighty");
}

void testToString() {
	struct StringNode* new_node = sa_new("world");
	sa_add(head, new_node);
	new_node = sa_new("!");
	sa_add(head->next, new_node);
	assert_eq_str("Hello world !",sa_concat(head, ' '));
}

void testAppend() {
	struct StringNode* new_node = sa_new("world");
	sa_append(head, new_node);
	new_node = sa_new("!");
	sa_append(head, new_node);
	assert_eq_str(head->next->data, "world");
	assert_eq_str(head->next->next->data, "!");
}
