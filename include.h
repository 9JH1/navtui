/** 
 * @file include.h 
 * @brief contains all function definitions
 *
 * This file includes every function, external variable, structure and enumerator 
 * used through out all of the code. this function is imported to most child files 
 * which means that the includes can be moderated very easily.
 **/
#ifndef INCLUDE 
#define INCLUDE 

// Third party
#include "src/remote/plib/plib.h"
#include "src/platform.h"
#include "src/json/cjson.h"
#include <curl/curl.h>

#include <gcrypt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#define MD5_DIGEST_LENGTH 16
#define SUBSONIC_SALT_LEN 7

/** 
 * @brief structure used in curl response fetching 
 * @param size amount of bytes in response 
 * @param memory malloc'd string response data. 
 **/
struct url_data {
  	size_t size;
    char *memory;
};

/**
 * @brief local config file 
 **/
typedef struct {
	char *username;
	char *password;
	char *server_url;
	char *version;
	char *name;
	
	char *url_end;
	int is_authed;
} auth_config;

/**
 * @brief Return codes for main program
 **/
typedef enum {
	SUCCESS = 0,
	FAIL = 1,
	HELP_MENU_REF = 2,
} return_codes;



/**
 * @brief Custom exit function
 * @param code Exit code
 **/
void quit(int code);

/**
 * @brief Custom help function, mostly just a @ref pl_help wrapper
 **/ 
void help(void);


static size_t write_callback(void *, size_t, size_t, void *);
char* fetch_url_content(const char*);
char random_char(void);
char *md5(const char *);
char *generate_random_string(int);
int auth_subsonic(auth_config *);
char *get_full_url(auth_config *,char *); 
#endif
