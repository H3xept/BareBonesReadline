#include "tests/wildcardStringTest.h"
 #include "tests/macros.h"
int main() { setup(); run_test(testCreateWildcardString);run_test(testCreateWildcardStringEndingWild);run_test(testCreateWildcardStringJustWild); teardown(); 	printf("Tests executed [1m3[0m"); puts(""); puts("");}