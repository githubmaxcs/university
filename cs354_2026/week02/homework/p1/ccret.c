/*======================================================================
 * PROJECT: C-cret Codex
 *----------------------------------------------------------------------
 * AUTHOR: Max Hanson 
 * EMAIL: mthanson5@wisc.edu
 * ADDITIONAL SOURCES: None
 * FILE: ccret.c
 * COURSE: COMP SCI 354 - Fall 2026
 * INSTRUCTOR: Dahl
 * COPYRIGHT: 2026, Dahl
 * Posting or sharing this file with anyone outside of course staff prohibited.
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "ccret.h"

/*
 * Entry point for this C-cret Codex application.  This program can be used
 * to encode and decode plain text using a variety of different substitution
 * ciphers.
 *
 * This program takes 2 or 3 command line arguments (in addition to this
 * executable's name).  The first argument is an optional -d, which specifies
 * that the message should be decoded rather than encoded with the given key.
 * The next argument is the key that should be used for encoding or decoding.
 * And the last argument is the message.  This program will the print out
 * the resulting encoded/decoded message to standard out.
 *
 * Examples:
 *   ./ccret A abc                -> encoded: ZYX
 *   ./ccret -d A ZYX             -> decoded: ABC
 *   ./ccret cc "hello, world"    -> encoded: JGNNQ, YQTNF
 */
int main(int argc, char** argv) {
	// ensure argument count is 3 or 4
        if (argc != 3 && argc != 4) {
                fprintf(stderr, "Usage: ccret [-d] KEY MESSAGE\n");
                return 1;
        }
	// ensure decode flag is handled
	if (argc == 4 && strcmp(argv[argc - 3], "-d") != 0) {
		fprintf(stderr, "Usage: ccret -d KEY MESSAGE\n");
		return 1;
	}

	// define parameters from arguments
	int decode = 0; // 0: encode message, 1: decode message
	if (argc == 4 && strcmp(argv[argc - 3], "-d") == 0) {
                decode = 1;
        }
        char* key = argv[argc - 2];
	char* message = argv[argc - 1];
        int msgLen = strlen(message);
        char output[msgLen + 1]; // fix length to hold copy of message

        ccret(key,decode,message,output);
        printf("%s: %s\n",!decode?"encoded":"decoded",output);

        return 0;
}

/*
 * This function does the heavy lifting for the C-cret Codex application.
 * Arugments:
 * key is a c-string reference to the key used for encoding/decoding
 * pDecode is either 0: encode message with key, otherwise message is decoded
 * message is a c-string to the message being encoded or decoded
 * output references the memory where the encoded/decoded string is written
 */
void ccret(char* key, int pDecode, char* message, char* output) {
        int keyLength = strlen(key);

	// if key starts with 'p' (lowercase or uppercase), use the polyalphabetic cipher
        if (key[0] == 'p' || key[0] == 'P') {
                
		// when the key length is one, use identity mapping in createSubstitutionMap
		if (keyLength == 1) {
                        // create cipher specific mapping from plain letters to encoded ones
                	char substitutionMap[LETTER_COUNT];
                	createSubstitutionMap(key,substitutionMap);
                	// when decoding a message, invert this map
                	if(pDecode) invertMap(substitutionMap);

                	// encode/decode each char in message, one at a time
                	for(int i=0;i<strlen(message)+1;i++) {
                        	char c = message[i];
                        	// convert char to a uppercase letter index 0-LETTER_COUNT
                        	int letterIndex = toupper(c) - 'A';
                        	if(letterIndex >=0 && letterIndex < LETTER_COUNT)
                                	// when that index is valid letter, use map for substitution
                                	c = substitutionMap[letterIndex] + 'A';
                        	output[i] = c;
                	}
			// return so that we don't continue
			return;
                }

                // when the key length isn't one, use subkeys from key
		int cycleIndex = 1;
                for(int i=0;i<strlen(message)+1;i++) {
                        char c = message[i];
                        int letterIndex = toupper(c) - 'A';
                        if(letterIndex >= 0 && letterIndex < LETTER_COUNT) {
                                // when index is valid letter, use pointer arithmetiic to define subkey
				char* subKey = key + cycleIndex;

                                // create cipher specific mapping from plain letters to encoded ones
                                char substitutionMap[LETTER_COUNT];
                                createSubstitutionMap(subKey, substitutionMap);
                                // when decoding a message, invert this map
                                if(pDecode) invertMap(substitutionMap);

                                c = substitutionMap[letterIndex] + 'A';
				
				// increment the cycle index
                                cycleIndex = (1 + cycleIndex) % keyLength;
                        }
                        output[i] = c;
                }
        }
	// 
        else {
                // create cipher specific mapping from plain letters to encoded ones
                char substitutionMap[LETTER_COUNT];
                createSubstitutionMap(key,substitutionMap);
                // when decoding a message, invert this map
                if(pDecode) invertMap(substitutionMap);

                // encode/decode each char in message, one at a time
                for(int i=0;i<strlen(message)+1;i++) {
                        char c = message[i];
                        // convert char to a uppercase letter index 0-LETTER_COUNT
                        int letterIndex = toupper(c) - 'A';
                        if(letterIndex >=0 && letterIndex < LETTER_COUNT)
                                // when that index is valid letter, use map for substitution
                                c = substitutionMap[letterIndex] + 'A';
                        output[i] = c;
                }
        }

}

