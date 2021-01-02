# Simple Factory Scheduler

# Overview
Given a list of orders, this simple scheduler returns a schedule for each of the three plantations (X,Y,Z) that makeup the factory.
To optimize the production of the three plants, you may choose one of two algorithms: First Come First Served (FCFS) / Shortest Job First (SJF).
The implementation of those algorithms is non-preemptive.

# Instructions
* Run the file "*Scheduler.c*"

# Commands
* Enter a command of the format "command [arguments]" as follows:
	* addPERIOD [start date] [end date], with dates in the format [year-month-day]
	* addORDER [Order Number] [Due Date] [Quantity] [Product Name]
	* addBATCH [batch file] with batch  file containing a list of commands
	* runPLS [Algorithm] | printREPORT > [Report file name], with Algorithm = FCFS or SJF 
	* exitPLS
# Test Data
  * The  test data folder contains four batch files containg a series of commands of various types
# Example
  *  The files "*report_01_FCFS.txt*" and "*report_01_SJF.txt*" contain the schedule of the Factory using FCFS and SJF respectively, on the orders of the batch file "*TestData/randomSizeTest.dat*"
  * The schedules include:
  	* The table of Accepted orders
	* The tables of Rejected orders
  	* The number of days each plant is used 
	* The number of products produced
	* The utilization of each plant (%)
	* The overall utilization of the factory (%)

