/*
This file is part of MIBMSCloudSystem Project.
Core.h/.cpp - The Core of Server/Client.
Copyright (c) 2021 SuYichen.
*/
#include "Core.h"
#include "MethodsLibrary.h"
#include "TaskInfo.h"
#include <vector>
#include <mutex>
#include <thread>
#include <JsonHandler.h>
#include <MIBMSCloud.h>
#include <INIOperation.h>

#ifdef SERVER
Core::Core()
{
    CPUCoresNum = GetCPUCoresNum();
    signed int ThreadNumLimit = (CPUCoresNum/2);
    printf("Server is now running on a %d Logical Cores CPU \n",CPUCoresNum);
    RecvTasks = {};
    SHandler=new sendhandler;
    MLC = new ModulesListContainer;
    CMyINI* p = new CMyINI();
    p->ReadINI("./Configs/ServerSettings.ini");
    std::string TaskHanlerThreadsNum= p->GetValue("Performance", "TaskHanlerThreadsNum");
    int s = atoi(TaskHanlerThreadsNum.c_str());
    if (s < 1) s = 1;
    else if (s > ThreadNumLimit) s= ThreadNumLimit;
    vector<thread*> t;
    for (int i = 0; i < s; i++) 
    {
        t.emplace_back(new thread(&Core::TaskHandler,this));
    }
    printf("ServerCore->TaskHandler共启用了%d个线程\n",s);
}

Core::~Core()
{
    delete MLC;
}
void Core::AddTask(SOCKET client, send_info info)
{
    
    RecvTaskInfo Newinfo = {client,info};
    RecvTasks.emplace_back(Newinfo);
}
void Core::ModuleLogout(SOCKET client)
{
    MLC->RemoveClientFromList(client);
}
std::string Core::FindModule(SOCKET client)
{
    return MLC->GetModuleName(client);
}
#endif // SERVER

#ifdef CLIENT
Core::Core()
{
    RecvTasks = {};
    thread* t1 = new thread(&Core::TaskHandler, this);
    SHandler = new sendhandler;
}
Core::~Core()
{
}
void Core::AddTask(int user, send_info info)
{
    RecvTaskInfo Newinfo = { user,info };
    RecvTasks.emplace_back(Newinfo);
}
#endif // CLIENT



bool  Core::IsBusy()
{
    return (sizeof(RecvTasks)>0);
}

void Core::TaskHandler()
{
    while (true)
    {
        some_mutex.lock();
        if (RecvTasks.size() > 0)
        {
            RecvTaskInfo info = RecvTasks[0];
            RecvTasks.erase(RecvTasks.begin());
            some_mutex.unlock();
            jsonhandler* Jhandler = new jsonhandler;
            int MT = Jhandler->_get_Json_value_int(info.Sinfo.info_content, "MesType");
            delete Jhandler;
            switch (MT)
            {
            case GEN:
            {
                GenerateNewClient(info.client, info.Sinfo.info_content);
            }
            #ifdef SERVER
            case COM:
            {
                CommandHandler(info.client, info.Sinfo.info_content);
            }
            #endif//SERVER
            case REP:
            {
                InfoReportHandler(info.client, info.Sinfo.info_content);
            }
            case ERR:

            {
                ErrorReportHandler(info.client, info.Sinfo.info_content);
            }
            case WAR:
            {
                WarningReportHandler(info.client, info.Sinfo.info_content);
            }
            default:
                break;
            }
        }
        else
        {
            some_mutex.unlock();
        }
    }
}



void Core::GenerateNewClient(SOCKET client, char info_content[1024])
{
    #ifdef SERVER
    std::string ModuleName;
    jsonhandler* JHandler=new jsonhandler;
    ModuleName=JHandler->_get_Json_value_string(info_content, "ModuleName");
    ModuleClientInfo MCInfo={client,ModuleName};
    cout << "模块 "+ModuleName+" 已注册！" << endl;
    MLC->AddClientInfoToList(MCInfo);
    #endif//  SERVER
    #ifdef CLIENT

    #endif // CLIENT

}


void Core::CommandHandler(SOCKET client,  char info_content[1024])
{
   #ifdef SERVER

   #endif//  SERVER
   #ifdef CLIENT
   jsonhandler* JHandler = new jsonhandler;
   bool State = JHandler->_get_Json_value_bool(info_content,"STATE");
   delete JHandler;
   #endif // CLIENT
}

void Core::InfoReportHandler(SOCKET client, char info_content[1024])
{
   #ifdef SERVER

   #endif//  SERVER
   #ifdef CLIENT

   #endif // CLIENT

}

void Core::ErrorReportHandler(SOCKET client, char info_content[1024])
{
   #ifdef SERVER

   #endif//  SERVER
   #ifdef CLIENT

   #endif // CLIENT

}

void Core::WarningReportHandler(SOCKET client,char info_content[1024])
{
    #ifdef SERVER

    #endif//  SERVER
    #ifdef CLIENT

    #endif // CLIENT

}

