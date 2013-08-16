/*
 * childProcess.cpp
 *
 *  Created on: 2013-1-30
 *      Author: keym
 */
#include"childProcess.h"
#include"commondata/commontype.h"
#include"messagehandle/messageHandle.h"
#include"commondata/magicNum.h"

#include"parentProcess.h"
#include<iostream>
#include<exception>
#include<stdio.h>
#include<string.h>
#include<deque>

static void packReqCloseMsg(void *buffer);

void childProcess::acceptClientSocket()
{
	if((this->_clientSocket = recv_fd(this->_parentSocket,write)) == magicnum::FAILIED)
	{
		std::cerr<<"childProcess::acceptClientSocket:recv_fd"<<std::endl;
		throw std::exception();
	}
	this->_isFreeFlag = 1;
}

void childProcess::InitializeProcess()
{
	handleEpollSocket::initializeEpoll();
	handleEpollSocket::addEpollSocket(this->_parentSocket);
	std::vector<int> vtemp = this->_parentInstance->GetDevFdVector();
	std::vector<int>::iterator iter = vtemp.begin();
	while(iter != vtemp.end())
	{
		handleEpollSocket::addEpollSocket(*iter);
		std::cout<<*iter<<std::endl;
		++ iter;
	}
}

void childProcess::CommunicateHandle()
{
	struct epoll_event events[this->_maxNumOfEpollfd + 1];
	std::deque<pid_t*> _dPid;
	int readbytes;
	int lastfd;
	char readbuf[magicnum::MSGHEADSIZE];
	for(;;)
	{
		int nfds;
		if((nfds=epoll_wait(_epfd,events,this->_maxNumOfEpollfd,magicnum::parentprocess::EPOLLTIMEOUT)) <= 0)
		{
			if (errno != EINTR)
			{
				std::cerr<<"parentProcess::CommunicationHandle:epoll_wait"<<std::endl;
				throw std::exception();
			}
			continue;
		}
		int i;
		for(i=0;i<nfds;i++)
		{
			if(events[i].events&EPOLLIN)
			{
				std::cout<<"chufa"<<getpid()<<std::endl;
				memset(readbuf,0,magicnum::MSGHEADSIZE);
				int evtfd = events[i].data.fd;

				if(evtfd != this->_parentSocket && evtfd != this->_clientSocket && !this->_isFreeFlag)continue;

				if((readbytes=RepeatRecv(evtfd,readbuf,sizeof(commontype::headInfo))) < 0)
				{
					//这里可能是客户端关闭或出现错误
					if(readbytes == magicnum::CLOSEED)
					{
						packReqCloseMsg(readbuf);
					}
				}
				if(evtfd != this->_parentSocket && evtfd != this->_clientSocket && this->_isFreeFlag)
				{
					RepeatRecv(evtfd,readbuf+sizeof(commontype::headInfo),sizeof(pid_t));
					std::cout<<"zhanyou:"<<getpid()<<sizeof(commontype::headInfo)+sizeof(pid_t)<<std::endl;
					dataInfo *data = (dataInfo*)malloc(sizeof(dataInfo));
					data->_pdata= (char*)malloc(sizeof(commontype::headInfo)+sizeof(pid_t));
					memcpy(data->_pdata,readbuf,sizeof(commontype::headInfo)+sizeof(pid_t));
					data->_size = sizeof(commontype::headInfo)+sizeof(pid_t);
					handleEpollSocket::packData(data);
					handleEpollSocket::modEpollSocket(this->_clientSocket,true);
					lastfd = evtfd;
					this->_isFreeFlag = 0;
					continue;
				}

				if(!this->_isFreeFlag && evtfd == this->_clientSocket)
				{
					RepeatRecv(evtfd,readbuf+sizeof(commontype::headInfo),sizeof(pid_t));
					std::cout<<"shifang:"<<getpid()<<std::endl;
//					dataInfo *data = (dataInfo*)malloc(sizeof(dataInfo));
//					data->_pdata= (char*)malloc(sizeof(commontype::headInfo)+sizeof(pid_t));
//					memcpy(data->_pdata,readbuf,sizeof(commontype::headInfo)+sizeof(pid_t));
//					data->_size = sizeof(commontype::headInfo)+sizeof(pid_t);
//					handleEpollSocket::packData(data);
//					handleEpollSocket::modEpollSocket(lastfd,true);
					this->_isFreeFlag = 1;
					continue;
				}



				//处理子进程指令
				messageHandle::getInstance()->msgHandle(readbuf,events[i].data.fd,this);
			}
			else if(events[i].events&EPOLLOUT)
			{
				//通知的顺序与投递的顺序相同
				handleEpollSocket::sendData(events[i].data.fd);
				handleEpollSocket::modEpollSocket(events[i].data.fd,false);
			}
			else if((events[i].events&EPOLLHUP)||(events[i].events&EPOLLERR))
			{
				throw std::exception();
			}
		}
	}
}

static void packReqCloseMsg(void *buffer)
{
	commontype::headInfo* _head = (commontype::headInfo *)buffer;
	_head->_type = magicnum::messagetype::CCREQCLOSED;
	_head->_size = 0;
}
