#include "eval.h"
#include "stdio.h"
#include "math.h"
#include "string.h"
#include "stdlib.h"

/********************************************************************
** 作者: sandyz987
** 作用: 计算表达式
** 形参: Expression string
** 返回: double结果
** 备注: 错误结果如下：
**      0:no error              1:wrong number of decimal points    
**      2:can't get top item at an empty stack  
**      3:can't pop item at an empty stack(number of brackets is invalid?)
**      4:can't get priority    5:too many arguments                
**      6:unexpect character   7:wrong number of arguments   8:math error
*********************************************************************/

#define PI 3.141592653
#define MAX_SIZE 1024
#define MAX_SIGN_NUM 26
#define MIN_NUM 1.0e-7
char *functionName[MAX_SIGN_NUM] = {">=", "<=", "!=", "==", ">", "<", "asin", "acos", "atan","sin", "cos", "tan",  "rand", "deg", "if", "rad", "log", "ln", "exp", "min", "max", "sign", "round", "floor", "abs", "sqrt"};
char nameTran[MAX_SIGN_NUM] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
int namePriority[MAX_SIGN_NUM] = {2,2,2,2,2,2,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8};//function's priority
int nameArgNum[MAX_SIGN_NUM] = {2,2,2,2,2,2,1,1,1,1,1,1,2,1,3,1,2,1,1,2,2,1,1,1,1,1};//Number of arguments

char operator[] = {'+','-','*','/','^','!','%','(',')',',','&','|','~','#'};
int priority[] = {3,3,4,4,5,5,5,-4,-5,-1,1,0,5,8};//Operator' priority
int operatorArgNum[] = {2,2,2,2,2,1,2,0,0,1,2,2,1,1};//Number of arguments


//I didn't use the struct to build a stack because pointer can reduce readability.
char operatorS[MAX_SIZE] = {0};//Operator stack
int operatorSTop = -1;
int isError = 0;//0=no error

typedef struct sign{
    int isOperator;//If isOperator == 0 use the num, else use the opera
    double num;
    char opera;
} SIGN;
SIGN signs[MAX_SIZE];int signsSize = 0;//To save the "infix expression" by using "struct SIGN"
SIGN reverseSigns[MAX_SIZE];int reverseSignsSize = 0;//To save the "Postfix Expression"
/*
 * Example:
 * if user input str = "1+2*3"
 * the signs(Stack) store 5 item : [isOperator=0,num=1,opera='\0'],[isOperator=1,num=0,opera='+'],[isOperator=0,num=2,opera='\0'],[isOperator=1,num=0,opera='*'],[isOperator=0,num=3,opera='\0']
 * the reverseSigns(Stack) store 5 item : [isOperator=0,num=2,opera='\0'],[isOperator=0,num=3,opera='\0'],[isOperator=1,num=0,opera='*'],[isOperator=0,num=1,opera='\0'],[isOperator=1,num=0,opera='+']
 */


int getPriority(char c){
    int i;
    for(i = 0; i < sizeof(operator); i++){
        if(operator[i] == c){
            return priority[i];
        }
    }
    for(i = 0; i < sizeof(nameTran); i++){
        if(nameTran[i] == c){
            return namePriority[i];
        }
    }
    isError = 4;
    return 0;
}

void pushSignOpera(char c){
    signs[signsSize].isOperator = 1;
    signs[signsSize].opera = c;
    signsSize++;
}

void pushSignNum(double n){
    signs[signsSize].isOperator = 0;
    signs[signsSize].num = n;
    signsSize++;
}


void pushReverseOpera(char c){
    reverseSigns[reverseSignsSize].isOperator = 1;
    reverseSigns[reverseSignsSize].opera = c;
    reverseSignsSize++;
}

void pushReverseNum(double n){
    reverseSigns[reverseSignsSize].isOperator = 0;
    reverseSigns[reverseSignsSize].num = n;
    reverseSignsSize++;
}


