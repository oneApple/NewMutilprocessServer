/*
 * deviceProcess.h
 *
 *  Created on: Mar 27, 2013
 *      Author: keym
 */

#ifndef DEVICEPROCESS_H_
#define DEVICEPROCESS_H_

class deviceProcess
{
private:
	int _parentfd;
public:
	deviceProcess(int pfd):_parentfd(pfd){}
	void CommunicationHandle();
};

#endif /* DEVICEPROCESS_H_ */
