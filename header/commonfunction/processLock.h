/*
 * processLock.h
 *
 *  Created on: Aug 16, 2013
 *      Author: keym
 */

#ifndef FILELOCK_H_
#define FILELOCK_H_

#include<fcntl.h>
#include</usr/include/x86_64-linux-gnu/sys/file.h>
#include</usr/include/x86_64-linux-gnu/sys/types.h>
#include</usr/include/x86_64-linux-gnu/sys/stat.h>
#include<unistd.h>

#define FLOCK_FILENAME "devprocess.lock"

#ifdef __cplusplus
extern "C" {
#endif

int Getfd_flock();

int Wlock_flock(int fd);

int Rlock_flock(int fd);

int Unlock_flock(int fd);


#ifdef __cplusplus
}
#endif

#endif /* FILELOCK_H_ */
