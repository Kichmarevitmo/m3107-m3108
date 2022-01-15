#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>

long int file_size(FILE *fin) {
    long int save_pos, size_of_file;
    save_pos = ftell(fin); 
    
    fseek(fin, 0L, SEEK_END);
    size_of_file = ftell(fin); 
    fseek(fin, save_pos, SEEK_SET);   
    return (size_of_file);
}

long int paragraph_amount(FILE *fp) {
    long int paragraphs = 0;
    wchar_t ch_UTF16, prevch_UTF16;
    fseek(fp, 0, SEEK_SET);
    int enter_amount = 2;
    while ((ch_UTF16 = fgetc(fp)) != WEOF) {
        if (iswspace(ch_UTF16) || ch_UTF16 == 0) {
            if(ch_UTF16 == L'\n')
                ++enter_amount;
        } else {
            if (prevch_UTF16 == L'\t' || enter_amount > 1)
                paragraphs++;
            enter_amount = 0;
        }
        prevch_UTF16 = ch_UTF16;
    }
    fseek(fp, 0, SEEK_SET);
    return (paragraphs);
}
long int paragraph_amount_2(FILE *fin) {
    long int paragraphs = 0;
    char ch, prevch;
    fseek(fin, 0, SEEK_SET);
    int enter_amount = 2;
    while ((ch = fgetc(fin)) != EOF) {
        if (isspace(ch)) {
            if(ch == '\n')
                ++enter_amount;
        } else {
            if (prevch == '\t' || enter_amount > 1)
                paragraphs++;
            enter_amount = 0;
        }
        prevch = ch;
    }
    fseek(fin, 0, SEEK_SET);
    return (paragraphs);
}

int main() {
    setlocale(LC_ALL, ""); // поддержка Юникода
    FILE *fin;
    wchar_t  ch_UTF16, last_UTF16 = WEOF;
        char  ch, last;
        int  prevch = '\0';
    int lines = 0, words = 0;
    wchar_t prevch_UTF16 = '\n';
    fin = fopen("abc.txt", "r");
    unsigned option;
    printf("Choose:\n");
    printf("1.UTF-16 LE , UTF-16 BE;\n");
    printf("2.ANSI, UTF-8;\n");
    scanf("%d", &option);
    switch (option) {
     case 1: 
    printf("File size=%ld\n", file_size(fin));
    
    while ((ch_UTF16 = fgetc(fin)) != WEOF) {
        last_UTF16 = ch_UTF16;
        if (ch_UTF16 == '\n')
            ++lines;
    }
    rewind(fin);
    while ((ch_UTF16 = fgetc(fin)) != WEOF) {
        if(iswalnum(ch_UTF16) && (iswspace(prevch_UTF16)))
            words++;
        if(ch_UTF16 != 0)
            prevch_UTF16 = ch_UTF16;
    }
    if (last_UTF16 == WEOF)
        printf("File is empty!\n");
    else if (last_UTF16 != L'\n') {
        ++lines;
        printf("Number of lines: %d\n", lines);
    }
    printf("Number of words: %d\n", words);
    printf("Number of paragraphs: %ld\n", paragraph_amount(fin));
    break;
    case 2:
    printf("File size=%ld\n", file_size(fin));

    while ((ch = fgetc(fin)) != EOF) {
        last = ch;
        if (ch == '\n')
            ++lines;
    }
    rewind(fin);
    while ((ch = fgetc(fin)) != EOF) {
        if (isspace(ch)) {
        } else {
            if (prevch == '\0' || isspace(prevch))
                words++;
        }
        prevch = ch;
    }
    if (last == EOF)
        printf("File is empty!");
    else if (last != '\n') {
        ++lines;
        printf("Number of lines: %d\n", lines);
    }
    printf("Number of words: %d\n", words);

    printf("Number of paragraphs: %ld\n", paragraph_amount_2(fin));
 break;
    }
    fclose(fin);
    return 0;
}



