/*
* main.cpp
*
*  Created on: Sep 10, 2018
*      Author: John Hall
*/
#include "Program.h"

int runMode = 0;		//runMode 0 executes our rendering scene,  runMode 1 executes the snippetHelloWorld scene

extern int snippetMain(int, const char*const*);

int main(int argc, char* argv[]) {
	if (runMode == 0) {
		Program p;
		p.start();
		return 0;
	}

	else if (runMode == 1) {
		return snippetMain(argc, argv);
	}

	else {
		return 0;
	}
}