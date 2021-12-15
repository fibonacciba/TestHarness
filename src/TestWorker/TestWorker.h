/////////////////////////////////////////////////////
//CSE687 object oriented design
//Fang Wang, Syracuse University
//
//Test Worker (child tester) class definition
/////////////////////////////////////////////////////
#ifndef TESTWORKER_H
#define TESTWORKER_H


#include "../Utils/MsgPassingComm/CommHeader.h"
#include "../Utils/TestRequest/TestRequest.h"
#include "../Utils/TestRequest/Properties.h"
#include "../Utils/Loader/DllLoader.h"
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

	using Requests = std::vector<TestRequest>;
	using LoggerFactory = Utilities::SingletonLoggerFactory<1, Utilities::NoLock>;

	class TestWorker
	{

	public:
		TestWorker(std::string name, std::string& address, size_t& port, std::string& to_address, size_t& to_port);
		~TestWorker();

		void start();
		void wait();
		void doTest();
		void setPath(std::string& path);
		void sendMessage(MsgPassingCommunication::Message& msg);
		void setSaveFilePath(std::string& save_path);
		void setSendFilePath(std::string& send_path);

		Property<Requests> requests;
		Property<std::string> dllPath;

	private:
		std::thread recv;
		std::string name_;
		std::string path_;
		LoggerFactory* logger_factory;
		std::ofstream log_file_;
		EndPoint from_;
		EndPoint to_;
		Comm childComm_;
	};
}

#endif