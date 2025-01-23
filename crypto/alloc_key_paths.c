#include <stdlib.h>
#include <string.h>

#define PUB_FILENAME "key_pub.pem"
#define PRI_FILENAME "key.pem"

/**
 * _alloc_key_paths - Allocates memory and builds the key file paths.
 * @folder: The folder path where the keys are saved.
 * @pub_path: Pointer to the public key path (output).
 * @pri_path: Pointer to the private key path (output).
 *
 * Return: 0 on success, -1 on failure.
 */
int _alloc_key_paths(const char *folder, char **pub_path, char **pri_path)
{
    if (!folder || !pub_path || !pri_path)
        return (-1);

    /* Allocate memory for public key path */
    *pub_path = calloc(1, strlen(folder) + strlen("/" PUB_FILENAME) + 1);
    if (!*pub_path)
        return (-1);
    strcpy(*pub_path, folder);
    strcat(*pub_path, "/" PUB_FILENAME);

    /* Allocate memory for private key path */
    *pri_path = calloc(1, strlen(folder) + strlen("/" PRI_FILENAME) + 1);
    if (!*pri_path)
    {
        free(*pub_path);
        return (-1);
    }
    strcpy(*pri_path, folder);
    strcat(*pri_path, "/" PRI_FILENAME);

    return (0);
}
