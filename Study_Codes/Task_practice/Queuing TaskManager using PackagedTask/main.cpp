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

	//�̸� task ����
	auto task = std::make_shared<std::packaged_task<ResultType()>>(
		std::bind(std::forward<F>(function), std::forward<Args>(args)...));

	std::cout << typeid(task).name() << std::endl;

	auto res = task->get_future();

	// �񵿱� ����
	std::async(std::launch::async, [task]() { (*task)(); });

	return res;
}

int main()
{
	using ResultType = std::invoke_result<decltype(print), int, int>::type;
	// ���� �Լ��� ���ؼ� ������ ���� ������ �����ϴ�.
	std::cout <<"PRINT�Լ��� ���� ��ȯ ��: " << typeid(ResultType).name() << '\n';

	using AFunctionResultType = std::invoke_result<decltype(&A::hi),A>::type;
	// class ������ ��� �Լ��� ��� Ŭ���� ��ü�� ���� ������ ���������.

	std::cout << "AfunctionResult�� �Լ� ��ȯ��: " << typeid(AFunctionResultType).name() << '\n';

	auto futuredata=setFuture(add,1,2);

	std::cout << "async�� ���� ���: " << futuredata.get();

	taskmanager::getInstance().initThread(4);
	auto fuda=taskmanager::getInstance().insertTask(add, 5, 4);
	std::cout << "\ntask queue�� �ڵ����� ���� ���� " << fuda.get() << std::endl;
	taskmanager::getInstance().insertTask(print,10,20);


}