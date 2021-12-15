/////////////////////////////////////////////////////
//CSE687 object oriented design
//Fang Wang, Syracuse University
//
//TestWorker.cpp : This file contains the 'main' function. Program execution begins and ends there.
/////////////////////////////////////////////////////




#include <string>
#include <functional>
#include <thread>

#include "TestWorker.h"


#define TESTWORKER

using namespace MsgPassingCommunication;

// Constructor for Test Worker class, setup its ip address and port number, start Comm instance
Testing::TestWorker::TestWorker(std::string name, std::string& address, size_t& port, std::string& to_address, size_t& to_port)
	:name_(name), to_(to_address, to_port), from_(address, port), childComm_(from_, "Comm")
{
	childComm_.start();
	path_ = childComm_.getSaveFilePath();
}

// Start doTests thread, get messages from TestHarness server and executes tests inside each test driver 
void Testing::TestWorker::start()
{
	std::cout << "\n\n-- Starting TestWorker Thread Listening On: " + from_.toString();
	recv = std::thread([&]() { doTest(); });
	recv.detach();
}

// Waits for TestWorker to shutdown 
void Testing::TestWorker::wait()
{
	if (recv.joinable())
		recv.join();
}

// Default destructor for Child tester class 
Testing::TestWorker::~TestWorker()
{
	std::cout << "\n\n-- TestWorker Has Been Destroyed...";
	log_file_.close();
	delete logger_factory;
	childComm_.stop();
}

// Set file receive path for Comm instance 
void Testing::TestWorker::setSaveFilePath(std::string& save_path)
{
	childComm_.setSaveFilePath(save_path);
}

// Set file send path for Comm instance 
void Testing::TestWorker::setSendFilePath(std::string& send_path)
{
	childComm_.setSendFilePath(send_path);
}

// Set test library path  
void Testing::TestWorker::setPath(std::string& path)
{
	dllPath(path);
}

