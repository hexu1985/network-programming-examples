#ifndef FILE_EXISTS_HPP
#define FILE_EXISTS_HPP

#ifdef WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

#include <string>

#ifndef F_OK
#define F_OK 0
#endif

static inline
bool file_exists(const std::string &file_path)
{
	return access(file_path.c_str(), F_OK) == 0;
}

#endif
