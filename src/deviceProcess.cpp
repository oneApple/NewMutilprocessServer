#include"deviceProcess.h"
#include"commonfunction/processLock.h"
#include"commondata/commontype.h"
#include"commondata/magicNum.h"
#include</usr/include/x86_64-linux-gnu/sys/socket.h>
#include</usr/include/x86_64-linux-gnu/sys/types.h>
#include<string.h>
#include<stdlib.h>
#include<iostream>


void deviceProcess::CommunicationHandle()
{
	char recvbuf[100];
	int n;
	int fd = Getfd_flock();
	Rlock_flock(fd);

	commontype::headInfo head;
	pid_t pid = getpid();
	head._type = magicnum::messagetype::PCMESSAGEPC;
	head._size = sizeof(pid);
	memcpy(recvbuf,&head,sizeof(head));
	memcpy(recvbuf+sizeof(head),&pid,sizeof(pid));
	sleep(10);
	send(this->_parentfd,recvbuf,sizeof(head) + sizeof(pid),0);
    std::cout<<"dev:send"<<pid<<std::endl;
	while((n = recv(this->_parentfd,recvbuf,sizeof(head) + sizeof(pid),0)) > 0)
	{
		sleep(50);
		send(this->_parentfd,recvbuf,n,0);
	}
	Unlock_flock(fd);
	exit(0);
}
