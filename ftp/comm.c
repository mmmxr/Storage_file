#include "comm.h"
#define MAX_LEN 1024
int cmd_analyse(const char* cmd)
{
    if(cmd==NULL)
    {
        return ERROR;

    }
    else if(strncmp(cmd, "ls",2)==0)   //列出本地文件列表指令
    {
        return LS;
    }
    else if(strncmp(cmd, "server ls", 9)==0)   //列出服务器文件列表指令
    {
        return SERVER_LS;
    }
    else if(strncmp(cmd, "quit", 4)==0)   //断开链接指令
    {
        return QUIT;
    }
    else if(strncmp(cmd, "download", 8)==0)    //从服务器下载文件指令
    {
        return DOWNLOAD;
    }
    else if(strncmp(cmd, "upload", 6)==0)    //上传文件到服务器指令
    {
        return UPLOAD;
    }
    else
    {
        return ERROR;
    }
}

int putlist(int sock)
{
    char buf[MAX_LEN] = {0};
    struct dirent* pd=NULL;
    DIR* pdir=opendir (".");
    while((pd = readdir(pdir))!=NULL)
    {
        if(pd->d_name[0]=='.')
        {
            continue;
        }
        sprintf(buf,"%s\n",pd->d_name);
        write(sock,buf,strlen(buf));
    }
    return 1;
}
int getlist(int sock)
{
    char buf[MAX_LEN];
    int flag = 0;

    int old_flag = fcntl(sock, F_GETFL, 0);
    int new_flag = old_flag | O_NONBLOCK;
    fcntl(sock, F_SETFL, new_flag);

    while(1)   //循环读取
    {
        int s=read(sock, buf, MAX_LEN);
        if(s<0)   //读错误
        {
            if (flag == 1){
                break;
            }
        } else if(s == 0) {
            if(flag == 1){
                break;
            }
        } else {
            flag = 1;
            printf("%s", buf);   //服务器端发送完毕，显示文件
        }
    }
    fcntl(sock, F_SETFL, old_flag);
    return 1;
}

int putfile(int sock, const char* filename)
{
    int fd = open(filename, O_RDONLY);
    printf("filename:%s\n", filename);
    char buf[1024] = {0};

    while(1) {
        int s = read(fd, buf, sizeof(buf));
        printf("buf %s\n",buf);
        if (s <= 0) {
            break;
        }
        write(sock, buf, strlen(buf));
    }
    return 1;
}

//?
int getfile(int sock, const char* filename)
{
    printf("filename:%s\n",filename);
    char buf[MAX_LEN]= {0};
    int fd = open(filename, O_RDWR | O_CREAT, 0644);

    // int old_flag = fcntl(sock, F_GETFL, NULL);
    // int new_flag = old_flag | O_NONBLOCK;
    // fcntl(sock, F_SETFL, new_flag);

    // char c = '\0';
    // while (1){
        int s = recv(sock, buf, sizeof(buf), O_NONBLOCK);
        if (s < 0) {
            perror("recv");
    //        break;
        } else if(s == 0) {
   // fcntl(sock, F_SETFL, old_flag);
     //       break;
        }
        write(fd, buf, sizeof(buf));
    // }
    return 1;
}