void deleteReverseItem(int pos){
    int i;
    for (i = pos + 1; i < reverseSignsSize; i++) {
        reverseSigns[i-1] = reverseSigns[i];
    }
    reverseSignsSize--;
}

void insertReverseNum(int pos, double n){
    int i;
    for (i = reverseSignsSize - 1; i >= pos; i--) {
        reverseSigns[i+1] = reverseSigns[i];
    }
    reverseSigns[pos].isOperator = 0;
    reverseSigns[pos].num = n;
    reverseSignsSize++;
}

int isNumber(char c){
    return (c>='0'&&c<='9')||(c=='.');
}

int isOperator(char c){
    int flag = 0,i;
    for(i = 0; i<sizeof(operator); i++){
        if (c == operator[i]){
            flag = 1;
        }
    }
    for(i = 0; i<sizeof(nameTran); i++){
        if (c == nameTran[i]){
            flag = 1;
        }
    }
    return flag;
}


void pushOpera(char opera){//Operator stack
    operatorS[++operatorSTop] = opera;
}


int isNotEmptyOperaS(){
    return operatorSTop != -1;
}


char popOpera(){
    return operatorS[operatorSTop--];
}

char getTopOpera(){
    if (operatorSTop != -1){
        return operatorS[operatorSTop];
    } else{
        isError = 2;
        return '\0';
    }
}


void replaceString(char s[], int pos, int len, char s1[]){//Replace the s from pos to len with s2
    int i;
    char s2[1000];
    int lenS1 = (int)strlen(s1);
    int lenS = (int)strlen(s);
    int j;
    //copy s to s2 and clear the s
    for (i = 0; i < lenS; i++) {
        s2[i] = s[i];
    }
    memset(s,'\0',sizeof(*s));
    for (i = 0; i < pos; ++i) {
        s[i] = s2[i];
    }
    for (i = pos; i < pos + lenS1; i++) {
        s[i] = s1[i - pos];
    }
    j = pos + lenS1;
    for (i = pos + len; i < lenS; i++){
        s[j++] = s2[i];
    }
    s[j] = '\0';
}

void tranString(char s[]){//Format string. For example "sin(3.14)+abs(-1)" is format to "J(3.14)+Y(-1)"
    int pos = 0;
    int i;
    while (pos < strlen(s)){
        for (i = 0; i < MAX_SIGN_NUM; i++) {
            if(pos + (int)strlen(functionName[i]) <= (int)strlen(s)){
                char tmp[20];
                memset(tmp,'\0',sizeof(tmp));
                strncpy(tmp,s + pos ,strlen(functionName[i]));
                if(strcmp(functionName[i], tmp) == 0){
                    char tmpChar[2] = {'\0', '\0'};
                    tmpChar[0] = nameTran[i];
                    replaceString(s, pos, (int)strlen(functionName[i]), tmpChar);
                }
            }

        }
        pos++;
    }
    if(s[0] == '-'){//decide whether the '-' is '#'
        char tmpChar[2] = {'#', '\0'};
        replaceString(s, 0, 1, tmpChar);
    }
    pos = 1;
    while (pos < strlen(s)){//decide whether the '-' is '#'
        if(isOperator(s[pos - 1]) && s[pos] == '-' && s[pos-1]!=')'){
            char tmpChar[2] = {'#', '\0'};
            replaceString(s, pos, 1, tmpChar);
        }
        pos++;
    }
}

int getOperaArgNum(char op){//Get operator's number of arguments.
    int i;
    for (i = 0; i < sizeof(nameTran); ++i) {
        if(nameTran[i] == op){
            return nameArgNum[i];
        }
    }
    for (i = 0; i < sizeof(operator); ++i) {
        if(operator[i] == op){
            return operatorArgNum[i];
        }
    }
    isError = 6;
    return 0;
}

int long fact(int n){//return the number's factor
    if (n < 0)
        return -1;
    if (n > 1)
        return fact(n - 1) * n;
    else
        return n;
}

