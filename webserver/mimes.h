typedef struct{
    char * mimeType;
    char * extension;
}MimeCorrespondance;

char *getExtension(char * url);
char *getMineType(char * url);
