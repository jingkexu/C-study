

#include <stdio.h>
#include <stdlib.h>

#define OK 0
#define offsetof(TYPE, MEMBER) ((size_t) & ((TYPE *)0)->MEMBER)   //标准函数，结构体成员偏移地址
#define container_of(ptr, type, member) ({          \
    const typeof(((type *)0)->member)*__mptr = (ptr);    \
         (type *)((char *)__mptr - offsetof(type, member)); })
                                                                 //标准函数，根据结构体成员地址，计算结构体起始地

// container_of(ptr,type,member)    已知结构体type的成员member的地址ptr，求解结构体type的起始地址

typedef struct __interface interface;

struct __interface
{
    int (*parse_param)(interface * p, char *buffer, int len);   //过程处理
    int (*reply_ack)(interface * p);                            //回复ACK
    int (*process_cmd)(interface * p);                          //处理命令
};

typedef struct
{
    interface base;         // 基础结构体 基础函数
    void (*ext)(void);      // 拓展函数
} CMD1;

typedef struct
{
    interface base;          //基础函数
    void (*xujingke)(void);  //拓展函数1
    void (*jintian)(void);   //拓展函数2
} CMD2;

int cmd1_parse(interface *p, char *buffer, int len)
{
    CMD1 *p_cmd = container_of(p, CMD1, base);  // 已知结构体 CMD1 的成员 base 的地址 p ，求解结构体CMD1的起始地址
    p_cmd->ext();
    printf("parse\n");
    return OK;
}
int cmd1_ack(interface *p)
{
    CMD1 *p_cmd = container_of(p, CMD1, base);  // 已知结构体 CMD1 的成员 base 的地址 p ，求解结构体CMD1的起始地址
    p_cmd->ext();
    printf("ack\n");
    return OK;
}
int cmd1_process(interface *p)
{
    CMD1 *p_cmd = container_of(p, CMD1, base); // 已知结构体 CMD1 的成员 base 的地址 p ，求解结构体CMD1的起始地址
    p_cmd->ext();
    printf("process\n");
    return OK;
}
void cmd1_ext(void)
{
    printf("cmd1_");
}

int cmd2_parse(interface *p, char *buffer, int len)
{
    CMD2 *p_cmd = container_of(p, CMD2, base); // 已知结构体 CMD2 的成员 base 的地址 p ，求解结构体CMD2的起始地址
    p_cmd->xujingke();
    p_cmd->jintian();
    printf("parse\n");
    return OK;
}
int cmd2_ack(interface *p)
{
    CMD2 *p_cmd = container_of(p, CMD2, base); // 已知结构体 CMD2 的成员 base 的地址 p ，求解结构体CMD2的起始地址
    p_cmd->xujingke();
    p_cmd->jintian();
    printf("ack\n");
    return OK;
}
int cmd2_process(interface *p)
{
    CMD2 *p_cmd = container_of(p, CMD2, base); // 已知结构体 CMD2 的成员 base 的地址 p ，求解结构体CMD2的起始地址
    p_cmd->xujingke();
    p_cmd->jintian();
    printf("process\n");
    return OK;
}
void xujingke(void)
{
    printf("xujingke shuo:");
}

void jintian(void)
{
    printf("jintian");
}

interface *CreateCMD1()         // 填充CMD1 结构体
{
    CMD1 *p = (CMD1 *)malloc(sizeof(CMD1));
    p->base.parse_param = cmd1_parse;
    p->base.reply_ack = cmd1_ack;
    p->base.process_cmd = cmd1_process;
    p->ext = cmd1_ext;
    return &p->base;
}

interface *CreateCMD2()         // 填充CMD2 结构体
{
    CMD2 *p = (CMD2 *)malloc(sizeof(CMD2));
    p->base.parse_param = cmd2_parse;
    p->base.reply_ack = cmd2_ack;
    p->base.process_cmd = cmd2_process;
    p->xujingke = xujingke;
    p->jintian = jintian;
    return &p->base;
}

//根据形参（命令）填充不同结构体
interface *get_processer(char cmd_type)
{
    interface *p = NULL;  
    switch (cmd_type)
    {
    case 1:
        p = CreateCMD1();     // 装载命令1  填充CMD1 结构体
        break;
    case 2:
        p = CreateCMD2();     // 装载命令2  填充CMD2 结构体
        break;
    }
    return p;
}

void func(char *buffer, int len)
{
    interface *p = get_processer(buffer[0]); // 装载结构体指针
    if (p == NULL)                           
        return;
    int ret = p->parse_param(p, buffer, len); // 执行 对应 parse_param 解析函数
    if (ret == OK)
    {
        p->reply_ack(p);                      // 回复 对应 ACK
        p->process_cmd(p);                    // 执行 对应 命令
    }
    free(p);
}
// 主函数
int main(int argc, char const *argv[])
{
    char buffer[] = {1, 2, 3, 4, 5, 6};
    func(buffer, sizeof(buffer));
    return 0;
}
