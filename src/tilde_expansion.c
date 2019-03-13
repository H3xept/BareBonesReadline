#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "string_utils.h"
#include "tilde_expansion.h"

#define HOME_ENV "HOME"

char* expand_tildes(const char* const string) {
	char* home_env = getenv(HOME_ENV);
	return su_replace_occurrencies_of(string, "~", home_env);
}
