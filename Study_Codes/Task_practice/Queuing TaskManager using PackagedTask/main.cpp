#include<iostream>
#include<future>
#include<type_traits>
#include "taskmanager.h"

void print(int a, int b)
{
	printf("%d %d\n", a, b);
}

struct A
{
	std::string hi()
	{
		return "hi a";
	}

};
int add(int a, int b)
{
	return a + b;
}

template <typename F, typename... Args>
std::future<typename std::invoke_result<F, Args...>::type> setFuture(F&& function, Args&&... args)
{
	using ResultType = typename std::invoke_result<F, Args...>::type;

	//미리 task 생성
	auto task = std::make_shared<std::packaged_task<ResultType()>>(
		std::bind(std::forward<F>(function), std::forward<Args>(args)...));

	std::cout << typeid(task).name() << std::endl;

	auto res = task->get_future();

	// 비동기 실행
	std::async(std::launch::async, [task]() { (*task)(); });

	return res;
}

int main()
{
	using ResultType = std::invoke_result<decltype(print), int, int>::type;
	// 전역 함수에 한해서 다음과 같은 행위가 가능하다.
	std::cout <<"PRINT함수에 대한 반환 값: " << typeid(ResultType).name() << '\n';

	using AFunctionResultType = std::invoke_result<decltype(&A::hi),A>::type;
	// class 내부의 멤버 함수의 경우 클래스 자체를 인자 값으로 던져줘야함.

	std::cout << "AfunctionResult의 함수 반환값: " << typeid(AFunctionResultType).name() << '\n';

	auto futuredata=setFuture(add,1,2);

	std::cout << "async의 대한 결과: " << futuredata.get();

	taskmanager::getInstance().initThread(4);
	auto fuda=taskmanager::getInstance().insertTask(add, 5, 4);
	std::cout << "\ntask queue가 자동으로 해준 업무 " << fuda.get() << std::endl;
	taskmanager::getInstance().insertTask(print,10,20);


}