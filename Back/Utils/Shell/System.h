#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include <string>

bool GetHighTime(std::string& strTime, bool toSecond = true);
// 执行命令，成功则返回 true， 失败则返回 false， 返回结果仅为命令正确执行的结果
bool Popen(const char* cmd, std::string& result);
// 执行命令，成功则返回 true， 失败则返回 false， 返回命令执行正确及错误的执行结果
bool Popen(const char* cmd, std::string& msg, std::string& err);
// 执行命令，成功则返回 true 失败则返回 false
// (该函数判断返回码是否为0， 若为0，则正确，当执行自定义的脚本时，请确保正确时的返回值为0)
bool ExeCmd(const char* cmd);
// 执行命令，成功则返回 执行码(若为0则是正确)  失败则返回 -1
int  System(const char* cmd);

#endif /* __SYSTEM_H__ */
