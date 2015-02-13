#ifndef TEST_IDENTIFICATION_H
#define TEST_IDENTIFICATION_H
#include "automaton.h"
//#include <opencv2/core/core.hpp>
//#include <opencv2/highgui/highgui.hpp>

#include <iostream>

Automaton* map(std::string filename);
void test_aid();
void test_lid();
void automaton_img(Automaton* a);

#endif // TEST_IDENTIFICATION_H
