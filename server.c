#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080  // 服务器监听的端口

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len;
    char buffer[1024] = {0};

    // 创建服务器端 socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // 配置服务器端地址信息
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;  // 允许接受任何 IP 地址的连接
    server_addr.sin_port = htons(PORT);  // 设置监听端口

    // 绑定 socket 到指定的端口
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // 监听连接请求
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d...\n", PORT);

    addr_len = sizeof(client_addr);

    // 等待客户端连接
    if ((client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len)) < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    // 打印客户端 IP 地址
    char *client_ip = inet_ntoa(client_addr.sin_addr);
    printf("Connection established with client: %s\n", client_ip);

    // 接收客户端发送的数据
    read(client_fd, buffer, sizeof(buffer));
    printf("Received from client: %s\n", buffer);

    // 向客户端发送响应
    char *message = "Hello from server!";
    send(client_fd, message, strlen(message), 0);

    // 关闭连接
    close(client_fd);
    close(server_fd);
    return 0;
}