double calculate(double *n, char op, int num){//Arguments are in *n. op is the operator. num is the number of arguments
    switch (op) {
        case ',':
            return n[num - 1];
        case '#':
            return -n[num - 1];
        case '+':
            return n[num - 1] + n[num - 2];
        case '-':
            return n[num - 1] - n[num - 2];
        case '*':
            return n[num - 1] * n[num - 2];
        case '/':
            return n[num - 2] != 0 ? n[num - 1] / n[num - 2] : (isError = 8, 0);
        case '%':
            return (double)((int)n[num - 1] % (int)n[num - 2]);
        case '^':
            return pow(n[num - 1] , n[num - 2]);
        case '!':
            return fact((int)n[num - 1]);
        case '&':
            return fabs(n[num - 1]) >= MIN_NUM && fabs(n[num - 2]) >= MIN_NUM;
        case '|':
            return fabs(n[num - 1]) >= MIN_NUM || fabs(n[num - 2]) >= MIN_NUM;
        case '~':
            return fabs(n[num - 1]) <= MIN_NUM;
        case 'A':
            return n[num - 1] >= n[num - 2];
        case 'B':
            return n[num - 1] <= n[num - 2];
        case 'C':
            return fabs(n[num - 1] - n[num - 2]) >= MIN_NUM;
        case 'D':
            return fabs(n[num - 1] - n[num - 2]) <= MIN_NUM;
        case 'E':
            return n[num - 1] > n[num - 2];
        case 'F':
            return n[num - 1] < n[num - 2];
        case 'G':
            return n[num - 1] <= 1 && n[num - 1] >= -1 ? asin(n[num - 1]) : (isError = 8, 0);
        case 'H':
            return n[num - 1] <= 1 && n[num - 1] >= -1 ? acos(n[num - 1]) : (isError = 8, 0);
        case 'I':
            return atan(n[num - 1]);
        case 'J':
            return sin(n[num - 1]);
        case 'K':
            return cos(n[num - 1]);
        case 'L':
            return tan(n[num - 1]);
        case 'M':
            return n[num - 1] >= 0 && n[num - 2] >= 0 && n[num - 2] - n[num - 1] >= 1 ? (rand() % ((int)n[num - 2] - (int)n[num - 1]) + 1) + (int)n[num - 1] : (isError = 8, 0);
        case 'N':
            return n[num - 1] / PI * 180.0;
        case 'O':
            return fabs(n[num - 1]) >= MIN_NUM ? n[num - 2] : n[num - 3] ;
        case 'P':
            return n[num - 1] / 180.0 * PI;
        case 'Q':
            return n[num - 1] != 1 && n[num - 1] > 0 && n [num - 2] > 0 ? log(n[num - 2]) / log(n[num - 1]) : (isError = 8, 0);
        case 'R':
            return n[num - 1] > 0 ? log(n[num - 1]) : (isError = 8, 0);
        case 'S':
            return exp(n[num - 1]);
        case 'T':
            return n[num - 1] <= n[num - 2] ? n[num - 1] : n[num - 2];
        case 'U':
            return n[num - 1] <= n[num - 2] ? n[num - 2] : n[num - 1];
        case 'V':
            return n[num - 1] >= 0 ? 1 : -1;
        case 'W':
            return (double)(int)(n[num - 1] + 0.5);
        case 'X':
            return (double)(int)(n[num - 1]);
        case 'Y':
            return n[num - 1] >= 0 ? n[num - 1] : - n[num - 1];
        case 'Z':
            return n[num - 1] >= 0 ? sqrt(n[num - 1]) : (isError = 8, 0) ;
        default://not find the operator
            isError = 6;
            return 0.0f;
    }
}

