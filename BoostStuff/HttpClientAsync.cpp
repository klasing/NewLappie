// HttpClientAsync.cpp
#include <iostream>
#include <iomanip>
#include <string>

// File client
#include "http_client_async.cpp"

using namespace std;
//****************************************************************************
//*                     get_input_from_user
//****************************************************************************
string get_input_from_user(
	const string& hdr,
	const int& width_header,
	const string& val,
	const int& width_value)
{
	string input_from_user = "";

	cout << setw(width_header)
		<< setiosflags(ios_base::left)
		<< setfill('.')
		<< hdr
		<< setw(width_value)
		<< setiosflags(ios_base::right)
		<< setfill('.')
		<< val
		<< ": ";
	cout.unsetf(ios::adjustfield);
	getline(cin, input_from_user, '\n');

	return input_from_user.length() > 0 ? input_from_user : val;
}

//****************************************************************************
//*                     get_connect_params_from_user
//****************************************************************************
void get_connect_params_from_user(
	string& ip_address,
	string& port_number)
{
	const string HDR_IP_ADDRESS = "IP Address";
	const string HDR_PORT_NUMBER = "Port";

	int width_header = HDR_IP_ADDRESS.length() + 3;
	int width_value = ip_address.length();

	ip_address = get_input_from_user(
		HDR_IP_ADDRESS,
		width_header,
		ip_address,
		width_value
	);
	port_number = get_input_from_user(
		HDR_PORT_NUMBER,
		width_header,
		port_number,
		width_value
	);
}

//****************************************************************************
//*                     get_file_names_from_user
//****************************************************************************
void get_file_names_from_user(
	const string& transfer_mode,
	string& file_name_on_target,
	string& file_name_on_destination)
{
	string HDR_FILE_NAME_ON_TARGET = "";
	string HDR_FILE_NAME_ON_DESTINATION = "";

	if (transfer_mode == "download") {
		HDR_FILE_NAME_ON_TARGET = "Target (server)";
		HDR_FILE_NAME_ON_DESTINATION = "Destination (client)";
	}
	else {
		HDR_FILE_NAME_ON_TARGET = "Target (client)";
		HDR_FILE_NAME_ON_DESTINATION = "Destination (server)";
	}

	int width_header = HDR_FILE_NAME_ON_DESTINATION.length() + 3;

	int width_value =
		(file_name_on_target.length() > file_name_on_destination.length()) ?
		file_name_on_target.length() :
		file_name_on_destination.length();

	file_name_on_target = get_input_from_user(
		HDR_FILE_NAME_ON_TARGET,
		width_header,
		file_name_on_target,
		width_value
	);

	file_name_on_destination = get_input_from_user(
		HDR_FILE_NAME_ON_DESTINATION,
		width_header,
		file_name_on_destination,
		width_value
	);
}

//****************************************************************************
//*                     do_transfer
//****************************************************************************
void do_transfer(
	const string& transfer_mode,
	const string& file_name_on_target,
	const string& file_name_on_destination,
	const string& ip_address,
	const string& port_number)
{
	char cmd[] = "http-client-async";
	char* mode = const_cast<char*>(transfer_mode.c_str());
	char* file_target = const_cast<char*>(file_name_on_target.c_str());
	char* file_destination = const_cast<char*>(file_name_on_destination.c_str());
	char* host = const_cast<char*>(ip_address.c_str());
	char* port = const_cast<char*>(port_number.c_str());
	char target[] = "/";
	char version[] = "1.0";

	int argc = 8;
	char* argv[] = { cmd, mode, file_target, file_destination,
		host, port, target, version };
	ns_http_client_async::http_client_async(argc, argv);
}

//****************************************************************************
//                      main
//****************************************************************************
int main() {
	const string IP_ADDRESS = "192.168.178.14";
	const string PORT_NUMBER = "8080";

	string ip_address = IP_ADDRESS;
	string port_number = PORT_NUMBER;

	string transfer_mode = "download";

	const string FILE_NAME_ON_SERVER_AT_DOWNLOAD = "index.html";
	const string FILE_NAME_ON_CLIENT_AT_DOWNLOAD = "index_client.html";

	string file_name_on_server_at_download = FILE_NAME_ON_SERVER_AT_DOWNLOAD;
	string file_name_on_client_at_download = FILE_NAME_ON_CLIENT_AT_DOWNLOAD;

	const string FILE_NAME_ON_CLIENT_AT_UPLOAD = "index_client.html";
	const string FILE_NAME_ON_SERVER_AT_UPLOAD = "index_server.html";

	string file_name_on_client_at_upload = FILE_NAME_ON_CLIENT_AT_UPLOAD;
	string file_name_on_server_at_upload = FILE_NAME_ON_SERVER_AT_UPLOAD;

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
			get_connect_params_from_user(
				ip_address,
				port_number
			);
			cerr << "IP Address: " << ip_address << ", Port: " << port_number << endl;
			break;
		case 2:
			transfer_mode = "download";
			get_file_names_from_user(
				transfer_mode,
				file_name_on_server_at_download,
				file_name_on_client_at_download
			);
			cerr << "Target (server): " << file_name_on_server_at_download << endl;
			cerr << "Destination (client): " << file_name_on_client_at_download << endl;
			do_transfer(
				transfer_mode,
				file_name_on_server_at_download,
				file_name_on_client_at_download,
				ip_address,
				port_number
			);
			break;
		case 3:
			transfer_mode = "upload";
			get_file_names_from_user(
				transfer_mode,
				file_name_on_client_at_upload,
				file_name_on_server_at_upload
			);
			cerr << "Target (client): " << file_name_on_client_at_upload << endl;
			cerr << "Destination (server): " << file_name_on_server_at_upload << endl;
			do_transfer(
				transfer_mode,
				file_name_on_client_at_upload,
				file_name_on_server_at_upload,
				ip_address,
				port_number
			);
			break;
		case 0:
			// the user wants to terminate
			bProceed = false;
			break;
		default:
			// the input, given by the user, is not an available option
			cout << "The entered number is not recognized, please try again." << endl;
			break;
		} // eof switch
	}

	return 0;
}
