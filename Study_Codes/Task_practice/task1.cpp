#include<iostream>
#include<vector>
#include<future>



int task_function(int val1, int val2) {
	return val1 * val2;
}


int main() {

	std::vector<std::future<int>> tasks;

	tasks.push_back(std::async(std::launch::async, task_function, 1,3));
	/*
		task 1Àå
		async( launch:: async , task_function, ÀÎÀÚ ....));
		
	*/
	int result = tasks[0].get();
	std::cout << result;

}