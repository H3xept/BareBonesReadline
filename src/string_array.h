#ifndef __STRING_ARRAY__
#define __STRING_ARRAY__

struct StringNode {
	char* data;
	struct StringNode* next;
};

struct StringNode* sa_new(const char* const data);
void sa_add(struct StringNode* node, struct StringNode* new_node);
void sa_append(struct StringNode* node, struct StringNode* new_node);
void sa_add_new(struct StringNode* head, const char* const data);
struct StringNode* sa_search(struct StringNode* node, const char* const data);
void sa_destroy(struct StringNode* head);
void sa_edit(struct StringNode* head, char* search_data, const char* const new_data);
void sa_remove(struct StringNode** head, const char* const data);
void sa_edit_prepend_all(struct StringNode* head, const char* const prep_string);
char* sa_concat(struct StringNode* head, const char symbol);
char* sa_get_shortest(const struct StringNode* const head);
int sa_get_size(const struct StringNode* const head);
void sa_escape_all_spaces(struct StringNode* head);
void sa_escape_non_escaped_spaces(struct StringNode* head);

#endif
