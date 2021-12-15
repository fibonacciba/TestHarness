/////////////////////////////////////////////////////////////////////////
// Author: Fang Wang, Syracuse University
// CSE687 object orriented Design
//
// TestPerfomer class definitioin
// 
//  -Load test request
//  -Each TestRequest provides names of Dlls to load and test
//  -Loads Test Dynamic Link Libraries(Dlls) from libPath.
//	-Executes tests defined in each Dll.
//	-Each tests logs ouputs using Tester class's logger
///////////////////////////////////////////////////////////////////////


#pragma once

#ifndef TESTPERFORMER_H
#define TESTPERFORMER_H


#include "../Utils/MsgPassingComm/CommHeader.h"
#include "../Utils/Logger/SingletonLoggerFactory.h"
#include "../Utils/Message/Message.h"
#include "../Utils/Cpp11-BlockingQueue/Cpp11-BlockingQueue.h"
#include "../Utils/Sockets/Sockets.h"
#include <iostream>
#include <vector>
#include <string>


using namespace MsgPassingCommunication;

namespace Testing
{

	/*using Requests = std::vector<TestRequest>;
	using LoggerFactory = Utilities::SingletonLoggerFactory<1, Utilities::NoLock>;*/

	class TestPerformer
	{
		public:
			TestPerformer(std::string& address, size_t& port, std::string& to_address, size_t& to_port);
			//~TestPerformer();

			void start();
			/*void wait();
			void setPath(std::string& path);
			void sendMessage(MsgPassingCommunication::Message& msg);
			void setSaveFilePath(std::string& save_path);
			void setSendFilePath(std::string& send_path);*/
			void watchMessage();
			void sendTestRequests(std::vector<std::string>& dllList,std::string);
			void myTest();

		

		private:
			std::thread recv;
			std::string path_;
			//LoggerFactory* logger_factory;
			std::ofstream log_file_;
			EndPoint from_;
			EndPoint to_;
			Comm PerformerComm_; 
	};

}

#endif


