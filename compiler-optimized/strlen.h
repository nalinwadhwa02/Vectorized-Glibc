unsigned long co_strlen(char* str){
    int i=0;
    while(str[i++]!='\0'){}
    return i;
}