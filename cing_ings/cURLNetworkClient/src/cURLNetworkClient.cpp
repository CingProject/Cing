#include "Cing-Precompiled.h"

#include "cURLNetworkClient.h"

namespace Cing
{

	static size_t responseCallback(void* buf, size_t size, size_t nmemb, void* userp)
	{
		if ( userp )
		{
			std::ostream& os = *static_cast<std::ostream*>(userp);
			std::streamsize len = size * nmemb;
			if ( os.write(static_cast<char*>(buf), len) )
				return len;
		}
		return 0;
	}

	cURLNetworkClient::cURLNetworkClient(void)
	{
		// nothing
	}


	cURLNetworkClient::~cURLNetworkClient(void)
	{
		// nothing
	}

	void cURLNetworkClient::setup()
	{
		curl_global_init(CURL_GLOBAL_ALL);
	}

	std::string& cURLNetworkClient::fetchJSON(std::string URL)
	{   
		std::ostringstream oss;

		struct curl_slist *headers = NULL; // init to NULL is important 
		curl_slist_append( headers, "Accept: application/json" );  
		curl_slist_append( headers, "Content-Type: application/json" );
		curl_slist_append( headers, "charsets: utf-8" );

		fetchRequest( headers, &URL, &oss );

		_lastResult = oss.str();

		return _lastResult;
	}

	std::string& cURLNetworkClient::fetchXML(std::string URL)
	{   
		std::ostringstream oss;

		struct curl_slist *headers = NULL; // init to NULL is important 
		curl_slist_append( headers, "Accept: application/xml" );  
		curl_slist_append( headers, "Content-Type: application/xml" );
		curl_slist_append( headers, "charsets: utf-8" );

		fetchRequest( headers, &URL, &oss );

		_lastResult = oss.str();

		return _lastResult;
	}

	void cURLNetworkClient::fetchRequest( curl_slist *headers, std::string* URL, std::ostringstream *oss )
	{
		CURLcode res(CURLE_FAILED_INIT);

		CURL *curl;
		curl = curl_easy_init();
		if (curl) 
		{
			curl_easy_setopt( curl, CURLOPT_HTTPHEADER, headers );
			curl_easy_setopt( curl, CURLOPT_URL, URL->c_str() );
			curl_easy_setopt( curl, CURLOPT_HTTPGET, 1 );
			curl_easy_setopt( curl, CURLOPT_HTTPHEADER, headers );
			curl_easy_setopt( curl, CURLOPT_FILE, oss );
			curl_easy_setopt( curl, CURLOPT_WRITEFUNCTION, &responseCallback );

			res = curl_easy_perform( curl );
			if  (CURLE_OK == res ) 
			{ 
				char *ct;         
				res = curl_easy_getinfo( curl, CURLINFO_CONTENT_TYPE, &ct );
				if ( (CURLE_OK == res) && ct )
				{
					LOG_TRIVIAL("response success: %s", ct);
				}
				else
				{
					LOG_ERROR("response error: %d", ct);
				}
			}

			curl_easy_cleanup( curl );
		}
	}

	void cURLNetworkClient::end()
	{
		curl_global_cleanup();
	}

};
