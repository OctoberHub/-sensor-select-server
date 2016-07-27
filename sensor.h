#ifndef _SENSOR_H_
#define _SENSOR_H_

/* 函数功能：传感器接口打开、关闭
 * 函数返回值：0 成功； -1 失败；
 */
int sen_open();
int sen_close();

/* 函数功能：执行传感器命令
 * 参数：
 * 		cmd/cmd_len：命令及长度；	输入
 * 		cmd_ret：命令执行结果及长度；输出； 
 * 		cmd_ret_len：当输入时，*cmd_ret_len 表示cmd_ret缓冲大小；输出时，表示cmd_ret命令执行结果长度；
 * 函数返回值：0 成功； -1 失败；暂时无错误码处理；
 */
int sen_exec(unsigned char *cmd, int cmd_len, unsigned char *cmd_ret, int *cmd_ret_len);


#endif

/*
 * 485传感器接口
 * v1.0 	2016/7/25		温湿度状态读取
 */
