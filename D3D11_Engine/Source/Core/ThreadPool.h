#pragma once
#include <concurrent_queue.h>
#include <thread>
#include <functional>
#include <mutex>
#include <atomic>
#include <array>

class ThreadPool
{
public:
	static constexpr int MaxThreadCount = 128;
public:
	void Initialize();
	void Uninitialize();

	/*작업할 함수 삽입*/
	void PushJob(std::function<void()> job);

	/*스레드 깨우기.*/
	void WorkJob();
	
	/*생성된 스레드 개수*/
	unsigned int GetThreadsCount() const { return threadsCount; }

	/*대기중인 스레드 개수*/
	unsigned int GetWaitingThreadCount() const;

	/*작업중인 스레드 개수*/
	unsigned int GetWorkerThreadsCount() const;

	/*작업 존재 유무*/
	bool IsJobsEmpty() { return jobsQueue.empty(); }
public:
	ThreadPool();
	~ThreadPool();

private:
	static void WorkerThreadsFunc(int threadIndex, ThreadPool* threadManager);

private:
	bool ThreadEventFunc();

private:
	unsigned int threadsCount = 0; //생성된 스레드 개수
	std::array<std::atomic_bool, MaxThreadCount> isThreadWork; //스레드 실행 여부.

private:
	std::vector<std::thread> workerThreads;
	Concurrency::concurrent_queue<std::function<void()>> jobsQueue;
	std::condition_variable cv_jobs;
	std::mutex mutex_jobs;

private:
	bool threadAllStop = false;

private:
	bool isInit = false;
};