// placeholder

#include "request.h"

accessRequest::accessRequest(char* cardNumber, char* lockerSegmentInfo, char* deviceIp,
	char* id, char* commandTypeName,
	char* isResponse, char* isNotification)
{




	/*for 
	std::vector<char> command;
	command.reserve()*/

	/*byte cardNumber[sizeof(cardNumber) / sizeof(cardNumber[0]) - 1];
	for (int i = 0; i < sizeof(cardNumber) / sizeof(cardNumber[0]) - 1; i++)
	{		
		cardNumber[i] = str_cardNumber[i];
	}

	deviceIp[0] = 'a';*/
}	

vector<byte> accessRequest::createLine(char* str, char* value, bool isString)
{
	vector<char> str_vec(str, str + sizeof str/sizeof str[0]);
	char* charArray = &str_vec[0];
	for (int i = 0; i < str_vec.size(); i++)
	{

	}

	vector<byte> value_vec(value, value + sizeof value/sizeof value[0]);
	vector<byte> space_vec = isString ? space : spaceNoString;
	vector<byte> end_vec = isString ? endline : endlineNoString;

	vector<byte> vectors[6] = {startline, str_vec, space_vec, value_vec, end_vec, nextline};

	vector<byte> command;
	command.reserve(vectors[0].size() + vectors[1].size() + vectors[2].size() + 
		vectors[3].size() + vectors[4].size() + vectors[5].size());

	for (int i = 0; i < sizeof vectors / sizeof vectors[0]; i++)
	{
		command.insert(command.end(), vectors[i].begin(), vectors[i].end());
	}

	return command;
}