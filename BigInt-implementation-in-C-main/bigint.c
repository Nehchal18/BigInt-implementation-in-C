#include "bigint.h"

//Functions

//Create
BigInt* createI(char* str){
    
    //error handling for unexpected values
    int e;
    int error = 0;
    if(!((str[0] >= '0' && str[0] <= '9') || (str[0] == '-'))){
        errno = 1;
        error = 1;
    }
    for(e = 1; e < strlen(str); e++){
        if(str[e] == 10){
            break;
        }
        if(!((str[e] >= '0' && str[e] <= '9') || (str[e] == '-'))){
            errno = 1;
            error = 1;
            break;
        }
    }
    
    if(error == 1){
        perror("ERROR : UNEXPECTED CHARECTER IN BIGINT DECLARATION");
        exit(EXIT_FAILURE);
    }
    
    //create implementation starts
    BigInt* num = (BigInt*) malloc(sizeof(BigInt));
    num->head = (Node*) malloc(sizeof(Node));
    num->tail = num->head;
    Node* tempNode = num->head;
    num->size = 0;
    int n = 0;
    if(str[0] == '-'){
        num->sign = 1;
        n++;
    }
    else{
        num->sign = 0;
    }
    
    for(int i = e - 1; i >= n ; i--){
        int tempNum = 0;
        int limit = 4;
        
        int mul = 1;
        while(limit-- && i >= n){
            tempNum += (str[i] - '0') * mul;
            mul *= 10;
            i--;
        }
        i++;
        
        tempNode->digits = tempNum;
        
        if(i != n){
            tempNode->prev = (Node*) malloc(sizeof(Node));
            tempNode->prev->next = tempNode;
            tempNode = tempNode->prev;
            
        }
        num->size++;
    }
    num->head = tempNode;
    num->tail->next = NULL;
    num->head->prev = NULL;
    return num;
}

//Rebuild
//internal use function
void _reBuildI(BigInt* num){
    
    Node* tempNode = num->head;
    
    while(tempNode != num->tail && tempNode->digits == 0){
        tempNode = tempNode->next;
        free(tempNode->prev);
        tempNode->prev = NULL;
        num->size--;
    }
    
    num->head = tempNode;
    num->head->prev = NULL;
    num->tail->next = NULL;
    return;
    
}

//Print
void printI(BigInt* numI){
    
    numI->head->prev = NULL;
    numI->tail->next = NULL;
    
    _reBuildI(numI);
    
    Node* tempNode = numI->head;
    
    if(numI->sign){
        printf("-");
    }
    
    int zero = 0;
    while(tempNode){
        if(tempNode != numI->head){    
            int limit = 4;
            
            if(tempNode->digits < 10){
                limit = 1;
            }
            else if(tempNode->digits < 100){
                limit = 2;
            }
            else if(tempNode->digits < 1000){
                limit = 3;
            }

            for(int i = 0; i < 4 - limit; i++){
                printf("0");
            }
        }
        printf("%d", tempNode->digits);
        tempNode = tempNode->next;
        zero = 1;
    }
    
}

//_PrintI
//internal use function
void _printI(BigInt* numI){
    
    Node* tempNode = numI->head;
    
    if(numI->sign){
        printf("-");
    }
    
    while(tempNode){
        printf("%d ", tempNode->digits);
        tempNode = tempNode->next;
    }
    
}

//clone
BigInt* _clone(BigInt* num1){
    BigInt* answer = createI("0");
    Node* answerNode = answer->tail;
    Node* numNode = num1->tail;
    answer->sign = num1->sign;
    answer->size = num1->size;
    
    while(numNode){
        answerNode->digits = numNode->digits;
        
        answerNode->prev = (Node*) malloc(sizeof(Node));
        answerNode->prev->next = answerNode;
        
        answerNode = answerNode->prev;
        numNode = numNode->prev;
    }
    
    answer->head = answerNode->next;
    _reBuildI(answer);
    
    return answer;
}

//Compare 1 (ignoring polarity)
int absCompI(BigInt* num1, BigInt* num2){
    
    if(num1->size > num2->size){
        return 1;
    }
    if(num1->size < num2->size){
        return -1;
    }
    
    Node* n1 = num1->head, *n2 = num2->head;
    
    while(n1){
        if(n1->digits > n2->digits){
            return 1;
        }
        if(n1->digits < n2->digits){
            return -1;
        }
        
        n1 = n1->next;
        n2 = n2->next;
    }
    
    return 0;
    
}

