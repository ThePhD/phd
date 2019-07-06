#pragma once

#ifndef PHD_OVERLOAD_TESTS_FUNCTIONS_HPP
#define PHD_OVERLOAD_TESTS_FUNCTIONS_HPP

#include <string>

int f(int);

int f(double);

int f(std::string);

int i_to_i(int a);

int d_to_i(double a);

int s_to_i(std::string s);

#endif // PHD_OVERLOAD_TESTS_FUNCTIONS_HPP
