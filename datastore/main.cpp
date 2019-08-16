#include <iostream>

#include "cli_client.h"

using namespace std;

int main()
{    
    frontend::cli_client client("namespace/");
    client.run();
    
    return 0;
}
