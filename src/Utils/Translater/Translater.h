/*
 *-------------------------------------
 * Author: Fang Wang, Syracuse University
 * CSE 687 Object Oriented Design C++
 * Syracuse University
 * Design Project: Test Harness
 *-------------------------------------
*  Package Operations:
* ---------------------
*  This C++\Cli Package contains one class, Translater.  It's purpose is to convert
*  managed messages, CsMessage, to native messages, Message, and back.
*
*/

// Project > Properties > C/C++ > Language > Conformance mode set to No
// Add references to System, System.Core

#include <string>
#include "../MsgPassingComm/IComm.h"
#include "CsMessage.h"
#include "../CommLibWrapper/CommLibWrapper.h"

using namespace System;
using namespace System::Text;

namespace MsgPassingCommunication
{
  public ref class Translater
  {

  public:
    Translater();
    void listen(CsEndPoint^ ep);
    bool postMessage(CsMessage^ msg);
    CsMessage^ getMessage();
    CsMessage^ fromMessage(Message& msg);
    Message fromCsMessage(CsMessage^ csMsg);

  private:
    CsEndPoint^ ep_;
    MsgPassingCommunication::CommFactory* pFactory;
    MsgPassingCommunication::IComm* pComm;
  };

  // Initialize endpoint 
  Translater::Translater()
  {
    ep_ = gcnew CsEndPoint;
  }

  // Set client listen endpoint and start Comm 
  void Translater::listen(CsEndPoint^ ep)
  {
    std::cout << "\n-----------------------------------------------------------\n";
    std::cout << "\nUsing CommFactory to create instance of Comm on native heap\n";
    std::cout << "\n-----------------------------------------------------------\n";

    ep_->machineAddress = ep->machineAddress;
    ep_->port = ep->port;
    pFactory = new CommFactory;
    pComm = pFactory->create(Sutils::MtoNstr(ep_->machineAddress), ep_->port);
    pComm->start();
    delete pFactory;
  }

  // Convert native message to managed message 
  CsMessage^ Translater::fromMessage(Message& msg)
  {
    CsMessage^ csMsg = gcnew CsMessage;
    Message::Attributes attribs = msg.attributes();
    for (auto attrib : attribs)
    {
      csMsg->add(Sutils::NtoMstr(attrib.first), Sutils::NtoMstr(attrib.second));
    }
    return csMsg;
  }

  // Convert managed message to native message
  Message Translater::fromCsMessage(CsMessage^ csMsg)
  {
    Message msg;
    auto enumer = csMsg->attributes->GetEnumerator();
    while (enumer.MoveNext())
    {
      String^ key = enumer.Current.Key;
      String^ value = enumer.Current.Value;
      msg.attribute(Sutils::MtoNstr(key), Sutils::MtoNstr(value));
    }
    return msg;
  }

  // Use Comm to post message
  inline bool Translater::postMessage(CsMessage^ csMsg)
  {
    std::cout << "\n-----------------------------------------------------------\n";
    std::cout << "\n              Posting Message In Translater\n";
    std::cout << "\n-----------------------------------------------------------\n";

    Message msg = this->fromCsMessage(csMsg);
    pComm->postMessage(msg);
	return true;
  }
  
  // Get message from Comm
  inline CsMessage^ Translater::getMessage()
  {
    std::cout << "\n-----------------------------------------------------------\n";
    std::cout << "\n              Getting Message In Translater\n";
    std::cout << "\n-----------------------------------------------------------\n";

    Message msg = pComm->getMessage();
    return fromMessage(msg);
  }
}