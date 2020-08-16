/*
 * Server.h
 *
 *  Created on: 15-Jan-2020
 *      Author: mayuri
 */

#ifndef SERVER_H_
#define SERVER_H_

#include<iostream>
#include<string>
#include<unordered_map>
#include"TellerBusinessLogic.h"
#include"AdminBusinessLogic.h"
#include<map>

using namespace std;



class ResponseBase
{
	int account_no;
	string customer_name;
	string address;
	int balance;
	string account_type;
	static unordered_map<int, ResponseBase> responseMap;

public:

	static unordered_map<int, ResponseBase>& get_response_map();


};


class RequestBase
{
	/*header */
	string tellerId;
	string branchId;
	string requestType;
	string enquiryType;
	string enqsubtype;
	string tranCode;

	string accountNo;
	string customerName;
	string custAddress;
	string accntType;
	string amount;
	string accntNoArr[10];
	string depositeAmnt;
	string withdrawAmnt;

	string custLimit;
	TellerBusinessLogic tellerObj;
	AdminBusinessLogic adminObj;

	/*request data*/
	unordered_map <string ,string> requestBaseMap;
public:

	void init_requestBase(string,string,string,string,string,string);// fill object with enquiry parameters
	void init_requestBase_map();

	void init_singleCust_req(string);
	void init_multipleCust_req(string);
	void init_updateCust_req(string,string);

	void init_depositAmnt_req(string,string);
	void init_withdrawAmnt_req(string,string);
	void display_map();

	unordered_map<string,string>& get_requestBase_map();
	map<int,string> execute_request();

	/*** getters ***/
	string get_cust_accnt_no();
	string get_cust_address();
	string get_cust_name();
	string get_cust_amount();
	string get_deposit_amnt();
	string get_withdraw_amnt();
	string get_cust_limit();




	/********virtual method ***/
	virtual void set_requestBase_map(){}

	virtual ~RequestBase()
	{
		cout <<" request base destructor called " <<endl;
	}

};

class singleCustEnq : public RequestBase
{
public:
	void set_requestBase_map();

	virtual ~singleCustEnq()
		{
			cout <<"  singleCustEnq destructor called " <<endl;
		}
};
class multipleCustEnq : public RequestBase
{
public:
	void set_requestBase_map();

	virtual ~multipleCustEnq()
		{
			cout <<"  multipleCustEnq destructor called " <<endl;
		}
};

class updateCustEnq : public RequestBase
{
	string address;
	string accnt_type;
public:
	void set_requestBase_map();


	virtual ~updateCustEnq()
		{
				cout <<"  updateCustEnq destructor called " <<endl;
		}
};

class depositAmount : public RequestBase
{

public:
	void set_requestBase_map();

		virtual ~depositAmount()
			{
				cout <<"  deposit amnt  destructor called " <<endl;
			}
};

class withdrawAmount : public RequestBase
{

public:
	void set_requestBase_map();

		virtual ~withdrawAmount()
			{
				cout <<"  withdraw amnt  destructor called " <<endl;
			}
};

class download_from_hostDB : public RequestBase
{

public:
	//void set_requestBase_map();

		//virtual ~download_from_hostDB()
		//	{
				//cout <<"  download_from_hostDB   destructor called " <<endl;
		//	}
};



#endif /* SERVER_H_ */
