#ifndef PROJECT_H
#define PROJECT_H

extern const unsigned char *INFOMEM;

int erase(void);
int save(char data, unsigned short address);
double readdouble(int offset);
int savedouble(double data, int offset);
char read(unsigned short address);

int run();

#endif // PROJECT_H
