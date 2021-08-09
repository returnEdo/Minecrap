#pragma once

#include <iostream>

#include "EscapeCodes.hpp"


#ifdef DEBUG
#define ASSERT(tCondition, tMessage) \
	if (not (tCondition)) \
	{ \
		std::cout << BOLD(RED("\tASSERTION FAILED!!")) << std::endl;	\
		std::cout << BOLD(GREEN("\tLine:\t")) << __LINE__ << std::endl; \
		std::cout << BOLD(CYAN("\tFile:\t")) << __FILE__ << std::endl; \
		std::cout << BOLD(BLUE("\tFunction:\t")) << __FUNCTION__ << std::endl; \
		std::cout << BOLD(MAGENTA("\tMessage:\t")) << tMessage << std::endl; \
		exit(1); \
	}

#define WARNING(tCondition, tMessage) \
	if (tCondition) \
	{ \
		std::cout << BOLD(YELLOW("\tWARNING!!")) << std::endl;	\
		std::cout << BOLD(GREEN("\tLine:\t")) << __LINE__ << std::endl; \
		std::cout << BOLD(CYAN("\tFile:\t")) << __FILE__ << std::endl; \
		std::cout << BOLD(BLUE("\tFunction:\t")) << __FUNCTION__ << std::endl; \
		std::cout << BOLD(MAGENTA("\tMessage:\t")) << tMessage << std::endl; \
	}

#define PRINT(tVal) \
	std::cout << "\t" << #tVal << ":\t" << tVal << std::endl;

#define DBG() \
	std::cout << BOLD(GREEN("\tLine:\t")) << __LINE__ << std::endl; \
	std::cout << BOLD(CYAN("\tFile:\t")) << __FILE__ << std::endl; \
	std::cout << BOLD(BLUE("\tFunction:\t")) << __FUNCTION__ << std::endl;

#else
#define ASSERT(tCondition, tMessage) 	{}
#define WARNING(tCondition, tMessage) 	{}
#define PRINT(tVal)			{}
#define DBG()				{}
#endif

#define COUNT(t_vec, t_type) \
	sizeof(t_vec) / sizeof(t_type)

