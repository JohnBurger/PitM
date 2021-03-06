//
// Reader.cc
//

#include <iostream>

#include <net/ethernet.h>    // For ETHER_ADDR_LEN // TODO?

#include <Socket/Address.hh>

#include "Packet.hh"
#include "Reader.hh"

using namespace PitM;

Monitor::Reader::Reader(Monitor &monitor, const String &interface) :
                 MT::Thread(),
                 BSD::Raw(interface, Protocols::All, true), // Promiscuous
                 monitor(monitor) {
   if (!Raw::Valid()) {
      delete this;
      return;
   } // if
   if (!Thread::Start()) {
      delete this;
      return;
   } // if
   monitor.Stop(this);
} // Reader::Reader(Monitor, interface)

bool Monitor::Reader::Send(const Monitor::Packet &packet) {
   Size wrote;
// BSD::Address to(All, index, packet.buffer, ETHER_ADDR_LEN); // TODO?
   return Raw::Send(packet.buffer,packet.Size(),wrote);
} // Reader::Send(Packet)

void *Monitor::Reader::Run() {
//   Raw::ResetFlag(IFF_RUNNING);

   Monitor::Packet *packet;
   for (ever) {
      if (!Monitor::Packet::pool.TryPop(packet)) { // Try to get empty packet
         packet = new Monitor::Packet();           // Couldn't, so create one
      } // if
      if (packet==nullptr) {
         break;
      } // if
#ifndef NDEBUG
      std::cout << '.' << std::flush;
#endif // !NDEBUG
      Size read;
      if (!RecvFrom(packet->buffer, sizeof packet->buffer, read,
                    packet->address, MSG_TRUNC)) {
         break;
      } // if
#ifndef NDEBUG
      std::cout << ',' << std::flush;
#endif // !NDEBUG
      IOCtl(SIOCGSTAMP, packet->Stamp(read, sizeof packet->buffer));
      monitor.other.Transmit(*packet); // Transmit packet on other
   } // for
   if (packet!=nullptr) {
      Packet::pool.Push(*packet);
   } // if
#ifndef NDEBUG
   std::cerr << '!' << std::endl;
#endif // !NDEBUG
   delete this;
   return nullptr;
} // Reader::Run()

Monitor::Reader::~Reader() {
   Raw::Close();
   monitor.reader.Swap(nullptr, this); // Set nullptr, but only if ==this
} // Reader::~Reader()