//Compare 2 (considering polarity)
int compI(BigInt* num1, BigInt* num2){
    
    if(num1->size > num2->size || (!num1->sign && num2->sign)){
        return 1;
    }
    if(num1->size < num2->size || (num1->sign && !num2->sign)){
        return -1;
    }
    
    Node* n1 = num1->head, *n2 = num2->head;
    
    while(n1){
        if(n1->digits > n2->digits){
            return 1;
        }
        if(n1->digits < n2->digits){
            return -1;
        }
        
        n1 = n1->next;
        n2 = n2->next;
    }
    
    return 0;
    
}

//add
BigInt* addI(BigInt* num1, BigInt* num2){
    
    
    if(num1->sign == num2->sign){
        int carry = 0;
        BigInt* answer = (BigInt*) malloc(sizeof(BigInt));
        answer->head = (Node*) malloc(sizeof(Node));
        answer->tail = answer->head;
        answer->sign = num2->sign;
        answer->size = 1;
        
        Node* tempNode = answer->head;
        
        Node* x = num1->tail, *y = num2->tail;
        
        while(x || y){
            int sum = carry;
            if(x){
                sum += x->digits;
            }
            if(y){
                sum += y->digits;
            }
            
            tempNode->digits = sum % 10000;
            carry = sum / 10000;
            if(x){
                x = x->prev;
            }
            if(y){
                y = y->prev;
            }
            
            if(x || y){
                tempNode->prev = (Node*) malloc(sizeof(Node));
                tempNode->prev->next = tempNode;
                tempNode = tempNode->prev;
                answer->size++;
            }
        }
        
        if(carry){
            tempNode->prev = (Node*) malloc(sizeof(Node));
            tempNode->prev->next = tempNode;
            tempNode = tempNode->prev;
            tempNode->digits = carry;
            answer->size++;
        }
        answer->head = tempNode;
        answer->head->prev = NULL;
        answer->tail->next = NULL;
        return answer;
    }
    else{
        //redirecting to subtraction
        BigInt* answer;
        if(num2->sign){
            num2->sign = 0;
            answer = subI(num1, num2);
            num2->sign = 1;
        }
        else{
            num1->sign = 0;
            answer = subI(num2, num1);
            num1->sign = 1;
        }
        answer->head->prev = NULL;
        answer->tail->next = NULL;
        return answer;
    }
}

//sub
BigInt* subI(BigInt* num1, BigInt* num2){
    
    //redirecting to addition
    if(num1->sign && !num2->sign){ // (-x) - y => (-x) + (-y)
        num2->sign = 1;
        BigInt* answer = addI(num1, num2);
        num2->sign = 0;
        answer->head->prev = NULL;
        answer->tail->next = NULL;
        return answer;
    }
    if(!num1->sign && num2->sign){ // x - (-y) => x + y
        num2->sign = 0;
        BigInt* answer = addI(num1, num2);
        num2->sign = 1;
        answer->head->prev = NULL;
        answer->tail->next = NULL;
        return answer;
    }
    
    
    //implementation of subtraction
    int borrow = 0;
    BigInt* answer = (BigInt*) malloc(sizeof(BigInt));
    answer->sign = 0;
    answer->head = (Node*) malloc(sizeof(Node));
    answer->tail = answer->head;
    answer->size = 1;
    Node* tempNode = answer->head;
    
    int comp = absCompI(num1, num2);
    if(!comp){
        answer->head->digits = 0;
        answer->head->prev = NULL;
        answer->tail->next = NULL;
        return answer;
    }
    Node* n1, *n2;
    if(comp == -1){
        n1 = num2->tail;
        n2 = num1->tail;
        answer->sign = !num2->sign;
    }
    else{
        n1 = num1->tail;
        n2 = num2->tail;
        answer->sign = num2->sign;
    }
    
    while(n1 || n2){
        int sub = borrow;
        borrow = 0;
        if(n1){
            if(n1->digits + sub < 0){
                borrow--;
                int num = 10000 + n1->digits;
                sub += num;
            }
            else{
                sub += n1->digits;
            }
        }
        if(n2){
            if((sub - n2->digits) < 0){
                borrow--;
                sub += 10000;
            }
            sub -= n2->digits;
        }
        
        tempNode->digits = sub;
        if(n1){
            n1 = n1->prev;
        }
        if(n2){
            n2 = n2->prev;
        }
        if(n1 || n2){
            tempNode->prev = (Node*) malloc(sizeof(Node));
            tempNode->prev->next = tempNode;
            tempNode = tempNode->prev;
            answer->size++;
        }
    }
    
    answer->head = tempNode;
    answer->head->prev = NULL;
    answer->tail->next = NULL;
    _reBuildI(answer);
    
    return answer;
    
}

