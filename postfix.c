/** 
 * postfix.c
 *
 * School of Computer Science, 
 * Chungbuk National University
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_STACK_SIZE 10
#define MAX_EXPRESSION_SIZE 20

// stack 내에서 우선순위, lparen = 0 가장 낮음 */
typedef enum{
	lparen = 0,  // ( 왼쪽 괄호 */
	rparen = 9,  // ) 오른쪽 괄호*/
	times = 7,   // * 곱셈 */
	divide = 6,  // / 나눗셈 */
	plus = 5,    // + 덧셈 */
	minus = 4,   // - 뺄셈 */
	operand = 1 // 피연산자 */
} precedence;

char infixExp[MAX_EXPRESSION_SIZE];   	// infix expression을 저장하는 배열 */
char postfixExp[MAX_EXPRESSION_SIZE];	// postfix로 변경된 문자열을 저장하는 배열 */
char postfixStack[MAX_STACK_SIZE];	// postfix로 변환을 위해 필요한 스택 */
int evalStack[MAX_STACK_SIZE];		// 계산을 위해 필요한 스택 */

int postfixStackTop = -1;  // postfixStack용 top */
int evalStackTop = -1;	   // evalStack용 top */

int evalResult = 0;	   // 계산 결과를 저장 */

void postfixpush(char x);
char postfixPop();
void evalPush(int x);
int evalPop();
void getInfix();
precedence getToken(char symbol);
precedence getPriority(char x);
void charCat(char* c);
void toPostfix();
void debug();
void reset();
void evaluation();

int main()
{
	char command;

	do{
		printf("----------------------------------------------------------------\n");
		printf("               Infix to Postfix, then Evaluation               \n");
		printf("----------------------------------------------------------------\n");
		printf(" Infix=i,   Postfix=p,  Eval=e,   Debug=d,   Reset=r,   Quit=q \n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);

		switch(command) {
		case 'i': case 'I':
			getInfix();
			break;
		case 'p': case 'P':
			toPostfix();
			break;
		case 'e': case 'E':
			evaluation();
			break;
		case 'd': case 'D':
			debug();
			break;
		case 'r': case 'R':
			reset();
			break;
		case 'q': case 'Q':
			break;
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');
    printf("[----- [전준석] [2021041051] -----]\n");
	return 1;
}

void postfixPush(char x)
{
    postfixStack[++postfixStackTop] = x;
}

char postfixPop()
{
    char x;
    if(postfixStackTop == -1)
        return '\0';
    else {
    	x = postfixStack[postfixStackTop--];
    }
    return x;
}

void evalPush(int x)
{
    evalStack[++evalStackTop] = x;
}

int evalPop()
{
    if(evalStackTop == -1)
        return -1;
    else
        return evalStack[evalStackTop--];
}

/**
 * infix expression을 입력받는다.
 * infixExp에는 입력된 값을 저장한다.
 */
void getInfix()
{
    printf("Type the expression >>> ");
    scanf("%s",infixExp);
}

precedence getToken(char symbol)
{
	switch(symbol) {
	case '(' : return lparen;
	case ')' : return rparen;
	case '+' : return plus;
	case '-' : return minus;
	case '/' : return divide;
	case '*' : return times;
	default : return operand;
	}
}

precedence getPriority(char x)
{
	return getToken(x);
}

/**
 * 문자하나를 전달받아, postfixExp에 추가
 */
void charCat(char* c)
{
	if (postfixExp == '\0')
		strncpy(postfixExp, c, 1);
	else
		strncat(postfixExp, c, 1);
}

/**
 * infixExp의 문자를 하나씩 읽어가면서 stack을 이용하여 postfix로 변경한다.
 * 변경된 postfix는 postFixExp에 저장된다.
 */
void toPostfix()
{
	// infixExp의 문자 하나씩을 읽기위한 포인터 
	char *exp = infixExp;
	char x; // 문자하나를 임시로 저장하기 위한 변수 
    
    while (*exp != '\0')    // exp를 증가시켜가면서, 문자를 읽고 postfix로 변경 
        {
            // 현재 문자가 피연산자인 경우
            if (getToken(*exp) == operand)
            {
                charCat(exp); // 피연산자를 postfixExp에 추가
            }
            else if (getToken(*exp) == lparen)  // 현재 문자가 '('인 경우
            {
                postfixPush(*exp); // '('를 postfix 스택에 push
            }
            else if (getToken(*exp) == rparen)  // 현재 문자가 ')'인 경우
            {
                while (getToken(postfixStack[postfixStackTop]) != lparen)
                {   // '('를 만날 때까지 postfix 스택에서 pop하고 postfixExp에 추가
                    charCat(&postfixStack[postfixStackTop]);
                    postfixPop();
                }
                postfixPop(); // '('를 스택에서 제거
            }
            else    
            {
                while (getPriority(postfixStack[postfixStackTop]) >= getPriority(*exp))
                {   // 스택의 top에 있는 연산자의 우선순위가 높거나 같으면 pop하고 postfixExp에 추가
                    charCat(&postfixStack[postfixStackTop]);
                    postfixPop();
                }
                postfixPush(*exp); // 현재 연산자를 postfix 스택에 push
            }
            exp++; // 다음 문자로 이동
        }
    while (postfixStackTop != -1)   // 스택에 남아있는 연산자들을 모두 pop하여 postfixExp에 추가
    {
        charCat(&postfixStack[postfixStackTop]);
        postfixPop();
    }

}
void debug()
{
	printf("\n---DEBUG\n");
	printf("infixExp =  %s\n", infixExp);
	printf("postExp =  %s\n", postfixExp);
	printf("eval result = %d\n", evalResult);

	printf("postfixStack : ");
	for(int i = 0; i < MAX_STACK_SIZE; i++)
		printf("%c  ", postfixStack[i]);

	printf("\n");

}

void reset()
{
	infixExp[0] = '\0';
	postfixExp[0] = '\0';

	for(int i = 0; i < MAX_STACK_SIZE; i++)
		postfixStack[i] = '\0';
         
	postfixStackTop = -1;
	evalStackTop = -1;
	evalResult = 0;
}

void evaluation()
{
    char *exp = postfixExp; // postfixExp를 읽기위한 포인터

    while(*exp != '\0'){
        if(getToken(*exp) == operand){  // 피연산자인 경우 바로 push
            evalPush(*exp - '0');
        }
        else{   // 연산자인 경우 pop하여 연산 후 다시 push
            int op1=evalPop();
            int op2=evalPop();
            switch(getToken(*exp)){
                case plus:
                    evalPush(op1+op2);
                    break;
                case minus:
                    evalPush(op1-op2);
                    break;
                case times:
                    evalPush(op1*op2);
                    break;
                case divide:
                    evalPush(op1/op2);
                    break;
            }
        }
        exp++;  // 다음 문자로 이동
    }
    evalResult=evalPop();   //  마지막으로 남은 값이 계산 결과
}