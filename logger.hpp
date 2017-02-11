#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include <iostream>
#include <fstream>
#include <queue>
#include <thread>
#include <iomanip>
#include <ctime>
#include <chrono>
#include <time.h>

class Logger
{
private:

	static std::queue<std::string> logQueue;
	static bool isProcessing;

public:

  // LOG
  static void Logger::log(std::string message)
  {
    // add this message to the queue
    logQueue.push(message);

    // if there is no worker processing, kick one off
    if (isProcessing == false)
    {
      std::cout << "process" << std::endl;

      isProcessing = true;
      std::thread(&Logger::processLogs);
    }
  };
};

// PROCESS LOGS
static void Logger::processLogs()
{
	// create logfile name from current date
	time_t rawtime;
	time(&rawtime);

	char szOut[255];
	auto size = strftime(szOut, 255, "%Y_%m_%d", std::gmtime(&rawtime));

	std::string fileName(szOut, size);
	fileName += ".log";

	std::cout << fileName << std::endl;

	// open log file
	std::ofstream logFile;
	logFile.open(fileName, std::ios_base::app);

	// keep processing while queue is still filled with log messages
	while (!logQueue.empty())
	{
		// dequeue a log item
		std::string logMessage = logQueue.front();
		logQueue.pop();

		logFile << logMessage;
	}

	// close logfile afterwards
	logFile.close();

	isProcessing = false;
};


#endif
