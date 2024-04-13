#include <errno.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

typedef struct {
  bool number_nonblank;
  bool mark_endl;
  bool number_all;
  bool squeeze;
  bool tab;
  bool print_nonprintable;
} config;

void set_table(const char *table[static 256]) {
  const char *original_table[] = {
      "\x00", "\x01", "\x02", "\x03", "\x04", "\x05", "\x06", "\x07", "\x08",
      "\x09", "\x0a", "\x0b", "\x0c", "\x0d", "\x0e", "\x0f", "\x10", "\x11",
      "\x12", "\x13", "\x14", "\x15", "\x16", "\x17", "\x18", "\x19", "\x1a",
      "\x1b", "\x1c", "\x1d", "\x1e", "\x1f", "\x20", "\x21", "\x22", "\x23",
      "\x24", "\x25", "\x26", "\x27", "\x28", "\x29", "\x2a", "\x2b", "\x2c",
      "\x2d", "\x2e", "\x2f", "\x30", "\x31", "\x32", "\x33", "\x34", "\x35",
      "\x36", "\x37", "\x38", "\x39", "\x3a", "\x3b", "\x3c", "\x3d", "\x3e",
      "\x3f", "\x40", "\x41", "\x42", "\x43", "\x44", "\x45", "\x46", "\x47",
      "\x48", "\x49", "\x4a", "\x4b", "\x4c", "\x4d", "\x4e", "\x4f", "\x50",
      "\x51", "\x52", "\x53", "\x54", "\x55", "\x56", "\x57", "\x58", "\x59",
      "\x5a", "\x5b", "\x5c", "\x5d", "\x5e", "\x5f", "\x60", "\x61", "\x62",
      "\x63", "\x64", "\x65", "\x66", "\x67", "\x68", "\x69", "\x6a", "\x6b",
      "\x6c", "\x6d", "\x6e", "\x6f", "\x70", "\x71", "\x72", "\x73", "\x74",
      "\x75", "\x76", "\x77", "\x78", "\x79", "\x7a", "\x7b", "\x7c", "\x7d",
      "\x7e", "\x7f", "\x80", "\x81", "\x82", "\x83", "\x84", "\x85", "\x86",
      "\x87", "\x88", "\x89", "\x8a", "\x8b", "\x8c", "\x8d", "\x8e", "\x8f",
      "\x90", "\x91", "\x92", "\x93", "\x94", "\x95", "\x96", "\x97", "\x98",
      "\x99", "\x9a", "\x9b", "\x9c", "\x9d", "\x9e", "\x9f", "\xa0", "\xa1",
      "\xa2", "\xa3", "\xa4", "\xa5", "\xa6", "\xa7", "\xa8", "\xa9", "\xaa",
      "\xab", "\xac", "\xad", "\xae", "\xaf", "\xb0", "\xb1", "\xb2", "\xb3",
      "\xb4", "\xb5", "\xb6", "\xb7", "\xb8", "\xb9", "\xba", "\xbb", "\xbc",
      "\xbd", "\xbe", "\xbf", "\xc0", "\xc1", "\xc2", "\xc3", "\xc4", "\xc5",
      "\xc6", "\xc7", "\xc8", "\xc9", "\xca", "\xcb", "\xcc", "\xcd", "\xce",
      "\xcf", "\xd0", "\xd1", "\xd2", "\xd3", "\xd4", "\xd5", "\xd6", "\xd7",
      "\xd8", "\xd9", "\xda", "\xdb", "\xdc", "\xdd", "\xde", "\xdf", "\xe0",
      "\xe1", "\xe2", "\xe3", "\xe4", "\xe5", "\xe6", "\xe7", "\xe8", "\xe9",
      "\xea", "\xeb", "\xec", "\xed", "\xee", "\xef", "\xf0", "\xf1", "\xf2",
      "\xf3", "\xf4", "\xf5", "\xf6", "\xf7", "\xf8", "\xf9", "\xfa", "\xfb",
      "\xfc", "\xfd", "\xfe", "\xff"};
  memcpy(table, original_table, sizeof original_table);
}

void set_endl(const char *table[static 256]) { table['\n'] = "$\n"; }

