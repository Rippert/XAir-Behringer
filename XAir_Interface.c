/*
  XAir_Interface.c
  
   This file is part of the XAir-Behringer utilities.

    XAir-Behringer utilities is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    XAir-Behringer utilities is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with XAir-Behringer utilities.  If not, see <http://www.gnu.org/licenses/>.
    
    Copyright 2014-2018 Patrick-Gilles Maillot
    Copyright 2018 Ted Rippert
 
   Created on: Jan 26, 2018
 
   Interface programs and scripts with an XAir mixer via OSC over network
   Includes code modified from X32_Behringer project written by Patrick-Gilles Maillot
   See: https://github.com/pmaillot/X32-Behringer.git
 
       Authors: Ted Rippert, Patrick-Gilles Maillot
 
 */
//
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define QUOTE  '"'
#define SQUOTE '\''

#ifdef __WIN32__
#include <winsock2.h>
#include <conio.h>
#define millisleep(a)	Sleep(a)
#else
#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <sys/fcntl.h>
#define closesocket(s) 	close(s)
#define millisleep(a)	usleep(a*1000)
#define WSACleanup()
#define SOCKET_ERROR -1
typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;
#endif


// External calls used
extern int Xsprint(char *bd, int index, char format, void *bs);
extern int Xcparse(char *buf, char *line);
extern void Xfdump(char *header, char *buf, int len, int debug);
extern void Xdump(char *buf, int len, int debug);

// External Global Variable


#ifdef __WIN32__
	#define BACKSPACE	8		// need to use <ctl>h
	#define EOL			13		// enter key
	#define BACKCHARS	2		// 2 chars to remove from line
	#define NO_CHAR		-1		// not a real/printable char
#else
	#define BACKSPACE	8		// backspace key OK
	#define EOL			'\n'	// enter key
	#define BACKCHARS	1		// 1 char to remove from line
	#define NO_CHAR		-1		// not a real/printable char
#endif

//
#define LINEMAX				512		// input line buffer size
#define BSIZE				512		// send/receive buffer sizes
#define XREMOTE_TIMEOUT		9		// timeout set to 9 seconds
//
int X32debug = 0;
int X32verbose = 1;
//
// Macros:
//
#define RPOLL													\
	do {														\
		FD_ZERO (&ufds);										\
		FD_SET (Xfd, &ufds);									\
		p_status = select(Xfd+1, &ufds, NULL, NULL, &timeout);	\
	} while (0);
//
#define RECV																\
do {																		\
	r_len = recvfrom(Xfd, r_buf, BSIZE, 0, 0, 0);							\
	if (X32verbose) {Xdump(r_buf, r_len, X32debug); fflush(stdout);}\
} while (0);
//
#define SEND																\
do {																		\
	if (s_delay) millisleep (s_delay);										\
	if (sendto (Xfd, s_buf, s_len, 0, Xip_addr, Xip_len) < 0) {				\
		perror ("Error while sending data");								\
		exit (EXIT_FAILURE);												\
	}																		\
} while(0);
//
#define XREMOTE()															\
do {																		\
	if (xremote_on) {														\
		now = time(NULL); 													\
		if (now > before + XREMOTE_TIMEOUT) { 								\
			if (X32debug) {Xfdump("->X", xremote, 12, 0); fflush(stdout);}	\
			if (sendto (Xfd, xremote, 12, 0, Xip_addr, Xip_len) < 0) {		\
				perror ("coundn't send data to XR18");						\
				exit (EXIT_FAILURE);										\
			}																\
			before = now;													\
		}																	\
	}																		\
} while (0);
//
#define CHECKXR()			\
	do {					\
		RPOLL 				\
		if (p_status > 0) {	\
			RECV			\
		}					\
	} while (p_status > 0);
//
//
#define TESTINPUT()																						\
	else if (strcmp(input_line, "exit") == 0) keep_on = 0;												\
	else if (strcmp(input_line, "quit") == 0) keep_on = 0;												\
	else if (strcmp(input_line, "xremote") == 0) printf(":: xremote is %s\n",((xremote_on)?"on":"off"));\
	else if (strcmp(input_line, "xremote off") == 0) 	xremote_on = 0;									\
	else if (strcmp(input_line, "xremote on") == 0) 	xremote_on = 1;									\
	else if (strcmp(input_line, "verbose") == 0) printf(":: verbose is %s\n",((X32verbose)?"on":"off"));\
	else if (strcmp(input_line, "verbose off") == 0) 	X32verbose = 0;									\
	else if (strcmp(input_line, "verbose on") == 0) 	X32verbose = 1;									\



