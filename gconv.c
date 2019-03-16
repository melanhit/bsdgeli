#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>

#include <geom/eli/g_eli.h>

int main(int argc, char **argv)
{
    unsigned char sector[sizeof(struct g_eli_metadata)];
    char *prov, *prov_out;
    int fd, error, ealgo;
    struct g_eli_metadata md;

    if (argc < 3) {
        printf("Missing args\n");
        return EXIT_FAILURE;
    }

    prov = argv[1];
    prov_out = argv[2];

    fd = open(prov, O_RDONLY);
    if (fd == -1) {
        printf("Cannot open %s\n", prov);
        return EXIT_FAILURE;
    }

    if (read(fd, sector, sizeof(sector)) != sizeof(sector)) {
        printf("Could not read  %s\n", prov);
        close(fd);
        return EXIT_FAILURE;
    }

    close(fd);

    if(eli_metadata_decode(sector, &md)) {
        printf("Could not decode metadata\n");
        return EXIT_FAILURE;
    }

    // set id algo
    ealgo = strtonum(argv[3], 1, 64, NULL);
    md.md_ealgo = ealgo;

    eli_metadata_encode(&md, sector);

    fd = open(prov_out, O_WRONLY | O_TRUNC | O_CREAT, 0600);
    if (fd == -1) {
        printf("Cannot open %s\n", prov_out);
        return EXIT_FAILURE;
    }

    if (write(fd, sector, sizeof(sector)) != sizeof(sector)) {
        printf("Could not write %s\n", prov_out);
        close(fd);
        return EXIT_FAILURE;
    }

    close(fd);

    return EXIT_SUCCESS;
}
