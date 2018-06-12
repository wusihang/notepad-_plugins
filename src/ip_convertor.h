#ifndef IP_CONVERTOR_H
#define IP_CONVERTOR_H

namespace my_plugin {

class ip_convertor{


public:
    static unsigned int ip2Num(const char* ip);
    static void num2Ip(char* buf,unsigned int num);
};

}

#endif // IP_CONVERTOR_H
