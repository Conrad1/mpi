#include <stdlib.h>
#include "common.h"

void friendlyExit(int code)
{
    std::cout << "Exiting " << __FILE__ << " - line " << __LINE__ << std::endl; 
    exit(code);
}
