/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Fang Wang, Syracuse University
// CSE687 object orriented Design
//
//  TestPerformer.cpp : This file contains the 'main' function. Program execution begins and ends there. 
//  Child tester's ip address and port are assigned by TestManager server.
//  Parameters are passed from TestHarness Server to Child tester through the Process Pooling framework.
/////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>
#include <functional>
#include <thread>
#include "TestPerformer.h"
#include <filesystem>
#include <vector>
#include <windows.h>

namespace fs = std::filesystem;

//define TEST_Performer

using namespace MsgPassingCommunication;

// Constructor for Test Performer class, setup its ip address and port number, start Comm instance
Testing::TestPerformer::TestPerformer(std::string& address, size_t& port, std::string& to_address, size_t& to_port)
	:to_(to_address, to_port), from_(address, port), PerformerComm_(from_, "PerformerComm_")
{
	PerformerComm_.start();
	path_ = PerformerComm_.getSaveFilePath();
	start();
}

void Testing::TestPerformer::start()
{
	std::cout << "\n\n-- Starting TestPerformer watch message thread listening on: " + from_.toString() << std::endl;
	recv = std::thread([&]() { watchMessage(); });
	recv.detach();

}


void Testing::TestPerformer::watchMessage() {
	std::cout << "-- TestPerformer watch message thread stared." << std::endl;
	while (true) {		
		MsgPassingCommunication::Message msg = PerformerComm_.getMessage();

		if (msg.command() == "result") {
			std::cout << " \n-- TestPerformer received a result from TestWorker: " << msg.value("result") << msg.value("dll") << "\n"<<std::endl;


		}
	}

}


void Testing::TestPerformer::sendTestRequests(std::vector<std::string>& dllList,std::string author) {


	MsgPassingCommunication::Message test;
	test.from(from_);
	test.to(to_);
	test.command("testReq");
	

	for (int i = 0; i < dllList.size(); ++i) {
		std::cout << "Sending test request to Test Manager with file name: " << dllList[i] << std::endl;
		test.attribute("sendingFile", dllList[i]);
		test.attribute("author",author);
		PerformerComm_.postMessage(test);
	}

}

void Testing::TestPerformer::myTest() {
	
	std::cout << "myTest() is stared!" << std::endl;

	MsgPassingCommunication::Message test;
	test.from(from_);
	test.to(to_);
	test.command("testReq");
	test.attribute("sendingFile", "Dll1.dll");
	

	std::cout << "\n-- Tester Sending Message to Server: " << test.command() << std::endl;
	PerformerComm_.postMessage(test);

	/*test.attribute("sendingFile", "Dll2.dll");
	PerformerComm_.postMessage(test);
	test.attribute("sendingFile", "Dll3.dll");
	PerformerComm_.postMessage(test);
	test.attribute("sendingFile", "Dll4.dll");
	PerformerComm_.postMessage(test);*/

	std::cout << "-- sending over !!!!!!!!!!!!!!!!!!!!!!!" << std::endl;

}


std::vector<std::string> split(const std::string& str, const std::string& pattern)
{
	std::vector<std::string> res;
	if (str == "")
		return res;

	std::string strs = str + pattern;
	size_t pos = strs.find(pattern);

	while (pos != strs.npos)
	{
		std::string temp = strs.substr(0, pos);
		res.push_back(temp);
		strs = strs.substr(pos + 1, strs.size());
		pos = strs.find(pattern);
	}

	return res;
}

void stuckThere() {
	while(true){}
}

int main()
{
    std::cout << "\n======================= [Test Performer] =======================";

	std::string addr_to = "localhost";
	size_t port_to = 8888;
	std::string addr_from = "localhost";
	size_t port_from = 9101;
	std::string send_path = "../SendFile";
	std::string receive_path = "../SaveFile";

	std::cout << "\n\n\nTest_Performer_Configuration:" << std::endl;
	std::cout << "-- Receive_From:  " << addr_from << std::endl;
	std::cout << "-- Test_Performer_Port:  " << port_from << std::endl;
	std::cout << "-- Sending_To:  " << addr_to << std::endl;
	std::cout << "-- Server_Port:  " << port_to << std::endl;
	std::cout << "-- Sending_Files_Path:  " << send_path << std::endl;
	std::cout << "-- Receiving_Files_Path:  " << receive_path << std::endl;

	Testing::TestPerformer tp(addr_from, port_from, addr_to, port_to);
	

	//tp.myTest();


	Sleep(2000);

	std::vector<std::string> testingfiles;

	std::cout << "\n\n\n=============================== [welcom to test harness] =============================== " << std::endl;
	std::cout << "\navailable dll files to be tested:\n" << std::endl;
	

	int dllnum = 0;
	std::string path = "../sendfile";
	for (const auto& entry : fs::directory_iterator(path)){
		std::cout << "[" << dllnum << "] " << entry.path().filename().u8string() << std::endl;
		testingfiles.push_back(entry.path().filename().u8string());
		dllnum++;
	}
	std::string author;
	std::cout << "\n\nplease enter operator's name: ";
	std::cin >> author;
	
	std::string choices;
	std::cout << "\nplease type into the dll file numbers to be tested (separated by comma):";
	std::cin >> choices;

	std::vector<std::string> choicelist;
	choicelist = split(choices, ",");

	std::vector<std::string> dlllist;

	for (int i = 0; i < choicelist.size(); ++i) {
		int dllindex = std::stoi(choicelist[i]);
		dlllist.push_back(testingfiles[dllindex]);
		
	}

	tp.sendTestRequests(dlllist,author);
	//tp.myTest();
		

	//getchar();
	stuckThere();
	return 0;


}





