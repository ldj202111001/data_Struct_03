/*
프로그램 내용: 중위표기 수식의 괄호 검사를 수행, 오류 여부 판별하고
			   후위 표기 수식으로 변환 후 계산 결과값을 출력하는 프로그램
실습날짜: 2024-04-03
학번: 202111001
이름: 이동재
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#define MAX_STACK_SIZE 100
#define MAX_INPUT_SIZE 100
typedef double element;

//스택구현
element data[MAX_STACK_SIZE];
int top;
char postfix[MAX_INPUT_SIZE]; // 후위 표기 수식을 저장할 문자열 배열
int j = 0;

void error(const char str[])
{
	printf("%s\n", str);
	exit(1);
}

void init_stack() { top = -1; }  // 스택 초기화
double size() { return top + 1; }
double is_empty() { return (top == -1); }
double is_full() { return (top == MAX_STACK_SIZE - 1); }

void push(element e)
{
	if (is_full())
		error("스택 포화 에러");
	else
	{
		data[++top] = e;
	}

}

element pop()
{
	if (is_empty())
		error("스택 공백 에러");
	else
		return data[top--];
}

element peek()
{
	if (is_empty())
		error("스택 공백 에러");
	else
		return data[top];
}

int check_matching(char expr[]) // 중위표기 수식 오류 여부 판별 함수
{
	int i = 0, prev;
	char ch;
	init_stack(); // 스택 초기화
	while (expr[i] != '\0') // 괄호 끝까지 반복
	{
		ch = expr[i++];   // i 증가
		if (ch == '[' || ch == '(' || ch == '{') //여는 괄호를 만났을 경우
			push(ch);                            // 스택에 푸쉬
		else if (ch == ']' || ch == ')' || ch == '}')  // 닫히는 괄호를 만났을 경우
		{
			if (is_empty()) // 스택이 비어있다면
				return 2;  // 에러발생 (케이스2)

			prev = pop(); // 스택 삭제후 반환 , prev: 이전 괄호
			if ((ch == ']' && prev != '[') ||           // 소괄호, 중괄호, 대괄호 서로가 맞지 않을경우
				(ch == ')' && prev != '(') ||
				(ch == '}' && prev != '{'))
			{
				return 3;                               // 에러발생 (케이스3)
			}
		}
	}
	if (is_empty() == 0)    // 스택이 비어 있지 않다면 (스택에 괄호가 남아있을경우)
		return 1;           // 에러발생 (케이스1)

	return 0;
}

int precedence(char op)  // 연산자의 우선순위를 반환하는 함수 (숫자가 높을수록 우선순위가 높다)
{
	switch (op) {
	case '(': case ')': return 0;
	case '+': case '-': return 1;
	case '*': case '/': return 2;
	}
	return -1; // 잘못입력한 경우
}
// 8/2-3+(3*2)  , 8/2/3-(3*2+   ,  1/2*4*(1/4)
void infix_to_postfix(char expr[]) // 중위 표기 수식을 후위 표기 수식으로 변환하는 함수 (매개변수 expr을 후위표기로 바꾸는 과정필요)
{

	int i = 0;
	char c, op; // c: 현재, op: 스택

	init_stack(); // 스택초기화
	while (expr[i] != '\0') // 문자열이 끝날때 까지 반복
	{
		c = expr[i++];
		if ((c >= '0' && c <= '9')) // 1.피연산자(숫자) 일 경우
		{
			printf("%c", c);       // 그냥 출력
			postfix[j++] = c;
		}
		else if (c == '(') // 2.여는 괄호일 경우
			push(c);       // 스택에 푸쉬
		else if (c == ')') // 3.닫히는 괄호일 경우
		{
			while (is_empty() == 0) // 종료조건1: 스택이 빌때까지
			{
				op = pop(); // 스택에서 꺼냄
				if (op == '(') break; //종료조건2: 여는 괄호를 만날때 까지
				else printf("%c", op);

				postfix[j++] = op;

			}
		}
		else if (c == '+' || c == '-' || c == '*' || c == '/') // 4.연산자일 경우
		{
			while (is_empty() == 0) // 종료조건1. 스택이 빌때까지
			{
				op = peek(); // op: 스택의 top에 있는 연산자
				if (precedence(c) <= precedence(op))  // 현재 선택된 연산자와 스택의 top에있는 연산자의 우선순위를 비교한다 , top이 더 클 경우
				{
					postfix[j++] = op;
					printf("%c", op);
					pop();             // 출력했으므로 삭제한다.
				}
				else break; // 종료조건2. 스택top에 있는 연산자의 우선순위가 더 낮을때
			}
			push(c);  // 선택한 연산자를 스택에 넣는다.
		}
	}

	// 문자가 다 끝남 이제 스택에 있는거 처리할 차례

	while (is_empty() == 0)   // 스택이 빌때까지 
	{
		postfix[j++] = peek();
		printf("%c", pop()); // 스택에 남아있는 것들을 전부 출력한다

	}
	printf("\n");

}

double calc_postfix(char expr[]) // 후위수식 표기 계산 함수
{
	char c;
	int i = 0;
	double val, val1, val2;

	init_stack();
	while (expr[i] != '\0') //문자열이 끝날때 까지 반복
	{
		c = expr[i++];
		if (c >= '0' && c <= '9')  // c가 숫자라면
		{
			val = c - '0';
			push(val);
		}
		else if (c == '+' || c == '-' || c == '*' || c == '/') // c가 연산자라면
		{
			val2 = pop();
			val1 = pop();
			switch (c)
			{
			case '+': push(val1 + val2); break;
			case '-': push(val1 - val2); break;
			case '*': push(val1 * val2); break;
			case '/': push(val1 / val2); break;
			}
		}
	}

	return pop();

}



int main()
{
	char expr[MAX_INPUT_SIZE];
	double result;
	int err;

	printf("중위표기 수식입력(띄어쓰기없이): ");
	scanf("%s", expr);

	// 중위표기 수식 오류판별
	err = check_matching(expr);
	if (err == 0)
	{
		printf("괄호정상 \n");

		// 후위 표기 수식으로 변환
		printf("후위 수식 표기로 변환: ");

		infix_to_postfix(expr);
		// 값 도출
		printf("\n계산 값: %.2lf", calc_postfix(postfix));  // 매개변수: 후위표기 수식
	}
	else
		printf("괄호오류 (조건%d 위반)\n", err);
	//doned
}