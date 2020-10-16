#pragma once

/*
定义一些公共结构体，用于处理粘包问题

粘包问题：
客户端有远程cmd和键盘钩子的功能
假定两个功能都是通过一个套接字传递信息
那么我们就需要在包中做一些操作，用于区分不同功能传递的数据

1.区分类型：定义一个标识符，区分不同类型的数据
2.区分类型数据的边界：定义一个长度，表示数据有多大

简单表示数据包封装后的样子
type---length----data
*/


//定义一些宏，来表示包的具体处理类型
#define PACKET_REQ_CMD 10001	//REQ-request	CMD-表示cmd类型数据	客户端-->服务端
#define PACKET_RLY_CMD 10002	//RLY-reply		CMD-表示cmd类型数据	服务端-->客户端

#define PACKET_REQ_KEYBOARD 20001	//键盘钩子请求	客户端-->服务端
#define PACKET_RLY_KEYBOARD 20002	//键盘钩子回复	服务端-->客户端


//结构体有对齐功能	按1字节对齐，会自动填充空字节
#pragma pack(push)		//表示原有的结构体对齐值大小压栈
#pragma pack(1)
struct MyPacket {
	/*
	unsigned表示无符号，无符号整型与有符号整型区别举例：
	一字节无符号整型表示范围：	0	~	255
	一字节有符号整型表示范围：-128	~	127
	*/
	unsigned int type;	//表示包类型
	unsigned int length;	//表示长度
	char data[1];	//定义一个1字节的数组
	/*
	最后的数组被称为动态数组/柔性数组，即数组大小待定的数组
	C语言中结构体的最后一个参数可以是大小未知的数组，可以通过结构体创建柔性数组
	
	用途：
	满足需要变长度的结构体，解决使用数组时内存的冗余和数组的越界问题
	对于柔性数组的这个特点，很容易构造出变成结构体，如缓冲区，数据包等等
	这样的变长数组常用于网络通信中构造不定长数据包，不会浪费空间浪费网络流量
	*/
};
#pragma pack(pop)		//表示原有的结构体对齐值大小出栈