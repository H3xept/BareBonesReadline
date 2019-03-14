#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <dirent.h> 

#include "wildcard_path.h"
#include "globber.h"
#include "tokenizer.h"
#include "string_utils.h"

static char globbing_symbols[] = {'*','?'};
static struct StringNode* matches = NULL;
static const char* det_path = NULL;

// Check if char c is in charset c_set
static unsigned int in(const char c, const char* const c_set) {
	for (int i = 0; i < strlen(globbing_symbols); i++) {
		if (globbing_symbols[i] == c) return 1;
	} return 0;
}

static struct StringNode * resources_in_dir(const char* const path) {
	struct StringNode* resources = NULL;
	struct dirent *entry = NULL;

	char* e_path = su_replace_occurrencies_of(path, "\\ ", " ");

    DIR *dir = opendir((e_path) ? e_path : "./");

    if (dir == NULL) {
    	return NULL;
    } 

    while((entry = readdir(dir)) != NULL) {
    	char* res_name = entry->d_name;
    	if (!resources) {
    		resources = sa_new(res_name);
    	} else {
    		sa_add_new(resources, res_name);
    	} 

    } closedir(dir);

    free(e_path);
    return resources;
}

char* last_determinisitc_path_for_string(const char* const string) {
	assert(string);
	
	int gc_index = strlen(string);
	char* path = calloc(strlen(string)+1, sizeof(char));

	for (int i = 0; i < strlen(string); i++) {
		if (in(*(string+i),globbing_symbols)) {
			gc_index = i;
			break;
		}
	} 

	memcpy(path, string, gc_index);
	for (int i = gc_index-1; i >= 0; i--) {
		if (*(path+i) == '/') {
			*(path+i+1) = '\0';
			path = realloc(path, strlen(path)+1);
			return path;
		}
	} 

	if (path)
		free(path);
	return NULL;
}

static unsigned int index_of_last_slash(char* string) {
	assert(string);
	unsigned int index = strlen(string);
	for (; index > 0; index--) {
		if (*(string+index) == '/') {
			break;
		}
	} return index;
}

static void filter_files_containing_s_at_index(char* substr, 
											   unsigned int index) {
	if (!substr) { return; }

	struct StringNode* current = matches;
	size_t substr_len = strlen(substr);

	while(current != NULL) {
		char* str = current->data + index_of_last_slash(current->data);
		size_t str_len = strlen(str);

		if ( (index > str_len | index+substr_len-1 > str_len) || 
			strncmp((str + index), substr, substr_len) ) {
			sa_remove(&matches, str);
		}
		current = current->next;
	}
}

static void filter_files_containing_s_after_index(char* substr, 
											   	  unsigned int index) {
	if (!substr) { return; }

	struct StringNode* current = matches;
	size_t substr_len = strlen(substr);

	while(current != NULL) {
		char* str = current->data + index_of_last_slash(current->data);
		size_t str_len = strlen(str);

		if ( (index > str_len | index+substr_len-1 > str_len) || 
			!(strstr((str+index), substr)) ) {
			sa_remove(&matches, str);
		}
		current = current->next;
	}
}

static void filter_ending_with(char* substr) {
	if (!substr) { return; }
	struct StringNode* current = matches;
	char* last = NULL;
	while(current != NULL) {
		last = strrchr(current->data, *substr);
		if (!last || strcmp(last, substr)) {
			sa_remove(&matches, current->data);
		}
		current = current->next;
	}
}

static void hanlde_question_mark(unsigned int cur_len) {

	struct StringNode* current = matches;

	while(current != NULL) {
		char* str = current->data + index_of_last_slash(current->data);
		size_t str_len = strlen(str);

		if (str_len < cur_len+1) {
			sa_remove(&matches, str);
		}
		current = current->next;
		continue;
	}
}

