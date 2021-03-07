char *
str_cut(char str[], int slice_from, int slice_to){
  if (str[0] == '\0')
    return NULL;

  char *buffer;
  size_t str_len, buffer_len;

  if (slice_to < 0 && slice_from > slice_to) {
    str_len = strlen(str);
    if (abs(slize_to) > str_len - 1)
      return NULL;

    if (abs(slize_from) > strlen)
      slice_from = (-1) * str_len;

    buffer_len = slice_to - slice_from;

  }

}
