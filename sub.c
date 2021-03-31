/*
* Title: Shellcode Custom ASCII Subtraction Encoder 
* Date: 29.03.2021
* Author: Xenofon Vassilakopoulos  
* github : @xen0vas
* Tested on: 
*
*
* gcc -m32 sub.c -o sub
* 
*  Usage : ./sub -s \x41\xff\x41\x41 -b \x0a\x0d\x2f\x3a\x3f\x40\x80\x81\x82
*
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include <limits.h>
#include <errno.h>
#include <stdint.h>

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"
#define RESET "\x1B[0m"

#define ZEROVAL 0x0 



char *hexToStrip(char *shellcode, size_t si)
{
size_t s = strlen(shellcode);
char *buf = (char*)malloc((s + 1) * sizeof(shellcode)*2);
char *stringToStrip = (char*)malloc((s + 1) * sizeof(shellcode)*2);
strcpy(stringToStrip,shellcode);
size_t stringLen = strlen(stringToStrip);
unsigned int i,j = 0;

char currentChar;

for (j = 0; j<stringLen+1;  j++) {
    currentChar = stringToStrip[j];
    if (currentChar != '\\' && currentChar != 'x')
    {
         sprintf(buf + strlen(buf),"%c",currentChar);
    }
}
return buf;
}

unsigned char *hexTochar(char *shellcode) {
    char *end;
    unsigned long int j = strtol(shellcode, &end, 16);
    char* str = (char*)calloc(strlen(shellcode), sizeof(char*));
    int i=0;
    for ( ;; ) {
        sprintf(str + i * sizeof(char),"%c", (int)j);
        i++;
        j = strtol(end, &end, 16);
	if (j == 0)
	    break; 
    }
    return str;
}

char *strrev(char *str)
{
    if (!str || ! *str)
        return str;

    int i = strlen(str) - 1, j = 0;

    char ch;
    while (i > j)
    {
        ch = str[i];
        str[i] = str[j];
        str[j] = ch;
        i--;
        j++;
    }
    return str;
}

unsigned char* toCharByte(char *byte, size_t bytelen)
{
    if (byte == NULL || !strcmp(byte, ""))
    {
    	  return NULL;
    }

    size_t k,len = bytelen; 

    char cbyte[len];
    strcpy(cbyte, byte);

    // allocate the 1/3 of the total char size
    unsigned char* str = (unsigned char*)calloc(len / 3, sizeof(str));
    unsigned char* chr = (unsigned char*)calloc(len / 3 , sizeof(chr));
    char* alpha = (char*)malloc((len / 3) * sizeof(alpha));
    char *ch = strtok(cbyte, "\\x");
    while(ch != NULL)
    {
        sprintf(alpha + strlen(alpha), "%s", ch );
        ch = strtok(NULL, "\\x");
    }

    for(k=0; alpha[k]!='\0'; k+=2)
    {
       sprintf(str + strlen(str), "%c%c ",  alpha[k], alpha[k+1]);
    }

    chr =  hexTochar(str);
    free(str);
    free(alpha);
    return chr;
}

char *charTohex(char *shellcode, size_t si)
{
char *chr = (char*)calloc(si+1 , sizeof(shellcode)*2);
int l;
for (l=0; l<si; l++) {
       sprintf(chr + strlen(chr),"\\x%02x", (unsigned char)(int)shellcode[l]);
}

return chr;
}

void nasm_banner()
{


printf(R"EOF
;                                    .   ,                 
;                                           .c. ;.                
;                     ..  ..    .     ..,;clc;x'.l                
;                    'kkoldxl;:o.       ,ldkkkkkld:               
;                    ,oxkkllkkk:.     .dkkkkkxkkkkkl.             
;                       ;kkkkkkkko,. .xkkkk:. .ckkkcx.            
;                        ,ko,;dkkk:. ,;kkkx     ;kklo:            
;                         .    ckkko. :kkkk.    ,kkkkk,           
;                              .xkkc; ckkkk;    .;cokkkd.         
;                               okkk,.ckkkkk'   ,   .:kk.         
;                               xkkxc ,odkkkk:. .l'.  ;:          
;                              .kkko;  c.okkkkkxookkxo.           
;                              :kkkd.  .,:lkkkkkkkkkl:d,..        
;                    .:. ''.   xkkdl   ..:dkkkkkklxkkkkkkk,       
;                     ccod:,. .kkk;'   .xkkkxdkkk,.kkkxdxkl       
;                    .xkkkkko..kkk.   .xkkko..xkkd ';.  .,.       
;                    lkkkkkkk:.kkk.   ;ckkx.  ckkk. ... c.        
;                   ;lkk..lkkd,kkk.   .kkko   ;kkk. .;dcd.        
;                   :dkk. 'kkx.okkl   ,kkkk.  :kkk.'dkkkkd.       
;                  .xkc.  ;kkk''kkk,  'kkkkx. okkk.:kkdckko.      
;                  lk'   .xkko. lkkk:..dlkkkx,kkkx.kkk. okd:      
;                  .:    ,kkx,  .okkkd';.dkkkkkkkd.kkk. 'cxk,     
;                        ;kkd    .ckkkkd:;kkkkkkkl.kkk;   .oc     
;                        ,kkk.     ,kkkkkkkkkkkkk, ldkk.   ..     
;                        .xkkx;.    :kkkkkkkkkkkk. .:kk,          
;                         ,kkkkkdl:;okkkkkkkkkkk;   lkk.          
;                          ,kkkkkkkkkkkkkkkkkkko.';dkkc           
;                           .lkkkkkkkkkkkkkkkkkkkkkkko.           
;                       .,. :xkkkkkkkkkkkkkkkkkkkkkkd.
;               

)EOF");


printf("; Author: Xenofon Vassilakopoulos \n; @xvass \n; Version 1.0");

printf("\n\n\n\n");

}

void banner()
{

printf("%s", KYEL);


printf( R"EOF(

                                    .   ,                 
                                           .c. ;.                
                     ..  ..    .     ..,;clc;x'.l                
                    'kkoldxl;:o.       ,ldkkkkkld:               
                    ,oxkkllkkk:.     .dkkkkkxkkkkkl.             
                       ;kkkkkkkko,. .xkkkk:. .ckkkcx.            
                        ,ko,;dkkk:. ,;kkkx     ;kklo:            
                         .    ckkko. :kkkk.    ,kkkkk,           
                              .xkkc; ckkkk;    .;cokkkd.         
                               okkk,.ckkkkk'   ,   .:kk.         
                               xkkxc ,odkkkk:. .l'.  ;:          
                              .kkko;  c.okkkkkxookkxo.           
                              :kkkd.  .,:lkkkkkkkkkl:d,..        
                    .:. ''.   xkkdl   ..:dkkkkkklxkkkkkkk,       
                     ccod:,. .kkk;'   .xkkkxdkkk,.kkkxdxkl       
                    .xkkkkko..kkk.   .xkkko..xkkd ';.  .,.       
                    lkkkkkkk:.kkk.   ;ckkx.  ckkk. ... c.        
                   ;lkk..lkkd,kkk.   .kkko   ;kkk. .;dcd.        
                   :dkk. 'kkx.okkl   ,kkkk.  :kkk.'dkkkkd.       
                  .xkc.  ;kkk''kkk,  'kkkkx. okkk.:kkdckko.      
                  lk'   .xkko. lkkk:..dlkkkx,kkkx.kkk. okd:      
                  .:    ,kkx,  .okkkd';.dkkkkkkkd.kkk. 'cxk,     
                        ;kkd    .ckkkkd:;kkkkkkkl.kkk;   .oc     
                        ,kkk.     ,kkkkkkkkkkkkk, ldkk.   ..     
                        .xkkx;.    :kkkkkkkkkkkk. .:kk,          
                         ,kkkkkdl:;okkkkkkkkkkk;   lkk.          
                          ,kkkkkkkkkkkkkkkkkkko.';dkkc           
                           .lkkkkkkkkkkkkkkkkkkkkkkko.           
                       .,. :xkkkkkkkkkkkkkkkkkkkkkkd.
               

)EOF");


printf("%s", KCYN);
printf("Ascii Subtraction Encoder\n");
printf("-------------------------\n");
printf("Author: Xenofon Vassilakopoulos \n@xvass \nVersion 1.0");
printf("%s\n\n", RESET);

}

unsigned char* concat(unsigned char* dest, size_t dest_size, unsigned char* src, size_t src_size) {
    unsigned char* concat = realloc(dest, dest_size + src_size);
    memcpy(concat + dest_size, src,  src_size);  
    return concat;
}

/**************
     Main  
***************/