void set_tab(const char *table[static 256]) { table['\t'] = "^I"; }
void set_nonprintable(const char *table[static 256]) {
  const char *v_table[] = {
      "^@",   "^A",   "^B",   "^C",    "^D",   "^E",   "^F",   "^G",   "^H",
      "\t",   "\n",   "^K",   "^L",    "^M",   "^N",   "^O",   "^P",   "^Q",
      "^R",   "^S",   "^T",   "^U",    "^V",   "^W",   "^X",   "^Y",   "^Z",
      "^[",   "^\\",  "^]",   "^^",    "^_",   " ",    "!",    "\"",   "#",
      "$",    "%",    "&",    "'",     "(",    ")",    "*",    "+",    ",",
      "-",    ".",    "/",    "0",     "1",    "2",    "3",    "4",    "5",
      "6",    "7",    "8",    "9",     ":",    ";",    "<",    "=",    ">",
      "?",    "@",    "A",    "B",     "C",    "D",    "E",    "F",    "G",
      "H",    "I",    "J",    "K",     "L",    "M",    "N",    "O",    "P",
      "Q",    "R",    "S",    "T",     "U",    "V",    "W",    "X",    "Y",
      "Z",    "[",    "\\",   "]",     "^",    "_",    "`",    "a",    "b",
      "c",    "d",    "e",    "f",     "g",    "h",    "i",    "j",    "k",
      "l",    "m",    "n",    "o",     "p",    "q",    "r",    "s",    "t",
      "u",    "v",    "w",    "x",     "y",    "z",    "{",    "|",    "}",
      "~",    "^?",   "M-^@", "M-^A",  "M-^B", "M-^C", "M-^D", "M-^E", "M-^F",
      "M-^G", "M-^H", "M-^I", "M-^J",  "M-^K", "M-^L", "M-^M", "M-^N", "M-^O",
      "M-^P", "M-^Q", "M-^R", "M-^S",  "M-^T", "M-^U", "M-^V", "M-^W", "M-^X",
      "M-^Y", "M-^Z", "M-^[", "M-^\\", "M-^]", "M-^^", "M-^_", "M- ",  "M-!",
      "M-\"", "M-#",  "M-$",  "M-%",   "M-&",  "M-'",  "M-(",  "M-)",  "M-*",
      "M-+",  "M-,",  "M--",  "M-.",   "M-/",  "M-0",  "M-1",  "M-2",  "M-3",
      "M-4",  "M-5",  "M-6",  "M-7",   "M-8",  "M-9",  "M-:",  "M-;",  "M-<",
      "M-=",  "M->",  "M-?",  "M-@",   "M-A",  "M-B",  "M-C",  "M-D",  "M-E",
      "M-F",  "M-G",  "M-H",  "M-I",   "M-J",  "M-K",  "M-L",  "M-M",  "M-N",
      "M-O",  "M-P",  "M-Q",  "M-R",   "M-S",  "M-T",  "M-U",  "M-V",  "M-W",
      "M-X",  "M-Y",  "M-Z",  "M-[",   "M-\\", "M-]",  "M-^",  "M-_",  "M-`",
      "M-a",  "M-b",  "M-c",  "M-d",   "M-e",  "M-f",  "M-g",  "M-h",  "M-i",
      "M-j",  "M-k",  "M-l",  "M-m",   "M-n",  "M-o",  "M-p",  "M-q",  "M-r",
      "M-s",  "M-t",  "M-u",  "M-v",   "M-w",  "M-x",  "M-y",  "M-z",  "M-{",
      "M-|",  "M-}",  "M-~",  "M-^?"};
  memcpy(table, v_table, sizeof v_table);
}

config cat_rflags_func(int argc, char **argv) {
  struct option long_option[] = {{"number-nonblank", 0, NULL, 'b'},
                                 {"number", 0, NULL, 'n'},
                                 {"squeeze-blank", 0, NULL, 's'},
                                 {NULL, 0, NULL, 0}};
  int current_flag = 0;
  config flags = {false, false, false, false, false, false};
  current_flag = getopt_long(argc, argv, "bevEnstT", long_option, NULL);
  for (; current_flag != -1;
       current_flag = getopt_long(argc, argv, "bevEnstT", long_option, NULL)) {
    switch (current_flag) {
      case 'b':
        flags.number_nonblank = true;
        break;
      case 'e':
        flags.mark_endl = true;
        flags.print_nonprintable = true;
        break;
      case 'v':
        flags.print_nonprintable = true;
        break;
      case 'E':
        flags.mark_endl = true;
        break;
      case 'n':
        flags.number_all = true;
        break;
      case 's':
        flags.squeeze = true;
        break;
      case 't':
        flags.tab = true;
        flags.print_nonprintable = true;
        break;
      case 'T':
        flags.tab = true;
        break;
      default:;
    };
  };
  return flags;
}

void print_func(FILE *file, config flags, const char *table[static 256]) {
  int ch = 0;
  int number_line = 1;
  int last_ch = '\n';
  bool sq_tmp = false;
  while (fread(&ch, 1, 1, file) > 0) {
    if (last_ch == '\n') {
      if (flags.squeeze && ch == '\n') {
        if (sq_tmp) continue;
        sq_tmp = true;
      } else
        sq_tmp = false;
      if (flags.number_nonblank) {
        if (ch != '\n') {
          printf("%6d\t", number_line);
          number_line = number_line + 1;
        }
      } else if (flags.number_all) {
        printf("%6d\t", number_line);
        number_line = number_line + 1;
      }
    }
    if (!*table[ch])
      printf("%c", '\0');
    else
      printf("%s", table[ch]);
    last_ch = ch;
  }
}

void s21_cat_func(int argc, char **argv, config flags,
                  const char *table[static 256]) {
  for (char **filename = &argv[1], **end = &argv[argc]; filename != end;
       ++filename) {
    if (**filename == '-') continue;
    FILE *file = fopen(*filename, "rb");
    if (errno) {
      fprintf(stderr, "error");
      perror(*filename);
      continue;
    }
    print_func(file, flags, table);
    fclose(file);
  }
}

int main(int argc, char **argv) {
  config flags = cat_rflags_func(argc, argv);
  const char *table[256];
  set_table(table);
  if (flags.print_nonprintable) set_nonprintable(table);
  if (flags.mark_endl) set_endl(table);
  if (flags.tab) set_tab(table);
  s21_cat_func(argc, argv, flags, table);
  return 0;
}
