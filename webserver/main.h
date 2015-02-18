

enum http_method {
	HTTP_GET ,
	HTTP_UNSUPPORTED ,
};
typedef struct{
	enum http_method method ;
	int major_version ;
	int minor_version ;
	char * url ;
} http_request ;



void gestion_client(int socket_client);
void initSignaux();
void traitement_signal (int sig );
int ligneVide(char * str);
char * fgets_or_exit ( char * buffer , int size , FILE * stream );
int parse_http_request ( const char * request_line , http_request * request );

