// placeholder

#include "request.h"
#include <iostream>

accessRequest::accessRequest(char* cardNumber, char* lockerSegmentInfo, char* deviceIp,
	char* id, char* commandTypeName, char* isResponse, char* isNotification)
{
	vector<char*> params{cardNumber, lockerSegmentInfo, deviceIp, id,};
}	

vector<byte> accessRequest::createLine(char* str, int strSize, char* value, int valueSize, bool isString)
{
	vector<byte> str_vec(str, str + strSize);
	vector<byte> value_vec(value, value + valueSize);
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