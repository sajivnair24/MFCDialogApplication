#include "stdafx.h"
#include "HttpRequest.h"

HttpRequest::HttpRequest()
{
}

HttpRequest::HttpRequest(HttpRequestParameters * parameters)
{
	params = parameters;
}

HttpRequest::~HttpRequest()
{
	delete params;
}

std::string HttpRequest::getMethod()
{
	return params->method;
}

std::string HttpRequest::getUrl()
{
	return params->url;
}

std::map <std::string, std::string> HttpRequest::getParameters()
{
	return params->parameters;
}

