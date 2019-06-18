// HttpClientAsync.cpp
#include <iostream>
#include <string>

// File client
#include "http_client_async.cpp"

using namespace std;
//****************************************************************************
//*                     get_input_from_user
//****************************************************************************
string get_input_from_user(const string& hdr, const string& val) {
	string input_from_user = "";

	cout << hdr << val << " ";
	getline(cin, input_from_user, '\n');

	return input_from_user.length() > 0 ? input_from_user : val;
}

//****************************************************************************
//*                     get_connect_params_from_user
//****************************************************************************
void get_connect_params_from_user(
	const string& IP_ADDRESS, string& ip_address,
	const string& PORT_NUMBER, string& port_number) {

	const string HDR_IP_ADDRESS = "IP Address...: ";
	const string HDR_PORT_NUMBER = "Port.........: ";

	ip_address = get_input_from_user(HDR_IP_ADDRESS, IP_ADDRESS);
	port_number = get_input_from_user(HDR_PORT_NUMBER, PORT_NUMBER);
}

//****************************************************************************
//*                     get_file_names_from_user
//****************************************************************************
void get_file_names_from_user(
	const string& FILE_NAME_ON_SOURCE,
	string& file_name_on_source,
	const string& FILE_NAME_ON_TARGET,
	string& file_name_on_target) {

	const string HDR_FILE_NAME_ON_SOURCE = "Source...: ";
	const string HDR_FILE_NAME_ON_TARGET = "Target...: ";

	file_name_on_source = get_input_from_user(
		HDR_FILE_NAME_ON_SOURCE, FILE_NAME_ON_SOURCE);
	file_name_on_target = get_input_from_user(
		HDR_FILE_NAME_ON_TARGET, FILE_NAME_ON_TARGET);
}

//****************************************************************************
//*                     main
//****************************************************************************
int main() {
	const string IP_ADDRESS = "192.168.178.14";
	const string PORT_NUMBER = "8080";
	const string FILE_NAME_ON_SERVER = "index.html";
	const string FILE_NAME_ON_CLIENT = "index_client.html";

	string ip_address = IP_ADDRESS;
	string port_number = PORT_NUMBER;
	string file_name_on_server = "";
	string file_name_on_client = "";

	bool bProceed = true;
	int iChar;

	while (bProceed) {
		cout << "Asynchronous HTTP Client" << endl;
		cout << "========================" << endl;
		cout << " 1) Connect" << endl;
		cout << " 2) Download" << endl;
		cout << " 3) Upload" << endl;
		cout << "Enter the number of a subject, or enter a zero to quit: ";

		cin >> iChar;
		// get rid of the new line character, to ensure the buffer sanity
		cin.get();

		switch (iChar) {
		case 1:
			get_connect_params_from_user(IP_ADDRESS, ip_address,
				PORT_NUMBER, port_number);
			cout << ip_address << ", " << port_number << endl;
			break;
		case 2: {
			get_file_names_from_user(FILE_NAME_ON_SERVER,
				file_name_on_server,
				FILE_NAME_ON_CLIENT,
				file_name_on_client);

			file_name_on_server.insert(0, "/");

			// do download
			char cmd[] = "http-client-async";
			char* host = const_cast<char*>(ip_address.c_str());
			char* port = const_cast<char*>(port_number.c_str());
			char* target = const_cast<char*>(file_name_on_server.c_str());
			char* destination = const_cast<char*>(file_name_on_client.c_str());
			char version[] = "1.0";
			char* argv[] = { cmd, host , port, target, destination, version };
			ns_http_client_async::http_client_async(6, argv);
		}
				break;
		case 3:
			get_file_names_from_user(FILE_NAME_ON_CLIENT,
				file_name_on_client,
				FILE_NAME_ON_SERVER,
				file_name_on_server);
			break;
		case 0:
			// the user wants to terminate
			bProceed = false;
			break;
		default:
			// the input, given by the user, is not an available option
			cout << "The entered number is not recognized, please try again." << endl;
		} // eof switch
	}

	return 0;
}
