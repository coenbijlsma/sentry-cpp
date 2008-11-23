#include "SentryHookPoint.h"
#include <iostream>

using std::cout;
using std::endl;

int main(int argc, char** argv){
    SentryHookPoint hookPoint("core.post_startup");
    cout << hookPoint.getName() << endl;
}
