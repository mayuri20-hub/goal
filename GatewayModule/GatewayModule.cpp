/*
 * GatewayModule.cpp
 *
 *  Created on: 15-Jan-2020
 *      Author: mayuri
 */

#include<iostream>
#include<stdlib.h>
#include "GatewayModule.h"
#include "Server.h"
#include<map>
#include<chrono>
//#include<boost/thread.hpp>

#include<pthread.h>.
#include <sys/socket.h>
#include<unistd.h>
#include<sys/types.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<string.h>
#include<string>
#include<thread>
#include<mutex>

#include<mysql/mysql.h>


	MYSQL *conn;
	MYSQL_RES *result;
	MYSQL_ROW row;
	const char *server;
	const char *user;
	const char *database;
	 const int port = 3306;

using namespace std;
//using namespace boost;
int clientSocket;
string str ;
int data_rec = 0;
int tran_id=0;
int opt=0; // cnt_element=0;

pthread_t tid1;
string cust_accnt_no;
string TranID;
mutex m1;
char *p;
//pthread_mutex_t m1;
/*void* read_write_TCPIP_thread(void* argc)
{*/

int i=1;
char read_buf[4096];
int querysize;
void read_write_TCPIP_thread()
{

	//char write_buf[4096];

	char *temp_buf = new char[100];
	cout << " in thread" <<endl;
	while (true)
	{
	    memset(read_buf, 0, 4096);
	    string query = "hello" ;
	   // Wait for client to send data
	     int bytesReceived = recv(clientSocket,read_buf,4096,0);

	      if (bytesReceived == -1)
	      {
	              cerr << "Error in recv(). Quitting" << endl;
	              break;
	      }
	      else if(bytesReceived>0)
	       {

	             cust_accnt_no = read_buf;

	             cust_accnt_no = cust_accnt_no.substr(4);
	             cout << " CUSTOMER_ACCOUNT NO = " << cust_accnt_no<<endl;

	             TranID = read_buf;

	             TranID = TranID.substr(0,3);
	             if(TranID=="102")
	             {
	            	 cout <<"got 102" <<endl;
	             }
	             opt = stoi(TranID);
	             cout << " TRANID = " << opt<<endl;
	       }
	         int i= strncmp("bye",read_buf,3);
	         if(i==0)	break;
	         while(1)
	         {
	        	// cout<<"hello";
	        	 if(data_rec==1)
	        	 	 {
	        		 cout << " break while loop in thread" <<endl;
	        		 break;
	        	 	 }
	         }
	         cout << "send user information to client =";
	         	 for(int i=0;str[i]!='\0';i++)
	           {
	        	   temp_buf[i] = str[i];
	                cout << temp_buf[i];
	           }
	           data_rec=0;
	           int lenth = strlen(temp_buf);
	           cout << "length of string  = " << lenth <<endl;
	         //  boost::lock_guard <boost::mutex> l1{m1};
	           std::lock_guard<std::mutex> l1(m1);
	        		   write(clientSocket,temp_buf,lenth-1);

	       }

	       //return NULL;
}
int setup_TCPIP()
{
	cout << " TCP_IP settings ";

			int listening= socket(AF_INET,SOCK_STREAM,0);
			if(listening==-1)
			{
				cout << " cant create socket";
				return -1;
			}

			sockaddr_in hint;
			hint.sin_family = AF_INET;
			hint.sin_port =htons(54018);
			inet_pton(AF_INET,"127.0.0.1",&hint.sin_addr);

			if(bind(listening,(struct sockaddr *) &hint,sizeof(hint))==-1)
			{
				cout << " cant bind to IP ";
				return -2;
			}


			if(listen(listening,SOMAXCONN) == -1)
			{
				cout << " erron in listen";
			}

			sockaddr_in client;
			socklen_t clientSize = sizeof(client);

		    clientSocket = accept(listening, (sockaddr*)&client, &clientSize);

		    char host[NI_MAXHOST];      // Client's remote name
		        char service[NI_MAXSERV];   // Service (i.e. port) the client is connect on

		        if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
		         {
		                cout << host << " connected on port " << service << endl;
		         }
		         else
		          {
		                inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		                cout << host << " connected on port " << ntohs(client.sin_port) << endl;
		          }

		        // Close listening socket
		           close(listening);
		        //   boost::thread t1{read_write_TCPIP_thread};
		          // t1.join();
		        //   pthread_create(&tid1,NULL,&read_write_TCPIP_thread,NULL); //need to add library in
		                 //gcc c++ linker ->libraries ->add pthread
		                 // cout << "join thread" <<endl;

return 0;


}

