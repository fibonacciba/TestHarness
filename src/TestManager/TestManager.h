/////////////////////////////////////////////////////////////////////////
// Fang Wang, Syracuse University
// CSE687 object orriented Design
//
// Project: Test Harness
// TestManager.h Definition of TestManager class
#pragma once
#ifndef TESTHARNESS_H
#define TESTMANAGER_H

#include "Process.h"
#include "../Utils/MsgPassingComm/CommHeader.h"
#include "../Utils/Sockets/Sockets.h"
#include "../Utils/Cpp11-BlockingQueue/Cpp11-BlockingQueue.h"
#include "../Utils/Message/Message.h"
#include "../Utils/Utilities/XmlReader.h"
#include "../Utils/Utilities/FileUtilities.h"
#include "../TestWorker/TestWorker.h"
#include "../Utils/FileSystem/FileSystem.h"
#include <vector>

using namespace MsgPassingCommunication;
namespace Testing
{
	class TestManager
	{
	public:
		TestManager(EndPoint from);
		~TestManager() = default;
		void start();
		void wait();
		void stop();
		void sendMessage(MsgPassingCommunication::Message msg);
		void recvMessages();
		void dispatchMessages();
		void setPath(std::string& send_path, std::string& save_path);
		std::string getSendPath();
		std::string getSavePath();
	private:
		BlockingQueue<MsgPassingCommunication::Message> readyQ_;
		BlockingQueue<MsgPassingCommunication::Message> requestQ_;
		std::thread recvr;
		std::thread dspat;
		Comm comm_;
		EndPoint to_;
		EndPoint from_;
		std::vector<Process> children_;
	};
}
#endif

