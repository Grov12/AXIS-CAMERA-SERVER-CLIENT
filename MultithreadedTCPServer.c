#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <capture.h>


#define PORT 7575

struct Request {
	int sock;

};

void *handle_send(void *vargs) {
	media_stream *     stream;
  	media_frame *      frame;
	struct Request *req = (struct Request *)vargs;
	char rcvBuffer[1024];
	char buff[1024];
	unsigned long long totalbytes = 0;
	char fromClient [28];

	
	read(req->sock,rcvBuffer,sizeof(rcvBuffer));
	printf("Data received: %s",rcvBuffer);

	stream = capture_open_stream(IMAGE_JPEG, "resolution=352x288&fps=15");

	frame = capture_get_frame(stream);
	capture_frame_free(frame);

	while(1) {
	frame = capture_get_frame(stream);
	totalbytes = capture_frame_size(frame);

	char sendBuffer[totalbytes];
        memcpy(sendBuffer,capture_frame_data(frame),sizeof(sendBuffer));
	send(req->sock,sendBuffer,totalbytes,0);
        capture_frame_free(frame);
	}
    
	
	close(req->sock);
	free(req);
	pthread_exit(0);


return 0;
	
}


void *server(void *vargp) {
	int sockfd;
	struct sockaddr_in serverAddr;
	int clientSocket;
	int newSocket;
	struct sockaddr_in newAddr;
	int socket_fd;

	socklen_t addr_size;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	memset(&serverAddr, '\0',sizeof(serverAddr));

	serverAddr.sin_family =AF_INET;
	serverAddr.sin_port =htons(7985);
	serverAddr.sin_addr.s_addr = inet_addr("192.168.20.252");
	
    	bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

	listen(sockfd, 5); 

	int n,rc;
	pthread_t tid;
	
        while(1) {
		
		struct Request *req = malloc(sizeof(struct Request));
		req->sock = accept(sockfd, (struct sockaddr*)&newAddr,&addr_size);
		rc = pthread_create(&tid,NULL,handle_send,(void*)req);
		pthread_detach(tid);
		
		
}
return 0;

	
}

int main() {
        
	int pserver;
	pthread_t p;
	pserver = pthread_create(&p,NULL,server,NULL);
	pthread_join(p,NULL);


	return 0;
		

}



