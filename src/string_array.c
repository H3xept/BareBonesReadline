#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "string_array.h"
#include "string_utils.h"

struct StringNode * sa_new(const char* const data) {
	assert(data);
	struct StringNode* node = calloc(1, sizeof(struct StringNode));
	node->data = calloc(strlen(data)+1, sizeof(char));
	strcpy(node->data, data);
	node->next = NULL;
	return node;
}

void sa_add(struct StringNode* head, struct StringNode* new_node) {
	assert(head);
	if (!new_node) { return; }

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

void sa_append(struct StringNode* node, struct StringNode* new_node) {
	assert(node);
	if (!new_node) { return; }
	struct StringNode* current = node;
	while(current->next != NULL) { current = current->next; }
	current->next = new_node;
}

struct StringNode * sa_search(struct StringNode* node, const char* const data) {
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

void sa_add_new(struct StringNode* head, const char* const data) {
	assert(head);
	struct StringNode* new_node = sa_new(data);
	sa_add(head, new_node);
}

void sa_edit(struct StringNode* head, char* search_data, const char* const new_data) {
	struct StringNode* node = sa_search(head, search_data);
	assert(node);
	
	if (node->data) {
		free(node->data);
		node->data = 0;
	}
	
	node->data = calloc(strlen(new_data)+1, sizeof(char));
	strcpy(node->data, new_data);
}

void sa_remove(struct StringNode** head, const char* const data) {
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
	if (!prep_string) return;
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

static int number_of_strings(struct StringNode* head) {
	int ret = 0;
	struct StringNode* curr = head;
	while(curr) {
		ret++;
		curr = curr->next;
	}
	return ret;
}

static char* naive_concat(struct StringNode* head) {
	struct StringNode* curr = head;
	char* ret = NULL;
	while(curr) {
		if (!ret) {
			ret = calloc(strlen(curr->data)+1, sizeof(char));
			strcpy(ret, curr->data);
		} else {
			ret = realloc(ret, strlen(ret)+strlen(curr->data)+1);
			strcat(ret, curr->data);
		}
		curr = curr->next;
	}
	return ret;
}

char* sa_concat(struct StringNode* head, const char symbol) {
	
	if (!symbol) {
		return naive_concat(head);
	}

	char* ret_str = NULL;
	struct StringNode* curr = head;
	while(curr) {
		int has_next = (curr->next != NULL);
		if (!ret_str) {
			ret_str = calloc(strlen(curr->data)+1+has_next, sizeof(char));
			strcpy(ret_str, curr->data);
			if (has_next) {
				*(ret_str+strlen(curr->data)) = symbol;
			}
		} else {
			size_t ret_str_len = strlen(ret_str);
			ret_str = realloc(ret_str, (strlen(ret_str)+strlen(curr->data)+1+has_next)*sizeof(char));
			strcat(ret_str, curr->data);
			if (has_next) {
				*(ret_str+ret_str_len+strlen(curr->data)) = symbol;
			}
			*(ret_str+ret_str_len+strlen(curr->data)+has_next) = '\0';
		}
		curr = curr->next;
	} return ret_str;
}

char* sa_get_shortest(const struct StringNode* const head) {
	assert(head);
	char* shortest = NULL;

	const struct StringNode* curr = head;
	while(curr) {
		if (!shortest) {
			shortest = curr->data;
		} else if (strlen(shortest) > strlen(curr->data)) {
			shortest = curr->data;
		} curr = curr->next;
	} return shortest;

}

int sa_get_size(const struct StringNode* const head) {
	assert(head);
	int len = 0;

	const struct StringNode* curr = head;
	while(curr) {
		len++;
		curr = curr->next;
	} return len;
}

void sa_escape_all_spaces(struct StringNode* head) {
	struct StringNode* curr = head;
	while(curr) {
		if (!curr->data) goto next_it;
		char* temp = su_replace_occurrencies_of(curr->data, " ", "\\ ");
		free(curr->data);
		curr->data = temp;
next_it: 
		curr = curr->next;
	}
}

void sa_escape_non_escaped_spaces(struct StringNode* head) {

	struct StringNode* curr = head;
	while(curr) {
		if (curr->data) {
			char* new_data = su_escape_spaces(curr->data);
			free(curr->data);
			curr->data = new_data;
		}
		curr = curr->next;
	}
}
