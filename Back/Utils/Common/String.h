#ifndef __STRING_H__
#define __STRING_H__

#include <string>
#include <vector>

class String
{
public:
        typedef std::vector<std::string> StringArray;
public:
        String();
        virtual ~String();
        
        static bool Split(const char* buf, char separator, StringArray &array);
};

#endif /* __STRING_H__ */
