/*
	gcc -o rev_polish_calc rev_polish_calc.c -lm
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>


#define NUMBER		'\0'
#define SIN		-2
#define COS		-3
#define POW		-4
#define EXP		-5
#define VAR		-6
#define EINVAL		-22

#define MAXOP		100
#define BUFFSIZE	100
#define MAXVAL		100
#define MAXVAR		26

#define M_PI 3.14159265358979323846
bool has_variable = 0;

int getop(char []);
void push(double);
double pop(void);
int getch(void);
void ungetch(int);
int check_math(char c);
int getvars(int v[]);
void printvars(int v[]);

int buffer[BUFFSIZE];
int bufp = 0;

int sp = 0;
double val[MAXVAL];

int main()
{
	int v[MAXVAR] = {0};
	char s[MAXOP];
	int isvar = 0;
	double op2;
	int type;

	memset(v, 0, MAXVAR);

	printf("Enter Variables [A-Z] <10 D 20 C> or press [Enter] to skip...\n");
	while(!(getvars(v)))
		;
	printvars(v);

	printf("Enter expression..\n");
	while((type = getop(s)) != EOF) {
		switch(type) {
			case NUMBER:
				push(atof(s));
				break;
			case VAR:
				push(v[s[0] - 'A']);
				break;
			case '=':
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
			case SIN:
				op2 = pop();
				push(sin(op2 * M_PI / 180.0));
				break;
			case COS:
				op2 = pop();
				push(cos(op2 * M_PI / 180.0));
				break;
			case POW:
				op2 = pop();
				push(pow(pop(),  op2));
				break;
			case EXP:
				push(exp(pop()));
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
	int i = 0, c, next, next_mth;

	while((s[0] = c = getch()) == ' ' || c == '\t')
		;

	/* If not digit */
	s[1] = '\0';

	if (isupper(c) && has_variable) {
		s[0] = c;
		return VAR;
	} else {
		printf("Invalid usage of variables please run again\n");
	}

	if (!isdigit(c) && c != '.' && c != '-') {
		if (isalpha(c)) {
			int ret_math;
			ret_math = check_math(c);
			s[3] = '\0';
			return ret_math;
		} else {
			return c;
		}
	}

	if (c == '-') {
		next = getch();
		if (!isdigit(next) && next != '.') {
			ungetch(next);
			return '-';
		}
		s [i = 1] = c = next;
	}

	if (isdigit(c))
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

int check_math(char c)
{
	int ret = -EINVAL, i = 1;
	char str[3];

	str[0] = c;

	while (i < 3)
	{
		c = getch();
		str[i] = c;
		i++;
	}

	if (!strcmp(str, "sin")) {
		ret = SIN;
	} else if (!strcmp(str, "cos")) {
		ret = COS;
	} else if (!strcmp(str, "pow")) {
		ret = POW;
	} else if (!strcmp(str, "exp")) {
		ret = EXP;
	} else {
		printf("Math operation is invalid: %s.\n", str);
	}

	return ret;
}

/* Return 1 if not getting otherwise */
int getvars(int vars[])
{
	int i = 0, c;
	char s[20];

	if ((s[0] = c = getch()) == '\n') { return 1; }
	else { ungetch(c); }

	while((s[0] = c = getch()) == ' ' || c == '\t')
		;

	s[1] = '\0';

	// Getting digit inot s[]
	if (isdigit(c))
		while(isdigit(s[++i] = c = getch()))
			;

	// Getting Variables
	if (!isalpha(c)) {
		while(isalpha(c = getch())) {
			if (isupper(c)) {
				vars[c - 'A'] = atoi(s);
				has_variable = 1;
			} else {
				printf("[%c] is not upper case!\n", c);
				continue;
			}
		}
	}

	s[i] = '\0';
	if (c == '\n')	{ return 1; }

	return 0;
}

void printvars(int v[]) {

	printf("Variables -> ");
	for (int i = 0; i < MAXVAR; i++) {
		printf("%d ", v[i]);
	}
	printf("\n");
}
