#ifndef BOOST_BEAST_EXAMPLE_COMMON_ROOT_CERTIFICATES_HPP
#define BOOST_BEAST_EXAMPLE_COMMON_ROOT_CERTIFICATES_HPP

#include <boost/asio/ssl.hpp>
#include <string>

namespace ssl = boost::asio::ssl; // from <boost/asio/ssl.hpp>

namespace detail {

	// The template argument is gratuituous, to
	// allow the implementation to be header-only.
	//
	template<class = void>
	void
		load_root_certificates(ssl::context & ctx, boost::system::error_code & ec)
	{
		std::string const cert =
			"-----BEGIN CERTIFICATE-----\n"
			"MIIDfzCCAmegAwIBAgIJAOXHAlO+SjxsMA0GCSqGSIb3DQEBCwUAMFYxCzAJBgNV\n"
			"BAYTAk5MMRAwDgYDVQQIDAdVVFJFQ0hUMRAwDgYDVQQHDAdVVFJFQ0hUMRIwEAYD\n"
			"VQQKDAlNWUNPTVBBTlkxDzANBgNVBAMMBkxBUFBJRTAeFw0xOTA2MzAxNTUwMTJa\n"
			"Fw00NjExMTUxNTUwMTJaMFYxCzAJBgNVBAYTAk5MMRAwDgYDVQQIDAdVVFJFQ0hU\n"
			"MRAwDgYDVQQHDAdVVFJFQ0hUMRIwEAYDVQQKDAlNWUNPTVBBTlkxDzANBgNVBAMM\n"
			"BkxBUFBJRTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAMTrvb6ACHC2\n"
			"k15Ay9tOdZTiBVxgsCZl1OEWxPO+QY9JNqrkFYLxT3u69L3MXmNgn88ZirOD7p1x\n"
			"bjClOT92bSX1kgWLbubGxqKu42NLBDZzPKvUVZf5d1sLE+4hdEd/gEEGZLtBjyOk\n"
			"e5nB+y4BYK7GRVJLbDS0Qwy+d5I4fx5oqFxYXx67HVqtpN2NHYVzLM5V+nCDec31\n"
			"qmBazFqAK9oNP2ffQKCN7VjCbFQOFm29+cvTzxQWb+KAk8KIi2eN2FuAbCJR4NdM\n"
			"gk4gLkvasqvp+iWnTZZciPqT+Ug5SAyhE59/iLcgEAaJbTw3VFqE/fPxVRoPYnX1\n"
			"aKaAfKeOX0sCAwEAAaNQME4wHQYDVR0OBBYEFGuL0/opcpoBr3j9iekEvipNK6ZP\n"
			"MB8GA1UdIwQYMBaAFGuL0/opcpoBr3j9iekEvipNK6ZPMAwGA1UdEwQFMAMBAf8w\n"
			"DQYJKoZIhvcNAQELBQADggEBAElV/jkQEh+/a5mmlp8ITHGDvHqwauyLB9RDbLdI\n"
			"vZDPl8rxMnMdN1gJj8uWiRTM/yqlQ7MZSClYzFjNvRXK379LBjPatmXJfPQHzP6O\n"
			"nFoF/kHK7itZmAIUhaQYDDpgPuiJ/oaWb67VnV86ohF7S4qTHPs+J+yzLX/FpF1F\n"
			"uiAwF33f08YudT3+FD6M7EgEzKxsliItGzcQ//VnV8U+gG5rEDX7vDOqQMIwYqA0\n"
			"fF6qDd+mdf/Hd8oxkqeOONYh65biUI9jAp7KLnLiReYXkH1D9JDPJFTeDNA5ZXQr\n"
			"iN/3XWoy7enMt1Qu2Q1jjormAG8nLhgaUC377xEIJE+j3a0=\n"
			"-----END CERTIFICATE-----\n";
		;

		ctx.add_certificate_authority(
			boost::asio::buffer(cert.data(), cert.size()), ec);
		if (ec)
			return;
	}

} // detail

// Load the root certificates into an ssl::context
//
// This function is inline so that its easy to take
// the address and there's nothing weird like a
// gratuituous template argument; thus it appears
// like a "normal" function.
//

inline
void
load_root_certificates(ssl::context& ctx, boost::system::error_code& ec)
{
	detail::load_root_certificates(ctx, ec);
}

inline
void
load_root_certificates(ssl::context& ctx)
{
	boost::system::error_code ec;
	detail::load_root_certificates(ctx, ec);
	if (ec)
		throw boost::system::system_error{ ec };
}

#endif