#include<stdio.h>
#include<stdlib.h>
#include<time.h>

/*
*	simple program to encrypt a file using a substitution cipher
*	I know that disk encryption utilities are available
*	this program can secure a file whose contents I want to hide
*	simple substitution cipher is used (easily broken)
*/

////////////////////////////////////////////////////////////////////////////////

/*
*	function to analyse the bytes of a file
*	file will be read as a stream of bytes, whatever the format of the file is
*	frequency of each byte is stored in an array
*/
void analyse(FILE *stream)
{
	// bytes read from the file
	char unsigned read_byte;

	/*
	*	array to store frequencies of characers
	*	static arrays initialized to zeros
	*	number of occurrences of 'item' are 'frequency[item]''
	*/
	int frequency[256] = {0};

	// write to the frequency array
	while(fscanf(stream, "%c", &read_byte) == 1)
	{
		frequency[read_byte]++;
	}

	// display frequencies
	int count;
	printf("byte\tfrequency\n");
	for(count = 0; count <= 255; count++)
	{
		if(frequency[count])
		{
			printf("%*d\t\t", 3, count);
			printf("%*d\n", (int)sizeof(int), frequency[count]);
		}
	}

	return;
}

////////////////////////////////////////////////////////////////////////////////

/*
*	perform some operation on a character
*	this results in encryption of a character
*/
char unsigned forwards(char unsigned r)
{
	r = r << 7 | r >> 1;
	return r ^ 34;
}

////////////////////////////////////////////////////////////////////////////////

/*
*	reverse the operation performed by 'forwards'
*	results in decryption of a character
*	if 'forwards' is changed, remember to also change 'backwards'
*/
char unsigned backwards(char unsigned r)
{
	r ^= 34;
	return r >> 7 | r << 1;
}

////////////////////////////////////////////////////////////////////////////////

/*
*	perform 'forwards' on each character of 'stream'
*	write the encrypted characters to a new file
*/
void encrypt(FILE *stream)
{
	// generate a random string to name the encrypted file
	srand(time(NULL));
	char unsigned code_file[] = "--------.crypt8";
	int count;
	for(count = 0; count < 8; count++)
	{
		code_file[count] = rand() % 58 + 65;
	}

	// open a file of that name
	FILE *out;
	out = fopen(code_file, "w");

	// write encrypted bytes to this file
	char unsigned read_byte;
	while(fscanf(stream, "%c", &read_byte) == 1)
	{
		fprintf(out, "%c", forwards(read_byte));
	}

	fclose(out);

	return;
}

////////////////////////////////////////////////////////////////////////////////

/*
*	perform 'backwards' on each character of 'stream'
*	write the encrypted characters to a new file
*/
void decrypt(FILE *stream)
{
	// generate a random string to name the decrypted file
	srand(time(NULL));
	char unsigned decode_file[] = "--------";
	int count;
	for(count = 0; count < 8; count++)
	{
		decode_file[count] = rand() % 58 + 65;
	}

	// open a file of that name
	FILE *out;
	out = fopen(decode_file, "w");

	// write decrypted bytes to this file
	char unsigned read_byte;
	while(fscanf(stream, "%c", &read_byte) == 1)
	{
		fprintf(out, "%c", backwards(read_byte));
	}

	fclose(out);

	return;
}

////////////////////////////////////////////////////////////////////////////////

// main
int main(int const argc, char const **argv)
{
	// check arguments
	if(argc < 3)
	{
		printf("usage:\n");
		printf("\t./stream_analysis.out <file name> <code>\n");
		printf("<code> must be one of:\n");
		printf("\ta (analyse)\n");
		printf("\te (encrypt)\n");
		printf("\td (decrypt)\n");
		return 1;
	}

	// try to open file
	FILE *stream;
	stream = fopen(argv[1], "r");
	if(!stream)
	{
		printf("Could not open \'%s\' for reading.\n", argv[1]);
		return 2;
	}

	// validate code
	switch(argv[2][0])
	{
		case 'a':
		{
			analyse(stream);
			break;
		}
		case 'e':
		{
			encrypt(stream);
			break;
		}
		case 'd':
		{
			decrypt(stream);
			break;
		}
		default:
		{
			printf("Invalid code \'%c\' given.\n", argv[2][0]);
			return 3;
		}
	}

	fclose(stream);

	return 0;
}
