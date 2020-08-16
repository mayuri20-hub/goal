/*
 * Server.cpp
 *
 *  Created on: 15-Jan-2020
 *      Author: mayuri
 */

#include<iostream>
#include"Server.h"
#include"TellerBusinessLogic.h"

#include<string>
#include<map>
using namespace std;


void RequestBase :: init_requestBase(string recTellerId,string recBranchId,string recReqType,string recEnqType,string enqSubType, string recTranCode)
{
	cout << " in RequestBase :: init_requestBase " <<endl;

	tellerId = recTellerId;
	branchId = recBranchId;
	requestType= recReqType;
	enquiryType =recEnqType;
	enqsubtype = enqSubType;
	tranCode = recTranCode;
}

void RequestBase :: init_requestBase_map() // fill map with request from client
{
	cout << "RequestBase :: init_requestBase_map -------" <<endl;

	requestBaseMap["TELLER_ID"] = tellerId;
	requestBaseMap["BRANCH_ID"] = branchId;
	requestBaseMap["REQUEST_TYPE"] = requestType;
	requestBaseMap["ENQ_TYPE"] = enquiryType;
	requestBaseMap["ENQ_SUB_TYPE"] = enqsubtype;
	requestBaseMap["TRAN_CODE"] = tranCode;
}

void RequestBase :: init_singleCust_req(string accnt_No) // fill cust accoubnt number in requestObj
{
	cout << "RequestBase :: init_singleCust_req -------" <<endl;

	accountNo = accnt_No;
}
void RequestBase :: init_multipleCust_req(string custlimit)
{
	cout << "RequestBase :: init_singleCust_req -------" <<endl;
	custLimit = custlimit;

}
void RequestBase :: init_updateCust_req(string accnt_No, string cust_address)
{
	cout << "RequestBase :: init_updateCust_req -------" <<endl;

	accountNo = accnt_No;
	custAddress = cust_address;
}
void RequestBase :: init_depositAmnt_req(string accnt_No, string amount)
{
	cout << "RequestBase :: init_deposit Cust_req -------" <<endl;

	accountNo = accnt_No;
	depositeAmnt = amount;
}
void RequestBase :: init_withdrawAmnt_req(string accnt_No, string amount)
{
	cout << "RequestBase :: init_withdrawCust_req -------" <<endl;

	accountNo = accnt_No;
	withdrawAmnt = amount;
}




void singleCustEnq :: set_requestBase_map() // set cust account number into map
{
	cout << "in  singleCustEnq :: set_requestBase_map " <<endl;
	unordered_map<string,string> & singleCustEnqMap = get_requestBase_map();
	// it returns already filled map which consis tof enquiry parameters

	singleCustEnqMap["CUST_ACCNTNO"] =get_cust_accnt_no(); //  add customer account number into map
}
void multipleCustEnq :: set_requestBase_map()
{
	cout << "in  multipleCustEnq :: set_requestBase_map " <<endl;
	unordered_map<string,string> & multipleCustEnqMap = get_requestBase_map();
	multipleCustEnqMap["CUST_LIMIT"] = get_cust_limit();

	//singleCustEnqMap["CUST_ACCNTNO"] =get_cust_accnt_no();
}

void updateCustEnq :: set_requestBase_map()
{
	cout << "in  updateCustEnq :: set_requestBase_map " <<endl;
	unordered_map<string,string> & updateCustEnqMap = get_requestBase_map();
	updateCustEnqMap["CUST_ACCNTNO"] =get_cust_accnt_no();
	updateCustEnqMap["CUST_ADDRESS"] =get_cust_address();
}
void depositAmount :: set_requestBase_map()
{
	cout << "in  depositAmount :: set_requestBase_map " <<endl;
	unordered_map<string,string> & depositAmntMap = get_requestBase_map();
	depositAmntMap["CUST_ACCNTNO"] =get_cust_accnt_no();
	depositAmntMap["DEPOSIT_AMNT"] =get_deposit_amnt();
}

void withdrawAmount :: set_requestBase_map()
{
	cout << "in  withdrawAmount :: set_requestBase_map " <<endl;
	unordered_map<string,string> & withdrawAmntMap = get_requestBase_map();
	withdrawAmntMap["CUST_ACCNTNO"] =get_cust_accnt_no();
	withdrawAmntMap["WITHDRAW_AMNT"] =get_withdraw_amnt();
}



unordered_map<string,string>& RequestBase :: get_requestBase_map()
{
	return requestBaseMap;
}

map<int,string> RequestBase :: execute_request()
{
	cout << "In server: execute_request -------" <<endl;
	map<int,string> responseCustMapServer;
	switch(stoi(requestBaseMap["REQUEST_TYPE"]))
	{
	case 0: // teller request executed
		tellerObj.initialize_model(requestBaseMap);// map send
		responseCustMapServer= tellerObj.execute_teller_request();//(responseObj.get_response_map());
		break;
	case 1:
		cout << " in admin request -------"<<endl;

		adminObj.execute_admin_request();
		break;
	}
	return responseCustMapServer;

}
string RequestBase :: get_cust_accnt_no()
{
	return accountNo;
}
string RequestBase :: get_cust_address()
{
	return custAddress;
}
string RequestBase :: get_deposit_amnt()
{
	return depositeAmnt;
}
string RequestBase :: get_withdraw_amnt()
{
	return withdrawAmnt;
}
string RequestBase :: get_cust_limit()
{
	return custLimit;
}


void RequestBase ::  display_map()
{
	for (auto itr = requestBaseMap.begin();itr!=requestBaseMap.end();itr++)
	{
		cout <<itr->first << "   =   " << itr->second <<endl;
	}
}



