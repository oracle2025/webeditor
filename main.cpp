#include <iostream>
#include "http.h"
#include "happyhttp.h"

const char* uri = "http://www.richardspindler.com/2017/02/25/modern-software-development-or-slow-as-molasses/";
const char* host = "www.richardspindler.com";
const char* path = "/2017/02/25/modern-software-development-or-slow-as-molasses/";

static int count=0;

// invoked when response headers have been received
void OnBegin( const happyhttp::Response* r, void* userdata )
{
    printf( "BEGIN (%d %s)\n", r->getstatus(), r->getreason() );
    count = 0;
}

// invoked to process response body data (may be called multiple times)
void OnData( const happyhttp::Response* r, void* userdata, const unsigned char* data, int n )
{
    fwrite( data,1,n, stdout );
    count += n;
}

// invoked when response is complete
void OnComplete( const happyhttp::Response* r, void* userdata )
{
    printf( "COMPLETE (%d bytes)\n", count );
}


void TestGET()
{
    //happyhttp::Connection conn( "www.scumways.com", 80 );
    happyhttp::Connection conn( host, 80 );
    conn.setcallbacks( OnBegin, OnData, OnComplete, 0 );

    conn.request( "GET", path );

    while( conn.outstanding() )
        conn.pump();
}

struct result
{
	std::string content;
	int count;
};
void gOnBegin( const happyhttp::Response* r, void* userdata )
{
	result *my_result = static_cast<result*>(userdata);
    //printf( "BEGIN (%d %s)\n", r->getstatus(), r->getreason() );
    my_result->count = 0;
}

// invoked to process response body data (may be called multiple times)
void gOnData( const happyhttp::Response* r, void* userdata, const unsigned char* data, int n )
{
	result *my_result = static_cast<result*>(userdata);
	std::string str((const char*)data, n);
	my_result->content += str;
    //fwrite( data,1,n, stdout );
    my_result->count += n;
}

// invoked when response is complete
void gOnComplete( const happyhttp::Response* r, void* userdata )
{
	result *my_result = static_cast<result*>(userdata);
    //printf( "COMPLETE (%d bytes)\n", count );
}

std::string get(const std::string &host, const std::string &path)
{
	result my_result;

    happyhttp::Connection conn( host.c_str(), 80 );
    conn.setcallbacks( gOnBegin, gOnData, gOnComplete, &my_result );

    conn.request( "GET", path.c_str() );

    while( conn.outstanding() )
        conn.pump();

	return my_result.content;
}

int main(int argc, const char *argv[])
{
	std::cout << "Hello HTTP" << std::endl;
	//TestGET();
	std::cout << get("oracle.bunfet.me", "/") << std::endl;
	return 0;
}

