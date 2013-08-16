/*
 * processLock.cpp
 *
 *  Created on: Aug 16, 2013
 *      Author: keym
 */
#include"commonfunction/processLock.h"

int Getfd_flock()
{
	int fd;
	fd=open(FLOCK_FILENAME,O_WRONLY|O_CREAT);
	return fd;
}

int Wlock_flock(int fd)
{
	return flock(fd,LOCK_EX);
}

int Rlock_flock(int fd)
{
	return flock(fd,LOCK_SH);
}

int Unlock_flock(int fd)
{
	return flock(fd,LOCK_UN);
}