static inline void filter_with_string(const WildcardString* const string) {
	assert(string);

	void (*static_filter)(char*, unsigned int) = (string->index >= 0) ?
		filter_files_containing_s_at_index:
		filter_files_containing_s_after_index;
	static_filter(string->before_gs, abs(string->index));

	char* gs = string->gs;
	if (gs) {
		switch(*gs) {
			case '*':
				break;
			case '?':
				hanlde_question_mark(string->index+strlen(string->before_gs));
				break;
		}
	}
	if (string->next) filter_with_string(string->next);
	else if (!gs) {filter_ending_with(string->before_gs);}
}

static void filter_replace_with_dir_contents() {

	if (!matches) { return; }

	struct StringNode* repl_matches = NULL;
	struct StringNode* current = matches;

	while(current) {
		assert(current->data);
		if (!strcmp(current->data, ".") || !strcmp(current->data, "..")) {
			goto next_loop;
		}

		struct StringNode* new_matches = NULL;
		size_t c_data_len = strlen(current->data);
		size_t det_path_len = strlen(det_path);

		char* new_subpath = calloc(c_data_len+2, sizeof(char));
		strcpy(new_subpath, current->data);
		*(new_subpath+c_data_len) = '/';

		char* full_path = calloc(det_path_len+c_data_len+1, sizeof(char));
		strcpy(full_path, det_path);
		strcat(full_path, current->data);

		new_matches = resources_in_dir(full_path);
		sa_edit_prepend_all(new_matches, new_subpath);

		if (!repl_matches) {
			repl_matches = new_matches;
		} else {
			sa_add(repl_matches, new_matches);
		}

		if (full_path)
			free(full_path);
		full_path = 0;

		if (new_subpath)
			free(new_subpath);

next_loop: 
		current = current->next;

	} if (repl_matches) { sa_destroy(matches); }

	matches = repl_matches;
}

static inline void filter_wildcards(const WildcardPath* const wildcard) {
	assert(wildcard);

	filter_with_string(wildcard->wstring);
	WildcardPath* next = wildcard->next;

	if (next) {
		filter_replace_with_dir_contents();
		if (next->wstring) {
			filter_wildcards(next);
		}
	}
}

struct StringNode * expand_string(const char* const string) {
	assert(string);

	if (matches) {
		matches = NULL;
	}

	if (det_path) {
		det_path = NULL;
	}

	det_path = last_determinisitc_path_for_string(string);

	size_t det_path_len = det_path ? strlen(det_path) : 0;
	
	const char* const to_glob = (strlen(string) == det_path_len) ? NULL : string+det_path_len;
	WildcardPath* wildcardPath = NULL;
	matches = resources_in_dir(det_path);

	if (to_glob) {
		 wildcardPath = wp_new(to_glob);
		 filter_wildcards(wildcardPath);
		 sa_edit_prepend_all(matches, det_path);
	}

	if (!matches) {
		matches = sa_new(string);
	}

	return matches;
}

static unsigned int contains_globbing_symbols(const char* const string) {
	for (int i = 0; i < strlen(string); i++) {
		if (in(*(string+i), globbing_symbols)) {
			return 1;
		}
	} return 0;
}

static struct StringNode * glob(const char* const string) {
	assert(string);
	struct StringNode* ret;
	if (contains_globbing_symbols(string)) {
		ret = expand_string(string);
	} else ret = sa_new(string);
	return ret;
}

char* glob_line(const char* const string) {
	#define SUBSTR_SEPARATOR ' '
	
	if (!string) { return ""; }

	char* globbed_line = NULL;
	char* to_glob = calloc(strlen(string)+1, sizeof(char));
	strcpy(to_glob, string);
	Tokenizer* tokenizer = new_tokenizer(to_glob, SUBSTR_SEPARATOR);
	struct StringNode* globbed = NULL;
	
	char* token = NULL;
	while((token = next_token(tokenizer))) {
		struct StringNode* nodes = glob(token);
		sa_escape_non_escaped_spaces(nodes);
		if (!globbed) {
			globbed = nodes;
		} else {
			sa_append(globbed, nodes);
		}
		free(token);
	}

	globbed_line = sa_concat(globbed, SUBSTR_SEPARATOR);

	if(globbed){
		sa_destroy(globbed);
	}
	destroy_tokenizer(tokenizer);

	return globbed_line;
}
