#include <stdlib.h>
#include "keymapper.h"

struct KeyMap * km_new(int key, Function* function) {
	struct KeyMap* node = calloc(1, sizeof(struct KeyMap));
	node->key = key;
	node->function = function;
	node->next = NULL;
	return node;
}

void km_add(struct KeyMap* head, struct KeyMap* new_node) {
	if (head->next == NULL) {
		head->next = new_node;
	} else {
		struct KeyMap* head_next = head->next;
		head->next = new_node;
		struct KeyMap* search_node = new_node;
		while(search_node->next != NULL) {
			search_node = search_node->next;
		} search_node->next = head_next;
	}
}

struct KeyMap * km_search(struct KeyMap* node, int key) {
	int start_key = node->key;
	struct KeyMap* search_node = node;
	do {
		if (search_node->key == key) {
			return search_node;
		} search_node = search_node->next;
	} while(search_node->key != start_key);
	return NULL;
}

void km_destroy(struct KeyMap* head) {
	struct KeyMap* current_node = head;
	do {
		struct KeyMap* tmp = current_node->next;
		free(current_node);
		current_node = tmp;
	} while(current_node != NULL);
	head = NULL;
}
