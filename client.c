#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "192.168.18.134"  // 替换为服务器的 IP 地址
#define PORT 8080  // 服务器监听端口

int main() {
    int sock = 0;
    struct sockaddr_in server_addr;
    char *message = "Hello from Kali client!";
    char buffer[1024] = {0};

    // 创建客户端 socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    // 转换 IP 地址格式
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("Invalid address");
        return -1;
    }

    // 连接到服务器
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        return -1;
    }

    // 发送消息到服务器
    send(sock, message, strlen(message), 0);

    // 接收服务器的响应
    int recv_size = recv(sock, buffer, sizeof(buffer) - 1, 0);
    if (recv_size > 0) {
        buffer[recv_size] = '\0';  // 确保字符串以 null 结尾
        printf("Received from server: %s\n", buffer);
    } else {
        printf("Failed to receive data from server\n");
    }

    // 关闭 socket 连接
    close(sock);
    return 0;
}
