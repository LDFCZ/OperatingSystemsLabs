#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <termios.h>
#include <ctype.h>

#define MAX_CLIENTS_AMOUNT 10

#define ADDRESS_BUF_SIZE 512
#define CACHE_SIZE 10
#define BUFFER_SIZE 1024
#define EMPTY -1

#define SERVER_PORT 2050

#define PORT_LEN 5



int socket_connect(char *host, in_port_t port) {
    printf("getting host... host = %s\n", host);
    struct hostent *hp = gethostbyname(host);
    if (hp == NULL) {
        perror("gethostbyname error");
        return -1;
    }
    printf("host recived!\n");

    struct sockaddr_in addr;
    memcpy(&addr.sin_addr, hp->h_addr, hp->h_length);
    addr.sin_port = htons(port);
    addr.sin_family = AF_INET;

    printf("opening host...\n");

    int sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == -1) {
        perror("socket error");
        return -1;
    }

    const int enable = 1;
    if (setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, &enable, sizeof(int)) == -1) {
        perror("setsockopt error");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == -1) {
        perror("connect error");
        return -1;
    }
    printf("host openned!\n");
    return sock;
}

typedef struct cache {
    int page_size;
    char *title;
    char *page;
} cache_t;

typedef struct url {
    char *host;
    char *path;
    int port;
} url_t;

void addr_init(struct sockaddr_in *addr, int port) {
    addr->sin_family = AF_INET;
    addr->sin_addr.s_addr = htonl(INADDR_ANY);
    addr->sin_port = htons(port);
}

int find_free_client_index(const int *clients) {
    int free_client_i = 0;
    while (free_client_i < MAX_CLIENTS_AMOUNT && clients[free_client_i] >= 0) {
        free_client_i++;
    }
    return free_client_i;
}

void accept_new_client(int listenfd, int *clients) {
    int free_client_i = find_free_client_index(clients);
    clients[free_client_i] = accept(listenfd, (struct sockaddr *)NULL, NULL);
    time_t ticks = time(NULL);
    char send_buff[BUFFER_SIZE + 1];
    snprintf(send_buff, sizeof(send_buff), "%.24s\r\n", ctime(&ticks));
    write(clients[free_client_i], send_buff, strlen(send_buff));
}

int try_accept_new_client(int listenfd, int *clients, int alreadyconnected_clients, struct timeval *timeout, fd_set *lfds) {
    if (alreadyconnected_clients < MAX_CLIENTS_AMOUNT) {
        if (select(listenfd + 1, lfds, NULL, NULL, timeout)) {
            printf("Can read from listen\n");
            accept_new_client(listenfd, clients);
            alreadyconnected_clients++;
        }
    }
    return alreadyconnected_clients;
}

void handle_client_disconnecting(int *clients, int *clients_http_sockets, int *cache_to_client, int *sent_bytes, int client_i) {
    printf("client %d disconnecting...\n", client_i);
    close(clients[client_i]);
    close(clients_http_sockets[client_i]);
    clients[client_i] = EMPTY;
    clients_http_sockets[client_i] = EMPTY;
    cache_to_client[client_i] = EMPTY;
    sent_bytes[client_i] = 0;
}

void free_URL(url_t *pUrl) {
    free(pUrl->path);
    free(pUrl->host);
    free(pUrl);
}

