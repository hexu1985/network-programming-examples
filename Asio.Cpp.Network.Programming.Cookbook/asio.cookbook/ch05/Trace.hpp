#ifndef TRACE_HPP
#define TRACE_HPP

#include <string>
#include <iostream>

class Trace {
public:
	Trace(const std::string &name);
	~Trace();

	void debug(const std::string &msg);

private:
	std::string theFunctionName;
};

inline
Trace::Trace(const std::string &name) : theFunctionName(name)
{
	std::cout << "Enter function " << name << std::endl;
}

inline
void Trace::debug(const std::string &msg)
{
	std::cout << msg << std::endl;
}

inline
Trace::~Trace()
{
	std::cout << "Exit function " << theFunctionName << std::endl;
}

#endif
