/*
���α׷� ����: ����ǥ�� ������ ��ȣ �˻縦 ����, ���� ���� �Ǻ��ϰ�
			   ���� ǥ�� �������� ��ȯ �� ��� ������� ����ϴ� ���α׷�
�ǽ���¥: 2024-04-03
�й�: 202111001
�̸�: �̵���
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#define MAX_STACK_SIZE 100
#define MAX_INPUT_SIZE 100
typedef double element;

//���ñ���
element data[MAX_STACK_SIZE];
int top;
char postfix[MAX_INPUT_SIZE]; // ���� ǥ�� ������ ������ ���ڿ� �迭
int j = 0;

void error(const char str[])
{
	printf("%s\n", str);
	exit(1);
}

void init_stack() { top = -1; }  // ���� �ʱ�ȭ
double size() { return top + 1; }
double is_empty() { return (top == -1); }
double is_full() { return (top == MAX_STACK_SIZE - 1); }

void push(element e)
{
	if (is_full())
		error("���� ��ȭ ����");
	else
	{
		data[++top] = e;
	}

}

element pop()
{
	if (is_empty())
		error("���� ���� ����");
	else
		return data[top--];
}

element peek()
{
	if (is_empty())
		error("���� ���� ����");
	else
		return data[top];
}

int check_matching(char expr[]) // ����ǥ�� ���� ���� ���� �Ǻ� �Լ�
{
	int i = 0, prev;
	char ch;
	init_stack(); // ���� �ʱ�ȭ
	while (expr[i] != '\0') // ��ȣ ������ �ݺ�
	{
		ch = expr[i++];   // i ����
		if (ch == '[' || ch == '(' || ch == '{') //���� ��ȣ�� ������ ���
			push(ch);                            // ���ÿ� Ǫ��
		else if (ch == ']' || ch == ')' || ch == '}')  // ������ ��ȣ�� ������ ���
		{
			if (is_empty()) // ������ ����ִٸ�
				return 2;  // �����߻� (���̽�2)

			prev = pop(); // ���� ������ ��ȯ , prev: ���� ��ȣ
			if ((ch == ']' && prev != '[') ||           // �Ұ�ȣ, �߰�ȣ, ���ȣ ���ΰ� ���� �������
				(ch == ')' && prev != '(') ||
				(ch == '}' && prev != '{'))
			{
				return 3;                               // �����߻� (���̽�3)
			}
		}
	}
	if (is_empty() == 0)    // ������ ��� ���� �ʴٸ� (���ÿ� ��ȣ�� �����������)
		return 1;           // �����߻� (���̽�1)

	return 0;
}

int precedence(char op)  // �������� �켱������ ��ȯ�ϴ� �Լ� (���ڰ� �������� �켱������ ����)
{
	switch (op) {
	case '(': case ')': return 0;
	case '+': case '-': return 1;
	case '*': case '/': return 2;
	}
	return -1; // �߸��Է��� ���
}
// 8/2-3+(3*2)  , 8/2/3-(3*2+   ,  1/2*4*(1/4)
void infix_to_postfix(char expr[]) // ���� ǥ�� ������ ���� ǥ�� �������� ��ȯ�ϴ� �Լ� (�Ű����� expr�� ����ǥ��� �ٲٴ� �����ʿ�)
{

	int i = 0;
	char c, op; // c: ����, op: ����

	init_stack(); // �����ʱ�ȭ
	while (expr[i] != '\0') // ���ڿ��� ������ ���� �ݺ�
	{
		c = expr[i++];
		if ((c >= '0' && c <= '9')) // 1.�ǿ�����(����) �� ���
		{
			printf("%c", c);       // �׳� ���
			postfix[j++] = c;
		}
		else if (c == '(') // 2.���� ��ȣ�� ���
			push(c);       // ���ÿ� Ǫ��
		else if (c == ')') // 3.������ ��ȣ�� ���
		{
			while (is_empty() == 0) // ��������1: ������ ��������
			{
				op = pop(); // ���ÿ��� ����
				if (op == '(') break; //��������2: ���� ��ȣ�� ������ ����
				else printf("%c", op);

				postfix[j++] = op;

			}
		}
		else if (c == '+' || c == '-' || c == '*' || c == '/') // 4.�������� ���
		{
			while (is_empty() == 0) // ��������1. ������ ��������
			{
				op = peek(); // op: ������ top�� �ִ� ������
				if (precedence(c) <= precedence(op))  // ���� ���õ� �����ڿ� ������ top���ִ� �������� �켱������ ���Ѵ� , top�� �� Ŭ ���
				{
					postfix[j++] = op;
					printf("%c", op);
					pop();             // ��������Ƿ� �����Ѵ�.
				}
				else break; // ��������2. ����top�� �ִ� �������� �켱������ �� ������
			}
			push(c);  // ������ �����ڸ� ���ÿ� �ִ´�.
		}
	}

	// ���ڰ� �� ���� ���� ���ÿ� �ִ°� ó���� ����

	while (is_empty() == 0)   // ������ �������� 
	{
		postfix[j++] = peek();
		printf("%c", pop()); // ���ÿ� �����ִ� �͵��� ���� ����Ѵ�

	}
	printf("\n");

}

double calc_postfix(char expr[]) // �������� ǥ�� ��� �Լ�
{
	char c;
	int i = 0;
	double val, val1, val2;

	init_stack();
	while (expr[i] != '\0') //���ڿ��� ������ ���� �ݺ�
	{
		c = expr[i++];
		if (c >= '0' && c <= '9')  // c�� ���ڶ��
		{
			val = c - '0';
			push(val);
		}
		else if (c == '+' || c == '-' || c == '*' || c == '/') // c�� �����ڶ��
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

	printf("����ǥ�� �����Է�(�������): ");
	scanf("%s", expr);

	// ����ǥ�� ���� �����Ǻ�
	err = check_matching(expr);
	if (err == 0)
	{
		printf("��ȣ���� \n");

		// ���� ǥ�� �������� ��ȯ
		printf("���� ���� ǥ��� ��ȯ: ");

		infix_to_postfix(expr);
		// �� ����
		printf("\n��� ��: %.2lf", calc_postfix(postfix));  // �Ű�����: ����ǥ�� ����
	}
	else
		printf("��ȣ���� (����%d ����)\n", err);
	//doned
}