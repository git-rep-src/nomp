#ifndef SSL_SOCKET_H
#define SSL_SOCKET_H

#include <string>

#include <openssl/ssl.h>
#include <openssl/bio.h>

using std::string;

class SSL_socket
{
public:
    SSL_socket();
    ~SSL_socket();
    
    bool is_started;

    bool start(string host, string port);
    bool write_read(const string *cmd, string *ret);

private:
    SSL_CTX *ctx;
    BIO *bio;
    SSL *ssl;

    void cleanup();
};

#endif
