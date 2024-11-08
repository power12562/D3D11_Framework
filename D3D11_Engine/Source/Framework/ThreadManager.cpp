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
			return; //������ ����
		}
		else
		{
			std::function<void()> job;
			if (threadManager.jobsQueue.try_pop(job))
			{
				threadManager.isThreadWork[threadIndex] = true; 
				job(); //ó��
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
		std::lock_guard<std::mutex> lock(mutex_jobs); //����� ��. mutex�� ���������� SRWLock.
		threadAllStop = true;
	} //������ ����鼭 ���
	cv_jobs.notify_all(); //�̺�Ʈ �ñ׳�
	for (auto& thread : workerThreads)
	{
		thread.join(); //���� ���
	}
	return;
}

void ThreadManager::PushJob(std::function<void()> job)
{
	jobsQueue.push(job);
	cv_jobs.notify_one(); //�̺�Ʈ �ñ׳�
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

