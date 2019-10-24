// A simple server in the internet domain using TCP
// The port nu1mber is passed as an argument

// Please note this is a C program
// It compiles without warnings with gcc

#include<iostream>
#include<ostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<sstream>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;

int processid[5];
int process[5];
int process1[5], process2[5], process3[5];
//int temp_em[12];
//int em1[12],em2[12],em3[12];



void error(char *msg)
{
	perror(msg);
	exit(1);
}


// void arrayCopy(int arr1[], int arr2[]){

//     for(int i=0; i<5; i++)
//     {
//         arr2[i]=arr1[i];
//     }
// }


int* encoding_function(int value, int w[]) {    //pass in the Value to convert to binary and return the temp_em by multiplying with the Walsh code


	static int temp_em[12];

	int temp_binary[3];

	int binary_for_0[3] = { -1,-1,-1 };     // assigns the decimal value a binary format 
	int binary_for_1[3] = { -1,-1,1 };
	int binary_for_2[3] = { -1,1,-1 };
	int binary_for_3[3] = { -1,1,1 };
	int binary_for_4[3] = { 1,-1,-1 };
	int binary_for_5[3] = { 1,-1,1 };
	int binary_for_6[3] = { 1,1,-1 };
	int binary_for_7[3] = { 1,1,1 };


	switch (value) {                  //determining decimal to binary value
									//if value == 0 or 1 switch statements chooses correct value and returns the correct binary value 
	case 0:
		copy(binary_for_0, binary_for_0 + 3, temp_binary);
		break;
	case 1:
		copy(binary_for_1, binary_for_1 + 3, temp_binary);
		break;
	case 2:
		copy(binary_for_2, binary_for_2 + 3, temp_binary);
		break;
	case 3:
		copy(binary_for_3, binary_for_3 + 3, temp_binary);
		break;
	case 4:
		copy(binary_for_4, binary_for_4 + 3, temp_binary);
		break;
	case 5:
		copy(binary_for_5, binary_for_5 + 3, temp_binary);
		break;
	case 6:
		copy(binary_for_6, binary_for_6 + 3, temp_binary);
		break;
	case 7:
		copy(binary_for_7, binary_for_7 + 3, temp_binary);
		break;

	}



	int count = 0;

	for (int i = 0; i < 3; i++) {                      //where encoding actually happens, with temp_em storing the multiplied binary number (3 digit)  
		for (int j = 0; j < 4; j++) {                   //with walsh code (4 digit) to create 12 digit long encoded message
			temp_em[count] = temp_binary[i] * w[j];
			//cout<<temp_em[count]<<" ";
			count++;
		}
	}


	return temp_em;
}



int main(int argc, char *argv[])
{
	int sockfd, newsockfd, portno, clilen;

	int em1[12], em2[12], em3[12];
	int EM[12];
	//char buffer[256];


	int w1[4] = { -1,1,-1,1 };
	int w2[4] = { -1,-1,1,1 };
	int w3[4] = { -1,1,1,-1 };



	struct sockaddr_in serv_addr, cli_addr;
	int n;
	if (argc < 2) {
		fprintf(stderr, "ERROR, no port provided\n");
		exit(1);
	}
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		error("ERROR opening socket");
	bzero((char *)&serv_addr, sizeof(serv_addr));
	portno = atoi(argv[1]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	if (bind(sockfd, (struct sockaddr *) &serv_addr,
		sizeof(serv_addr)) < 0)
		error("ERROR on binding");



	for (int i = 0; i < 3; i++) {
		listen(sockfd, 5);
		clilen = sizeof(cli_addr);
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, (socklen_t *)&clilen);
		if (newsockfd < 0)
			error("ERROR on accept");



		n = read(newsockfd, &process, 5 * sizeof(int));

		if (process[0] == 1)
		{
			process1[1] = process[1];
			process1[2] = process[2];

			//arrayCopy(process,process1);
			processid[0] = newsockfd;
		}
		else if (process[0] == 2)
		{
			process2[1] = process[1];
			process2[2] = process[2];
			//arrayCopy(process,process2);
			processid[1] = newsockfd;
		}
		else
		{
			process3[1] = process[1];
			process3[2] = process[2];
			//arrayCopy(process,process3);
			processid[2] = newsockfd;

		}



	}

	cout << "Here is the message from child 1: Value = " << process1[2] << ", Destination = " << process1[1] << endl;
	cout << "Here is the message from child 2: Value = " << process2[2] << ", Destination = " << process2[1] << endl;
	cout << "Here is the message from child 3: Value = " << process3[2] << ", Destination = " << process3[1] << endl;







	int *proc1_em_1 = encoding_function(process1[2], w1);
	for (int i = 0; i < 12; i++) {
		em1[i] = *(proc1_em_1 + i);
	}


	int *proc2_em_2 = encoding_function(process2[2], w2);
	for (int i = 0; i < 12; i++) {
		em2[i] = *(proc2_em_2 + i);
	}


	int *proc3_em_3 = encoding_function(process3[2], w3);
	for (int i = 0; i < 12; i++) {
		em3[i] = *(proc3_em_3 + i);
	}



	for (int i = 0; i < 12; i++) {
		EM[i] = em1[i] + em2[i] + em3[i];
	}




	int em1final[16], em2final[16], em3final[16]; 

	for (int i = 0; i < 12; i++) {   //message to send for each child process
		em1final[i] = EM[i];
		em2final[i] = EM[i];
		em3final[i] = EM[i];
	}


	//process 1

	if (process1[1] == 1) {
		em1final[12] = w1[0];    //attach the 4 digit walsh code at end 
		em1final[13] = w1[1];
		em1final[14] = w1[2];
		em1final[15] = w1[3];
	}
	else if (process2[1] == 1) {
		em1final[12] = w2[0];
		em1final[13] = w2[1];
		em1final[14] = w2[2];
		em1final[15] = w2[3];
	}
	else if (process3[1] == 1) {
		em1final[12] = w3[0];
		em1final[13] = w3[1];
		em1final[14] = w3[2];
		em1final[15] = w3[3];
	}



	//process 2


	if (process1[1] == 2) {
		em2final[12] = w1[0];
		em2final[13] = w1[1];
		em2final[14] = w1[2];
		em2final[15] = w1[3];
	}
	else if (process2[1] == 2) {
		em2final[12] = w2[0];
		em2final[13] = w2[1];
		em2final[14] = w2[2];
		em2final[15] = w2[3];
	}
	else if (process3[1] == 2) {
		em2final[12] = w3[0];
		em2final[13] = w3[1];
		em2final[14] = w3[2];
		em2final[15] = w3[3];
	}


	//process 3

	if (process1[1] == 3) {
		em3final[12] = w1[0];
		em3final[13] = w1[1];
		em3final[14] = w1[2];
		em3final[15] = w1[3];
	}
	else if (process2[1] == 3) {
		em3final[12] = w2[0];
		em3final[13] = w2[1];
		em3final[14] = w2[2];
		em3final[15] = w2[3];
	}
	else if (process3[1] == 3) {
		em3final[12] = w3[0];
		em3final[13] = w3[1];
		em3final[14] = w3[2];
		em3final[15] = w3[3];
	}












	n = write(processid[0], &em1final, 16 * sizeof(int));
	if (n < 0) error("ERROR writing to socket");
	sleep(1);

	n = write(processid[1], &em2final, 16 * sizeof(int));
	if (n < 0) error("ERROR writing to socket");
	sleep(1);

	n = write(processid[2], &em3final, 16 * sizeof(int));
	if (n < 0) error("ERROR writing to socket");
	sleep(1);





	return 0;
}
