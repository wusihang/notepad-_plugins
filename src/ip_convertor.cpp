#include "ip_convertor.h"
#include<stdio.h>

namespace my_plugin {

unsigned int ip_convertor::ip2Num(const char* ip){
    unsigned int tmpInt;
    unsigned int tmpIp;
    tmpInt = tmpIp = 0;
    for(int i=0;*(ip+i)!='\0';i++){
        if(*(ip+i)=='.'){
            tmpIp = 256*tmpIp+tmpInt;
            tmpInt=0;
        }else{
            tmpInt = 10*tmpInt + (*(ip+i)-'0');
        }
    }
    tmpIp = 256*tmpIp+tmpInt;
    return tmpIp;
}
void ip_convertor::num2Ip(char* buf, unsigned int num){
    sprintf(buf,"%u.%u.%u.%u",num>>24,(num>>16)&0xFF,(num>>8)&0xFF,num&0xFF);
}

}
