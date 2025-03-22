#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXOP           100
#define NUMBER          '\0'
#define BUFFSIZE        100
#define MAXVAL          100

int getop(char []);
void push(double);
double pop(void);
int getch(void);
void ungetch(int);

int buffer[BUFFSIZE];
int bufp = 0;

int sp = 0;
double val[MAXVAL];

int main()
{
	int type;
	double op2;
	char s[MAXOP];

	while((type = getop(s)) != EOF) {
		switch(type) {
			case NUMBER:
				push(atof(s));
				break;
			case '+':
				push(pop() + pop());
				break;
				case '-':
				op2 = pop();
				push(pop() - op2);
				break;
			case '*':
				push(pop() * pop());
				break;
			case  '/':
				op2 = pop();
				if (op2 != 0.0)
					push(pop() / op2);
				else
					printf("error: zero divisor\n");
				break;
			case '%':
				op2 = pop();
				push(((int)pop()) % (int)op2);
				break;
			case '?':
				if (sp > 0)
					printf("Prints the TOP: %g\n", val[sp - 1]);
				else
					printf("error: stack is empty (?)\n");
				break;
			case 'd':
				if (sp > 0) {
					push(val[sp - 1]);
					printf("Dublicated top element: %g\n", val[sp - 1]);
				} else
					printf("error: stack is empty (d)\n");
				break;
			case 's':
				if (sp > 1) {
					printf("Swapping... %g and %g\n", val[sp - 1], val[sp - 2]);
					double temp = val[sp - 1];
					val[sp - 1] = val[sp - 2];
					val[sp - 2] = temp;
					printf("Swapped... %g and %g\n", val[sp - 1], val[sp - 2]);
				} else {
					printf("error: not enough elements to swap\n");
				}
				break;
			case 'c':
				if (sp > 0) {
					printf("Clearing stack...\n");
					sp = 0;
				} else {
					printf("error: not element to clear\n");
				}
				break;
			case '\n':
				printf("Result: %.8g\n", pop()); // Print final result
				break;
			default:
				printf("error: unknown command %s\n", s);
				break;
		}
	}

	return 0;
}

void push(double f)
{
	if (sp < MAXVAL)
		val[sp++] = f;
	else
		printf("error: stack full, can't push %g\n",f);
}

double pop(void)
{
	if (sp > 0)
		return val[--sp];
	else {
		printf("error: stack is empty");
		return 0.0;
	}
}

int getop(char s[])
{
	int i = 0, c, next;

	while((s[0] = c = getch()) == ' ' || c == '\t')
		;

	/* If not digit */
	s[1] = '\0';

	if (!isdigit(c) && c != '.' && c != '-')
		return c;

	if (c == '-') {
		next = getch();
		if (!isdigit(next) && next != '.') {
			ungetch(next);
			return '-';
		}
		s [i = 1] = c = next;
	}

	if ( isdigit(c))
		while(isdigit(s[++i] = c = getch()))
			;

	if (c == '.')
		while (isdigit(s[++i] = c = getchar()))
			;
	s[i] = '\0';
	if (c != EOF)
		ungetch(c);
	return NUMBER;
}

int getch(void)
{
	return bufp > 0 ? buffer[--bufp] : getchar();
}

void ungetch(int c)
{
	if (bufp > BUFFSIZE)
		printf("ungetch: too many characters\n");
	else
		buffer[bufp++] = c;
}