//mul
BigInt* mulI(BigInt* num1, BigInt* num2){
    
    int size = num1->size + num2->size;
    
    BigInt* answer = (BigInt *) malloc(sizeof(BigInt));
    answer->head = (Node* ) malloc(sizeof(Node));
    answer->tail = answer->head;
    answer->size = 1;
    Node* tempNode = answer->tail;
    Node* array[size];
    answer->sign = num1->sign ^ num2->sign;
    
    answer->head->prev = NULL;
    answer->tail->next = NULL;
    
    for(int i = 0; i < size; i++){
        array[i] = tempNode;
        tempNode->digits = 0;
        Node* temp = (Node*) malloc(sizeof(Node));
        tempNode->prev = temp;
        temp->next = tempNode;
        tempNode = tempNode->prev;
        answer->size++;
    }
    answer->head = tempNode;
    answer->head->digits = 0;
    
    Node* n1 = num1->tail, *n2 = num2->tail;
    int i = 0, j = 0;
    
    while(n1){
        while(n2){
            array[i + j]->digits += n1->digits * n2->digits;
            array[i + j + 1]->digits += array[i + j]->digits / 10000;
            array[i + j]->digits %= 10000;
            n2 = n2->prev;
            j++;
        }
        i++;
        n1 = n1->prev;
        n2 = num2->tail;
        j = 0;
    }
    answer->head->prev = NULL;
    answer->tail->next = NULL;
    _reBuildI(answer);
    
    return answer;
    
}

//_half
//internal use function
void _halfI(BigInt* num){
    Node* tempNode = num->head;
    int carry = 0;
    num->tail->next = NULL;
    
    while(tempNode){
        int tempNum = tempNode->digits + (carry * 10000);
        
        carry = 0;
        if(tempNum % 2){
            carry = 1;
        }
        
        tempNode->digits = tempNum / 2;
        
        tempNode = tempNode->next;
    }
    
    _reBuildI(num);
    return;
}

//div
BigInt* divI(BigInt* num1, BigInt* num2){
    if(num2->head->digits == 0){
        errno = 1;
        perror("ERROR : DIVIDING BY ZERO");
        exit(EXIT_FAILURE);
    }
    BigInt* start = createI("0");
    BigInt* end = num1;
    BigInt* one = createI("1");
    BigInt* answer;
    int n1s = num1->sign;
    int n2s = num2->sign;
    num1->sign = 0;
    num2->sign = 0;
    int remain = 1;
    
    int count = 0;
    while(absCompI(start, end) < 1){
        BigInt* mid = addI(start, end);
        _halfI(mid);
        BigInt* tempAnswer = mulI(num2, mid);
        
        if(absCompI(tempAnswer, num1) == 1){
            end = subI(mid, one);
        }
        else if(absCompI(tempAnswer, num1) == -1){
            answer = mid;
            start = addI(mid, one);
        }
        else{
            remain = 0;
            answer = mid;
            break;
        }
    }
    
    answer->sign = n1s ^ n2s;
    if(remain && answer->sign){
        answer = subI(answer, one);
    }
    
    num1->sign = n1s;
    num2->sign = n2s;
    
    answer->head->prev = NULL;
    answer->tail->next = NULL;
    return answer;
}

//pow
BigInt* powI(BigInt* num1, BigInt* num2){
    if(num2->head->digits == 0){
        BigInt* one = createI("1");
        return one;
    }
    
    BigInt* half = _clone(num2);
    _halfI(half);
    
    BigInt* temp = powI(num1, half);
    temp = mulI(temp, temp);
    
    if(!(num2->tail->digits % 2)){
        return temp;
    }
    return mulI(temp, num1);
}

//gcd
BigInt* gcdI(BigInt* num1, BigInt* num2){
    BigInt* n1 = num1;
    BigInt* n2 = num2;
    
    if(absCompI(n1, n2) == -1){
        BigInt* temp = n1;
        n1 = n2;
        n2 = temp;
    }
    
    if(n2->head == n2->tail && (n2->head->digits == 0)){
        return n1;
    }
    
    BigInt* newN1 = modI(n1, n2);
    return gcdI(newN1, n2);
}

//lcm
BigInt* lcmI(BigInt* num1, BigInt* num2){
    BigInt* answer = mulI(num1, num2);
    BigInt* gcd = gcdI(num1, num2);
    answer = divI(answer, gcd);
    
    return answer;
}

//mod
BigInt* modI(BigInt* num1, BigInt* num2){
    BigInt* q = divI(num1, num2);
    return subI(num1, mulI(q, num2));
}

//free
void freeI(BigInt* num){
    Node* tempNode = num->head;
    
    while(tempNode->next){
        tempNode = tempNode->next;
        free(tempNode->prev);
    }
    free(tempNode);
    free(num);
    
    return;
}