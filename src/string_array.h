#ifndef __STRING_ARRAY__
#define __STRING_ARRAY__

typedef int Function ();

struct StringNode {
	char* data;
	struct StringNode* next;
};

struct StringNode* sa_new(char* data);
void sa_add(struct StringNode* node, struct StringNode* new_node);
void sa_add_new(struct StringNode* head, char* data);
struct StringNode* sa_search(struct StringNode* node, char* data);
void sa_destroy(struct StringNode* head);
void sa_edit(struct StringNode* head, char* search_data, char* new_data);
void sa_remove(struct StringNode** head, char* data);
void sa_edit_prepend_all(struct StringNode* head, const char* const prep_string);

int string_starts_with(char* str, char* prefix);
int string_ends_with(char* str, char* suffix);

#endif
