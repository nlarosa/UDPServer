
/*

Nicholas LaRosa
CSE 30264, Program 2

usage: udpserver 

*/

#include <sys/socket.h>
#include <sys/time.h>
#include <time.h>

#include <netdb.h>
#include <netinet/in.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BUFFER 1400
#define PORT 9979

int main( int argc, char** argv )
{
	int sockfd, inputBytes, outputBytes, isHost, err, i, j;
	
	struct sockaddr_in serverAddress, clientAddress;
	struct addrinfo *hostInfo, *p;
	struct timeval timer;
	struct tm * timeFormat;

	socklen_t socketSize;
	time_t currentTime;
	
	char sendLine[ BUFFER + 1 ];
	char recvLine[ BUFFER + 1 ];
	char timeStamp[ 16 ];
	char ipstr[ INET6_ADDRSTRLEN ];

	if( argc != 1 )
	{
		printf( "\nusage: udpserver\n\n" );
		exit( 1 );
	}
	
	if( ( sockfd = socket( AF_INET, SOCK_DGRAM, 0 ) ) < 0 )		// open datagram socket, address family internet
	{
		perror( "Server - socket() error" );
		exit( 1 );
	}
	
	memset( ( char * )&serverAddress, 0, sizeof( struct sockaddr_in ) );	// secure enough memory for the server socket
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons( PORT );					// set server port
	serverAddress.sin_addr.s_addr = htonl( INADDR_ANY );			// set server address
	
	if( bind( sockfd, ( struct sockaddr * )&serverAddress, sizeof( struct sockaddr_in) ) < 0 )
	{
		perror( "Server - bind() error" );
		exit( 1 );
	}
	
	while( 1 )
	{
		socketSize = sizeof( clientAddress );
		
		if( ( inputBytes = recvfrom( sockfd, recvLine, BUFFER, 0, ( struct sockaddr * )&clientAddress, &socketSize ) ) < 0 )
		{
			perror( "Server - recvfrom() error" );
			exit( 1 );
		}	
		
		recvLine[ inputBytes ] = '\0';	

		for( i = 0; i < strlen( recvLine ); i++ )
		{
			j = strlen( recvLine ) - i - 1;
			sendLine[ i ] = recvLine[ j ];				// invert the input buffer
		}
	
		sendLine[ inputBytes ] = '\0';					// end the string just in case

		gettimeofday( &timer, NULL );
		timeFormat = localtime( &timer.tv_sec );
		sprintf( timeStamp, " Timestamp: %02d:%02d:%02d.%06d\n\0", timeFormat->tm_hour, timeFormat->tm_min, timeFormat->tm_sec, timer.tv_usec );

		strcat( sendLine, timeStamp );

		if( ( outputBytes = sendto( sockfd, sendLine, strlen( sendLine ), 0, ( struct sockaddr * )&clientAddress, sizeof( struct sockaddr_in ) ) ) < 0 )
		{
			perror( "Server - sendto() error" );
			exit( 1 );
		}
	}
}

