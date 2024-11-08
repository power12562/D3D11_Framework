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

	/*�۾��� �Լ� ����*/
	void PushJob(std::function<void()> job);
	
	/*������ ������ ����*/
	unsigned int GetThreadsCount() const { return threadsCount; }

	/*������� ������ ����*/
	unsigned int GetWaitingThreadCount() const;

	/*�۾����� ������ ����*/
	unsigned int GetWorkerThreadsCount() const;

private:
	ThreadManager();
	virtual ~ThreadManager() override;

private:
	static void WorkerThreadsFunc(int threadIndex);
	static bool ThreadEventFunc();

private:
	unsigned int threadsCount = 0; //������ ������ ����
	std::vector<bool> isThreadWork; //������ ���� ����.

private:
	std::vector<std::thread> workerThreads;
	Concurrency::concurrent_queue<std::function<void()>> jobsQueue;
	std::condition_variable cv_jobs;
	std::mutex mutex_jobs;

private:
	bool threadAllStop = false;
};