int main(int argc, char **argv)
{ 


if (  argc < 2 || argc < 3 || argc < 4 || argc < 5  )
{
	banner();
	printf("%s", KRED);
    printf("[!] Usage: ./and-sub -s <shellcode> -b <badchars>");
    printf("%s", RESET);
    printf("\n\nArgs: \n\t [-s] The shellcode to encode \n\t [-j] Encode a long jump \n\t [-b] The bad characters to filter out \n\n");
    return 1;
}

if ( ( ( strncmp(argv[1],"-s",2) != 0) && (strncmp(argv[1], "-j",2) != 0)   ) || strncmp(argv[3],"-b",2) != 0  )
{
	 banner();
	 printf("\n\n");
	 printf("%s", KRED);
	 printf("\n\n[x] Error: Provide a valid option. \n\n[!] Usage: ./and-sub -s <shellcode> -b <badchars>\n\nArgs: \n\t [-s] The shellcode to encoded \n\t [-j] encode a long jump  \n\t [-b] The bad characters to filter out \n\n");
	 printf("%s", RESET);
	 return 1;

}


/*********************************************************

--- Check if the provided shellcode is divisible by 4 ---

*********************************************************/

unsigned char *divisible = toCharByte(argv[2], strlen(argv[2])*2);

size_t si = strlen(divisible);

char *chr = charTohex(divisible, si);

size_t calc = strlen(chr) / 4;

int divres = calc % 4;

if ( divres != 0)
{
	banner();
	printf("%s",KRED);
	printf("\n[x] The shellcode is not divisible by 4. PLease add extra padding and try again\n\n");
	printf("%s", RESET);
	return 0;
}


//---------------------------------------------------

nasm_banner();

unsigned char *badchars  = toCharByte(argv[4], strlen(argv[4]));

srand((unsigned int)time(NULL));


/**********************************************

---Find bytes zeroing out the registers---

**********************************************/

unsigned char *subres = "\xbb\xbb\xbb\xbb" ;
unsigned char *buff = (char*)malloc(sizeof(subres)*2);
unsigned char *b = (char*)malloc(sizeof(subres)*2);
unsigned char *b2 = (char*)malloc(sizeof(subres)*2);

unsigned char *buf_bytes = (char*)malloc(sizeof(char*));

char *push = (char*)malloc(sizeof(push)*2);
char *pop_esp = (char*)malloc(sizeof(pop_esp)*2);
char *pop = (char*)malloc(sizeof(pop)*2);
char *sub_ = (char*)malloc(sizeof(char));
char *and = (char*)malloc(sizeof(and)*2);

for ( int h = 0; h < 4; h++)
{

// These are some good chars - change this to make it relevant to your environment
// Provide only bytes used to zero out the register in the AND process   
unsigned char nobadchars[] = "\x25\x2a\x2d\x31\x32\x35\x4a\x4d\x4e\x50\x51\x52\x53\x55\x5c";

int co = 0;
for (int i = 0; i < strlen(nobadchars)+1; i++)
{
	for ( int j=0; j<strlen(badchars); j++)
	{
		if ( nobadchars[i] == badchars[j] )
		{
			memcpy(&buf_bytes[co], (unsigned char *)&nobadchars[i], sizeof(nobadchars[i]));
			co++;
		}
	}
}

unsigned char _25_[] = "\xbb";
unsigned char _2a_[] = "\xbb";
unsigned char _2d_[] = "\xbb";
unsigned char _31_[] = "\xbb";
unsigned char _32_[] = "\xbb";
unsigned char _35_[] = "\xbb";
unsigned char _4a_[] = "\xbb";
unsigned char _4d_[] = "\xbb";
unsigned char _4e_[] = "\xbb";
unsigned char _50_[] = "\xbb";
unsigned char _55_[] = "\xbb";
unsigned char _5c_[] = "\xbb";


if (  *buf_bytes == '\0' )
{
		_2a_[0] = '\x2a';
		_50_[0] = '\x50';
}


for ( int o = 0; o < strlen(buf_bytes); o++)
{
	if( (int)buf_bytes[o] != 37 ) 
		_25_[0] = '\x25' ;
	
	if( (int)buf_bytes[o] != 42 ) 
		_2a_[0] = '\x2a';
	
	if( (int)buf_bytes[o] != 45 ) 
       	_2d_[0] = '\x2d';
	
	if( (int)buf_bytes[o] != 49 ) 
		_31_[0] = '\x31';   
	
	if( (int)buf_bytes[o] != 50 ) 
		_32_[0] = '\x32';
	
	if((int)buf_bytes[o] != 53 ) 
		_35_[0] = '\x35';
	
	if((int)buf_bytes[o] != 74 ) 
		_4a_[0] = '\x4a';
	
	if((int)buf_bytes[o] != 77 ) 
		_4d_[0] = '\x4d';
	
	if((int)buf_bytes[o] != 78 ) 
		_4e_[0] = '\x4e';
	
	if((int)buf_bytes[o] != 80 ) 
		_50_[0] = '\x50';
	
	if((int)buf_bytes[o] != 85 ) 
		_55_[0] = '\x55';
	
	if((int)buf_bytes[o] != 92 ) 
		_5c_[0] = '\x5c';
}


if (  *buf_bytes == '\0' )
{
	push = "push eax" ;
	pop = "pop eax"; 
	pop_esp = "pop esp";
	sub_ = "eax";
	and = "and eax";

	if ( ( _50_[0] & _2a_[0] ) == 0 )
	{
		b[h] = _2a_[0];
		b2[h] = _50_[0]; 
	}
}
else if ( ( (int)_50_[0] == 80 && (int)_2a_[0] == 42 ) ) 
{
	push = "push eax" ;
	pop = "pop eax"; 
	pop_esp = "pop esp";
	sub_ = "eax";
	and = "and eax";

	if ( ( _50_[0] & _2a_[0] ) == 0 )
	{
		b[h] = _2a_[0];
		b2[h] = _50_[0]; 
	}
}
else if ( (int)_55_[0] !=187 && (int)_2a_[0] != 187 )
{
	push = "push ebp";
	pop = "pop ebp";
	pop_esp = "pop esp";
	sub_ = "ebp";
	and = "and ebp";

	if ( ( _55_[0] & _2a_[0] ) == 0 )
	{
		b[h] = _2a_[0];
		b2[h] = _55_[0]; 
	}
}
else if ( (int)_50_[0] != 187 && (int)_25_[0] != 187 )
{

	push = "push eax" ;
	pop = "pop eax";
	pop_esp = "pop esp";
	sub_ = "eax";
	and = "and eax"; 
	
	if ( ( _50_[0] & _25_[0] ) == 0 )
	{
		b[h] = _25_[0];
		b2[h] = _50_[0]; 
	}
}

}	

char *chrbyte1 = charTohex(b2, strlen(b2));
char *chrstrip = hexToStrip(chrbyte1, strlen(chrbyte1));
char *chrbyte2 = charTohex(b, strlen(b));
char *chrstrip2 = hexToStrip(chrbyte2, strlen(chrbyte2));

//---------------------------------------------------------------

unsigned char *shellcode; 
unsigned char *shell ;

shellcode = toCharByte(argv[2], strlen(argv[2]));

char *shellcode3 =  (char*)malloc(sizeof(shellcode3)*2);

int k = 0;
int l = 0;

int str_size = strlen(shellcode)-1;

char *shellcode2=(char*)malloc(sizeof(shellcode)*2);
char *reverse = strrev(shellcode);
unsigned char *hex = charTohex(reverse, str_size+1);



for (int i=0; i<(strlen(hex)*2); i++) {
    memcpy(&shellcode3[0],",",sizeof(shellcode[i])*2);

    k = i % 17;
    if (k != 0)
    {
         shellcode2[i] = hex[l];
         l++;
    }
    else
    {
         shellcode2[i] = shellcode3[0];
    }
}

int tokens = 0;
int i = 0;
unsigned char **res = NULL;

res = malloc(sizeof(**res));

size_t shellsize = strlen(shellcode2);


unsigned char *token = strtok(shellcode2, ",");

unsigned char **ptr;

size_t shellcode_len = (size_t)sizeof(token);

if ( shellcode_len == 0 || shellcode_len > SIZE_MAX / sizeof(char) )
{       
       errno = ENOMEM;
	   printf("Allocation Error", errno);
	   return 0; 
}

for (tokens = 1; token; ++tokens) {
	ptr = realloc(res, tokens * sizeof(*res));
	res = ptr; 
	res[tokens - 1] = malloc(strlen(token) + 1);
    strncpy(res[tokens - 1], token, strlen(token) + 1);
    token = strtok(NULL, ",");
}

printf("global _start\n");
printf("section .text\n\n");
printf("_start:\n\n");


size_t counter=strlen(*res)-1;

if( ( (shellsize/4)-2) < 32 )
{
    counter = (strlen(*res)/2)-1;
}

for ( int k=0; k<counter; k++ )
{


if (res[k] == NULL)
{
	return 0;
}

/*-------------------------------------------------------*/

size_t ressize= sizeof(res[k]);

if ( ressize == 0 || ressize > SIZE_MAX / sizeof(char) )
{
           errno = ENOMEM;
           printf("Allocation Error", errno);
           return 0;
}

unsigned char *tohex = toCharByte(res[k], ressize);

/*-------------------------------------------------------*/

unsigned char *shellcode4=(unsigned char*)malloc(sizeof(shellcode)*2);
memset( shellcode4, '\0', sizeof(shellcode4)*2);

unsigned char *shellcode5=(unsigned char*)malloc(sizeof(shellcode)*2);
memset( shellcode5, '\0', sizeof(shellcode5)*2);

unsigned char *ffff=(unsigned char*)malloc(sizeof(shellcode)*2);
memset(ffff , '\0', sizeof(ffff)*2);

for (int i=0; i<sizeof(shellcode)*2; i++) {
        memcpy(&shellcode4[i],(unsigned char*)&tohex[i], sizeof(tohex[i])*2);
}

for (int i=0; i<(sizeof(shellcode4)*2); i++)
{
	if ( i != 3 )
	{
		shellcode5[i] = ZEROVAL - tohex[i] - 1 & 0xff ;
		memcpy(&ffff[i], (unsigned char*)&shellcode5[i], sizeof(shellcode5[i])*2);
	}
	else
	{
		shellcode5[i] = ZEROVAL - tohex[i] & 0xff ;
		memcpy(&ffff[i], (unsigned char *)&shellcode5[i], sizeof(shellcode5[i])*2);
	}
}


unsigned char *buffer = (char*)malloc(sizeof(shellcode)*2);

unsigned char *sub=(unsigned char*)malloc(sizeof(sub));
memset(sub , '\0', sizeof(sub));


for (int i=0; i<sizeof(ffff)*2; i++) {
        memcpy(&sub[i],(unsigned char*)&ffff[i], sizeof(ffff[i])*2);
}

unsigned char *first=(unsigned char*)malloc(sizeof(ffff)*2);
memset(first , '\0', sizeof(ffff)*2);

for (int i=0; i<sizeof(ffff)*2; i++) {
       memcpy(&first[i],(unsigned char*)&ffff[i], sizeof(ffff[i])*2);
}

unsigned char *second=(unsigned char*)malloc(sizeof(ffff)*2);
memset(second, '\0', sizeof(ffff)*2);

for (int i=0; i<sizeof(ffff)*2; i++) {
        memcpy(&second[i],(unsigned char*)&ffff[i], sizeof(ffff[i])*2);
}

unsigned char *third=(unsigned char*)malloc(sizeof(ffff)*2);
memset(third, '\0', sizeof(ffff)*2);

for (int i=0; i<sizeof(ffff)*2; i++) {
       memcpy(&third[i],(unsigned char*)&ffff[i], sizeof(ffff[i])*2);
}

// placeholder 
unsigned char rp[] = "\xbb";
printf("\n");

bool found = false ; 
bool bad = true; 
int count = 0;
int l = 1;

bool t = false;

for (int i=0; i<(sizeof(ffff)); i++)
{

	int j = 0;
	count = 0;
	bad = true; 

	while (1)
	{
		l=1;
		if ( j == 4 ) 
		{		      
	              break;
		}

		start:
		buffer[0] = rand() & 0xff;
		
		
		for (int o = 0; o<=strlen(badchars); o++)
		{
			if ( buffer[0] == badchars[o] )
			{
				count = count + 1;
				goto start;
			}
		}

		if (count != 0)
		{
			bad = false;
			memcpy(&rp[0],(unsigned char*)&buffer[0],sizeof(buffer[0]));
		}
		else
		{
			bad = true;
			count = 0;
		}
	
		if ( ((int)ffff[i] > (int)rp[0]) || ((int)ffff[i] == 0) && bad == false)
		{
	   		sub[0] = rp[0] & 0xff;

			start1:
			buffer[1] = rand() & 0xff;

			for (int o = 0; o<=strlen(badchars); o++)
			{
				if ( buffer[1] == badchars[o]  )
				{
					count = count + 1;
					goto start1;
				}
			}

			if (count != 0)
			{
				bad = false;
				memcpy(&rp[0],(unsigned char*)&buffer[1],sizeof(buffer[1]));
			}
			else
			{
				bad = true;
				count = 0;
			}

	       		if (  ( ((int)ffff[i] == 0) || ( (int)sub[0] != 0 ) && bad == false) )
			{

  				sub[1] = rp[0] & 0xff ;


				start2:
				buffer[2] = rand() & 0xff;

				for (int o = 0; o<=strlen(badchars); o++)
				{
					if ( buffer[2]  == badchars[o] )
					{
						count = count + 1;
						goto start2;
					}
				}

				if (count != 0)
				{
					bad = false;
					memcpy(&rp[0],(unsigned char*)&buffer[2],sizeof(buffer[2]));
				}
				else
				{
					bad = true;
					count = 0;
				}

		       	if ( ((int)ffff[i] == 0 ) || ((int)sub[1] > (int)rp[0]) && ((int)sub[1] != 0 ) && bad == false )
				{
					sub[2] = rp[0] & 0xff ;
					
					if ( (int)ffff[i] == 1 )
						sub[3] = sub[0];
					else if ( (int)ffff[i+1] == 1 )
						sub[3] = (ffff[i]-1) - sub[0]; 
					else if ( (int)ffff[i] == 0 )
						sub[3] = sub[0];
					else if ( (int)ffff[i+1] == 0 )
						sub[3] = (ffff[i]-1) - sub[0]; 
					else
						sub[3] = ffff[i] - sub[0];

					if ( ( (int)sub[3] > (int)sub[1] ) )  	
						sub[4] = sub[3] - sub[1];
						if ( ((int)sub[4] == (int)sub[2] ) )
						{

							if ( (int)ffff[i+1] == 1 )
							{
								sub[5] = (ffff[i]-1) - sub[0] - sub[1] - sub[2];
							}
							else if ( (int)ffff[i]==1)
							{
								sub[5] = 257 - (int)sub[0] - (int)sub[1] - (int)sub[2];
							}
							else if ( (int)ffff[i+1] == 0 )
							{
								sub[5] = (ffff[i]-1) - sub[0] - sub[1] - sub[2];
							}
							else if ( (int)ffff[i]==0 )
							{
								sub[5] = 256 - (int)sub[0] - (int)sub[1] - (int)sub[2];
							}
							else
							{
								sub[5] = ffff[i] - sub[0] - sub[1] - sub[2];
							}
							
	                        // long jump encoding	
							if ( strncmp(argv[1],"-j",2) == 0 )
							{
								if ( (int)ffff[i] == 0 ) 	
							 	{	
									if(  ( (int)sub[2] + (int)sub[1] + (int)sub[0] ) == 255 )
									{
									 	memcpy(&third[i], (unsigned char*)&sub[2], sizeof(sub[2]));
										memcpy(&second[i], (unsigned char*)&sub[1], sizeof(sub[1]));
										memcpy(&first[i], (unsigned char*)&sub[0], sizeof(sub[0]));	
	 								 	l=0;
	 									break;
									}
									else
									{
										l=1;
									}
								}
								else 
								{
									if ( (int)sub[5] == 0  )
									{
										 memcpy(&third[i] , (unsigned char*)&sub[2], sizeof(sub[2]));
		 							     memcpy(&second[i], (unsigned char*)&sub[1], sizeof(sub[1]));
										 memcpy(&first[i] , (unsigned char*)&sub[0], sizeof(sub[0]));	
										 l=0;
										 break;
									}
									else
										l=1;
								}

							}
							else  if ( strncmp(argv[1],"-j",2) != 0 )				
							{
									if ( (int)sub[5] == 0  )
									{
									 	memcpy(&third[i] , (unsigned char*)&sub[2], sizeof(sub[2]));
									 	memcpy(&second[i], (unsigned char*)&sub[1], sizeof(sub[1]));
									 	memcpy(&first[i] , (unsigned char*)&sub[0], sizeof(sub[0]));	
									 	l=0;
									 	break;
									}
									else
										l=1;
							}
						}
				}
			}
	}
	if ( l==0 )
	{
		j++;
	}
        else if (l==1)
	{
		j = 0;
	}
	} // while 
} // for

char *ptx = hexToStrip(res[k], sizeof(res[k]));
ptx = hexToStrip(res[k], strlen(res[k]));

char *tochar = charTohex(ffff, sizeof(ffff));
char *strip  = hexToStrip(tochar, strlen(tochar));

printf(";-------------------------------------------");
printf("\n");
printf("\n\n; Token : %s\n\n",res[k]);

printf("; 0x0 - 0x%.8s = 0x%.8s\n\n",ptx, strip);
printf("\n");

printf("%s, 0x%s\n",and, chrstrip );
printf("%s, 0x%s\n",and, chrstrip2 ); 

char *revres1 = charTohex(first, sizeof(first));
char *revstrip1 = hexToStrip(revres1, strlen(revres1)); 
printf("sub %s, 0x%s\n",sub_, revstrip1); 

char *revres2 = charTohex(second, sizeof(second));
char *revstrip2 = hexToStrip(revres2, strlen(revres2)); 
printf("sub %s, 0x%s\n",sub_, revstrip2); 

char *revres3 = charTohex(third, sizeof(third));
char *revstrip3 = hexToStrip(revres3, strlen(revres3)); 
printf("sub %s, 0x%s\n",sub_, revstrip3);
printf("%s\n",push);

printf("\n");
free(tohex);

if ( shellcode5 != NULL ) memset( shellcode5, '\0', sizeof(shellcode5)*2);
if ( shellcode4 != NULL ) memset( shellcode4, '\0', sizeof(shellcode4)*2);
if ( ffff != NULL ) memset( ffff, '\0', sizeof(ffff)*2);

}

free(res);
printf("\n\n");

return 0; 
}

//@xvass
