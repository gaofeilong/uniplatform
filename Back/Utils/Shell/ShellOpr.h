#ifndef _SHELL_OPR_H_
#define _SHELL_OPR_H_

#include <string>
#include <vector>

using std::string;
using std::vector;

int IsProRunning(const string& cmd, bool& isRunning);
int Execute(const string& cmd, vector<string>& revInfo);

#endif //_SHELL_OPR_H_
