#include <stdio.h>
#include <stdlib.h>
#include "simple_crypto.h"
#include <string.h>
#include <assert.h>


/*
 *				SIMPLE_CRYPTO.C          
 *  String manipulation functions and encryption algorithms.
 *
 *	 ------> @look README.txt for more info.
 *  ------> @look "simple_crypto.h"
 */












/*
 * This function is similar to the already defined strlen func in @<string.h> library.
 * It has been defined here for testing purposes and more accessibility.
 */
int charArraySize(char * str) 
{
	int i;
	if(str==NULL)
		return 0;
	for (i=0;  str[i]!='\0' && str[i]!='\n'; i++);

	return i;
}


/*
 * Simple function that prints a String by iterating through each character in the character array.
 */
void printString(char* text)
{
	int i;
	
	for(i=0; i<charArraySize(text); i++)
	{
		fprintf(stderr,"%c", text[i]);
	}
	fprintf(stderr,"\n");
}


/*
 * This function is focused on deleting a specific character from an character Array at a specific index.
 * The way it is implemented it reduces the size of that Array by 1 whenever it is called.
 *
 * @arg ---> Pointer at character. Containing a defined amount of memory space allocated to a String Array.
 * @arg ---> Integer value defining the index. **Function ensures case of "OutOfBounds_Exception".
 *
 *
 * @used ---> By filter input functions.
 * @used ---> By string manipulation functions.
 */
void delete_character(char* text, int index) 
{
	int i;
	int size = charArraySize(text);

	if(index < 0 || index >= size)
	{
		fprintf(stderr,"Error: index out of bounds");
		return;
	}

	for(i=index;i<size-1; i++)
	{
		text[i]=text[i+1];

	}
	text[size-1]='\0';
}


/*
 * This function is focused on shifting Leftwise the elements of a character Aray by a defined amount.
 *
 * @notice The shifted character is not lost but put in the end position(tail) of the array.
 * This function is implemented recursively.
 *
 * @arg ---> Pointer at character. The array to be shifted. Containing a defined amount of memory space allocated to a String Array.
 * @arg ---> Integer defining the part size of the array thats is going to be shifted. **MUST BE POSITIVE**.
 * @arg ---> Integer defining the amount of shifting to be done. **MUST BE POSITIVE**.
 *
 * 
 *	@used By creating the 2D shifted Alphabet table for the Vigenere' Cipher algorithm.
 *
 */
void shift_character(char* text, int n, int shift)
{
	if(n<=0 || shift <0) return;
   if (shift == 0) return; 
 
   char c = text[0]; 
 
   for(int i=1 ; i <n ; i++) 
      text[i-1] = text[i]; 
 
   text[n-1] = c; 
 
   shift_character(text, n, shift-1);

}


/*
 * Function that implements the filtering of the special characters in a String.
 *
 * @arg ---> Pointer at character. The array to be filtered. Containing a defined amount of memory space allocated to a String Array.
 *
 * Iteratively traversed, a character Array is being broken down to a character values and fixed accordingily.
 * Function ensures array size correction by inserting the '\0' character at the end of it.
 *
 * @used By read string input for OTP and Caesars Encryption algorithms.
 * 
 */
char* filter_special_characters(char* text)
{
	int size = charArraySize(text);
	int i, temp;
	char ch;

	for(i=0; i<size; i++)
	{
		ch = text[i];
		if(ch < 48 || (ch>57 && ch<65) || (ch>90 && ch<97) || ch > 122)
		{
			delete_character(text, i--);
			size--;
		} 

 	}

 	return text;
}


/*
 * Function that implements the filtering of any character except Capitals. ONLY CAPITALS ALLOWED!.
 * 
 * @arg ---> Pointer at character. The array to be filtered. Containing a defined amount of memory space allocated to a String Array.
 *
 * Iteratively traversed, a character Array is being broken down to a character values and fixed accordingily.
 * Function ensures array size correction by inserting the '\0' character at the end of it.
 *
 * @used by read string input for Vigenere's Cipher Encryption algorithm.
 *
 */
