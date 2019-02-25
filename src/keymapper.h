#ifndef __KEY_MAPPER__
#define __KEY_MAPPER__

typedef int Function ();

struct KeyMap {
	int key;
	Function* function;
	struct KeyMap* next;
};

struct KeyMap* km_new(int key, Function* function);
void km_add(struct KeyMap* node, struct KeyMap* new_node);
struct KeyMap* km_search(struct KeyMap* node, int key);
void km_destroy(struct KeyMap* head);

#endif
