#pragma once
#include <Framework/TSingleton.h>
#include <concurrent_queue.h>
#include <thread>
#include <functional>
#include <mutex>
#include <atomic>

class ThreadManager;
extern ThreadManager& threadManager;

class ThreadManager : public TSingleton<ThreadManager>
{
	friend class TSingleton;
public:
	void Initialize();
	void Uninitialize();

	/*작업할 함수 삽입*/
	void PushJob(std::function<void()> job);
	
	/*생성된 스레드 개수*/
	unsigned int GetThreadsCount() const { return threadsCount; }

	/*대기중인 스레드 개수*/
	unsigned int GetWaitingThreadCount() const;

	/*작업중인 스레드 개수*/
	unsigned int GetWorkerThreadsCount() const;

private:
	ThreadManager();
	virtual ~ThreadManager() override;

private:
	static void WorkerThreadsFunc(int threadIndex);
	static bool ThreadEventFunc();

private:
	unsigned int threadsCount = 0; //생성된 스레드 개수
	std::vector<bool> isThreadWork; //스레드 실행 여부.

private:
	std::vector<std::thread> workerThreads;
	Concurrency::concurrent_queue<std::function<void()>> jobsQueue;
	std::condition_variable cv_jobs;
	std::mutex mutex_jobs;

private:
	bool threadAllStop = false;
};