char* string_parser(char* str)
{
int i = 0;
char str_p[256][256];
char *p = strtok(str, " ");
while(p != NULL) 
	{
		strcpy(str_p[i], p);
		i++;
		p = strtok(NULL, " ");
	}
return str_p;
}