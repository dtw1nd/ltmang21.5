#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"  // Địa chỉ IP của telnet_server
#define SERVER_PORT 8080       // Cổng của telnet_server
#define BUFFER_SIZE 1024

int main() {
    int client_socket;
    struct sockaddr_in server_address;
    char buffer[BUFFER_SIZE];

    // Tạo socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Lỗi khi tạo socket");
        exit(EXIT_FAILURE);
    }

    // Thiết lập thông tin địa chỉ server
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SERVER_PORT);
    server_address.sin_addr.s_addr = inet_addr(SERVER_IP);

    // Kết nối tới server
    if (connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        perror("Lỗi khi kết nối tới server");
        exit(EXIT_FAILURE);
    }

    printf("Đã kết nối tới server. Nhập tên người dùng và mật khẩu.\n");

    // Nhập tên người dùng và mật khẩu từ bàn phím
    char username[20];
    char password[20];
    printf("Tên người dùng: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = 0;  // Xóa ký tự newline từ chuỗi
    printf("Mật khẩu: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = 0;  // Xóa ký tự newline từ chuỗi

    // Gửi tên người dùng và mật khẩu tới server
    snprintf(buffer, sizeof(buffer), "user %s pass %s", username, password);
    send(client_socket, buffer, strlen(buffer), 0);

    // Nhận phản hồi từ server
    memset(buffer, 0, sizeof(buffer));
    recv(client_socket, buffer, sizeof(buffer), 0);
    printf("Phản hồi từ server: %s\n", buffer);

    // Kiểm tra phản hồi từ server
    if (strcmp(buffer, "Đăng nhập thành công\n") == 0) {
        // Yêu cầu nhập lệnh và gửi cho server
        printf("Nhập lệnh: ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = 0;  // Xóa ký tự newline từ chuỗi
        send(client_socket, buffer, strlen(buffer), 0);

        // Nhận kết quả từ server và hiển thị
        memset(buffer, 0, sizeof(buffer));
        recv(client_socket, buffer, sizeof(buffer), 0);
        printf("Kết quả: %s\n", buffer);
    }

    // Đóng kết nối
    close(client_socket);

    return 0;
}
