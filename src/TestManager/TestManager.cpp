// TestManager.cpp : This file contains the 'main' function. Program execution begins and ends there.
/*
 *-------------------------------------
 * Fang Wang, Syracuse University
 * CSE 687 Object Oriented Design C++
 * Project: Test Harness
 *
 *-------------------------------------
 * Implemented member functions in TestManager class.
 * Defined the entry point for TestHarness package.
 * TestManager spawnes Client process and Child
 * processes by using the Process poll framework provided
 * by Professor, configuration data, like ip address, port
 * number, server name and file pathes, are sent by Testness
 * server to each child processes.
 */




#include "TestManager.h"

#include <thread>
#include <chrono>



namespace Testing
{
	// Constructor for TestManager, require lib MsgPassingComm: EndPoint, comm_, from_
	TestManager::TestManager(EndPoint from)
		: comm_(from, "recvr"), from_(from)
	{
		comm_.start();
		start();

	}


	void TestManager::start()
	{
		std::string WorkerPath = "../Debug/TestWorker.exe";

		//Spawn Test_Worker Processes
		Process p1;
		p1.application(WorkerPath);

		Process p2;
		p2.application(WorkerPath);

		Process p3;
		p3.application(WorkerPath);

		std::ostringstream os;
		os << from_.address << " " << from_.port << " " << "[Test_Worker_1] " << "localhost " << "9001 " << "../SendFile ../SaveFile";
		std::string cmdLine_Child_1 = os.str();
		os.str("");

		os << from_.address << " " << from_.port << " " << "[Test_Worker_2] " << "localhost " << "9002 " << "../SendFile ../SaveFile";
		std::string cmdLine_Child_2 = os.str();
		os.str("");

		os << from_.address << " " << from_.port << " " << "[Test_Worker_3] " << "localhost " << "9003 " << "../SendFile ../SaveFile";
		std::string cmdLine_Child_3 = os.str();
		os.str("");

		p1.title("[Test_Worker_1]");
		p2.title("[Test_Worker_2]");
		p3.title("[Test_Worker_3]");

		p1.commandLine(cmdLine_Child_1);
		p2.commandLine(cmdLine_Child_2);
		p3.commandLine(cmdLine_Child_3);

		p1.create();
		p2.create();
		p3.create();

		std::function<void(void)> callback = []()
		{
			std::cout << "\n\n-- A Test Worker Has Exited...";
		};

		p1.setCallBackProcessing(callback);
		p1.registerCallback();
		p2.setCallBackProcessing(callback);
		p2.registerCallback();
		p3.setCallBackProcessing(callback);
		p3.registerCallback();


		std::cout << "\n\n-- Starting TestManager Server Receiving Thread Listening On: " << from_.toString();
		recvr = std::thread([&]() { recvMessages(); });
		recvr.detach();

		std::cout << "\n\n-- Starting TestManager Server Dispatching Thread:";
		dspat = std::thread([&]() { dispatchMessages(); });
		dspat.detach();
	}

	// Wait for TestManager Server to shutdow
	void TestManager::wait()
	{
		if (recvr.joinable())
			recvr.join();
		if (dspat.joinable())
			dspat.join();
	}

	// Stop TestManager server 
	void TestManager::stop()
	{
		MsgPassingCommunication::Message stop;
		stop.command("TestHarnessStop");
		stop.to(from_);  // send to self
		stop.from(from_);
		comm_.postMessage(stop);
	}

	// Send a message anywhere you want 
	void TestManager::sendMessage(MsgPassingCommunication::Message msg)
	{
		comm_.postMessage(msg);
	}

	// Revceive message 
	void TestManager::recvMessages()
	{
		while (true)
		{
			MsgPassingCommunication::Message msg = comm_.getMessage();
			std::cout << "\n\n-- Test_Manager_Server Has Received Messages: " + msg.name();
			//msg.show();
			if (msg.command() == "ready")
			{
				std::cout << "\n\n-- Test_Manager_Server enqueues a Ready_Message From A Test_Worker..";
				readyQ_.enQ(msg);
			}
			else if (msg.command() == "testReq")
			{
				std::cout << "\n\n-- Test_Manager_Server enqueues a Test_Request_Message From TestPerformer..";
				requestQ_.enQ(msg);
			}
			else if (msg.command() == "stop")
			{
				std::cout << "\n\n-- Test_Manager_Server enqueues Stop_Testing_Message..";
				requestQ_.enQ(msg);
			}
			else
			{
				std::cout << "\n\n-- Test_Manager_Server has received an invalid message command: " << msg.command();
				std::cout << "\n-- Message title: " << msg.name();
			}
		}
	}


	// Dequeue messages and send to testers 
	void TestManager::dispatchMessages()
	{
		while (true)
		{
			MsgPassingCommunication::Message trMsg = requestQ_.deQ();
			std::cout << "\n-- Test_Manager_Server_Dispatch deQ Request_Message: " + trMsg.command();

			MsgPassingCommunication::Message rdyMsg = readyQ_.deQ();
			std::cout << "\n-- Test_Manager_Server_Dispatch deQ Ready_Message: " + rdyMsg.command();

			// Sending a request to ready TestWorker
			trMsg.to(rdyMsg.from());
			std::cout << "\n-- Test_Manager_Server_Dispatch Sending Message to Test Worker: " + trMsg.command();
			comm_.postMessage(trMsg);
		}
	}

	//----< Setup file sending and receiving path >------------------------
	void TestManager::setPath(std::string& send_path, std::string& save_path)
	{
		comm_.setSendFilePath(send_path);
		comm_.setSaveFilePath(save_path);
	}

	//----< Get file send path >------------------------
	std::string TestManager::getSendPath()
	{
		return comm_.getSendFilePath();
	}

	//----< Get file receive path >------------------------
	std::string TestManager::getSavePath()
	{
		return comm_.getSaveFilePath();
	}
}



int main()
{
	std::cout << "------------  [Test Manager Server]  ------------" << std::endl;


	EndPoint from("localhost", 8888);
	Testing::TestManager th(from);

	std::string send_path = "../SendFile";
	std::string save_path = "../SaveFile";
	th.setPath(send_path, save_path);

	std::cout << "\n\n----------------------------------------------------------" << std::endl;
	std::cout << "-- Test Server Configuration: " << std::endl;
	std::cout << "-- Network Domain: " << "Test_Manager_Server" << std::endl;
	std::cout << "-- Port Address: " << 8888 << std::endl;
	std::cout << "-- Test Driver Send Path: " << send_path << std::endl;
	std::cout << "-- Test Driver Save Path: " << save_path << std::endl;


	getchar();//gets a character (an unsigned char) from stdin
	
}



