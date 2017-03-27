#ifndef GENERAL_H

#define null NULL
#include <sstream>

#define SSTR( x ) static_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()

#define GENERAL_H

#endif // GENERAL_H
