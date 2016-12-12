#pragma once
#include <string>
#include <map>

struct HttpRequestParameters
{
	std::string method;
	std::string url;
	int         port;
	std::map <std::string, std::string> parameters;
};


class HttpRequest
{
	HttpRequestParameters* params;

public:
	HttpRequest::HttpRequest();
	HttpRequest(HttpRequestParameters* parameters);
	~HttpRequest();

	std::string getMethod();
	std::string getUrl();

	std::map <std::string, std::string> getParameters();

};

