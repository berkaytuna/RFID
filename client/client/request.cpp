// placeholder

#include "request.h"
#include <iostream>

vector<byte> accessRequest::createRequest(vector<char*> params, vector<int> sizes)
{
	vector<char*> strings{ str_cardNumber, str_lockerSegmentInfo, str_deviceIp, str_devicePort, str_id, str_commandTypeName, str_isResponse, str_isNotification };
	vector<int> strSizes{ 11, 18, 9, 11, 3, 16, 11, 15 };

	vector<byte> commandLines[params.size()];
	int commandSize = 0;
	for (int i = 0; i < params.size(); i++)
	{
		bool isLastLine = (i == params.size() - 1) ? true : false;
		commandLines[i] = createLine(strings[i], strSizes[i], params[i], sizes[i], isString_array[i], isLastLine);
		commandSize += commandLines[i].size();
	}

	vector<byte> command;
	command.reserve(commandSize + 2);
	command.insert(command.end(), init.begin(), init.end());
	for (int i = 0; i < params.size(); i++)
	{
		command.insert(command.end(), commandLines[i].begin(), commandLines[i].end());
	}
	command.insert(command.end(), last.begin(), last.end());

	return command;
}

vector<byte> accessRequest::createLine(char* str, int strSize, char* value, int valueSize, bool isString, bool isLastLine)
{
	vector<byte> str_vec(str, str + strSize - 1);
	vector<byte> value_vec(value, value + valueSize - 1);
	vector<byte> space_vec = isString ? space : spaceNoString;
	vector<byte> endline_vec = isString ? endline : endlineNoString;
	vector<byte> end_vec = isLastLine ? endlineNull : endline_vec;

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