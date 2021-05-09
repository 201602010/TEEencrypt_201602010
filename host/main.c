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
#include <tee_client_api.h>

/* To the the UUID (found the the TA's h-file(s)) */
#include <TEEencrypt_ta.h>

int main(int argc, char* argv[])
{
	printf("TEEencrypt Start\n");
	TEEC_Result res;
	TEEC_Context ctx;
	TEEC_Session sess;
	TEEC_Operation op;
	TEEC_UUID uuid = TA_TEEENCRYPT_UUID;
	uint32_t err_origin;
	FILE *open;
	FILE *write;
	char *text;
	char key[2] = "A";
	char *fileRoot;
	char *encRoot;
	char *keyRoot;
	int len=0;

	res = TEEC_InitializeContext(NULL, &ctx);
	if (res != TEEC_SUCCESS)
		errx(1, "TEEC_InitializeContext failed with code 0x%x", res);

	res = TEEC_OpenSession(&ctx, &sess, &uuid,
			       TEEC_LOGIN_PUBLIC, NULL, NULL, &err_origin);
	if (res != TEEC_SUCCESS)
		errx(1, "TEEC_Opensession failed with code 0x%x origin 0x%x",
			res, err_origin);

	if(!strcmp(argv[1], "-e")) {
		fileRoot = malloc(strlen(argv[2]) + 6);
		strcpy(fileRoot, "/root/");
		strcat(fileRoot, argv[2]);
		open = fopen(fileRoot, "r");
		if(open == NULL) {
			printf("NO FILE");
			return 0;
		}
		free(fileRoot);
		fseek(open, 0, SEEK_END);
		len = ftell(open);
		text = malloc(len);
		memset(text, 0, len);
		fseek(open, 0, SEEK_SET);
		fread(text, len, 1, open);
		fclose(open);
		printf("InputText : %s\n", text);
		printf("TextLen : %d\n", len);

		memset(&op, 0, sizeof(op));
		op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_OUTPUT, TEEC_MEMREF_TEMP_OUTPUT,
						 TEEC_NONE, TEEC_NONE);
		op.params[0].tmpref.buffer = text;
		op.params[0].tmpref.size = len;
		op.params[1].tmpref.buffer = key;
		op.params[1].tmpref.size = 2;

		memcpy(op.params[0].tmpref.buffer, text, len);
		memcpy(op.params[1].tmpref.buffer, key, 2);

		res = TEEC_InvokeCommand(&sess, TA_TEEENCRYPT_CMD_ENC_VALUE, &op,
					 &err_origin);
		if (res != TEEC_SUCCESS)
			errx(1, "TEEC_InvokeCommand failed with code 0x%x origin 0x%x",
				res, err_origin);

		printf("Finsih Encrypt\n");
		printf("EncText : %s\n", op.params[0].tmpref.buffer);
		printf("Enckey : %s\n", op.params[1].tmpref.buffer);

		encRoot = malloc(strlen(argv[2]) + 7);
		strcpy(encRoot, "/root/ENC_");
		strcat(encRoot, argv[2]);
		
		write = fopen(encRoot, "w");
		fwrite((char *) op.params[0].tmpref.buffer, len, 1, write);
		fclose(write);

		keyRoot = malloc(strlen(argv[2]) + 7);
		strcpy(keyRoot, "/root/KEY_");
		strcat(keyRoot, argv[2]);

		write = fopen(keyRoot, "w");
		fwrite((char *) op.params[1].tmpref.buffer, len, 1, write);
		fclose(write);
	}
	else if(!strcmp(argv[1], "-d")) {
		encRoot = malloc(strlen(argv[2]) + 6);
		strcpy(encRoot, "/root/");
		strcat(encRoot, argv[2]);
		open = fopen(encRoot, "r");
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
		printf("InputText : %s\n", text);
		printf("TextLen : %d\n", len);

		keyRoot = malloc(strlen(argv[3]) + 6);
		strcpy(keyRoot, "/root/");
		strcat(keyRoot, argv[3]);
		open = fopen(keyRoot, "r");
		if(open == NULL) {
			printf("NO FILE");
			return 0;
		}
		fread(key, 2, 1, open);
		fclose(open);
		printf("InputKey : %s\n", key);
		
		memset(&op, 0, sizeof(op));
		op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_OUTPUT,TEEC_MEMREF_TEMP_OUTPUT,
						 TEEC_NONE, TEEC_NONE);
		op.params[0].tmpref.buffer = text;
		op.params[0].tmpref.size = len-1;
		op.params[1].tmpref.buffer = key;
		op.params[1].tmpref.size = 2;

		memcpy(op.params[0].tmpref.buffer, text, len);
		memcpy(op.params[1].tmpref.buffer, key, 2);

		res = TEEC_InvokeCommand(&sess, TA_TEEENCRYPT_CMD_DEC_VALUE, &op,
					 &err_origin);

		if (res != TEEC_SUCCESS)
			errx(1, "TEEC_InvokeCommand failed with code 0x%x origin 0x%x",
				res, err_origin);

		printf("Finsih Decrypt\n");
		printf("DecText : %s\n", op.params[0].tmpref.buffer);

		fileRoot = malloc(strlen(argv[3]) + 6);
		strcpy(fileRoot, "/root/");
		strncat(fileRoot, &argv[3][4], strlen(argv[3])-3);
		write = fopen(fileRoot, "w");
		fwrite((char *) op.params[0].tmpref.buffer, len, 1, write);
		fclose(write);
	}
	else {
		printf("Wrong Input");
	}

	TEEC_CloseSession(&sess);
	TEEC_FinalizeContext(&ctx);

	return 0;
}
