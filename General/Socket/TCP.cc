//
// TCP.cc
//

#include "TCP.hh"

using namespace BSD;

TCP::TCP(FD::Type client) :
     Socket(client) {
} // TCP::TCP(client)

TCP::TCP(TCP &tcp) :
     Socket(tcp) {
} // TCP::TCP(TCP)