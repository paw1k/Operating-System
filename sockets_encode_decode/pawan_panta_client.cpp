// Please note this is a C program
// It compiles without warnings with gcc
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sstream>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <sys/wait.h>

using namespace std;

void error(char *msg)
{
	perror(msg);
	exit(0);
}


int decode_function(int arr[]) {

	int Decode_Message[12];
	int walsh[4];

	for (int i = 12; i < 16; i++) {

		walsh[i - 12] = arr[i];  //taking out the walsh code and storing in walsh[]

	}


	int count = 0;


	for (int i = 0; i < 9; i = i + 4) {
		for (int j = 0; j < 4; j++) {
			Decode_Message[i + j] = arr[i + j] * walsh[j];

		}
	}





	int dm_finished_1 = (Decode_Message[0] + Decode_Message[1] + Decode_Message[2] + Decode_Message[3]) / 4;

	int dm_finished_2 = (Decode_Message[4] + Decode_Message[5] + Decode_Message[6] + Decode_Message[7]) / 4;
	int dm_finished_3 = (Decode_Message[8] + Decode_Message[9] + Decode_Message[10] + Decode_Message[11]) / 4;

	//cout<<endl<<dm1<<" "<<dm2<<" "<<dm3<<endl;

	if (dm_finished_1 == -1 && dm_finished_2 == -1 && dm_finished_3 == -1) {   //determining which value should be returned, basically like binary style.
		return 0;
	}
	else if (dm_finished_1 == -1 && dm_finished_2 == -1 && dm_finished_3 == 1) {
		return 1;
	}
	else if (dm_finished_1 == -1 && dm_finished_2 == 1 && dm_finished_3 == -1) {
		return 2;
	}
	else if (dm_finished_1 == -1 && dm_finished_2 == 1 && dm_finished_3 == 1) {
		return 3;
	}
	else if (dm_finished_1 == 1 && dm_finished_2 == -1 && dm_finished_3 == -1) {
		return 4;
	}
	else if (dm_finished_1 == 1 && dm_finished_2 == -1 && dm_finished_3 == 1) {
		return 5;
	}
	else if (dm_finished_1 == 1 && dm_finished_2 == 1 && dm_finished_3 == -1) {
		return 6;
	}
	else if (dm_finished_1 == 1 && dm_finished_2 == 1 && dm_finished_3 == 1) {
		return 7;
	}



}



int main(int argc, char *argv[])
{
	int sockfd, portno, n;
	int process1[5];
	int process2[5];
	int process3[5];
	pid_t pid;

	int Server_EM1[16], Server_EM2[16], Server_EM3[16];

	struct sockaddr_in serv_addr;
	struct hostent *server;

	//char buffer[256];
	if (argc < 3) {
		fprintf(stderr, "usage %s hostname port\n", argv[0]);
		exit(0);
	}

	process1[0] = 1, process2[0] = 2; process3[0] = 3;
	cin >> process1[1] >> process1[2];
	cin >> process2[1] >> process2[2];
	cin >> process3[1] >> process3[2];

	cout << "Child 1, sending value: " << process1[2] << " to child process " << process1[1] << endl;
	cout << "Child 2, sending value: " << process2[2] << " to child process " << process2[1] << endl;
	cout << "Child 3, sending value: " << process3[2] << " to child process " << process3[1] << endl;


	portno = atoi(argv[2]);

	int i;


	for (i = 0; i < 3; i++)
	{
		if ((pid = fork()) == 0)
			break;
	}


	if (pid == 0)
	{


		sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if (sockfd < 0)
			error("ERROR opening socket");
		server = gethostbyname(argv[1]);
		if (server == NULL) {
			fprintf(stderr, "ERROR, no such host\n");
			exit(0);
		}
		bzero((char *)&serv_addr, sizeof(serv_addr));
		serv_addr.sin_family = AF_INET;
		bcopy((char *)server->h_addr,
			(char *)&serv_addr.sin_addr.s_addr,
			server->h_length);
		serv_addr.sin_port = htons(portno);
		if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
			error("ERROR connecting");



		if (i == 0)
		{
			n = write(sockfd, &process1, 5 * sizeof(int));
			n = read(sockfd, &Server_EM1, 16 * sizeof(int));


			// for(int i=0; i<16; i++){
			//     cout<<EM1[i]<<" ";
			// }



			cout << "Child 1" << endl << "Signal:";

			for (int i = 0; i < 12; i++) {
				cout << Server_EM1[i] << " ";
			}
			cout << endl << "Code: ";
			for (int i = 12; i < 16; i++) {
				cout << Server_EM1[i] << " ";
			}

			int value = decode_function(Server_EM1);

			cout << endl << "Recieved Value = " << value << endl << endl;

		}

		else if (i == 1)
		{

			n = write(sockfd, &process2, 5 * sizeof(int));
			n = read(sockfd, &Server_EM2, 16 * sizeof(int));


			//  for(int i=0; i<16; i++){
			//     cout<<EM2[i]<<" ";
			// }


			cout << "Child 2" << endl << "Signal:";

			for (int i = 0; i < 12; i++) {
				cout << Server_EM2[i] << " ";
			}
			cout << endl << "Code: ";
			for (int i = 12; i < 16; i++) {
				cout << Server_EM2[i] << " ";
			}

			int value = decode_function(Server_EM2);


			cout << endl << "Recieved Value = " << value << endl << endl;
		}

		else if (i == 2)
		{

			n = write(sockfd, &process3, 5 * sizeof(int));
			n = read(sockfd, &Server_EM3, 16 * sizeof(int));


			//  for(int i=0; i<16; i++){
			//     cout<<EM3[i]<<" ";
			// }


			cout << "Child 3" << endl << "Signal:";

			for (int i = 0; i < 12; i++) {
				cout << Server_EM3[i] << " ";
			}
			cout << endl << "Code: ";
			for (int i = 12; i < 16; i++) {
				cout << Server_EM3[i] << " ";
			}

			int value = decode_function(Server_EM3);


			cout << endl << "Recieved Value = " << value << endl << endl;
		}





	}
	else {
		for (int i = 0; i < 3; i++)
		{
			wait(NULL);
		}
	}








	return 0;
}
