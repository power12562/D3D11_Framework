#include "InstanceIDManager.h"

InstanceIDManager& instanceIDManager = InstanceIDManager::GetInstance();

InstanceIDManager::InstanceIDManager()
{

}

InstanceIDManager::~InstanceIDManager()
{

}

unsigned int InstanceIDManager::getUniqueID()
{
    if (!availableIDs.empty())
    {
        unsigned int id = availableIDs.front();
        availableIDs.pop();
        activeIDs.insert(id);
        return id;
    }

    // availableIDs�� ������� ���ο� ID�� ����
    unsigned int newID = nextID++;
    activeIDs.insert(newID);
    return newID;
}

void InstanceIDManager::returnID(unsigned int id)
{
    if (activeIDs.find(id) != activeIDs.end()) 
    {
        activeIDs.erase(id);
        availableIDs.push(id);
    }
    else
    {
        __debugbreak(); //�ν��Ͻ� ���̵� ����.
    }
}