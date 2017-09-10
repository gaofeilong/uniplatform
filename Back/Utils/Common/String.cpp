#include "String.h"

//
// String
//
bool String::Split(const char* buf, char separator, StringArray &array)
{
        if (buf == NULL){
                return false;
        }

        int                 p1  = 0;
        std::string         temp;

        for (int i = 0; ; i++)
        {
                if ( *(buf + i) == separator ){
                        temp.assign( (char *)(buf + p1), i - p1 );
                        array.push_back(temp);
                        p1 = i + 1;
                }
                else if ( *(buf + i) == 0 ) {
                        if (i == p1){
                                break;
                        }
                        temp.assign( (char *)(buf + p1), i - p1 );
                        array.push_back(temp);
                        break;
                }
        }

        return true;
}