/*
 * Creates a substitution map that can be used for encoding messages using
 * a variety of different ciphers.  This function makes use of the helper
 * functions: createAtbashMap, createCaesarMap, and createMixedMap.  When
 * the first letter of a key is 'a') it creates the map for an Atbash cipher, 
 * 'c') it creates the map for a Caesar cipher, otherwise) it creates a map
 * for a Mixed Alphabet cipher.
 */
void createSubstitutionMap(char* key, char* map) {
	// use identity map for polyalphabetic cipher with key length one
	if (strlen(key) == 1 && (*key == 'p' || *key == 'P')) {
        	for(int i=0;i<LETTER_COUNT;i++) map[i] = i;
	}
	else if (*key == 'a' || *key == 'A') {
                createAtbashMap(key, map);
        }
        else if (*key == 'c' || *key == 'C') {
                createCaesarMap(key, map);
        }
        else {
                createMixedMap(key, map);
        }
}

/*
 * This function inverts a substitution map so that the result can be used for
 * decoding messages rather than encoding them.  For example, if A maped to X
 * in the input map, then X will map back to A in that map after calling this
 * function.
 */
void invertMap(char* map) {
	
	char invertedMap[LETTER_COUNT];	
	for(int i=0;i<LETTER_COUNT;i++)
		invertedMap[(int)map[i]] = i;
	memcpy(map,invertedMap,LETTER_COUNT*sizeof(char));
}

/*
 * This function takes a key and creates a map where alphabetical
 * characters are mapped to their mirror in the alphabet, e.g.,
 * A->Z, B->Y, and C->Z. Keys must begin with 'A' for Atbash.
 * 
 * Examples:
 *   ./ccret abcdef abc                  -> encoded: ZYX
 *   ./ccret -d abc lmno                 -> decoded: ONML
 */
void createAtbashMap(char* key, char* map) {
	// check that the key is 'A'; if not, error
        if (*key != 'a' && *key != 'A') {
                fprintf(stderr, "createAtbashMap must use key 'A'.\n");
                return;
        }

        // update the map to be the reverse
        // since A is 1 but we 0 index arrays, the length is letter count - 1
        for(int i=0;i<LETTER_COUNT;i++) {
                map[i] = LETTER_COUNT - 1 - i;
        }

}

/*
 * This function takes a key and creates a map where
 * the key's length is used as an offset for the mapping,
 * shifting the alphabet and wrapping around to the beginning.
 * Keys must begin with 'C' for Caesar. 
 *
 * Examples:
 *   ./ccret cba abc                  -> encoded: DEF
 *   ./ccret -d caesar xyz            -> decoded: RST
 */
void createCaesarMap(char* key, char* map) {
	// check that the key is 'C'; if not, error
        if (*key != 'c' && *key != 'C') {
                fprintf(stderr, "createCaesarMap must use key 'C'.\n");
                return;
        }

        // update the map to be offset by length of the key
        size_t keyLength = strlen(key);
        for(int i=0;i<LETTER_COUNT;i++) {
                map[i] = (i + keyLength) % LETTER_COUNT;
        }
}


/*
 * This function takes a key and creates a map where alphabetical 
 * characters in the key are used for the map,
 * excluding dulpicates, and only using up to the number of
 * letters in the alphabet for the mapping.
 *
 * Examples:
 *   ./ccret b2i+z abc                -> encoded: BIZ
 *   ./ccret -d X XYZ                 -> decoded: AYZ
 */
void createMixedMap(char* key, char* map) {
        // check that the key is 'A' or 'C'; if so, error
        if (*key == 'a' || *key == 'A' || *key == 'c' || *key == 'C') {
                fprintf(stderr, "createMixedMap must not use key 'A' or 'C'.\n");
                return;
        }

        // track which characters have been used in the alphabet
        // create map index to update map
        int used[LETTER_COUNT] = {0};
        int mapIndex = 0;

        // update the map with key information
        for(int i=0;key[i]!='\0';i++) {
                char c = key[i];
                int letterIndex = toupper(c) - 'A';
                // when index is valid letter, update map
                if (letterIndex >=0 && letterIndex < LETTER_COUNT) {
                        if (!used[letterIndex]) {
				map[mapIndex++] = letterIndex;
                                used[letterIndex] = 1;
                        }
                }
        }

        // fill the remaininig letter mappings not included in the key
        for (int i=0;i<LETTER_COUNT;i++) {
                if (!used[i]) {
                        map[mapIndex++] = i;
                }
        }
}
// EOF -----------------------------------------------------------------
