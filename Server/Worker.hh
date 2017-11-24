//
// Worker.hh
//

#ifndef Worker_hh
#define Worker_hh

#include "../General/Thread/Thread.hh"

#include "../General/Socket/TCP.hh"

#include "../General/WWW/HTTP/Request.hh"

#include "../PitM.hh"

class PitM::Worker : public BSD::TCP,
                     private MT::Thread {

friend Server;

private: // Methods

   Worker(BSD::TCP &client, const BSD::Address &address);

   virtual ~Worker() = default;

private: // GET Responses

   // Reply to GET or HEAD Request
   bool GET(bool head);

   // Create and send Home Page
   bool SendHomePage(bool head);

   // Create and send Style Sheet
   bool SendStyleSheet(bool head);

   // Create and send Config Page
   bool SendConfigPage(bool head);

   // Send file using Linux's ::sendfile()
   bool SendFile(bool head,const char *path);

   // Send a linked-in binary 'file'
   bool SendObj(bool head,const void *obj, const void *size);

private: // POST Responses

   // Accept submitted data
   bool POST();

   // Configure
   bool Config();

   // Close the application
   bool Quit();

private: // Run() methods

   // Parse the next line
   bool Parse();

   // Process the next line
   bool Process();

   // Reply to Process()ed Request
   bool Reply();

private: // Thread overrides

   overrides void *Run();

private: // Enums

   enum States {
      RequestLine,
      RequestHeader,
      RequestBody,
      RequestDone
   }; // States

private: // Variables

   WWW::HTTP::Request request;

   States state;

   String line;

   char buffer[2048];

   unsigned read;

   unsigned pos;

   unsigned start;

   unsigned contentLength;

}; // Worker

#endif // Worker_hh
