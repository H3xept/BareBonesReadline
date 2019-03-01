#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <dirent.h> 

#include "globber.h"

static char globbing_symbols[] = {'*','?'};
static struct StringNode* matches;

static struct StringNode * resources_in_dir(const char* const det_path) {
	struct StringNode* resources = NULL;
	struct dirent *entry;

    DIR *dir = opendir(det_path);
    
    if (dir == NULL) {
    	return NULL;
    } 

    while((entry = readdir(dir)) != NULL) {
    	char* res_name = entry->d_name;

    	if (!strcmp(res_name,".") || !strcmp(res_name, "..")) {
    		continue;
    	}

    	if (!resources) {
    		resources = sa_new(res_name);
    	} else {
    		sa_add_new(resources, res_name);
    	}

    } closedir(dir);

    free(entry);
    return resources;
}

// Check if char c is in charset c_set
static unsigned int in(char c, char* c_set) {
	for (int i = 0; i < strlen(globbing_symbols); i++) {
		if (globbing_symbols[i] == c) return 1;
	} return 0;
}

struct StringComponents * components_with_globbing_symbol_index(const char* const string, int index) {
	struct StringComponents* ret = calloc(1, sizeof(struct StringComponents));
	char* before_gs = calloc(index+1, sizeof(char));
	char* after_gs = calloc(strlen(string)-index, sizeof(char));

	for (int i = 0; i < index; i++) {
		*(before_gs+i) = *(string+i);
	}

	for (int i = index+1; i < strlen(string); i++) {
		*(after_gs+i-index-1) = *(string+i);
	}

	before_gs = realloc(before_gs, strlen(before_gs));
	after_gs = realloc(after_gs, strlen(after_gs));
	
	ret->before_gs = before_gs;
	ret->after_gs = after_gs;

	return ret;
}

int handle_star(char* det_path, char* to_glob, unsigned int gs_index) {
	return 0;
}

char* last_determinisitc_path_for_string(const char* const string) {
	assert(string);
	
	int gc_index = strlen(string);
	char* path = calloc(strlen(string), sizeof(char));

	for (int i = 0; i < strlen(string); i++) {
		if (in(*(string+i),globbing_symbols)) {
			gc_index = i;
		}
	} 

	memcpy(path, string, gc_index+1);

	for (int i = strlen(path); i > 0; i--) {
		if (*(path+i) == '/') {
			*(path+i+1) = '\0';
			path = realloc(path, strlen(path));
			return path;
		}
	} free(path);
	return NULL;
}

static void filter_files_starting_with(char* sstring) {
	assert(matches);
	struct StringNode* current = matches;
	while(current != NULL) {
		char* str = current->data;
		if (string_starts_with(str, sstring)) {
			current = current->next;
			continue;
		} sa_remove(&matches, str);
		current = current->next;
	}
}

static void filter_files_ending_with(char* estring) {
	assert(matches);
	struct StringNode* current = matches;
	while(current != NULL) {
		char* str = current->data;
		if (string_ends_with(str, estring)) {
			current = current->next;
			continue;
		} sa_remove(&matches, str);
		current = current->next;
	}
}

static void hanlde_question_mark(char* before_gs) {
	assert(matches);
	int before_gs_len = (before_gs) ? strlen(before_gs) : 0;
	struct StringNode* current = matches;
	while(current != NULL) {
		char* str = current->data;
		if (strlen(str) >= before_gs_len+1) {
			current = current->next;
			continue;
		} sa_remove(&matches, str);
		current = current->next;
	}
}

static struct StringNode* glob_chain(const char* const string,
						 const char* const det_path, 
						 unsigned int gs_index) {

	if (!matches) {
		struct StringNode* nodes = resources_in_dir(det_path);
		matches = nodes;
	}

	struct StringComponents* components = components_with_globbing_symbol_index(string, gs_index);
	
	if (components->before_gs != NULL) {
		filter_files_starting_with(components->before_gs);
		if (matches == NULL) { return NULL; }
	}

	switch(*(string+gs_index)){
		case '*':
			break;
		case '?':
			hanlde_question_mark(components->before_gs);
			break;
	}

	if (components->after_gs != NULL) {
		filter_files_ending_with(components->after_gs);
		if (matches == NULL) { return NULL; }
	}

	return matches;
}

struct StringNode * expand_string(const char* const string) {

	assert(string);
	struct StringNode* ret;
	const char* const det_path = last_determinisitc_path_for_string(string);
	size_t det_path_len = strlen(det_path);

	if (matches) {
		sa_destroy(matches);
		matches = 0;
	}

	for (int i = strlen(det_path); i < strlen(string); i++) {
		if (in(*(string+i), globbing_symbols)) {
			const char* const to_glob = string+det_path_len;
			glob_chain(to_glob, det_path, i-det_path_len);
			break;
		}
	}

	sa_edit_prepend_all(matches, det_path);
	
	return matches;
}