char* filter_CAPS_ONLY(char* text)
{
	int size = charArraySize(text);
	int i, temp;
	char ch;

	for(i=0; i<size; i++)
	{
		ch = text[i];
		if(ch<65 || ch>90)
		{
			delete_character(text, i--);
			size--;
		} 

 	}

 	return text;
}



/*
 * Function implemented to create a suitable key for the Viegenere encryption algorithm. It can be used also to 
 *
 *
 */
char* key_Correction(char** txt, int size)
{
	char* text = *txt;
	char* key;
	int i,j=0;
	int txt_size = charArraySize(text);



	key = malloc( size*sizeof(char) );

	if(key==NULL)
	{
		fprintf(stderr, "Error: Memory allocation has failed.\n");
		exit(EXIT_FAILURE);
	}

	for(i=0; i<size; i++)
	{


		if(j >= txt_size)
			j=0;
		key[i] = text[j];
	
	
		j++;
	}

	key[size] = '\0';



	return key;
}

int charAt(char* text, int ch)
{
	int size = charArraySize(text);
	int i;


	for(i=0; i<size; i++)
	{
		if(text[i] == ch)
			return i;
	}

	return -1;
}

void copyString(char* destination, char source[ROWS][COL],int r,int c)
{
	int i;
	int size = charArraySize(destination);

	for(i=0; i<size; i++)
	{
		destination[i] = source[r][i];
	}

}


/*
 * 
 *
 *
 */
char *read_PlainTextInput(int filter_lvl)
{
	char buffer[MAX_BUFFER];
	char* inp;

	
	fprintf(stderr, "Enter input: ");
	scanf("%s", buffer);

	if(charArraySize(buffer)==0)
		fprintf(stderr,"Input empty. Try again.\n");
	
		 


	inp = (char*)malloc(sizeof(char)*4);

	switch(filter_lvl)
	{
		case 0:
			strcpy(inp, filter_special_characters(buffer));
			break;
		case 1:
			strcpy(inp, filter_CAPS_ONLY(buffer));
			break;
		default:
			fprintf(stderr, "\nInvalid filter code.\n");
			exit(EXIT_FAILURE);
	}
	

	return inp;
}

int* read_Caesars_shiftCode()
{
	int code=0;
	int* p_code;

	p_code = &code;

	while(code<=0)
	{
		fprintf(stderr, "Enter code: ");
		scanf("%d", &code);
		if(code <= 0)
			fprintf(stderr, "\nInvalid code. Try again.\n");
		if(code > 26)
			code = code %26;

	}
	return p_code;

}

char* generateOTPKeys(int N)
{
	FILE* fp;
	int i=0;
	char c;
	char* temp;

	temp = malloc(sizeof(char)*N);

	if(temp==NULL)
	{
		fprintf(stderr, "Error: Memory allocation has failed.\n");
		exit(EXIT_FAILURE);
	}

	fp = fopen("/dev/urandom", "r");

	if(fp == NULL)
	{
		fprintf(stderr,"Error: Could not open file.\n");
		exit(EXIT_FAILURE);
	}

	while(i < N) 
	{
		c = fgetc(fp);
		if( feof(fp)){
			break;
		}

		temp[i] = c;

		i++;
	}

	fclose(fp);

	return temp;

}


void free_keys(char* dp){

	free(dp);
}

int verify_printability(char ch)
{
	if(ch < 33 || ch > 126)
		return -1;
	else
		return 1;
}

char reconfigure_key()
{
	char ch;
	FILE* fp;

	fp = fopen("/dev/urandom", "r");

	if(fp == NULL)
	{
		fprintf(stderr,"Error: Could not open file.\n");
		//exit(EXIT_FAILURE);
		return 0;
	}

	ch = fgetc(fp);

	fclose(fp);

	return ch;

}

char *encryption_OTP(char** inp, char** k)
{	
	int i;
	char ch, new_key;
	char* encrypted;
	char* key;
	char* input;

	input = *inp;
	key = *k;

	int size = charArraySize(input);

 
	encrypted = malloc( size*sizeof(char) );

	if(encrypted==NULL)
	{
		fprintf(stderr, "Error: Memory allocation has failed.\n");
		exit(EXIT_FAILURE);
	}


	for(i=0; i<size; i++)
	{
		ch = input[i] ^ key[i];
	
		while(verify_printability(ch) == -1)
		{
			new_key = reconfigure_key();
			ch = new_key ^ input[i];
			if(verify_printability(ch) == 1)
			{
				key[i] = new_key;
				
			}

		}
		encrypted[i] = ch;

	}
	if(charArraySize(encrypted)>=size)
		encrypted[size] = '\0';


	return encrypted;

}

