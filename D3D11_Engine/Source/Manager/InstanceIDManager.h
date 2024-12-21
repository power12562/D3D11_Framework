#pragma once
#include <iostream>
#include <set>
#include <queue>
#include <Core/TSingleton.h>

class InstanceIDManager;
extern InstanceIDManager& instanceIDManager;

class InstanceIDManager : public TSingleton<InstanceIDManager>
{
    friend class TSingleton;
    InstanceIDManager();
    virtual ~InstanceIDManager() override;
private:
    std::set<unsigned int> activeIDs;  // ��� ���� ID�� ����
    std::queue<unsigned int> availableIDs;       // ���� ������ ID ���
    unsigned int nextID = 0;
public:
    unsigned int getUniqueID();
    void returnID(unsigned int id);
};