// Get messages, if the message is a test request, execute tests in its attaching test driver 
void Testing::TestWorker::doTest()
{
	// Generating a log file
	auto logger_ = logger_factory->getInstance();
	
	bool test_res = true;
	logger_->setTerminator("");
	std::string logFileSpec = "../TestLog/" + name_ + "_log.txt";
	

	log_file_.open(logFileSpec, std::ofstream::out | std::ofstream::trunc);
	log_file_.close();
	log_file_.open(logFileSpec, std::ios::app);

	//std::cout << "########################################################->" << logFileSpec << std::endl;
	logger_->addStream(&log_file_);


	logger_->write("\n\n-------------------- [Initializing Test Worker] --------------------");

	Testing::DateTime dt;
	logger_->write("\n\n-- Testing time: " + dt.now());
	logger_->write("\n-- Log path: ../TestLog/");

	MsgPassingCommunication::Message ready;
	ready.from(from_);
	ready.to(to_);
	ready.command("ready");

	logger_->write("\n-- Tester Sending Mesage to Server: " + ready.command());
	childComm_.postMessage(ready);
	bool result = true;

	while (true)
	{
		MsgPassingCommunication::Message msg = childComm_.getMessage();

		logger_->write("\n-------------------- [Attention] --------------------");
		logger_->write("\n\n-- Tester " + from_.toString() + " Has Received A Message: " + msg.name());

		logger_->write("\n\n-------------------- [Message Content] --------------------");
		logger_->write("\n\n-- Message Title: " + msg.name());
		logger_->write("\n-- Message Command: " + msg.value("command"));
		logger_->write("\n-- Message Operator: " + msg.value("author"));
		logger_->write("\n-- Message From: " + msg.from().toString());

		//std::vector<std::string> dlls = msg.dll_value("sendingFile");
		std::string dll = msg.value("sendingFile");
		logger_->write("\n-- Message File: ");

		/*for (std::string item : dlls)
			logger_->write("                      " + item + "\n");*/
		logger_->write("                      " + dll + "\n");

		logger_->write("\n");

		if (msg.command() == "testReq")
		{
			logger_->write("\n\n-------------------- [Test Task Starting] --------------------");

			/*for (int i = 0; i < dlls.size(); i++)
			{*/
				//std::string fileSpec = path_ + "\\" + dlls[i];
				std::string fileSpec = path_ + "\\" + dll;
				logger_->write("\n\n-- File Is Sucessfully Loaded: " + fileSpec);
				logger_->write("\n-- Executing Tests Extracted From: " + fileSpec);


				logger_->write("\n\n-------------------- [Test Result] --------------------");

				DllLoader dllLoader = DllLoader(fileSpec);
				dllLoader.TestITest();
				TestResult testResult = dllLoader.getTestResult();
				result = testResult.test_result_;

				std::cout << testResult.dll_path_ << std::endl;
				std::cout << testResult.test_result_ << std::endl;
				std::cout << testResult.log_message_ << std::endl;


				if (testResult.test_result_)
				{
					//logger_->write("\n\n-- All Tests Passed: " + fileSpec + "-- ^;^\n");
					logger_->write("\n\n-- Test Passed: " + fileSpec + "-- ^;^\n");
				}
				else
				{
					//logger_->write("\n-- At Least One Test Failed: " + fileSpec + "--!\n");
					logger_->write("\n-- Test Test Failed: " + fileSpec + "--!\n");
				}

				logger_->write("\n-- Detailed log file for each test library is named: log_" + msg.name() + ".txt");
				logger_->write("\n-- Test report summary is named: "+ name_ +"_log.txt");
				logger_->write("\n-- Only test report summary will be sent back to client in the");
				logger_->write("\n-- form of Message.");

			//}
			MsgPassingCommunication::Message log_res_msg;
			log_res_msg.command("result");
			DateTime dt;
			log_res_msg.name(msg.name());
			log_res_msg.from(from_);
			log_res_msg.to(msg.from());
			std::string res;

			if (result)
				res = "------ [Passed] ------";
			else
				res = "------ [Failed] ------";

			log_res_msg.attribute("result", res);
			log_res_msg.attribute("author", msg.value("author"));
			//log_res_msg.attribute("dll", dlls[0]);
			log_res_msg.attribute("dll", dll);
			log_res_msg.attribute("time", dt.now());
			log_res_msg.attribute("log", "logFileSpec");

			log_file_.flush();
			

			childComm_.postMessage(log_res_msg);
			childComm_.postMessage(ready);
		}
	}
}

// Send a message to anywhere you want 
void Testing::TestWorker::sendMessage(MsgPassingCommunication::Message& msg)
{
	childComm_.postMessage(msg);
}


#ifdef TESTWORKER
int main(int argc, char* argv[])
{
	// std::cout << "\n  Demonstrating Test Worker Handles Test Request";
	std::cout << "\n======================= [Test Worker] =======================";

	if (argc < 7)
	{
		std::cout << "\n-- Invalid Parameters..." << std::endl;
		return 0;
	}

	//Handle parameters sent by TestManager server
	std::string addr_to = argv[0];
	size_t port_to = std::stoi(argv[1]);
	std::string child_name = argv[2];
	std::string addr_from = argv[3];
	size_t port_from = std::stoi(argv[4]);
	std::string send_path = argv[5];
	std::string receive_path = argv[6];

	std::cout << "\n\n\nTest_Worker_Configuration:" << std::endl;
	std::cout << "-- Test_Worker_Name:  " << argv[2] << std::endl;
	std::cout << "-- Receive_From:  " << argv[3] << std::endl;
	std::cout << "-- Tester_Port:  " << std::stoi(argv[4]) << std::endl;
	std::cout << "-- Sending_To:  " << argv[0] << std::endl;
	std::cout << "-- Server_Port:  " << std::stoi(argv[1]) << std::endl;
	std::cout << "-- Sending_Files_Path:  " << argv[5] << std::endl;
	std::cout << "-- Receiving_Files_Path:  " << argv[6] << std::endl;

	//Create a Test Worker instance and then setup it.
	Testing::TestWorker ct(child_name, addr_from, port_from, addr_to, port_to);
	ct.start();
	ct.setPath(receive_path);

	getchar();
}
#endif