int main()
{
	cout << " In a gateway Module " <<endl;

	RequestBase *requestCustObj;
	map<int,string> responseCustMapGateway;
	int i=1;

	string custlimit, cust_addr,depositeAmnt,withdrawAmnt;
	cout << "before main" <<endl;
	setup_TCPIP(); /* setup of TCPIP */

	std::thread t1(read_write_TCPIP_thread);


	cout << " leave main" <<endl;
	while(1)
	{

		switch(opt)
		{
			case 101: /* single cust enq*/
				cout<< "in 101" <<endl;
				requestCustObj = new singleCustEnq();


				/*****Filling requestBase object with enquiry parameters from client such as tellerId, branchId, requestType, enquiryType, enqsubtype,tranCode *****/



				requestCustObj->init_requestBase("frankTeller","SBIBranch","0TellerReq","5DatabaseEnq","20CustEnqInfo","11CustEnqTranCode");


				// fill map with request enuqry parameters from client
				requestCustObj->init_requestBase_map();

				requestCustObj->init_singleCust_req(cust_accnt_no); // fill customer account number in requestObj
				requestCustObj->set_requestBase_map();
				//requestCustObj->display_map();
				cout <<endl;
				str.clear();
				responseCustMapGateway = requestCustObj->execute_request();
				cout << endl <<endl;
				cout << "Display Customer basic details : " <<endl;
				for(auto itr = (responseCustMapGateway.begin());itr!=responseCustMapGateway.end();itr++)
				{
							cout << itr->first << "  " << itr->second <<"  ";
							 str += itr->second;
							str +='-';
				}
				cout <<endl;
				str = str.substr(6);
				data_rec=1; // FLAG SET FOR SENDING INFORM FROM SERVER TO CLIENT
				cout << endl<<endl;
				opt=0;
				if(cust_accnt_no == "10019")
				{
					opt = 10;
				}


				delete requestCustObj;
				break;


			case 2: /*** get multiple customer data */
				requestCustObj = new multipleCustEnq();
				requestCustObj->init_requestBase("frankTeller","SBIBranch","0TellerReq","5DatabaseEnq","24multipleCustEnq","11CustEnqTranCode");
				requestCustObj->init_requestBase_map();

				cout << "enter number of first slot of customers data want  =  " ;
				cin >> custlimit;
				/*custAccountNo = new string[custSize];
				cout << " enter customer account no  = " ;
				for(int i = 0;i<custSize;i++)
				{
					cin>> custAccountNo[i];
				}
				cout << endl;

				for(int i = 0;i<custSize;i++)
				{
									cout<< custAccountNo[i] << endl;;
				}*/

				requestCustObj->init_multipleCust_req(custlimit);
				requestCustObj->set_requestBase_map();
				requestCustObj->display_map();

				responseCustMapGateway = requestCustObj->execute_request();
				cout << "Display get cust slot : " << custlimit <<endl;

					for(auto itr= responseCustMapGateway.begin();itr!=responseCustMapGateway.end();itr++)
					{
						cout << itr->first << "  " << itr->second <<"  ";
						if(i==5)
						{
							cout <<endl;
							i=0;
						}
						i++;
					}

				cout << endl<<endl;
				delete requestCustObj;


				break;
			case 3: /*update customer details*/
				requestCustObj =  new updateCustEnq();
				requestCustObj->init_requestBase("frankTeller","SBIBranch","0TellerReq","5DatabaseEnq","21CustUpdate","11CustEnqTranCode");
				requestCustObj->init_requestBase_map();
				cout << " Enter customer number = ";
				cin>>cust_accnt_no;
				cout << " update address = ";
				cin >> cust_addr;
				requestCustObj->init_updateCust_req(cust_accnt_no,cust_addr);
				requestCustObj->set_requestBase_map();
				cout <<endl;

				//responseCustMapGateway = requestCustObj->execute_request();
				/*cout << "Display updated Customer basic details : " <<endl;
				for(auto itr = responseCustMapGateway.begin();itr!=responseCustMapGateway.end();itr++)
					{
								cout << itr->second <<"  ";
					}*/
								cout << endl<<endl;
								delete requestCustObj;
				//responseCustMapGateway
				break;
			case 104: /*** deposit amount ***/
				requestCustObj =  new depositAmount();
				requestCustObj->init_requestBase("frankTeller","SBIBranch","0TellerReq","5DatabaseEnq","22DepositeAmnt","11CustEnqTranCode");
				requestCustObj->init_requestBase_map();

				cout << " Enter customer number = ";
				cin>>cust_accnt_no;
				cout << " Enter deposit amount = ";
				cin >> depositeAmnt;

				requestCustObj->init_depositAmnt_req(cust_accnt_no,depositeAmnt);
				requestCustObj->set_requestBase_map();

				responseCustMapGateway = requestCustObj->execute_request();
				delete requestCustObj;
				break;
			case 105: /*** withdraw money ****/
				requestCustObj =  new withdrawAmount();
				requestCustObj->init_requestBase("frankTeller","SBIBranch","0TellerReq","5DatabaseEnq","23WithdrawAmnt","11CustEnqTranCode");
				requestCustObj->init_requestBase_map();

				cout << " Enter customer number = ";
				cin>>cust_accnt_no;
				cout << " Enter withdrawal amount = ";
				cin >> withdrawAmnt;

				requestCustObj->init_withdrawAmnt_req(cust_accnt_no,withdrawAmnt);
				requestCustObj->set_requestBase_map();


				responseCustMapGateway = requestCustObj->execute_request();
				delete requestCustObj;

				break;

			case 6:  /** download tables from host db to local host  --->>> admin request ***/



			requestCustObj =  new download_from_hostDB();
			requestCustObj->init_requestBase("admin1","SBIBranch","1adminReq","6hostEnq","25downloadTables","11CustEnqTranCode");
			requestCustObj->init_requestBase_map();

				responseCustMapGateway = requestCustObj->execute_request();
				delete requestCustObj;

				break;

		}
		if(opt == 10)
		{
			goto l1;
		}

	}
	l1:
	cout << " program ends" <<endl;
	cout << " exit thread";

	 t1.join();
	// pthread_join(tid1,NULL);

		                    // Close the socket
		                    close(clientSocket);


	/*requestObj.init_request_map();

	unordered_map<string,string> responseCustMapGateway = requestObj.execute_request();

		cout << "IN GATEWAY FINAL MODEL RESPONSE CUST OBJ = " <<endl;
	for(auto itr = responseCustMapGateway.begin();itr!=responseCustMapGateway.end();itr++)
		{
			cout << itr->second <<"  ";
		}
	cout << " hello  teller Project end " <<endl;
*/

}

