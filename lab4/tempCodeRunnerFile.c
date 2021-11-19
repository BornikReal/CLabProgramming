 * fgetc(musicf);
    size_of_tag += 128 * 128 * fgetc(musicf);
    size_of_tag += 128 * fgetc(musicf);
    size_of_tag += fgetc(musicf);
    printf("%d\n", size_of_tag