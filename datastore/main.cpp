#include <iostream>
#include "utils.h"
#include "dbimp.h"


using namespace std;

int main()
{    
    const std::string name = "cubes";
    const std::string table_template = "{edge:int}";
    core::DbImp dbImp("");
    dbImp.create_table(name, table_template);
    dbImp.remove_table(name);
    cout << utils::string::format("v_%s_log.table", "cubes") << endl;
    return 0;
}
