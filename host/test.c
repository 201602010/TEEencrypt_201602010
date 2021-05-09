/*
 * Copyright (c) 2016, Linaro Limited
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* OP-TEE TEE client API (built by optee_client) */
//#include <tee_client_api.h>

/* To the the UUID (found the the TA's h-file(s)) */
//#include <TEEencrypt_ta.h>

int main(int argc, char* argv[])
{
	printf("TEEencrypt Start\n");
//	TEEC_Result res;
//	TEEC_Context ctx;
//	TEEC_Session sess;
//	TEEC_Operation op;
//	TEEC_UUID uuid = TA_TEEENCRYPT_UUID;
//	uint32_t err_origin;
	FILE *open;
	FILE *write;
	char *text;
	char *key;
	char *fileName;
	int len=0;
	int len2=0;

//	res = TEEC_InitializeContext(NULL, &ctx);
//	if (res != TEEC_SUCCESS)
//		errx(1, "TEEC_InitializeContext failed with code 0x%x", res);
//
//	res = TEEC_OpenSession(&ctx, &sess, &uuid,
//			       TEEC_LOGIN_PUBLIC, NULL, NULL, &err_origin);
//	if (res != TEEC_SUCCESS)
//		errx(1, "TEEC_Opensession failed with code 0x%x origin 0x%x",
//			res, err_origin);

	printf("%s\n", argv[0]);
	printf("%s\n", argv[1]);
	printf("%s\n", argv[2]);
	printf("%s\n", argv[3]);
	if(!strcmp(argv[2], "-e")) {
		open = fopen(strcat("/root/",argv[3]), "r");
		if(open == NULL) {
			printf("NO FILE");
			return 0;
		}
		fseek(open, 0, SEEK_END);
		len = ftell(open);
		text = malloc(len + 1);
		memset(text, 0, len + 1);
		fseek(open, 0, SEEK_SET);
		fread(text, len, 1, open);
		fclose(open);
		printf("%s\n", text);

//		memset(&op, 0, sizeof(op));
//		op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_OUTPUT, TEEC_MEMREF_TEMP_OUTPUT,
//						 TEEC_NONE, TEEC_NONE);
//		op.params[0].tmpref.buffer = text;
//		op.params[0].tmpref.size = len;
//		op.params[1].tmpref.buffer = "A";
//		op.params[1].tmpref.size = 1;

//		memcpy(op.params[0].tmpref.buffer, text, len);

//		res = TEEC_InvokeCommand(&sess, TA_TEEENCRYPT_CMD_ENC_VALUE, &op,
//					 &err_origin);

//		if (res != TEEC_SUCCESS)
//			errx(1, "TEEC_InvokeCommand failed with code 0x%x origin 0x%x",
//				res, err_origin);

		write = fopen(strcat(strcat("/root/","ENC"),argv[3]), "w");
		fwrite("A", len, 1, write);
		fclose(write);

		write = fopen(strcat(strcat("/root/","KEY"),argv[3]), "w");
		fwrite("A", len, 1, write);
		fclose(write);
		free(text);
	}
	else if(!strcmp(argv[2], "-d")) {
		open = fopen(strcat("/root/",argv[3]), "r");
		if(open == NULL) {
			printf("NO FILE");
			return 0;
		}
		fseek(open, 0, SEEK_END);
		len = ftell(open);
		text = malloc(len + 1);
		memset(text, 0, len + 1);
		fseek(open, 0, SEEK_SET);
		fread(text, len, 1, open);
		fclose(open);
		printf("%s\n", text);

		open = fopen(strcat("/root/",argv[4]), "r");
		if(open == NULL) {
			printf("NO FILE");
			return 0;
		}
		fgets(key, 1, open);
		fclose(open);
		printf("%s\n", key);
		

//		memset(&op, 0, sizeof(op));
//		op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_OUTPUT, TEEC_MEMREF_TEMP_OUTPUT,
//						 TEEC_NONE, TEEC_NONE);
//		op.params[0].tmpref.buffer = text;
//		op.params[0].tmpref.size = len;
//		op.params[1].tmpref.buffer = key;
//		op.params[1].tmpref.size = 1;

//		memcpy(op.params[0].tmpref.buffer, text, len);

//		res = TEEC_InvokeCommand(&sess, TA_TEEENCRYPT_CMD_ENC_VALUE, &op,
//					 &err_origin);

//		if (res != TEEC_SUCCESS)
//			errx(1, "TEEC_InvokeCommand failed with code 0x%x origin 0x%x",
//				res, err_origin);

		len2 = (int)strlen(argv[3]);
		strncpy(fileName, &argv[3][3], len2-3);
		write = fopen(strcat("/root/",fileName), "w");
		fwrite("A", len, 1, write);
		fclose(write);
		free(text);
	}
	else {
		printf("Wrong Input");
	}
/*
	memset(&op, 0, sizeof(op));
	op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_OUTPUT, TEEC_VALUE_INOUT,
					 TEEC_NONE, TEEC_NONE);
	op.params[0].tmpref.buffer = plaintext;
	op.params[0].tmpref.size = len;
	op.params[1].value.a = 42;

	printf("========================Encryption========================\n");
	printf("Please Input Plaintext : ");
	//scanf("%[^\n]s",plaintext);
	// HERE FILE OPEN READ WRITE CLOSE PART
	FILE *open = fopen("/root/FILENAME.TXT", "r");
	FILE *write = fopen("FILENAME.TXT", "w");
	memcpy(op.params[0].tmpref.buffer, plaintext, len);

	res = TEEC_InvokeCommand(&sess, TA_TEEENCRYPT_CMD_ENC_VALUE, &op,
				 &err_origin);
	if (res != TEEC_SUCCESS)
		errx(1, "TEEC_InvokeCommand failed with code 0x%x origin 0x%x",
			res, err_origin);

	memcpy(ciphertext, op.params[0].tmpref.buffer, len);
	printf("Ciphertext : %s\n", ciphertext);

	printf("========================Decryption========================\n");
	printf("Please Input Ciphertext : ");
	getchar();
	scanf("%[^\n]s",ciphertext);

	memcpy(op.params[0].tmpref.buffer, ciphertext, len);
	res = TEEC_InvokeCommand(&sess, TA_TEEENCRYPT_CMD_DEC_VALUE, &op,
				 &err_origin);
	if (res != TEEC_SUCCESS)
		errx(1, "TEEC_InvokeCommand failed with code 0x%x origin 0x%x",
			res, err_origin);
	memcpy(plaintext, op.params[0].tmpref.buffer, len);
	printf("Plaintext : %s\n", plaintext);
*/

//	TEEC_CloseSession(&sess);
//	TEEC_FinalizeContext(&ctx);

	return 0;
}