url_t *parse_URL(char *url_buffer) {
    url_t *url = (url_t *)malloc(sizeof(url_t));
    url->path = NULL;
    url->host = NULL;
    url->port = 80;

    size_t url_buffer_size = strlen(url_buffer);

    int start_port_i = 0;
    for (size_t str_i = 0; str_i < url_buffer_size; str_i++) {
        if (url_buffer[str_i] == ':') {
            if (start_port_i != 0) {
                free_URL(url);
                return NULL;
            }
            start_port_i = str_i;
            char port[PORT_LEN + 1] = {0};
            size_t portstr_i = str_i + 1;
            int port_i = 0;
            while (port_i < PORT_LEN && portstr_i < url_buffer_size && isdigit(url_buffer[portstr_i])) {
                port[port_i++] = url_buffer[portstr_i];
                portstr_i++;
                str_i++;
            }
            url->port = atoi(port);
        }
        if (url_buffer[str_i] == '/') {
            if (str_i + 1 == url_buffer_size) {
                url_buffer[str_i] = '\0';
            }
            url->host = (char *)malloc(sizeof(char) * (start_port_i == 0 ? str_i + 1 : start_port_i));
            url->path = (char *)malloc(sizeof(char) * (url_buffer_size - str_i));
            strncpy(url->host, url_buffer, start_port_i == 0 ? str_i + 1: start_port_i);
            strncpy(url->path, &(url_buffer[str_i + 1]), url_buffer_size - str_i);
            url->host[start_port_i == 0 ? str_i: start_port_i - 1] = '\0';
            url->path[url_buffer_size - str_i - 1] = '\0';
            printf("host = %s\n", url->host);
            printf("path = %s\n", url->path);
            break;
        }
    }
    return url;
}

void read_to_cache(cache_t *cache, int current_cache_size, const int *clients_http_sockets, int client_i) {
    int offset = 0;
    int read_bytes = 0;
    while ((read_bytes = read(clients_http_sockets[client_i], &cache[current_cache_size].page[offset], BUFFER_SIZE)) != 0) {
        offset += read_bytes;
        printf("cache[%d].page size = %d\n", current_cache_size, cache[current_cache_size].page_size);
        printf("read_bytes = %d. offset = %d\n", read_bytes, offset);
        cache[current_cache_size].page_size = offset;
        cache[current_cache_size].page = (char *)realloc(cache[current_cache_size].page, offset + BUFFER_SIZE + 1);
    }
}

int try_find_at_cache(const cache_t *cache, int cacheEntitiesAmount, int *cache_to_client, int *sent_bytes, int client_i, const char *url_buffer) {

    for (int i = 0; i < cacheEntitiesAmount; i++) {
        if (strcmp(cache[i].title, url_buffer) == 0) {
            cache_to_client[client_i] = i;
            sent_bytes[client_i] = 0;
            printf("Page found in cache!\n");
            return 1;
        }
    }
    return 0;
}

char *create_request(const url_t *url) {
    char *request = (char *)malloc(sizeof(char) * (ADDRESS_BUF_SIZE + 16));
    strcpy(request, "GET /");
    strcat(request, url->path);
    strcat(request, "\r\n"); // \0 
    return request;
}

void send_request(const int *clients_http_sockets, int client_i, const url_t *url) {
    if (url->path == NULL) {
        write(clients_http_sockets[client_i], "GET /\r\n", strlen("GET /\r\n"));
    }
    else {
        char *request = create_request(url);
        printf("REQUEST: %s", request);
        write(clients_http_sockets[client_i], request, strlen(request));
        free(request);
    }
}

int resend_from_cache(const cache_t *cache, int connectedClientsAmount, int *clients, int *cache_to_client, int *sent_bytes, int client_i) {
    int written_bytes = 0;
    if (cache_to_client[client_i] != EMPTY) {
        if (sent_bytes[client_i] < cache[cache_to_client[client_i]].page_size) {
            int bytes_left = cache[cache_to_client[client_i]].page_size - sent_bytes[client_i];
            if ((written_bytes = write(clients[client_i], &(cache[cache_to_client[client_i]].page[sent_bytes[client_i]]), BUFFER_SIZE < bytes_left ? BUFFER_SIZE : bytes_left)) != 0) {
                sent_bytes[client_i] += written_bytes;
            }
            else if (written_bytes == EMPTY) {
                printf("Client disconnected!\n");
                cache_to_client[client_i] = EMPTY;
                clients[client_i] = EMPTY;
                connectedClientsAmount--;
            }
        }
    }
    return connectedClientsAmount;
}

