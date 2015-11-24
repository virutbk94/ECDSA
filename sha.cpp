#include<fstream>
#include<cstdlib>
#include <openssl/sha.h>

using namespace std;

bool sha_256(char *path,char* outputBuffer)
{
    FILE *file = fopen(path, "rb");
    if(!file) return false;

    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    const int bufSize = 32768;
    unsigned char *buffer = (unsigned char *)malloc(bufSize);
    int bytesRead = 0;
    if(!buffer) return false;
    while((bytesRead = fread(buffer, 1, bufSize, file)))
    {
        SHA256_Update(&sha256, buffer, bytesRead);
    }
    SHA256_Final(hash, &sha256);

    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        sprintf(outputBuffer + (i * 2), "%02x", hash[i]);
    }
    outputBuffer[64] = '\0';
    fclose(file);
    free(buffer);
    return true;
}


#include<fstream>
#include<cstdlib>
#include <openssl/sha.h>

using namespace std;

bool sha_256(char *path,char* outputBuffer)
{
    FILE *file = fopen(path, "rb");
    if(!file) return false;

    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    const int bufSize = 32768;
    unsigned char *buffer = (unsigned char *)malloc(bufSize);
    int bytesRead = 0;
    if(!buffer) return false;
    while((bytesRead = fread(buffer, 1, bufSize, file)))
    {
        SHA256_Update(&sha256, buffer, bytesRead);
    }
    SHA256_Final(hash, &sha256);

    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        sprintf(outputBuffer + (i * 2), "%02x", hash[i]);
    }
    outputBuffer[64] = '\0';
    fclose(file);
    free(buffer);
    return true;
}

