#include <stdio.h>
#include <string.h>
#include "socket.h"
#include "main.h"
#include "gestion_requete.h"
#include "client.h"
#include "signaux.h"
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "stats.h"
#include <sys/mman.h>
#include "mimes.h"


char *getExtension(char *url){

    char * tmp;
    while((tmp=strchr(url, '.'))!=NULL){
    	tmp++;
        url=tmp;
    }
    
    return url;
}

char *getMineType(char * url){
    //tableau contenant 58 extensions de fichier et le type mime qui leur correspond (mime_correspondance) ( by Alex cuvillier <3)
    MimeCorrespondance mimeCorrespondances[58]={{"application/java-archive", "jar"}, {"application/java-vm", "class"}, {"application/javascript", "js"}, {"application/json", "json"}, {"application/octet-stream", "bin"}, {"application/pdf", "pdf"}, {"application/rar", "rar"}, {"application/xml", "xml"}, {"application/xml", "xsl"}, {"application/xml", "xsd"}, {"application/zip", "zip"}, {"application/vnd.ms-excel", "xls"}, {"application/vnd.ms-excel", "xlb"}, {"application/vnd.ms-excel", "xlt"}, {"application/vnd.ms-powerpoint", "ppt"}, {"application/vnd.ms-powerpoint", "pps"}, {"application/x-7z-compressed", "7z"}, {"application/x-bittorrent", "torrent"}, {"application/x-debian-package", "deb"}, {"application/x-debian-package", "udeb"}, {"application/x-dvi", "dvi"}, {"application/x-gtar-compressed", "tgz"}, {"application/x-gtar-compressed", "taz"}, {"application/x-iso9660-image", "iso"}, {"application/x-md5", "md5"}, {"application/x-msdos-program", "exe"}, {"application/x-msdos-program", "bat"}, {"application/x-msdos-program", "dll"}, {"application/x-redhat-package-manager", "rpm"}, {"application/x-ruby", "rb"}, {"application/x-sh", "sh"}, {"application/x-sql", "sql"}, {"application/x-tar", "tar"}, {"audio/flac", "flac"}, {"audio/mpeg", "mp3"}, {"audio/x-ms-wma", "wma"}, {"audio/x-wav", "wav"}, {"image/gif", "gif"}, {"image/jpeg", "jpeg"}, {"image/jpeg", "jpg"}, {"image/jpeg", "jpe"}, {"image/png", "png"}, {"text/css", "css"}, {"text/csv", "csv"}, {"text/x-chdr", "h"}, {"text/x-csrc", "c"}, {"text/x-java", "java"}, {"text/x-sh", "sh"}, {"text/x-perl", "pl"}, {"text/x-perl", "pm"}, {"video/mpeg", "mpeg"}, {"video/mp4", "mp4"}, {"video/x-msvideo", "avi"}, {"video/x-matroska", "mkv"}, {"application/vnd.oasis.opendocument.text", "odt"}, {"application/vnd.oasis.opendocument.presentation", "odp"}, {"text/html", "html"}, {"text/plain", "txt"}};
    int i;
    char *extension;
    int taille=sizeof(mimeCorrespondances)/sizeof(MimeCorrespondance);
	extension=getExtension(url);

   	
    for(i=0; i<taille; i++){
        if(strcmp(extension, mimeCorrespondances[i].extension)==0){
            //renvoyer le type mime correspondant
            return mimeCorrespondances[i].mimeType;
        }
    }
    //si l'extension du fichier demande ne correspond a aucune traitee par le serveur
    return "text/plain";
}

