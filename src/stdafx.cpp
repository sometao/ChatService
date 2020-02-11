/*
stdafx.h于stdafx.cpp，这两个文件用于建立一个预编译的头文件".PCH"和一个预定义的类型文件"STDAFX.OBJ"。
由于MFC体系结构非常大，各个源文件中都包含许多头文件，如果每次都编译的话比较费时。
那么,如果我们把常用的MFC头文件都放在stdafx.h中，如afxwin.h、afxext.h、afxdisp.h、afxcmn.h等，
然后再让stdafx.cpp包含这个stdafx.h文件。这样，由于编译器可以识别哪些文件已经编译过，
所以stdafx.cpp就只编译一次(这里一定要有CPP文件,如果只有.h是不行的,因为头文件是不能被编译的)，
并生成所谓的预编译头文件(因为它存放的是头文件编译后的信息，故而得名)。
*/

#include "stdafx.h"

