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

int cert_item_len(uchar ** ps)
{
    uchar*  s = *ps;
    int     len;
    int     lenlen;

    len = *s++;
    if ((len & 0x80) != 0)
    {
        lenlen = len & 0x7f;
        len = 0;
        while (lenlen--)
            len = (len << 8) +*s++;
    }

    *ps = s;
    return len;
}

void cert_item(uchar ** ps, int level, int seq)
{
    uchar*  s = *ps;
    uchar   type;
    int     len;
    uchar*  end;
    int     i;
    int     sub_seq;

    type = *s++;
    len = cert_item_len(&s);
    switch (type & 0x1f)
    {
    case 0x02:  // integer
        while (*s == 0)
        {
            s++;
            len--;
        }

        switch (seq)
        {
        case 1:
            dump_hex("static unsigned char modulus[] = {", s, len);
            fprintf(fp, "};\n\n");
            break;

        case 4:
            dump_hex("static unsigned char prime1[] = {", s, len);
            fprintf(fp, "};\n\n");
            break;

        case 5:
            dump_hex("static unsigned char prime2[] = {", s, len);
            fprintf(fp, "};\n\n");
            break;

        case 6:
            dump_hex("static unsigned char exponent1[] = {", s, len);
            fprintf(fp, "};\n\n");
            break;

        case 7:
            dump_hex("static unsigned char exponent2[] = {", s, len);
            fprintf(fp, "};\n\n");
            break;

        case 8:
            dump_hex("static unsigned char coefficient[] = {", s, len);
            fprintf(fp, "};\n\n");
            break;
        }

        s += len;
        break;

    case 0x03:  // Big string
    case 0x04:  // Octet string
    case 0x05:  // NULL
    case 0x06:  // Object
    case 0x13:  // String
    case 0x14:
    case 0x16:
    case 0x17:  // Time
    case 0x18:
        s += len;
        break;

    case 0x10:  // Sequence
        end = s + len;
        sub_seq = 0;
        while (s < end)
            cert_item(&s, level + 1, sub_seq++);
        break;

    case 0x11:  // Set
        end = s + len;
        while (s < end)
            cert_item(&s, level + 1, 0);
        break;

    default:
        if (type == 0xa0)
        {
            //
            //	Explicit
            //
            end = s + len;
            while (s < end)
                cert_item(&s, level + 1, 0);
        }
        else
        {
            fprintf(stderr, "Unexpected type 0x%02x\n", type);
            s += len;
        }
        break;
    }

    *ps = s;
}

static void usage()
{
    fprintf(stderr, "\
usage: make_mykey -n <symbol_name> [-o outfile] infile\n\
");
    exit(1);
}

#define BUFLEN  8192
uchar   buf[BUFLEN];

#define GETARG  ((argv[0][2] != '\0' || argc < 2) ? &argv[0][2] : (argc--, *++argv))

int main(int argc, char ** argv)
{
    uchar*  s;
    char*   outfile;
    char*   name;

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

    fread(buf, 1, BUFLEN, fp);
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
    s = buf;
    cert_item(&s, 0, 0);
    fprintf(fp,
            "CPSPrivateKey %s = {\n\tsizeof(modulus), modulus,\n\tsizeof(prime1), prime1,\n\tsizeof(prime2), prime2,\n\tsizeof(exponent1), exponent1,\n\tsizeof(exponent2), exponent2,\n\tsizeof(coefficient), coefficient\n};\n",
        name);
    if (fp != stdout)
        fclose(fp);
}
