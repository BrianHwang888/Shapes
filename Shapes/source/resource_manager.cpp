#include"../header/resource_manager.h"

const char* read_file(const char* file_path) {
	FILE* fp;
	long length;
	char* buffer;

	errno_t err = fopen_s(&fp, file_path, "r");
	if (fp == NULL) {
		fprintf(stderr, "Failed to read file: %s\n", file_path);
		system("pause");
		exit(EXIT_FAILURE);
	}
	printf("Successfully read %s\n", file_path);
	fseek(fp, 0L, SEEK_END);
	length = ftell(fp);

	fseek(fp, 0L, SEEK_SET);
	buffer = new char[length + 1];
	fread(buffer, 1, length, fp);
	buffer[length] = '\0';
	fclose(fp);

	for (int i = 0; i < length; i++) {
		if (!(buffer[i] >= 0 && buffer[i] <= 128))
			buffer[i] = ' ';
	}
	return buffer;
}