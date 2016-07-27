#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include "rs485.h"
#include <stdio.h> /*标准输入输出定义*/
#include <stdlib.h> /*标准函数库定义*/
#include <unistd.h> /*linux 标准函数定义*/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> /*文件控制定义*/
#include <termios.h> /*PPSIX 终端控制定义*/
#include <errno.h> /*错误号定义*/
#include <pthread.h> /*线程库定义*/

#define DEVICE_485 "/dev/ttyATH0"

	 int val = -1;
	 static int ret = 0;
	 int senfd;
	 int len,i;
	struct serial_rs485 rs485;	
	unsigned char dcmd[8]; 
	unsigned char sbuf[100];

/*
*串口初始化
*/
void serial_init(int fd)
{
	struct termios options;
	tcgetattr(fd, &options);
	options.c_cflag |= ( CLOCAL | CREAD );
	options.c_cflag &= ~CSIZE;
	options.c_cflag &= ~CRTSCTS;
	options.c_cflag |= CS8;
	options.c_cflag &= ~CSTOPB; 
	options.c_iflag |= IGNPAR;
	options.c_iflag &= ~(ICRNL | IXON);
	options.c_oflag = 0;
	options.c_lflag = 0;
	cfsetispeed(&options, B9600);//设置波特率
	cfsetospeed(&options, B9600);//设置输出波特率
	tcsetattr(fd,TCSANOW,&options);
}


/* 函数功能：传感器接口打开、关闭
 * 函数返回值：0 成功； -1 失败；
 */
int sen_open()
{
	 if ((senfd = open(DEVICE_485, O_RDWR | O_NOCTTY ))<0){      
		printf("err: can't open serial port!\n");  
        return -1;  
    } 
	serial_init(senfd);
	printf("sen_open ok \n");
	printf("serial init ok !\n");
	return 0;
}

int sen_close()
{
	int ret;
	ret = close(senfd);
	printf("sen_close ok \n");
	return ret;
}


/* 函数功能：执行传感器命令
 * 参数：
 * 		cmd/cmd_len：命令及长度；	输入
 * 		cmd_ret：命令执行结果及长度；输出； 
 * 		cmd_ret_len：当输入时，*cmd_ret_len 表示cmd_ret缓冲大小；输出时，表示cmd_ret命令执行结果长度；
 * 函数返回值：0 成功； -1 失败；暂时无错误码处理；
 */
int sen_exec( unsigned char *cmd, int cmd_len, unsigned char *cmd_ret, int *cmd_ret_len)
{
	memset(&rs485, 0, sizeof(rs485));
	for (i=0;i<cmd_len;i++)
	{	
		dcmd[i] = cmd[i];
		if (5 == i)
		{
			*cmd_ret_len=(int)(dcmd[5])*2+5;	
			printf("cmd_ret_len= %d\n",*cmd_ret_len);
		}
	}
	
	rs485.flags=SER_RS485_ENABLED;
	rs485.padding[0]=DB120_CH_485_DE2;
	ret =ioctl(senfd,TIOCSRS485,&rs485);//  command 1 ---  SEND
	printf("result: %d\n", ret);	
	if((ret=write (senfd, dcmd, cmd_len))>0)
	{
		printf("ret=%d\n",ret);
	
	}
	else
	{	
		printf("WRITE ERR !\n");
		return -1;
	}
		rs485.flags=SER_RS485_RTS_AFTER_SEND;
		rs485.padding[0]=DB120_CH_485_DE2;
		ret =ioctl(senfd,TIOCSRS485,&rs485);

		len=0;
		int nub = 0;
	do {
		if(ret = (read(senfd, sbuf, sizeof(sbuf))) <0)
		{	
			printf("READ ERR!\n");
			return -1;
		} 
		
		printf("%02x ",sbuf[0]);
		cmd_ret[nub]=sbuf[0];
		
		printf("cmd_ret=%02x ", cmd_ret[nub]);
		++nub;
		printf("%d\n", nub);
			
		printf("\n");
		
		if (nub==(*cmd_ret_len))break;
	} while(1);
			
	return 0;	
}

int main()
{	
	unsigned char cmd[8] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x01, 0x84,0x0A}; 
	sen_open();
	unsigned char *cmd_ret;
	int a = 7;
	sen_exec( cmd, 8, cmd_ret,&a);
	sen_close();
	return 0;
}





	
