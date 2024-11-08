#include "ThreadManager.h"
using namespace Concurrency;

ThreadManager& threadManager = ThreadManager::GetInstance();

void ThreadManager::WorkerThreadsFunc(int threadIndex)
{
	while (true)
	{
		{
			std::unique_lock<std::mutex> lock(threadManager.mutex_jobs);
			threadManager.cv_jobs.wait(lock, ThreadEventFunc);
		}
		if (threadManager.threadAllStop)
		{
			return; //스레드 종료
		}
		else
		{
			std::function<void()> job;
			if (threadManager.jobsQueue.try_pop(job))
			{
				threadManager.isThreadWork[threadIndex] = true; 
				job(); //처리
				threadManager.isThreadWork[threadIndex] = false;
			}
		}
	}
}

bool ThreadManager::ThreadEventFunc()
{
	return !threadManager.jobsQueue.empty() || threadManager.threadAllStop;
}

void ThreadManager::Initialize()
{
	threadsCount = std::thread::hardware_concurrency();
	workerThreads.reserve(threadsCount);
	isThreadWork.resize(threadsCount);
	for (unsigned int i = 0; i < threadsCount; i++)
	{
		workerThreads.emplace_back(WorkerThreadsFunc, i);
	}
	return;
}

void ThreadManager::Uninitialize()
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
	return;
}

void ThreadManager::PushJob(std::function<void()> job)
{
	jobsQueue.push(job);
	cv_jobs.notify_one(); //이벤트 시그널
}

unsigned int ThreadManager::GetWaitingThreadCount() const
{
	unsigned int count = 0;
	for (unsigned int i = 0; i < threadsCount; i++)
	{
		if(!isThreadWork[i])
			count++;
	}
	return count;
}

unsigned int ThreadManager::GetWorkerThreadsCount() const
{
	unsigned int count = 0;
	for (unsigned int i = 0; i < threadsCount; i++)
	{
		if(isThreadWork[i])
			count++;
	}
	return count;
}

ThreadManager::ThreadManager()
{
}

ThreadManager::~ThreadManager()
{
}

