#include<iostream>
#include<vector>
#include<queue>
#include<mutex>
#pragma once


template<typename T>
class RingBuffer
{

public:
	RingBuffer(const uint32_t _buffSize) :buffSize(_buffSize), head(0), tail(0)
	{
		buffer = std::make_unique<T[]>(buffSize);
	};
	virtual ~RingBuffer() {};

	bool isEmpty() const noexcept 
	{
		return (isFull == false && head == tail);
	};

	void enQueue(T data) noexcept 
	{
		std::lock_guard<std::mutex>lock(bufferMutex);
		buffer.get()[(head++ % buffSize)] = data;
	};

	T deQueue() noexcept 
	{
		std::lock_guard<std::mutex>	lock(bufferMutex);
		return buffer.get()[tail++ % buffSize];
	};

	uint32_t isAvaliable() noexcept
	{
		return head - tail;
	}

private:
	
	const uint32_t						buffSize;
	
	std::unique_ptr<T[]>				buffer;
	
	int									head;
	
	int									tail;
	
	bool								isFull;
	
	std::mutex							bufferMutex;
};

