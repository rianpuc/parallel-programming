#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

static unsigned char scattered[64 * 64];
unsigned char source[64 * 64];
static unsigned char gathered[64];
static unsigned long quotients[64];
static volatile unsigned int observed[64];
static unsigned char even_indices[64];
static unsigned char odd_indices[64];

int main(int argc, char **argv)
{
	size_t count = argc > 1 ? (size_t)strtoull(argv[1], NULL, 10) : 64;

	if (count > 64)
		count = 64;

	unsigned long divisor = count + 1;
	int mode = argc;

	for (size_t i = 0; i < count; ++i) {
		switch (mode) {
		case 1:
			gathered[i] = source[i * i];
			break;
		case 2:
			scattered[i * i] = (unsigned char)i;
			break;
		case 3:
			quotients[i] = ((unsigned long)i * 7 + 1) / divisor;
			break;
		case 4:
			observed[i] = (unsigned int)i;
			break;
		case 5:
			if ((i & 1U) == 0)
				even_indices[i] = (unsigned char)i;
			else
				odd_indices[i] = (unsigned char)i;
			break;
		default:
			break;
		}
	}

	(void)fwrite(scattered, sizeof scattered[0], sizeof scattered, stdout);
	(void)fwrite(gathered, sizeof gathered[0], count, stdout);
	(void)fwrite(quotients, sizeof quotients[0], count, stdout);
	(void)fwrite(even_indices, sizeof even_indices[0], count, stdout);
	(void)fwrite(odd_indices, sizeof odd_indices[0], count, stdout);
	return 0;
}

/* GCC 16.2.1:
 * comando: gcc -O3 -fopt-info-vec-missed devec.c -o devec
 *
 * devec.c:23:23: missed: not vectorized: unsupported control flow in loop.
 *   O caso 5 escolhe entre dois stores conforme a paridade de i.
 *
 * devec.c:35:16: missed: not vectorized: volatile type: observed[i_85] ={v} _91;
 *   O store em observed[i] e volatile.
 *
 * devec.c:32:17: missed: not vectorized: no vectype for stmt: quotients[i_65] = _75;
 *   scalar_type: long unsigned int
 *   O GCC nao encontrou tipo vetorial para o store em quotients[i].
 *
 * devec.c:26:24: missed: not vectorized: not suitable for gather load
 *   _4 = source[powmult_43];
 *   source[i * i] le enderecos nao contiguos.
 *
 * devec.c:29:21: missed: not vectorized: not suitable for scatter store
 *   scattered[powmult_57] = _58;
 *   scattered[i * i] grava em enderecos nao contiguos.
 *
 */
