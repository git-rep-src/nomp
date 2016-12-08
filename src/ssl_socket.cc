#include "ssl_socket.h"

SSL_socket::SSL_socket() :
    is_started(false),
    ctx(NULL),
    bio(NULL),
    ssl(NULL)
{
}

SSL_socket::~SSL_socket()
{
    cleanup();
}

bool SSL_socket::start(string host, string port)
{
    if ((ctx != NULL) && (bio != NULL))
        cleanup();
    
    OpenSSL_add_all_algorithms();
    
    if (SSL_library_init() < 0)
        return false;
    if ((ctx = SSL_CTX_new(TLSv1_2_client_method())) == NULL)
        return false;

    bio = BIO_new_ssl_connect(ctx);
    BIO_get_ssl(bio, &ssl);
    if (ssl == NULL)
        return false;
    
    SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);
    BIO_set_conn_hostname(bio, (host + ":" + port).c_str());

    if (BIO_do_connect(bio) <= 0)
        return false;
    if (BIO_do_handshake(bio) <= 0)
        return false;
    
    is_started = true;
    
    return true;
}
 
bool SSL_socket::write_read(const string *cmd, string *ret)
{
    char buf[1024];
    
    if (BIO_write(bio, (*cmd).c_str(), strlen((*cmd).c_str())) > 0) {
        for(;;) {
            int bytes = BIO_read(bio, buf, 1024);
            buf[bytes] = 0;
            *ret += buf;
            if ((bytes < 1024) && !BIO_should_retry(bio))
                break;
        }
    } else {
        return false;
    }

    return true;
}

void SSL_socket::cleanup()
{
    SSL_CTX_free(ctx);
    BIO_free_all(bio);
    
    ctx = NULL;
    bio = NULL;
    ssl = NULL;
}
