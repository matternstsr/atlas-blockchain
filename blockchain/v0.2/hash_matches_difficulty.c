#include <stdint.h>
#include <stdio.h>

#define SHA256_DIGEST_LENGTH 32

int hash_matches_difficulty(uint8_t const hash[SHA256_DIGEST_LENGTH],
							uint32_t difficulty)
{
	uint32_t bit_count = 0;
	uint32_t byte_index, bit_index;

	/* Iterate through each byte in the hash */
	for (byte_index = 0; byte_index < SHA256_DIGEST_LENGTH; byte_index++)
	{
		/* Iterate through each bit in the byte */
		for (bit_index = 7; bit_index > 0; bit_index--)
		{
			/* Check if the current bit is 0 */
			if ((hash[byte_index] >> bit_index) & 1)
				return (bit_count >= difficulty);
			bit_count++;
		}
	}
	return (bit_count >= difficulty);
}
