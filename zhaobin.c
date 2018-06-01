

#include <stdio.h>
#include <stdlib.h>

#define OK 0
#define offsetof(TYPE, MEMBER) ((size_t) & ((TYPE *)0)->MEMBER)
#define container_of(ptr, type, member) ({          \
    const typeof(((type *)0)->member)*__mptr = (ptr);    \
         (type *)((char *)__mptr - offsetof(type, member)); })

typedef struct __interface interface;

struct __interface
{
    int (*parse_param)(interface * p, char *buffer, int len);
    int (*reply_ack)(interface * p);
    int (*process_cmd)(interface * p);
};

typedef struct
{
    interface base;
    void (*ext)(void);
} CMD1;

typedef struct
{
    interface base;
    void (*xujingke)(void);
    void (*jintian)(void);
} CMD2;

int cmd1_parse(interface *p, char *buffer, int len)
{
    CMD1 *p_cmd = container_of(p, CMD1, base);
    p_cmd->ext();
    printf("parse\n");
    return OK;
}
int cmd1_ack(interface *p)
{
    CMD1 *p_cmd = container_of(p, CMD1, base);
    p_cmd->ext();
    printf("ack\n");
    return OK;
}
int cmd1_process(interface *p)
{
    CMD1 *p_cmd = container_of(p, CMD1, base);
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
    CMD2 *p_cmd = container_of(p, CMD2, base);
    p_cmd->xujingke();
    p_cmd->jintian();
    printf("parse\n");
    return OK;
}
int cmd2_ack(interface *p)
{
    CMD2 *p_cmd = container_of(p, CMD2, base);
    p_cmd->xujingke();
    p_cmd->jintian();
    printf("ack\n");
    return OK;
}
int cmd2_process(interface *p)
{
    CMD2 *p_cmd = container_of(p, CMD2, base);
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

interface *CreateCMD1()
{
    CMD1 *p = (CMD1 *)malloc(sizeof(CMD1));
    p->base.parse_param = cmd1_parse;
    p->base.reply_ack = cmd1_ack;
    p->base.process_cmd = cmd1_process;
    p->ext = cmd1_ext;
    return &p->base;
}

interface *CreateCMD2()
{
    CMD2 *p = (CMD2 *)malloc(sizeof(CMD2));
    p->base.parse_param = cmd2_parse;
    p->base.reply_ack = cmd2_ack;
    p->base.process_cmd = cmd2_process;
    p->xujingke = xujingke;
    p->jintian = jintian;
    return &p->base;
}

interface *get_processer(char cmd_type)
{
    interface *p = NULL;
    switch (cmd_type)
    {
    case 1:
        p = CreateCMD1();
        break;
    case 2:
        p = CreateCMD2();
        break;
    }
    return p;
}

void func(char *buffer, int len)
{
    interface *p = get_processer(buffer[0]);
    if (p == NULL)
        return;
    int ret = p->parse_param(p, buffer, len);
    if (ret == OK)
    {
        p->reply_ack(p);
        p->process_cmd(p);
    }
    free(p);
}

int main(int argc, char const *argv[])
{
    char buffer[] = {1, 2, 3, 4, 5, 6};
    func(buffer, sizeof(buffer));
    return 0;
}
