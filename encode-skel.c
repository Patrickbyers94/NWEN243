#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* Program developed for NWEN243, Victoria University of Wellington
   Author: Kris Bubendorfer (c) 2015.
   Uses a modified caeser I & II

   Compile:  gcc -o encode encode.c

   // Make a file with a plain aplhabet in it:

   % cat > test
   ABCDEFGHIJKLMNOPQRSTUVWXYZ
   ^D

   // test it - this should be the correct result when encoding.

   % cat test | ./encode "i came, i saw"
   key: HJKLNOPQRTUVICAMESWXYZBDFG - 26
   HJKLNOPQRTUVICAMESWXYZBDFG

 */

char upcase(char ch){
  if(islower(ch))
    ch -= 'a' - 'A';
  return ch;
}

int containsCharacter(char* word, char c){
    int i;
    for(i = 0; i < strlen(word); i++){
        if(word[i] == c) return 1;
    }
    return 0;
}

char* fixkey(char* s){
  int i, j;
  char plain[26]; // assume key < length of alphabet, local array on stack, will go away!

  for(i = 0, j = 0; i < strlen(s); i++){
    if(isalpha(s[i])){
      plain[j++] = upcase(s[i]);
    }
  }
  plain[j] = '\0';
  return strcpy(s, plain);
}

char* removeDuplicates(char* key){
    int i, j = 0;
    char* newKey = (char*)malloc(sizeof(char)*strlen(key));
    for(i = 0; i < strlen(key); i++){
        if(containsCharacter(newKey, key[i]) == 0){
            newKey[j++] = key[i];
        }
    }
   return strcpy(key, newKey);
}

char nextCharacter(char c){
    char* alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int i;
    for(i = 0; i < strlen(alphabet); i++){
        if(alphabet[i] == c) {
            if(i == 25) return alphabet[0];
            else return alphabet[i + 1];
        }
    }
}

int getAlphaIndex(char c){
    char* alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int i;
    for(i = 0; i < strlen(alphabet); i++){
        if(alphabet[i] == c) {
            return i;
        }
    }
}


void buildtable (char* key, char* encode){

  // This function needs to build an array of mappings in the 'encode' array from plaintext characters
  // to encypered characters.  The encode array will be indexed by the plaintext char.  To
  // make this a useful 0-26 index for the array, 'A' will be stubtracted from it (yes you
  // can do this in C).  You can see this in the main(){} below.  The values in the array
  // will be the cipher value, in the example at the top A -> H, B -> J, etc.

  // You are implementing a Caesar 1 & 2 combo Cypher as given in handout.
  // Your code here:
   int offset = strlen(key) - 1;
   char* alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  // probably need to declare some stuff here!

  fixkey(key); // fix the key, i.e., uppercase and remove whitespace and punctuation
  removeDuplicates(key);

  int i, j = 0;
  char lastChar = 'a';
  for(i = offset; j < 26; i++){
    if(i > 25) i = 0;
    if(j < strlen(key)) {
        encode[i] = key[j];

    }
    else{
        char nextChar =  nextCharacter(lastChar);
        while(containsCharacter(key, nextChar) == 1){
          nextChar = nextCharacter(nextChar);
        }
        encode[i] = nextChar;
    }
    lastChar = encode[i];
    j++;
  }

  // Do some stuff here to make a translation between plain and cypher maps.

}

int main(int argc, char **argv){
  // format will be: 'program' key {encode|decode}
  // We'll be using stdin and stdout for files to encode and decode.

  // first allocate some space for our translation table.

  char* encode = (char*)malloc(sizeof(char)*26);
  char ch;

  if(argc != 2){
    printf("format is: '%s' key", argv[0]);
    exit(1);
  }

  // Build translation tables, and ensure key is upcased and alpha chars only.

  buildtable(argv[1], encode);

  // write the key to stderr (so it doesn't break our pipes)

  fprintf(stderr,"key: %s - %d\n", encode, strlen(encode));

  // the following code does the translations.  Characters are read
  // one-by-one from stdin, translated and written to stdout.

  ch = fgetc(stdin);
  while (!feof(stdin)) {
    if(isalpha(ch)){        // only encrypt alpha chars
      ch = upcase(ch);      // make it uppercase
      fputc(encode[ch-'A'], stdout);
    }else
      fputc(ch, stdout);
    ch = fgetc(stdin);      // get next char from stdin
  }
}

