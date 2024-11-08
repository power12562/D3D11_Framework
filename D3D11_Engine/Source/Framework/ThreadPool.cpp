#include "ThreadPool.h"
using namespace Concurrency;

void ThreadPool::WorkerThreadsFunc(int threadIndex, ThreadPool* threadManager)
{
	while (true)
	{
		{
			std::unique_lock<std::mutex> lock(threadManager->mutex_jobs);
			threadManager->cv_jobs.wait(lock, [threadManager](){ return threadManager->ThreadEventFunc(); });
		}
		if (threadManager->threadAllStop)
		{
			return; //스레드 종료
		}
		else
		{
			std::function<void()> job;
			if (threadManager->jobsQueue.try_pop(job))
			{
				threadManager->isThreadWork[threadIndex] = true; 
				job(); //처리
				threadManager->isThreadWork[threadIndex] = false;
			}
		}
	}
}

bool ThreadPool::ThreadEventFunc()
{
	return !jobsQueue.empty() || threadAllStop;
}

void ThreadPool::Initialize()
{
	if (isInit)
		Uninitialize();

	threadsCount = std::thread::hardware_concurrency();
	if (threadsCount > MaxThreadCount)
		threadsCount = MaxThreadCount;

	workerThreads.reserve(threadsCount);
	for (unsigned int i = 0; i < threadsCount; i++)
	{
		workerThreads.emplace_back(WorkerThreadsFunc, i, this);
		isThreadWork[i] = false;
	}

	isInit = true;
	return;
}

void ThreadPool::Uninitialize()
{
	{
		std::lock_guard<std::mutex> lock(mutex_jobs); //쓰기시 락. mutex는 내부적으로 SRWLock.
		threadAllStop = true;
	} //스코프 벗어나면서 언락
	cv_jobs.notify_all(); //이벤트 시그널
	for (auto& thread : workerThreads)
	{
		thread.join(); //종료 대기
	}

	isInit = false;
	return;
}

void ThreadPool::PushJob(std::function<void()> job)
{
	jobsQueue.push(job);
}

void ThreadPool::WorkJob()
{
	cv_jobs.notify_all();
}

unsigned int ThreadPool::GetWaitingThreadCount() const
{
	unsigned int count = 0;
	for (unsigned int i = 0; i < threadsCount; i++)
	{
		if(!isThreadWork[i])
			count++;
	}
	return count;
}

unsigned int ThreadPool::GetWorkerThreadsCount() const
{
	unsigned int count = 0;
	for (unsigned int i = 0; i < threadsCount; i++)
	{
		if(isThreadWork[i])
			count++;
	}
	return count;
}

ThreadPool::ThreadPool()
{

}

ThreadPool::~ThreadPool()
{
	if (isInit)
		Uninitialize();
}

