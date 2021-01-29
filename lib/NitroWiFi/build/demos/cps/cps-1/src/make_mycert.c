//
//  $Revision: 1.5 $    $Date: 2005/07/14 07:46:38 $
//
//	Copyright (C) Ubiquitous Corp. 2001-2005. All rights reserved.
//
#include <stdio.h>

typedef unsigned char   uchar;
typedef unsigned long   ulong;

FILE*   fp;

static void dump_hex(char* title, uchar* s, int len)
{
    int i;

    fprintf(fp, "%s", title);
    for (i = 0; i < len; i++)
    {
        if (i % 8 == 0)
            fprintf(fp, "\n\t");
        fprintf(fp, "0x%02x", *s++);
        if (i < len - 1)
            fprintf(fp, ",");
    }

    fprintf(fp, "\n");
}

static void usage()
{
    fprintf(stderr, "\
usage: make_mycert -n <symbol_name> [-o outfile] infile\n\
");
    exit(1);
}

#define BUFLEN  8192
uchar   buf[BUFLEN];

#define GETARG  ((argv[0][2] != '\0' || argc < 2) ? &argv[0][2] : (argc--, *++argv))

int main(int argc, char ** argv)
{
    char*   outfile;
    char*   name;
    int     buflen;

    outfile = NULL;
    name = NULL;
    while (argc > 1 && **(argv + 1) == '-')
    {
        argc--;
        argv++;
        switch (argv[0][1])
        {
        case 'o':
            outfile = GETARG;
            break;

        case 'n':
            name = GETARG;
            break;

        default:
            usage();
            break;
        }
    }

    if (name == NULL)
        usage();
    fp = fopen(*++argv, "rb");
    if (fp == NULL)
    {
        fprintf(stderr, "failed to open %s.\n", *argv);
        exit(1);
    }

    buflen = fread(buf, 1, BUFLEN, fp);
    fclose(fp);
    if (outfile == NULL)
        fp = stdout;
    else if ((fp = fopen(outfile, "w")) == NULL)
    {
        fprintf(stderr, "failed to create %s.\n", outfile);
        exit(1);
    }

    fprintf(fp, "#include <nitro.h>\n");
    fprintf(fp, "#include <nitroWiFi.h>\n\n");
    dump_hex("static unsigned char certificate[] = {", buf, buflen);
    fprintf(fp, "};\n\n");
    fprintf(fp, "CPSCertificate %s = {sizeof(certificate), certificate};\n", name);
    if (fp != stdout)
        fclose(fp);
}
