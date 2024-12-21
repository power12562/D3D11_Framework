#pragma once
#include <vector>

class StaticBlockMemoryPool
{
public:
	StaticBlockMemoryPool();
	~StaticBlockMemoryPool();

	bool Initialize(size_t MemoryBlockSize, size_t MemoryPageSize = 4096);
	void Uninitialize();

	/**�޸𸮸� ��û�մϴ�. �Ҵ� �����ϰų� �̹� ������� Index�� �����ϸ� nullptr�� ��ȯ�մϴ�. */
	void* Allocate(size_t blockIndex);

	/**�޸𸮸� �ݳ��մϴ�. */
	void Free(size_t blockIndex);
private:
	size_t memoryBlockSize;
	size_t memoryPageSize;
	size_t blockMaxCount;

private:
	bool AllocateMemoryPage();
	std::vector<void*> memoryPage;

private:
	std::vector<bool> activeIDs;

private:
	bool GetUniqueID(size_t id);
	void ReturnID(size_t id);
};