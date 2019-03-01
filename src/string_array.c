#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "string_array.h"


struct StringNode * sa_new(char* data) {
	struct StringNode* node = calloc(1, sizeof(struct StringNode));
	node->data = data;
	node->next = NULL;
	return node;
}

void sa_add(struct StringNode* head, struct StringNode* new_node) {
	if (head->next == NULL) {
		head->next = new_node;
	} else {
		struct StringNode* head_next = head->next;
		head->next = new_node;
		struct StringNode* search_node = new_node;
		while(search_node->next != NULL) {
			search_node = search_node->next;
		} search_node->next = head_next;
	}
}

struct StringNode * sa_search(struct StringNode* node, char* data) {
	assert(node);
	struct StringNode* search_node = node;

	do {
		if (!strcmp(search_node->data,data)) {
			return search_node;
		} search_node = search_node->next;
	} while(search_node != NULL);

	return NULL;
}

void sa_destroy(struct StringNode* head) {
	struct StringNode* current_node = head;
	do {
		struct StringNode* tmp = current_node->next;
		if (current_node->data)
			free(current_node->data);
		if (current_node)
			free(current_node);
		current_node = tmp;
	} while(current_node != NULL);
	head = NULL;
}

void sa_add_new(struct StringNode* head, char* data) {
	assert(head);
	struct StringNode* new_node = sa_new(data);
	sa_add(head, new_node);
}

void sa_edit(struct StringNode* head, char* search_data, char* new_data) {
	struct StringNode* node = sa_search(head, search_data);
	assert(node);
	node->data = new_data;
}

void sa_remove(struct StringNode** head, char* data) {
	struct StringNode* d_head = *head;
	assert(d_head && d_head->data);
	struct StringNode* current = d_head;
	struct StringNode* before = NULL;
	do {
		if (!strcmp(current->data,data)) {
			if (before) {
				before->next = current->next;
			} else {
				*head = current->next;
			}
		} 
		before = current;
		current = current->next;
	}while(current != NULL);
}

void sa_edit_prepend_all(struct StringNode* head, const char* const prep_string) {
	struct StringNode* current = head;
	while(current != NULL) {

		size_t current_data_len = strlen(current->data);
		size_t prep_string_len = strlen(prep_string);
		size_t temp_len;

		char* temp = calloc(current_data_len+1,sizeof(char));
		strcpy(temp, current->data);
		temp_len = strlen(temp);

		current->data = calloc(current_data_len+prep_string_len+1, sizeof(char));

		memcpy(current->data, prep_string, prep_string_len);
		memcpy(current->data+prep_string_len, temp, temp_len);

		current = current->next;
	}
}

// String utilities 

int string_starts_with(char* str, char* prefix) {
	assert(str && prefix);
	size_t str_len = strlen(str);
	size_t prefix_len = strlen(prefix);
	return str_len < prefix_len ? 0 : !strncmp(str, prefix, prefix_len);
}

int string_ends_with(char* str, char* suffix) {
	assert(str && suffix);
	size_t str_len = strlen(str);
	size_t suffix_len = strlen(suffix);
	return str_len < suffix_len ? 0 : !strcmp(str+str_len-suffix_len, suffix);
}
