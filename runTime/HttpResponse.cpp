#include "HttpResponse.hpp"
#include <sstream>

HttpResponse::HttpResponse(void) {}

HttpResponse::HttpResponse(HttpResponse const &cpy) {}

HttpResponse::~HttpResponse(void) {}

HttpResponse &HttpResponse::operator=(HttpResponse const &rhs) {}

std::string HttpResponse::createSerializedResponse(void) const {
    std::string ret;

    /*
     * Response	=	Status-Line               ; Section 6.1
                    *(( general-header        ; Section 4.5
                    | response-header        ; Section 6.
                    | entity-header ) CRLF)  ; Section 7.
                    CRLF
                    [ message-body ]          ; Section 7.2

     *
     */
    // Status line
    ret = this->createStatusLine();
    // General header
    ret += "Connection: close\r\n";
    ret += // TODO: ler o rfc amanhã e pegar daqui.
}

std::string HttpResponse::createStatusLine(void) const {
    std::stringstream ss;

    ss << this->_status_code;
    return (std::string("HTTP/1.1 " + ss.str() + "" +
                        this->getReasonPhrase(this->_status_code) + "\r\n"));
}

std::string HttpResponse::getServer(void) const {
    return ("WebserDosMaluco/1.0");
}

std::string HttpResponse::getReasonPhrase(int statusCode) {
    switch (statusCode) {
    case (100):
        return ("Continue");
    case (101):
        return ("Switching Protocols");
    case (200):
        return ("OK");
    case (201):
        return ("Created");
    case (202):
        return ("Accepted");
    case (203):
        return ("Non-Authoritative Information");
    case (204):
        return ("No Content");
    case (205):
        return ("Reset Content");
    case (206):
        return ("Partial Content");
    case (300):
        return ("Multiple Choices");
    case (301):
        return ("Moved Permanently");
    case (302):
        return ("Found");
    case (303):
        return ("See Other");
    case (304):
        return ("Not Modified");
    case (305):
        return ("Use Proxy");
    case (307):
        return ("Temporary Redirect");
    case (400):
        return ("Bad Request");
    case (401):
        return ("Unauthorized");
    case (402):
        return ("Payment Required");
    case (403):
        return ("Forbidden");
    case (404):
        return ("Not Found");
    case (405):
        return ("Method Not Allowed");
    case (406):
        return ("Not Acceptable");
    case (407):
        return ("Proxy Authentication Required");
    case (408):
        return ("Request Timeout");
    case (409):
        return ("Conflict");
    case (410):
        return ("Gone");
    case (411):
        return ("Length Required");
    case (412):
        return ("Precondition Failed");
    case (413):
        return ("Payload Too Large");
    case (414):
        return ("URI Too Long");
    case (415):
        return ("Unsupported Media Type");
    case (416):
        return ("Range Not Satisfiable");
    case (417):
        return ("Expectation Failed");
    case (426):
        return ("Upgrade Required");
    case (500):
        return ("Internal Server Error");
    case (501):
        return ("Not Implemented");
    case (502):
        return ("Bad Gateway");
    case (503):
        return ("Service Unavailable");
    case (504):
        return ("Gateway Timeout");
    case (505):
        return ("HTTP Version Not Supported");
    default:
        return ("Unknown Status Code");
    }
}
