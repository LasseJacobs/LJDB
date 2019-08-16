
/* 
 * File:   cli_client.h
 * Author: lasse
 *
 * Created on August 16, 2019, 4:47 PM
 */

#ifndef CLI_CLIENT_H
#define CLI_CLIENT_H

#include "dbfacade.h"
#include <string>


namespace frontend {

class cli_client {
public:
    cli_client(const std::string& path);
    
    void run();

private:
    LJDBFacade __database;
    
};

}

#endif /* CLI_CLIENT_H */