char *decryption_OTP(char** ciph, char** k)
{

	int i;
	char ch;
	char* cipher = *ciph;
	char* key = *k;

	int size=charArraySize(key);		
	char* decrypted = malloc( sizeof(char)*size );

	if(decrypted==NULL)
	{
		fprintf(stderr, "Error: Memory allocation has failed.\n");
		exit(EXIT_FAILURE);
	}


	for (i=0; i<size; i++)
	{
		ch = cipher[i]^key[i];
		decrypted[i]=ch;
	}

	if(charArraySize(decrypted)>=size)
		decrypted[size] = '\0';

	return decrypted;

}

char *encryption_Caesars(char** inp, int* cd)
{
	char* input;
	char* encrypted;
	char temp;

	input = *inp;

	int size= charArraySize(input);
	int code = *cd;
	int i, offset;

	encrypted = malloc( size*sizeof(char));

	if(encrypted==NULL)
	{
		fprintf(stderr, "Error: Memory allocation has failed.\n");
		exit(EXIT_FAILURE);
	}


	for (i=0; i<size; i++)
	{

		temp = (input[i] + code);
		if(temp > 57 && temp < 65)
		{
			offset = temp - 57;
			encrypted[i] = 47+offset;
		}
		else if(temp > 90 && temp < 97)
		{
			offset = temp - 90;
			encrypted[i] = 64+offset;
		}
		else if (temp > 122)
		{
			offset = temp - 122;
			encrypted[i] = 96 +offset;
		}
		else
			encrypted[i] = temp;

	}
	encrypted[size]='\0';

	return encrypted;
}

char *decryption_Caesars(char** ciph, int* cd)
{
	char* cipher;
	char* decrypted;

	cipher = *ciph;

	int size= charArraySize(cipher);
	int code = *cd;
	int i, offset;

	decrypted = malloc( size*sizeof(char));

	if(decrypted==NULL)
	{
		fprintf(stderr, "Error: Memory allocation has failed.\n");
		exit(EXIT_FAILURE);
	}


	for (i=0; i<size; i++)
	{
		decrypted[i] = cipher[i] - code;

	}

	decrypted[size]='\0';

	return decrypted;
}



char *encryption_Vigenere(char** txt, char** k)
{
	char* p_rows;
	char rows[26];
	char table[26][26];
	int i, j, z;

	p_rows = rows;


	/*
	 * Setting up a String Array with the English Alphabet.
	 */
	for(j=0; j<26; j++)
	{		
		rows[j]=j+65;
	}

	/*
	 * Setting up a character table 26x26 containing English alphabet Characters. Each subsequent row is
	 * shifted left by 1 position. The shifted values are tailed at the corresponding row and not lost.
	 */
	for(i=0; i<26; i++)
	{
		
		for(j=0; j<26; j++)
		{	
			table[i][j] = rows[j];
			//fprintf(stderr, "%c",rows[j]);
		}

		shift_character(p_rows, 26, 1);
		//fprintf(stderr, "\n");


	}
		
	char* text = *txt;
	char* key = *k;

	char* encrypted;
	int size = charArraySize(text);

	encrypted = malloc( size*sizeof(char));

	if(encrypted==NULL)
	{
		fprintf(stderr, "Error: Memory allocation has failed.\n");
		exit(EXIT_FAILURE);
	}

	for(z=0; z<size; z++)
	{

		i = charAt(rows, text[z]);
		j = charAt(rows, key[z]);
		encrypted[z] = table[i][j];
		
	}
	encrypted[size] ='\0';





	return encrypted;
}



