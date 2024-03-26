bool isalpha(unsigned char c) {
    if ((c > 64 && c < 91) || (c > 96 && c < 123))
        return true;
    return false;
}

unsigned char tolower(unsigned char c) {
    if (c > 64 && c < 91)
        c += 32;
    return c;
}
