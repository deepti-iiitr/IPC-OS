#include <bits/stdc++.h>
#include<unistd.h>
#include<sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

using namespace std;

void writer(int ky, string write)
{
    key_t key = ftok("./IPC.cpp",ky);
    int shmid = shmget(key,1024,0666|IPC_CREAT);
    char *str = (char*) shmat(shmid,(void*)0,0);
    sprintf(str, "%s", write.c_str());
    shmdt(str);
}

string reader(int ky)
{
    key_t key = ftok("./IPC.cpp",ky);
    int shmid = shmget(key,1024,0666|IPC_CREAT);
    char *str = (char*) shmat(shmid,(void*)0,0);
    string data=string(str);
    shmdt(str);
    return data;
}

void destroyer(int ky)
{
  key_t key = ftok("./IPC.cpp",ky);
  int shmid = shmget(key,1024,0666|IPC_CREAT);
  shmctl(shmid,IPC_RMID,NULL);
}

class mutx {
public:

  mutx(int k,int c=1)
  {
    key=k;
    if(reader(k)=="")writer(key,to_string(c).c_str());
  }

  void wait()
  {
    while (stoi(reader(key))==0);
    writer(key,to_string(0).c_str());
  }

  void signal()
  {
    writer(key,to_string(1).c_str());
  }

private:
    int key;
};

class semaphore {
public:

  semaphore(int k,int c=0)
  {
    key=k;
    if(reader(k)=="")
    {
      count=c;
      writer(key,to_string(count).c_str());
    }
  }

  void wait()
  {
    while (stoi(reader(key))<=0);
    count=stoi(reader(key));
    count--;
    writer(key,to_string(count).c_str());
  }

  void signal()
  {
    count=stoi(reader(key));
    count++;
    writer(key,to_string(count).c_str());
  }

private:
    int count;
    int key;
};
