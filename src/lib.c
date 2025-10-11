/**
 * @file lib.c 
 * @breif this file contains all of the function definitions from in 
 *        @ref include.h.
 **/
#include "../include.h"



void quit(int code){
	printf("\rexiting db with code %d..\n",code);
	exit(code);
}


void help(){
	printf("db [options]\n");
	pl_help();
}
static size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct url_data *mem = (struct url_data*)userp;

    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    if (ptr == NULL) {
        // Out of memory
        fprintf(stderr, "not enough memory (realloc returned NULL)\n");
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0; // Null-terminate the string

    return realsize;
}

// Function to fetch URL content using libcurl
char* fetch_url_content(const char* url) {
    CURL *curl_handle;
    CURLcode res;
    struct url_data chunk;

    chunk.memory = malloc(1); // Start with a 1-byte buffer
    chunk.size = 0;           // No data yet

    curl_global_init(CURL_GLOBAL_ALL);

    // Initialize a curl session
    curl_handle = curl_easy_init();
    if (curl_handle == NULL) {
        fprintf(stderr, "curl_easy_init() failed\n");
        free(chunk.memory);
        curl_global_cleanup();
        return NULL;
    }

    // Set the URL
    curl_easy_setopt(curl_handle, CURLOPT_URL, url);
    curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);

    res = curl_easy_perform(curl_handle);
    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        free(chunk.memory);
        chunk.memory = NULL; // Indicate failure
    }

    // Clean up
    curl_easy_cleanup(curl_handle);
    curl_global_cleanup();

    return chunk.memory; // Returns the dynamically allocated string, caller must free it
}


char random_char(void) {
    static int seeded = 0;
    if (!seeded) {
        srand(time(NULL));
        seeded = 1;
    }

    // Define URL-safe characters: A-Z, a-z, 0-9, -, ., _, ~
    const char *url_safe_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~";
    int len = strlen(url_safe_chars); // 66 characters

    return url_safe_chars[rand() % len];
}


char *md5(const char *data) {
  if (!data) {
    return NULL;
  }

  gcry_md_hd_t md_handle;
  gcry_error_t err;
  char *hex_hash;

  if (!gcry_check_version(GCRYPT_VERSION)) {
    fprintf(stderr, "Libgcrypt version mismatch\n");
    return NULL;
  }
  gcry_control(GCRYCTL_INITIALIZATION_FINISHED, 0);

  err = gcry_md_open(&md_handle, GCRY_MD_MD5, 0);
  if (err) {
    fprintf(stderr, "Failed to open MD5 handle: %s\n", gcry_strerror(err));
    return NULL;
  }

  gcry_md_write(md_handle, data, strlen(data));

  unsigned char *digest = gcry_md_read(md_handle, GCRY_MD_MD5);
  if (!digest) {
    gcry_md_close(md_handle);
    return NULL;
  }

  hex_hash = malloc((MD5_DIGEST_LENGTH * 2) + 1);
  if (!hex_hash) {
    gcry_md_close(md_handle);
    return NULL;
  }

  for (size_t i = 0; i < MD5_DIGEST_LENGTH; i++) {
    sprintf(hex_hash + (i * 2), "%02x", digest[i]);
  }

  gcry_md_close(md_handle);

  return hex_hash;
}

char* generate_random_string(int len){
	char *out = malloc(len * sizeof(char));

	for(int i = 0;i < len;i++)
		out[i] = random_char();

	return out;
}

int auth_subsonic(auth_config *in){
	char *salt = generate_random_string(SUBSONIC_SALT_LEN);
	
	// combine the salt and password
	char combined[SUBSONIC_SALT_LEN + strlen(in->password) + 1];
	sprintf(combined,"%s%s",in->password,salt);	
	char *md5_password = md5(combined);

	// combine it all 
	size_t size = snprintf(NULL ,0 ,"?u=%s&t=%s&s=%s&v=%s&c=%s&f=json",
			in->username,
			md5_password,
			salt,
			in->version,
			in->name);

	char *out = malloc(size * sizeof(char) + 1);
	sprintf(out,"?u=%s&t=%s&s=%s&v=%s&c=%s&f=json",
			in->username,
			md5_password,
			salt,
			in->version,
			in->name);

	in->url_end = out;

	free(salt);
	free(md5_password);

	in->is_authed = 1;
		return 0;
}


char *get_full_url(auth_config * config,char * path){
	size_t size = snprintf(NULL,0,"%s%s%s",
			config->server_url,
			path,
			config->url_end);
	
	char *out =  malloc(size * sizeof(char));
	sprintf(out,"%s%s%s",
			config->server_url,
			path,
			config->url_end);

	return out;
}
