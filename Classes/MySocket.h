#pragma once

#ifdef _WIN32
#include <winsock.h>
typedef int	socklen_t;
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <arpa/inet.h>
typedef int SOCKET;
#define INVALID_SOCKET	-1
#define SOCKET_ERROR	-1

#endif


class ODSocket {

public:
	ODSocket(SOCKET sock = INVALID_SOCKET);
	~ODSocket();

	// Create socket object for snd/recv data
	bool Create(int af, int type, int protocol = 0);

	// Connect socket
	bool Connect(const char* ip, unsigned short port);

	// Bind socket
	bool Bind(unsigned short port);

	// Listen socket
	bool Listen(int backlog = 500); 

	// Accept socket
	bool Accept(ODSocket& s, char* fromip = NULL);

	// Send socket
	int Send(const char* body, int body_len, int flags =0);

	// Recv socket
	int Recv(char* buf, int len, int flags = 0);

	// Close socket
	int Close();

	// Get errno
	int GetError();

	//#pragma region just for win32
	static int Init();	
	static int Clean();

	// Domain parse
	static bool DnsParse(const char* domain, char* ip);

	ODSocket& operator = (SOCKET s);

	operator SOCKET();

protected:
	SOCKET m_sock;
};