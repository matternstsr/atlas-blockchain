#include "blockchain.h"

/**
 * hash_matches_difficulty - Checks if a provided hash matches a difficulty
 * @hash: hash to check
 * @difficulty: Difficulty of block to match to
 * Return: 1 on match, 0 on fail
 */
int hash_matches_difficulty(uint8_t const hash[SHA256_DIGEST_LENGTH],
							uint32_t difficulty)
{
	uint8_t i = 0, count = 0, msb = 128;
	size_t j = 0;

	if (!hash)
		return (0);

	for (; i < SHA256_DIGEST_LENGTH; i++)
	{
		if (hash[i] == '\000')
			count += 8;
		else
			break;
	}
	for (; j < (sizeof(uint8_t) * 8); j++)
	{
		if ((hash[i] << j) & msb)
			break;
		count++;
	}
	return (count >= difficulty);
}
