#define _GNU_SOURCE
#include <errno.h>
#include <getopt.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char *pattern;
  int size_pattern;
  bool e_pattern;
  bool f_pattern;
  bool invert;
  bool count_line;
  bool match_file;
  bool number_line;
  bool few_files;
  bool hide_names;
  bool hide_errors;
  bool print_only_match;
  int i_flag;
} config;

void *safe_malloc(int size) {
  void *tmp;
  tmp = malloc(size);
  if (!tmp) exit(1);
  return tmp;
}

void add_pattern_func(char **string, int *size, char const *tmp, bool check) {
  if (!check) {
    int tmp_size = strlen(tmp);
    *string = safe_malloc(tmp_size + 1);
    memcpy(*string, tmp, tmp_size);
    (*string)[tmp_size] = '\0';
    *size = tmp_size + 1;
  } else {
    int tmp_size = strlen(tmp);
    *string = realloc(*string, *size + tmp_size + 1);
    (*string)[*size - 1] = '|';
    memcpy(*string + *size, tmp, tmp_size);
    (*string)[tmp_size + *size] = '\0';
    *size = *size + tmp_size + 1;
  }
}

void read_from_file_func(const char *filename, config *flags, bool ef_check) {
  FILE *file = NULL;
  file = fopen(filename, "rb");
  if (file == NULL) {
    fprintf(stderr, "error ");
    perror(filename);
    if (ef_check) free(flags->pattern);
    exit(1);
  }
  char *line = NULL;
  size_t len = 0;
  char *pattern = NULL;
  while (getline(&line, &len, file) > 0) {
    if (*line == '\n') continue;
    pattern = strtok(line, "\n");
    add_pattern_func(&flags->pattern, &flags->size_pattern, pattern, ef_check);
    ef_check = true;
  }
  fclose(file);
  free(line);
}

void init_flags_func(int argc, char **argv, config *flags) {
  int current_flag = 0;
  bool ef_check = false;
  while ((current_flag = getopt_long(argc, argv, "e:f:ivclnhso", 0, 0)) != -1) {
    switch (current_flag) {
      case 'e':
        flags->e_pattern = true;
        add_pattern_func(&flags->pattern, &flags->size_pattern, optarg,
                         ef_check);
        ef_check = true;
        break;
      case 'f':
        flags->f_pattern = true;
        read_from_file_func(optarg, flags, ef_check);
        ef_check = true;
        break;
      case 'i':
        flags->i_flag = REG_ICASE;
        break;
      case 'v':
        flags->invert = true;
        break;
      case 'c':
        flags->count_line = true;
        break;
      case 'l':
        flags->match_file = true;
        break;
      case 'n':
        flags->number_line = true;
        break;
      case 'h':
        flags->hide_names = true;
        break;
      case 's':
        flags->hide_errors = true;
        break;
      case 'o':
        flags->print_only_match = true;
        break;
      default:;
    };
  };
}

void print_func(config *flags, char const *filename, int count,
                char const *line, int count_line, regmatch_t *match) {
  if (flags->match_file) {
    printf("%s\n", filename);
  } else if (flags->count_line) {
    if (flags->few_files && !flags->hide_names)
      printf("%s:%d\n", filename, count_line);
    else
      printf("%d\n", count_line);
  } else if (flags->print_only_match) {
    if (flags->few_files && !flags->hide_names) {
      if (flags->number_line)
        printf("%s:%d:%.*s\n", filename, count,
               (int)(match->rm_eo - match->rm_so), line + match->rm_so);
      else
        printf("%s:%.*s\n", filename, (int)(match->rm_eo - match->rm_so),
               line + match->rm_so);
    } else {
      if (flags->number_line)
        printf("%d:%.*s\n", count, (int)(match->rm_eo - match->rm_so),
               line + match->rm_so);
      else
        printf("%.*s\n", (int)(match->rm_eo - match->rm_so),
               line + match->rm_so);
    }
  } else if (flags->number_line) {
    if (flags->few_files && !flags->hide_names)
      printf("%s:%d:%s", filename, count, line);
    else
      printf("%d:%s", count, line);
  } else if (flags->few_files && !flags->hide_names)
    printf("%s:%s", filename, line);
  else
    printf("%s", line);
}

void preprint_func(FILE *file, config *flags, regex_t *preg,
                   char const *filename) {
  char *line = NULL;
  size_t len = 0;
  regmatch_t match = {0, 0};
  int count = 0;
  int count_line = 0;
  while (getline(&line, &len, file) > 0) {
    count = count + 1;
    int reg_output = 0;
    reg_output = regexec(preg, line, 1, &match, 0);
    if (flags->invert && reg_output) {
      if (flags->match_file) {
        print_func(flags, filename, count, line, count_line, &match);
        break;
      }
      if (flags->count_line) {
        count_line = count_line + 1;
        continue;
      }
      if (flags->print_only_match) break;
      print_func(flags, filename, count, line, count_line, &match);
    }
    if (!flags->invert && !reg_output) {
      if (flags->match_file) {
        print_func(flags, filename, count, line, count_line, &match);
        break;
      }
      if (flags->count_line) {
        count_line = count_line + 1;
        continue;
      }
      if (flags->print_only_match) {
        print_func(flags, filename, count, line, count_line, &match);
        char *line_o_flag = line + match.rm_eo;
        while (!regexec(preg, line_o_flag, 1, &match, 0)) {
          print_func(flags, filename, count, line_o_flag, count_line, &match);
          line_o_flag = line_o_flag + match.rm_eo;
        }
      } else
        print_func(flags, filename, count, line, count_line, &match);
    }
  }
  if (flags->count_line && !flags->match_file)
    print_func(flags, filename, count, line, count_line, &match);
  free(line);
}

int s21_grep_func(int argc, char **argv, config *flags) {
  char **pattern = NULL;
  char **filename = NULL;
  regex_t preg = {0};
  if (!flags->e_pattern && !flags->f_pattern) {
    pattern = argv;
    filename = argv + 1;
  } else {
    pattern = &(flags->pattern);
    filename = argv;
  }
  if (*pattern == NULL) {
    fprintf(stderr, "[[[[patterns not found]]]]\n");
    return 1;
  }
  if (regcomp(&preg, *pattern, flags->i_flag | REG_EXTENDED)) {
    free(flags->pattern);
    fprintf(stderr, "!!!regex error!!!\n");
    return 1;
  }
  free(flags->pattern);
  if ((argc > 2 && !flags->e_pattern) || (argc > 1 && flags->e_pattern))
    flags->few_files = true;
  FILE *file = NULL;
  for (; *filename; filename++) {
    file = fopen(*filename, "rb");
    if (file == NULL) {
      if (!flags->hide_errors) {
        fprintf(stderr, "cant open file \n");
        perror(*filename);
      }
      continue;
    }
    preprint_func(file, flags, &preg, *filename);
    fclose(file);
  }
  regfree(&preg);
  return 0;
}

int main(int argc, char **argv) {
  config flags = {NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  int check = 0;
  init_flags_func(argc, argv, &flags);
  argv += optind;
  argc -= optind;
  check = s21_grep_func(argc, argv, &flags);
  if (check) return 1;
  return 0;
}