void calculateOpera(char op, int pos){//Change the reverseSigns(stack) when calculating
    int num = getOperaArgNum(op);
    int i;
    double n[10] = {0};
    int size = 0;
    double ans;

    if (pos >= num){
        for (i = 0; i < num; ++i) {
            if(reverseSigns[pos - 1 - i].isOperator != 1){
                n[size++] = reverseSigns[pos - 1 - i].num;
            } else{
                isError = 7;
                break;
            }
            deleteReverseItem(pos - i);
        }
        deleteReverseItem(pos - i);

        ans = calculate(n, op, num);
        insertReverseNum(pos - num, ans);
    }else {
        isError = 7;
    }
}

double eval(char s[]){
    double number = 0;
    int numberUsed = 0;
    int numberPoint = 0;
    int i;
    operatorSTop = -1;
    signsSize = 0;
    reverseSignsSize = 0;

    srand(0);//set srand!
    isError = 0;
    //tranString(s);   !!!!You must decide whether use "tranString" function here or before eval() execute. Because tranString() use too much time.
    while(*s != '\0'){
        if (isNumber(*s)){
            numberUsed = 1;
            if (*s == '.'){
                if (numberPoint != 0){
                    isError = 1;
                }
                numberPoint = 1;
                s++;
                continue;
            }
            if(numberPoint == 0){
                number *= 10.0;
                number += *s - '0';
            }else{
                number += pow(10,-(numberPoint++)) * (*s - '0');
            }
        }
        if (isOperator(*s)){
            if (numberUsed == 1){
                numberUsed = 0;
                pushSignNum(number);
                number = 0;
                numberPoint = 0;
            }
            pushSignOpera(*s);
        }

        s++;
    }
    if(numberUsed != 0){
        pushSignNum(number);
    }
    if(isError){
        return 0.0f;
    }
    //start calculating the sign stack

    for(i = 0; i < signsSize; i++){
        SIGN sign = signs[i];
        if(sign.isOperator != 1){
            //is number
            pushReverseNum(sign.num);
        }else{
            //is operator
            if(sign.opera == '('){
                pushOpera(sign.opera);
            }else if(sign.opera == ')'){
                while(getTopOpera() != '('){
                    if(isNotEmptyOperaS()){
                        pushReverseOpera(popOpera());
                    } else{
                        isError = 3;
                        break;
                    }
                }
                if(isNotEmptyOperaS()){
                    popOpera();
                }
            }else{
                while(isNotEmptyOperaS() && getPriority(getTopOpera()) >= getPriority(sign.opera)){
                    pushReverseOpera(popOpera());
                }
                pushOpera(sign.opera);
            }
        }
    }
    while (isNotEmptyOperaS()){
        char tmp = popOpera();
        if(tmp != '(' && tmp != ')'){
            pushReverseOpera(tmp);
        }
    }

//===========================up --This code block is to test print the "infix expression" and the "Postfix Expression"
//    for(i = 0; i < signsSize; i++){
//        if(!signs[i].isOperator){
//            printf("%f,",signs[i].num);
//        }else{
//            printf("%c,",signs[i].opera);
//        }
//    }
//    printf("\n");
//    for(i = 0; i < reverseSignsSize; i++){
//        if(!reverseSigns[i].isOperator){
//            printf("%f,",reverseSigns[i].num);
//        }else{
//            printf("%c,",reverseSigns[i].opera);
//        }
//    }
//    printf("\n");
//============================down  --This code block is to test print the "infix expression" and the "Postfix Expression"

    //start calculate the expression by reverse (Postfix) expression
    while(!isError){
        int pos = -1;
        for (i = 0; i < reverseSignsSize; i++) {
            if(reverseSigns[i].isOperator == 1){
                pos = i;
                break;
            }
        }
        if(pos == -1){
            break;
        }else{
            calculateOpera(reverseSigns[i].opera, pos);
        }
    }


    if(isError){
        return 0.0f;
    }
    if(reverseSignsSize != 1){
        isError = 5;
        return 0.0f;
    } else{
        return reverseSigns[0].num;
    }

}