//
//
//
//
int
main(int argc, char **argv)
{
struct sockaddr_in	Xip;
struct sockaddr*	Xip_addr = (struct sockaddr *)&Xip;
int 				Xfd;		// our socket
char				Xip_str[20], Xport_str[8];
int				r_len, s_len, p_status;
char				r_buf[BSIZE];
char				s_buf[BSIZE];
//
int				xremote_on;
char				xremote[12] = "/xremote";			// automatic trailing zeroes
int				l_index;
char				input_line[LINEMAX + 4];
int				input_intch;						// addresses limitations in certain C compilers wit getopt()
int				keep_on, do_keyboard, s_delay, filein;
FILE*			fdk = NULL;
time_t			before, now;
//
fd_set 			ufds;
struct timeval	timeout;
//
#ifdef __WIN32__
WSADATA 			wsa;
int					Xip_len = sizeof(Xip);	// length of addresses
unsigned long 		mode;
#else
socklen_t			Xip_len = sizeof(Xip);	// length of addresses
#endif
//
// Initialize communication with XR server at IP ip and PORT port
// Set default values to match your XR desk
	strcpy (Xip_str, "192.168.0.64");
	strcpy (Xport_str, "10024");
//
// Manage arguments
	filein = 0;
	do_keyboard = 1;
	s_delay = 1;

	while ((input_intch = getopt(argc, argv, "i:p:d:k:f:s:t:v:h")) != -1) {
		switch (input_intch) {
		case 'i':
			strcpy(Xip_str, optarg );
			break;
		case 'p':
			strcpy(Xport_str, optarg );
			break;
		case 'd':
			sscanf(optarg, "%d", &X32debug);
			break;
		case 'f':
			filein = 1;
			sscanf(optarg, "%s", input_line);
			break;
		case 'k':
			sscanf(optarg, "%d", &do_keyboard);
			break;
		case 's':
			filein = 2;
			sscanf(optarg, "%s", input_line);
			break;
		case 't':
			sscanf(optarg, "%d", &s_delay);
			break;
		case 'v':
			sscanf(optarg, "%d", &X32verbose);
			break;
		default:
		case 'h':
			printf("usage: XAir_Interface [-i OSC Server ipv4 address]\n");
			printf("                      [-p OSC Server port]\n");
			printf("                      [-d 0/1, [0], debug option]\n");
			printf("                      [-v 0/1  [1], verbose option]\n");
			printf("                      [-k 0/1  [1], keyboard mode on]\n");
			printf("                      [-t int  [1], delay between batch commands in ms]\n");
			printf("                      [-s file, reads X32node formatted data lines from 'file']\n");
			printf("                      [-f file, sets batch mode on, getting input data from 'file']\n");
			printf("                      default IP is 192.168.0.64:10024\n\n");
			printf(" If option -s file is used, the program reads data from the provided file \n");
			printf(" until EOF has been reached, and exits after that.\n\n");
			printf(" If option -f file is used, the program runs in batch mode, taking data from\n");
			printf(" the provided file until EOF has been reached, or 'exit' or 'kill' entered.\n\n");
			printf(" If not killed or no -f option, the program runs in standard mode, taking data\n");
			printf(" from the keyboard or <stdin> on linux systems.\n");
			printf(" While executing, the following commands can be used:\n");
			printf("   '#line of text.....': will print out the input line as a comment line\n");
			printf("   'exit' | 'quit': will quit the current mode\n");
			printf("   'kill': [batch mode only] will exit the progam\n");
			printf("   'time <value>': will change the delay between batch commands to <value>\n");
			printf("   'verbose <on|off>': will change the verbose mode\n");
			printf("   'verbose': will return the current verbose mode\n");
			printf("   'xremote <on|off>': will change the xremote mode\n");
			printf("   'xremote': will return the current xremote mode\n");
			printf("   '' (empty line) [standard mode only]: will repeat the last entered command\n\n");
			printf(" All other commands are parsed and sent to XR18.\n");
			printf(" Typical XR18 command structure:\n");
			printf("   <command> [<format> [<data> [<data> [...]]]], where for example:\n");
			printf("      command: /info, /status, /ch/00/mix/fader, ...\n");
			printf("      format: ',i' ',f' ',s' or a combination: ',siss' ',ffiss' ...\n");
			printf("      data: a list of int, float or string types separated by a space char...\n\n");
			return(0);
			break;
		}
	}
#ifdef __WIN32__
//Initialize winsock
	if (WSAStartup (MAKEWORD( 2, 2), &wsa) != 0) {
		printf ("Failed. Error Code : %d", WSAGetLastError());
		exit (EXIT_FAILURE);
	}
#endif
//
// Load the XR18 address we connect to; we're a client to XR18, keep it simple.
	// Create UDP socket
	if ((Xfd = socket (PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
		perror ("failed to create XR18 socket");
		exit (EXIT_FAILURE);
	}
	// Construct  server sockaddr_in structure
	memset (&Xip, 0, sizeof(Xip));				// Clear struct
	Xip.sin_family = AF_INET;					// Internet/IP
	Xip.sin_addr.s_addr = inet_addr(Xip_str);	// IP address
	Xip.sin_port = htons(atoi(Xport_str));		// server port/
//
// Set receiving from XR18 to non blocking mode
// The 500ms timeout is used for delaying the printing of '.' at startup.
	timeout.tv_sec = 0;
	timeout.tv_usec = 500000; //Set timeout for non blocking recvfrom(): 500ms
	FD_ZERO(&ufds);
	FD_SET(Xfd, &ufds);
// make stdin (fd = 0) I/O nonblocking
#ifdef __WIN32__
	mode = 1;
	p_status = ioctlsocket(0, FIONBIO, &mode);

#else
	fcntl(0, F_SETFL, fcntl(0, F_GETFL, 0) | O_NONBLOCK);
#endif
//
// All done. Let's send and receive messages
// Establish logical connection with XRAir server
//	printf("Connecting to XAir.");
//
	keep_on = 1;
	xremote_on = X32verbose;	// Momentarily save X32verbose
	X32verbose = 0;
	s_len = Xsprint(s_buf, 0, 's', "/xinfo");
//	while (keep_on) {
//		SEND  				// command /info sent;
//		RPOLL 				// read data if available
//		if (p_status < 0) {
//			printf("Polling for data failed\n");
//			return 1;		// exit on receive error
//		} else if (p_status > 0) {
//			RECV 			// We have received data - process it!
//			if (strcmp(r_buf, "/xinfo") == 0)
//				break;		// Connected!
//		}					// ... else timeout
//		printf("."); fflush(stdout);
//	}
//	printf(" Done!\n");
//
// Set 1ms timeout to get faster response from XR18 (when testing for /xremote data).
	timeout.tv_usec = 1000; 	//Set timeout for non blocking recvfrom(): 1ms
	X32verbose = xremote_on;	// Restore X32verbose
	xremote_on = 0;
	l_index = 0;
	before = 0;
	if (filein) {
		if ((fdk = fopen(input_line, "r")) == NULL) {
			printf ("Cannot read file: %s\n", input_line);
			exit(EXIT_FAILURE);
		}
		if (filein == 1) {
			while (keep_on) {
				XREMOTE()
				if (fgets(input_line, LINEMAX, fdk)) {
					input_line[strlen(input_line) - 1] = 0;
					// Check for program batch mode commands
					if (input_line[0] == '#') printf("---comment: %s\n", input_line);
					TESTINPUT()			// Test for input data checks
					// Additional batch-mode input data checks
					else if (strcmp(input_line, "kill") == 0) {keep_on = 0; do_keyboard = 0;}
					else if (strncmp(input_line, "time", 4) == 0) {sscanf(input_line+5, "%d", &s_delay); printf(":: delay is: %d\n", s_delay);}
					else if (strlen(input_line) > 1) { // XR18 command line
						s_len = Xcparse(s_buf, input_line);
						SEND			// send batch command
					}
					CHECKXR()			// Check if XR sent something back
				} else {
					keep_on = 0;
				}
			}
			printf ("---end of batch mode file\n");
			fflush (stdout);
			if (fdk) fclose(fdk);
		} else {	// filein = 2 ('s' option)
			timeout.tv_usec = 10000; // Set timeout to 10ms
			do_keyboard = 0;	// force exit after end-of-file
			while (fgets(input_line, LINEMAX, fdk) != NULL) {
				// skip comment lines and blank lines
				if ((input_line[0] != '#') && (strlen(input_line) > 1)) {
					input_line[strlen(input_line) - 1] = 0;	// avoid trailing '\n'
				    char *inptr = input_line;
				    char inword[128];
				    char argwords[128];
				    int  posn;
				    int inquote=0;
					int argnum=0;
				    sscanf(inptr, "%127s%n", inword, &posn);
				    inptr += posn;
				    s_len = Xsprint(s_buf, 0, 's', inword);
				    r_len = 0;
			        while (sscanf(inptr, "%127s%n", inword, &posn) == 1)
			        {
			        		inptr += posn;
			        		if (inquote == 1) {
			        			strcat(argwords, inword);
			        			if ((inword[strlen(inword)-1] == QUOTE) || (inword[strlen(inword)-1] == SQUOTE)) {
				            		argwords[strlen(argwords)-1] = 0;
				            		strcpy(inword, argwords);
				            		inquote = 0;
			        			} else {
				        			strcat(argwords, " ");
			        			}
			        		}
			            if ((inword[0] == QUOTE) || (inword[0] == SQUOTE)) {
			            		inquote = 1;
			            		strcpy(argwords, inword+1);
			            		if ((inword[strlen(inword)-1] == QUOTE) || (inword[strlen(inword)-1] == SQUOTE)) {
				            		argwords[strlen(argwords)-1] = 0;
				            		strcpy(inword, argwords);
				            		inquote = 0;
			            		}
			            		strcat(argwords, " ");
			            }
			            if (inquote == 0) {
			            		argnum+=1;
			            		r_len = Xsprint(r_buf, r_len, 's', inword);
			            }
			        }
			        inword[0] = ',';
			        for(int i = 1; i <= argnum; i = i + 1 ) {
			              inword[i] = 's';
			              posn = i;
			        }
			        inword[posn+1] = 0;
			        s_len = Xsprint(s_buf, s_len, 's', inword);
					memcpy(s_buf+s_len, r_buf, r_len * sizeof(char));
					s_len += r_len;
					SEND				// send data to XR
					if (X32verbose) {    // No use checking for a response if not printing them
						CHECKXR()		// XR18 will echo back the line
					}
				}
			}
            printf ("---end of file\n");
            fflush (stdout);
            if (fdk) fclose(fdk);
		}
	}
// Done with the file (if there was one)
// revert to keyboard/interactive mode if enabled
	if (do_keyboard) {
		keep_on = 1;
		while (keep_on) {
			XREMOTE()	// process /xremote if needded
		    // build command by reading keyboard characters (from stdin)
#ifdef __WIN32__
			if (kbhit()) {
				input_intch = _getch();
#else
			input_intch = getc(stdin);
			{
#endif
				if (input_intch == EOL) {
					if (l_index) {
#ifdef __WIN32__
						printf("\n");
#endif
						// Check for program interactive mode commands
						input_line[l_index] = 0;
						if (input_line[0] == '#') printf("---comment: %s\n", input_line);
						TESTINPUT()			// Test for input data checks
						else { 				// new line/command
							s_len = Xcparse(s_buf, input_line);
							SEND // send parsed data
						}
						l_index = 0;
					} else {
						printf ("resending\n");
						SEND // empty line: resend previous parsed data
					}
				} else {
					if (l_index < LINEMAX) {
						// parse input_intch values, building new command
						if (input_intch != NO_CHAR) {
#ifdef __WIN32__
							printf("%c", (char)input_intch);
#endif
							input_line[l_index++] = (char)input_intch;
						}
						if (input_intch == BACKSPACE) {
#ifdef __WIN32__
							printf(" \b");
#endif
							l_index -= BACKCHARS; // remove char if backspace entered
							if (l_index < 0) l_index = 0;
						}
					} else {
						printf("!!! line too long - ignored line\n");
						l_index = 0;
					}
				}
			}
			CHECKXR()			// Check if XR sent something back
		}
	}
	close(Xfd);
	return 0;
}
