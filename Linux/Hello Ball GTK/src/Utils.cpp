/*
 * Utils.cpp
 *
 *  Created on: Mar 5, 2015
 *      Author: pepo
 */

#include <typeinfo>
#include <wordexp.h>
#include <unistd.h>
#include <cstring>

#include "Utils.h"


std::string Utils::execFolderPath()
{
	char execPath[2048];
	readlink("/proc/self/exe", execPath, 2048);
	size_t l = strlen(execPath);
	size_t i = l - 1;
	bool found = false;
	while (!found && i >= 0) {
		if (execPath[i] == '/') {
			execPath[i] = '\0';
			found = true;
		}
		i -= 1;
	}
	std::string path;
	path += execPath;
	return path;
}
