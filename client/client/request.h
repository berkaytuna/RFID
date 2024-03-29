// placeholder

#pragma once

#include <stdint.h>
#include <string>
#include <vector>

using namespace std;

typedef uint8_t byte;

const vector <byte> init{ 0x7B, 0x0D, 0x0A };
const vector <byte> last{ 0x7D };
const vector <byte> nextline{ 0x0D, 0x0A };
const vector <byte> startline{ 0x20, 0x20, 0x22 };
const vector <byte> endline{ 0x22, 0x2C };
const vector <byte> endlineNoString{ 0x2C };
const vector <byte> endlineNull{ };
const vector <byte> space{ 0x22, 0x3A, 0x20, 0x22 };
const vector <byte> spaceNoString{ 0x22, 0x3A, 0x20 };

class accessRequest
{
public:
	char str_cardNumber[11] = "CardNumber";
	char str_lockerSegmentInfo[18] = "LockerSegmentInfo";
	char str_deviceIp[9] = "DeviceIp";
	char str_devicePort[11] = "DevicePort";
	char str_id[3] = "ID";
	char str_commandTypeName[16] = "CommandTypeName";
	char str_isResponse[11] = "IsResponse";
	char str_isNotification[15] = "IsNotification";

	/*char cardNumber[9] = "00000000";
	char lockerSegmentInfo[5] = "null";
	char deviceIp[];
	char devicePort = "1";
	char id = "00000000-0000-0000-0000-000000000000";
	char commandTypeName = "CDI_Shared_PCL.DataObjects.AccessRequest";
	char isResponse = "false";
	char isNotification = "false";*/

	bool isString_array[8] = { true, false, true, false, true, false, false, false };

	vector <byte> createRequest(vector<char*> params, vector<int> sizes);
	vector<byte> createLine(char* str, int strSize, char* value, int valueSize, bool isString, bool isLastLine);
};