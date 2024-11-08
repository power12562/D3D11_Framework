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

	/*�۾��� �Լ� ����*/
	void PushJob(std::function<void()> job);

	/*������ �����.*/
	void WorkJob();
	
	/*������ ������ ����*/
	unsigned int GetThreadsCount() const { return threadsCount; }

	/*������� ������ ����*/
	unsigned int GetWaitingThreadCount() const;

	/*�۾����� ������ ����*/
	unsigned int GetWorkerThreadsCount() const;

	/*�۾� ���� ����*/
	bool IsJobsEmpty() { return jobsQueue.empty(); }
public:
	ThreadPool();
	~ThreadPool();

private:
	static void WorkerThreadsFunc(int threadIndex, ThreadPool* threadManager);

private:
	bool ThreadEventFunc();

private:
	unsigned int threadsCount = 0; //������ ������ ����
	std::array<std::atomic_bool, MaxThreadCount> isThreadWork; //������ ���� ����.

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