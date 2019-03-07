#ifndef __globber_Test__
#define __globber_Test__

void setup();
void testRootPathFromLocalString();
void testRootPathFromActualPath();
void testRootPathFromActualPathWithStarGlobbingSymbols();
void testRootPathWithOnlyStarGlobbingSymbols();
void testQuestionMarkGlobbingSymbol();
void testExpandString();
void testExpandNonExpandableString();
void testGlobLineSingleExpand();
void teardown();

#endif