#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>

typedef struct {
    uint32_t magic;         // The word "DATA"
    uint32_t unk;           // Unknown
    uint32_t num_files;     // Number of files
} bin_header_t;

typedef struct {
    char file_name[0x44];   // Name of sub file
    char unk[0xc0];         // Unknown
    uint32_t file_size;     // Size of sub file
} file_header_t;

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Not enough arguments; need an input file!\n");
        return 0;
    }

    FILE *bin_file = fopen(argv[1], "rb");
    if (bin_file == NULL) {
        fprintf(stderr, "Error opening file '%s': %s\n", argv[1], strerror(errno));
        return 0;
    }

    printf("%-44s%-10s\n", "File", "Size");

    bin_header_t bin_header;
    fread(&bin_header, sizeof(bin_header), 1, bin_file);

    for (int i = 0; i < bin_header.num_files; ++i) {
        file_header_t file_header;
        fread(&file_header, sizeof(file_header), 1, bin_file);

        printf("%-44s%-10u\n", file_header.file_name, file_header.file_size);

        uint8_t *data = malloc(file_header.file_size);
        fread(data, file_header.file_size, 1, bin_file);

        FILE *out_file = fopen(file_header.file_name, "wb+");
        fwrite(data, 1, file_header.file_size, out_file);
        fclose(out_file);

        free(data);
    }

    fclose(bin_file);
    printf("\nDone decompressing!\n");
    return 0;
}
