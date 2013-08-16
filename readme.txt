1、水平触发所以如果来不及接受，则下次仍会触发，不会丢失
	struct epoll_event ev;
	ev.data.fd=fd;
	//ev.events=EPOLLIN|EPOLLET;
	ev.events=EPOLLIN;
	epoll_ctl(_epfd,EPOLL_CTL_ADD,fd,&ev);
}
在此处决定是边缘（EPOLLET）还是水平触发（默认）
2、会产生惊群现象
3、将设备句柄放入到各个子进程，则一但某个设备发生所有的子进程都会被触发，但是只有一个子进程处理事件
