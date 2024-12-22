#pragma once
#include<Windows.h>
#include<iostream>
#include<mutex>
#include<thread>

class FileInfo
{
public:

	FileInfo(std::string_view _filePath, std::string_view _fileId) : filePath(_filePath),fileId(_fileId)
	{
	}

	~FileInfo()
	{
		if (buffer != nullptr)
		{
			delete[] buffer;
		}

		CloseHandle(mFileHandle);
	}

	void CreateFileSession(std::string_view _filePath, std::string_view _fileId)
	{
		filePath = _filePath;
		fileId = _fileId;
	}

	FileInfo* operator=(FileInfo* _FileInfo)
	{
		return this;
	}

	void ReadFileAsyc()
	{
		std::thread innerReadThread =std::thread([this]() {ReadFileAsnycImpl(); });
		innerReadThread.detach();
	}

	char* GetFileBuffer()
	{
		int count = 5;
		while (readcompletion == false)
		{
			if (count == 0)
			{
				std::cerr << "ReadFile doesn't Complete or Some Error Terminate FileReadThread" << std::endl;
				break;
			}
			count--;
			Sleep(200);
		}

		return buffer;
	}

	uint32_t GetFileSize_()
	{
		return fileSize;
	}

	HANDLE* GetFileHandle()
	{
		return &mFileHandle;
	}

private:

	void ReadFileAsnycImpl()
	{
		mFileHandle = CreateFileA(filePath.c_str(), GENERIC_ALL, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
		if (mFileHandle == INVALID_HANDLE_VALUE)
		{
			std::cerr << "file error occur Error code: " << GetLastError() << std::endl;
			return;
		}
		fileSize = GetFileSize(mFileHandle, NULL);

		buffer = new char[fileSize];

		DWORD readBytes = 0;
		readOverlapped.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		if (readOverlapped.hEvent == NULL)
		{
			std::cerr << "fail to allocate event" << std::endl;
			return;
		}
		bool result=ReadFile(mFileHandle, buffer, fileSize, &readBytes, &readOverlapped);
		if (result == 0 && GetLastError() != ERROR_IO_PENDING)
		{
			std::cerr << "Readfile Fail " << GetLastError() << std::endl;
			return;
		}
		DWORD waitResult=WaitForSingleObject(readOverlapped.hEvent, INFINITE);
		if (waitResult == WAIT_OBJECT_0)
		{
			result=GetOverlappedResult(mFileHandle, &readOverlapped, &readBytes, FALSE);
			if (result == false)
			{
				std::cerr << "Failed To Get Overlapped Result error: " << GetLastError() << std::endl;
				{
					std::lock_guard<std::mutex> lock(mtx);
					readFailFlag = true;
				}
			}
		}
	
		CloseHandle(readOverlapped.hEvent);

		{
			std::lock_guard<std::mutex> lock(mtx);
			readcompletion = true;
		}
	}

	std::mutex mtx;
	HANDLE mFileHandle;
	std::string filePath;
	std::string fileId;
	char* buffer = nullptr;
	uint32_t fileSize = 0;
	OVERLAPPED readOverlapped = { 0, };
	bool readcompletion = false;
	bool readFailFlag = false;
};