char *decryption_Vigenere(char** ciph, char** k)
{
	char* p_rows;
	char rows[26];
	char table[26][26];
	int i, j, z;

	p_rows = rows;


	/*
	 * Setting up a String Array with the English Alphabet.
	 */
	for(j=0; j<26; j++)
	{		
		rows[j]=j+65;
	}

	/*
	 * Setting up a character table 26x26 containing English alphabet Characters. Each subsequent row is
	 * shifted left by 1 position. The shifted values are tailed at the corresponding row and not lost.
	 */
	for(i=0; i<26; i++)
	{
		
		for(j=0; j<26; j++)
		{	
			table[i][j] = rows[j];
			//fprintf(stderr, "%c",rows[j]);
		}

		shift_character(p_rows, 26, 1);
		//fprintf(stderr, "\n");


	}

	char* cipher = *ciph;
	char* key = *k;

	char* decrypted;
	int size = charArraySize(cipher);

	decrypted = malloc( size*sizeof(char));

	if(decrypted==NULL)
	{
		fprintf(stderr, "Error: Memory allocation has failed.\n");
		exit(EXIT_FAILURE);
	}

	char shifted_row[26];
	for(z=0; z<size; z++)
	{

		i = charAt(rows, key[z]);
		copyString(shifted_row, table, i, 0);
		j = charAt(shifted_row, cipher[z]);

		decrypted[z] = rows[j] ;
		
	}
	decrypted[size] ='\0';


	return decrypted;
}


int main()
{	
	char* input;
	char* keys;
	char* encrypted;
	char* decrypted;
	int code;

 /*
  *	One-Time-PAD Encryption section. 
  *
  * Input prompt from user. Special characters are being filtered and removed.
  * Initial buffer size is 50.
  * Keys are generated via the /dev/urandom file from the Linux dev system. A character
  * string is fetched at the size of the input String accordingly.
  *
  * Encryption and Decryption functions accordingly.
  */
	input = read_PlainTextInput(0);


	keys = generateOTPKeys(charArraySize(input));
	encrypted = encryption_OTP(&input, &keys);
	decrypted = decryption_OTP(&encrypted, &keys);

	fprintf(stderr, "\n\n[OTP] input: %s \n", input);
	fprintf(stderr, "[OTP] encrypted: %s\n", encrypted);
	fprintf(stderr, "[OTP] decrypted: %s\n\n", decrypted);


 /*
  *	Caesar's Cipher Encryption section.
  *
  * Input prompt from user. Special characters are being filtered. Input left is either
  * a number 0-9 or a letter a-z lowercase or uppercase A-Z. 
  * Shift code is also prompt by the user. The amount of right shift in character ASCII integer value
  * the letter or the number will be shifted at.
  *
  * Encryption and Decryption functions accordingly.
  */

	input = read_PlainTextInput(0);
	code = *read_Caesars_shiftCode();

	encrypted = encryption_Caesars(&input, &code);
	decrypted = decryption_Caesars(&encrypted, &code);

	fprintf(stderr, "\n[Caesars] input: %s \n", input);
	fprintf(stderr, "[Caesars] code: %d \n", code);
	fprintf(stderr, "[Caesars] encrypted: %s \n", encrypted);
	fprintf(stderr, "[Caesars] decrypted: %s \n\n", decrypted);



 /*
  *	Vigenere's Cipher Encryption section.
  *
  * Input prompt from user. Only Capital Letters are allowed in this one.
  * Special characters, lowercase characters and numbers are being filtered and removed.
  *
  * Secondary key input required. Same filters apply on it as well.
  * 
  * Encrytpion and Decryption functions accodringly.
  */

	input = read_PlainTextInput(1);
	keys = read_PlainTextInput(1);
	keys = key_Correction(&keys, charArraySize(input));

	encrypted = encryption_Vigenere(&input, &keys);
	decrypted = decryption_Vigenere(&encrypted, &keys);
	
	fprintf(stderr, "\n[Vigenere] input: %s \n", input);
	fprintf(stderr, "[Vigenere] key: %s \n", keys);
	fprintf(stderr, "[Vigenere] encrypted: %s \n", encrypted);
	fprintf(stderr, "[Vigenere] decrypted: %s \n", decrypted);



  /*
   *	Malloc is used to hold memory for these String variables. Their use is multiple during the
   *  main function. 
   *  Freeing them in the end.
 	*
   */ 

	free_keys(input);
	free_keys(keys);
	free_keys(encrypted);
	free_keys(decrypted);



		
	return 0;
}

