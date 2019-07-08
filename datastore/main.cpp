#include <iostream>
#include "utils.h"


using namespace std;

int main()
{
    cout << utils::string::format("v_%s_log.table", "cubes") << endl;
    return 0;
}
