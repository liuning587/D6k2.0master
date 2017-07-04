
#include "log2file.h"

#include <stdio.h>

namespace MYLIB
{
        int GetFileSize(const char* filename)
        {
                FILE *fp;
                if((fp=fopen(filename,"r"))==NULL)
                        return 0;
                fseek(fp,0,SEEK_END);
                int nsize = ftell(fp);    //return NULL;
                fclose(fp);

                return nsize;
        }

        void Log2File(const char* filename,const char* log,bool IsAppend,int nMaxSizeInMB)
        {
                FILE* fp = NULL;
                if(IsAppend)
                {
                        int nSize = MYLIB::GetFileSize(filename);
                        if(nSize > nMaxSizeInMB*1024*1024)
                                fp = fopen(filename, "w");
                        else
                                fp=fopen(filename,"at+");
                }
                else
                {
                        fp = fopen(filename, "w");
                }

                if(fp==NULL)
                        return;

                string strMsg(log);
                strMsg += "\n";
                fwrite(strMsg.c_str(),strMsg.length(),1,fp);
                fclose(fp);
        }

}//end namespace MYLIB
