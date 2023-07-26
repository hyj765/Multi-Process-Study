#include<iostream>
#include<vector>
#include<thread>
#include<functional>
#include<mutex>
#include<condition_variable>

std::mutex mtx;
std::condition_variable cv;
bool isReady = false;

void threadA() {
	std::unique_lock<std::mutex> lock(mtx);
	cv.wait(lock, []() {return isReady; });

	std::cout << "Thread A is Awake" << std::endl;
	isReady = false;
	cv.notify_one();
}

void threadB() {
	{
		std::lock_guard<std::mutex> lock(mtx);
		isReady = true;
	}
	cv.notify_one();

	std::unique_lock<std::mutex> lock(mtx);
	cv.wait(lock, []() {return !isReady; });
	std::cout << "thread A complete Notificatioin from thread A " << std::endl;
}



int main() {

	std::thread thA(threadA);
	std::thread thB(threadB);

	thA.join();
	thB.join();

	return 0;
}