int main(int argc, char *argv[]) {

    int listenfd = 0;
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    cache_t *cache = (cache_t *)malloc(sizeof(cache_t) * CACHE_SIZE);

    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    int clients[MAX_CLIENTS_AMOUNT];
    int clients_http_sockets[MAX_CLIENTS_AMOUNT];
    int cache_to_client[MAX_CLIENTS_AMOUNT];
    int sent_bytes[MAX_CLIENTS_AMOUNT];

    for (int k = 0; k < MAX_CLIENTS_AMOUNT; k++) {
        clients[k] = EMPTY;
        clients_http_sockets[k] = EMPTY;
        cache_to_client[k] = EMPTY;
        sent_bytes[k] = EMPTY;
    }

    // error read
    addr_init(&serv_addr, SERVER_PORT);

    bind(listenfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    listen(listenfd, MAX_CLIENTS_AMOUNT);

    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;

    fd_set lfds;
    fd_set cfds;
    fd_set sfds;

    printf("listenfd = %d\n", listenfd);

    int connected_clients = 0;
    int cache_size = 0;
    while (1) {
        FD_ZERO(&lfds);
        FD_SET(listenfd, &lfds);

        FD_ZERO(&sfds);
        FD_SET(0, &sfds);
        if (select(1, &sfds, NULL, NULL, &timeout)) {
            break;
        }

        connected_clients = try_accept_new_client(listenfd, clients, connected_clients, &timeout, &lfds);

        for (int client_i = 0; client_i < MAX_CLIENTS_AMOUNT; client_i++) {

            if (clients[client_i] < 2)
                continue;

            FD_ZERO(&cfds);
            FD_SET(clients[client_i], &cfds);

            if (select(clients[client_i] + 1, &cfds, NULL, NULL, &timeout)) {
                char url_buffer[ADDRESS_BUF_SIZE];
                //int read_bytes = read(clients[client_i], &url_buffer, ADDRESS_BUF_SIZE);
                int read_bytes = fgets(&url_buffer, ADDRESS_BUF_SIZE, clients[client_i]);
                url_buffer[strlen(url_buffer) - 1] = '\0';
                if (read_bytes) {
                    url_buffer[read_bytes] = '\0';
                    printf("\"%s\"\n", url_buffer);
                    if (strcmp(url_buffer, "/exit") == 0) {
                        connected_clients--;
                        handle_client_disconnecting(clients, clients_http_sockets, cache_to_client, sent_bytes, client_i);
                        continue;
                    }
                    url_t *url = parse_URL(url_buffer);
                    if (url == NULL) {
                        printf("URL parsing fail\n");
                        continue;
                    }
                    printf("Connecting socket...\n");
                    printf("PORT=%d\n", url->port);
                    printf("client_i = %d, MAX_CLIENTS_AMOUNT = %d\n", client_i, MAX_CLIENTS_AMOUNT);

                    int find_at_cache = try_find_at_cache(cache, cache_size, cache_to_client, sent_bytes, client_i, url_buffer);

                    if (!find_at_cache) {
                        clients_http_sockets[client_i] = socket_connect(url->host, url->port);

                        if (clients_http_sockets[client_i] == EMPTY) {
                            write(clients[client_i], "Failed connect!\n", strlen("Failed connect!\n"));
                            continue;
                        }
                        printf("Socket connected.\n");

                        send_request(clients_http_sockets, client_i, url);

                        cache[cache_size].title = (char *)malloc(sizeof(char) * strlen(url_buffer));
                        strcpy(cache[cache_size].title, url_buffer);

                        cache[cache_size].page_size = BUFFER_SIZE;
                        cache[cache_size].page = (char *)malloc(BUFFER_SIZE + 1);

                        cache_to_client[client_i] = cache_size;
                        sent_bytes[client_i] = 0;

                        printf("reading...\n");

                        read_to_cache(cache, cache_size, clients_http_sockets, client_i);

                        close(clients_http_sockets[client_i]);

                        cache[cache_size].page[cache[cache_size].page_size + 1] = '\0';
                        cache_size++;
                        printf("cache read!\n");
                    }

                    free_URL(url);
                }
            }
            connected_clients = resend_from_cache(cache, connected_clients, clients, cache_to_client, sent_bytes, client_i);
        }
    }
}
