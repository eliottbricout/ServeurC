int parse_http_request ( const char * request_line , http_request * request );
int ligneVide(char * str);
char *fgets_or_exit ( char *buffer , int size , FILE *stream );
void skip_headers ( FILE * client );
void send_status ( FILE * client , int code , const char * reason_phrase );
void send_response ( FILE * client , int code , const char * reason_phrase ,const char * message_body );
char * rewrite_url ( char * url );
int check_and_open ( const char * url , const char * document_root );
int get_file_size(int fd);
void get_file_size(int fd)
int copy(int in,File* out);
