// HttpClientAsyncSSL.cpp
#include <iostream>

#include "http_client_async_ssl.cpp"

int main()
{
	std::cout << "Http Client Async SSL" << std::endl;
	std::cout << "=====================" << std::endl;

	char cmd[] = "http-client-async-ssl";
	char host[] = /*"127.0.0.1";*/ "192.168.178.14";
	char port[] = "8080";// "443";
	char target[] = "/index.html";
	char version[] = "1.0";
	char* argv[] = { cmd, host , port, target, version };
	int argc = 5;
	ns_http_client_async_ssl::http_client_async_ssl(argc, argv);

	int iChar;
	std::cin >> iChar;
}