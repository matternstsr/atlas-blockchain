#include <stdint.h>
#include <stdio.h>

#define SHA256_DIGEST_LENGTH 32  /* 256 bits = 32 bytes */

/**
 * hash_matches_difficulty - checks if the hash meets the specified difficulty
 * @hash: the SHA256 hash to check
 * @difficulty: the required number of leading zeros in the hash
 * Return: 1 if hash meets or exceeds the difficulty, 0 otherwise
 */

int hash_matches_difficulty(uint8_t const hash[SHA256_DIGEST_LENGTH],
							uint32_t difficulty)
{
	uint32_t bit_count = 0;
	uint32_t byte_index;
	int bit_index;  /* Use signed int for bit_index */

	/* Iterate through each byte in hash */
	for (byte_index = 0; byte_index < SHA256_DIGEST_LENGTH; byte_index++)
	{
		/* Iterate through each bit in byte */
		for (bit_index = 7; bit_index >= 0; bit_index--)
		{
			/* Check if current bit is 0 */
			if ((hash[byte_index] >> bit_index) & 1)
				/* If bit is 1, stop counting leading zeros */
				return (bit_count >= difficulty);
			/* If bit is 0, increment bit_count */
			bit_count++;
		}
	}
	/* After loop, check if total leading zeros are enough to meet difficulty */
	return (bit_count >= difficulty);
}
