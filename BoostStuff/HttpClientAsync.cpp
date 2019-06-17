// File client
#include "http_client_async.cpp"

int main() {
	char cmd[] = "http-client-async";
	char host[] = "192.168.178.14";//"lappie";//"127.0.0.1";
	char port[] = "8080";
	char target[] = "/";
	char version[] = "1.0";
	char* argv[] = { cmd, host , port, target, version };
	ns_http_client_async::http_client_async(5, argv);

	int iChar;
	std::cin >> iChar;
}
