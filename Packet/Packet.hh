//
// Packet.hh
//

#ifndef Packet_hh
#define Packet_hh

#include "../PitM.hh"

class PitM::Packet : private _PitM_ {

public: // Static methods

   static bool Start();

   static void Reconfigure();

}; // Packet

#endif // Packet_hh
