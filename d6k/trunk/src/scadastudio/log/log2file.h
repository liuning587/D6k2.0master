#ifndef _LOG_2_FILE_H
#define _LOG_2_FILE_H

#include <string>
using namespace std;

namespace MYLIB
{
        int GetFileSize(const char* filename);
        void Log2File(const char* filename,const char* log,bool IsAppend,int nMaxSizeInMB=30);
}


#endif //_LOG_2_FILE_H