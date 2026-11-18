#include <stdio.h>
#include <ctype.h>
int word_count(const char s[]){int count=0,in=0;for(int i=0;s[i];i++){if(!isspace((unsigned char)s[i])){if(!in)count++;in=1;}else in=0;}return count;}
int palindrome(const char s[]){int l=0,r=0;while(s[r])r++;r--;while(l<r){while(l<r&&!isalpha((unsigned char)s[l]))l++;while(l<r&&!isalpha((unsigned char)s[r]))r--;if(tolower((unsigned char)s[l])!=tolower((unsigned char)s[r]))return 0;l++;r--;}return 1;}
void upper(char s[]){for(int i=0;s[i];i++)s[i]=(char)toupper((unsigned char)s[i]);}
void reverse(char s[]){int l=0,r=0;char t;while(s[r])r++;r--;while(l<r){t=s[l];s[l++]=s[r];s[r--]=t;}}
int main(void){char msg[]="Meet at the library at 5 PM";char code[]="Madam, I'm Adam";printf("Words: %d\n",word_count(msg));printf("Palindrome: %d\n",palindrome(code));upper(msg);printf("Uppercase: %s\n",msg);reverse(msg);printf("Reversed: %s\n",msg);return 0;}