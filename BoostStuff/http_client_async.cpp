// http-client-async.cpp
// Taken from:
// https://www.boost.org/doc/libs/1_70_0/libs/beast/example/http/client/async/http_client_async.cpp
#define _WIN32_WINNT 0x0601

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/strand.hpp>
#include <boost/filesystem/fstream.hpp>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>
namespace ns_http_client_async {
	//***************************************************************************
	//*                    fail
	//***************************************************************************
	// Report a failure
	inline void
		fail(beast::error_code ec, char const* what)
	{
		std::cerr << what << ": " << ec.message() << "\n";
	}

	//***************************************************************************
	//*                    session
	//***************************************************************************
	// Performs an HTTP GET and prints the response
	class session : public std::enable_shared_from_this<session>
	{
		tcp::resolver resolver_;
		beast::tcp_stream stream_;
		beast::flat_buffer buffer_; // (Must persist between reads)
		http::request<http::empty_body> req_;
		http::response<http::string_body> res_;
		std::string mode_ = "";
		std::string file_target_ = "";
		std::string file_destination_ = "";

	public:
		// Objects are constructed with a strand to
		// ensure that handlers do not execute concurrently.
		explicit
			session(net::io_context& ioc)
			: resolver_(net::make_strand(ioc))
			, stream_(net::make_strand(ioc))
		{
		}

		// Start the asynchronous operation
		void
			run(
				char* mode,
				char* file_target,
				char* file_destination,
				char* host,
				char* port,
				char* target,
				int version)
		{
			mode_ = mode;

			if (!std::strcmp("download", mode)) {
				file_target_ = "/" + std::string(file_target);
				file_destination_ = file_destination;
				// Set up an HTTP GET request message
				req_.method(http::verb::get);
				req_.target(file_target_);
				req_.version(version);
				req_.set(http::field::host, host);
				req_.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
			}

			if (!std::strcmp("upload", mode)) {
				file_target_ = file_target;
				file_destination_ = "/" + std::string(file_destination);
				// Set up an HTTP PUT request message
				req_.method(http::verb::put);
				req_.target(file_destination_);
				req_.version(version);
				req_.set(http::field::host, host);
				req_.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
				// experimental
				req_.set(http::field::content_type, "text/plain");
				std::string str = "bla bla";
				req_.set(http::field::body, str);
				req_.prepare_payload();
			}

			// Look up the domain name
			resolver_.async_resolve(
				host,
				port,
				beast::bind_front_handler(
					&session::on_resolve,
					shared_from_this()));
		}

		void
			on_resolve(
				beast::error_code ec,
				tcp::resolver::results_type results)
		{
			if (ec)
				return fail(ec, "resolve");

			// Set a timeout on the operation
			stream_.expires_after(std::chrono::seconds(30));

			// Make the connection on the IP address we get from a lookup
			stream_.async_connect(
				results,
				beast::bind_front_handler(
					&session::on_connect,
					shared_from_this()));
		}

		void
			on_connect(beast::error_code ec, tcp::resolver::results_type::endpoint_type)
		{
			if (ec)
				return fail(ec, "connect");

			// Set a timeout on the operation
			stream_.expires_after(std::chrono::seconds(30));

			// Send the HTTP request to the remote host
			http::async_write(stream_, req_,
				beast::bind_front_handler(
					&session::on_write,
					shared_from_this()));
		}

		void
			on_write(
				beast::error_code ec,
				std::size_t bytes_transferred)
		{
			boost::ignore_unused(bytes_transferred);

			if (ec)
				return fail(ec, "write");

			// Receive the HTTP response
			http::async_read(stream_, buffer_, res_,
				beast::bind_front_handler(
					&session::on_read,
					shared_from_this()));
		}

		void
			on_read(
				beast::error_code ec,
				std::size_t bytes_transferred)
		{
			boost::ignore_unused(bytes_transferred);

			if (ec)
				return fail(ec, "read");

			// Write the message to standard out
			std::cout << res_ << std::endl;

			// Store the received file on disk
			if (!std::strcmp("download", mode_.c_str())) {
				boost::filesystem::path p{ file_destination_ };
				boost::filesystem::ofstream ofs{ p };
				ofs << res_.body();
			}

			// Gracefully close the socket
			stream_.socket().shutdown(tcp::socket::shutdown_both, ec);

			// not_connected happens sometimes so don't bother reporting it.
			if (ec && ec != beast::errc::not_connected)
				return fail(ec, "shutdown");

			// If we get here then the connection is closed gracefully
		}
	};

	//***************************************************************************
	//*                    http_client_async
	//***************************************************************************
	inline int http_client_async(int argc, char** argv)
	{
		// Check command line arguments.
		if (argc != 7 && argc != 8)
		{
			std::cout <<
				"Usage: http-client-async <mode: download or upload> <target file> <destination file> <host> <port> <target> [<HTTP version: 1.0 or 1.1(default)>]\n" <<
				"Example:\n" <<
				"    http-client-async download fileT fileD 127.0.0.1 8080 / \n" <<
				"    http-client-async upload fileT fileD 127.0.0.1 8080 / 1.0\n";
			return EXIT_FAILURE;
		}
		auto const mode = argv[1];
		auto const file_target = argv[2];
		auto const file_destination = argv[3];
		auto const host = argv[4];
		auto const port = argv[5];
		auto const target = argv[6];
		int version = argc == 8 && !std::strcmp("1.0", argv[7]) ? 10 : 11;

		// The io_context is required for all I/O
		net::io_context ioc;

		// Launch the asynchronous operation
		// set the file name where a downloaded file is stored on the client side
		// set parameter to determine a download or an upload
		std::make_shared<session>(ioc)->run(
			mode,
			file_target,
			file_destination,
			host,
			port,
			target,
			version);

		// Run the I/O service. The call will return when
		// the get operation is complete.
		ioc.run();

		return EXIT_SUCCESS;
	}
}
