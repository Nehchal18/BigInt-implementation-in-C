#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "bigint.h"

int main(){
    char str1[1000];
    char str2[1000];
    
    printf("Enter Number 1 : ");
    fgets(str1, 1000, stdin);
    BigInt* num1 = createI(str1);
    
    printf("Enter Number 2 : ");
    fgets(str2, 1000, stdin);
    BigInt* num2 = createI(str2);
    
    printf("Select an operation:\n");
    printf("1. Add\n2. Subtract\n3. Multiply\n4. Divide\n5. Power\n6. GCD\n7. LCM\n8. Modulus\n9. Absolute Compare\n10. Compare\n");
    printf("Enter your choice: ");
    int choice;
    scanf("%d", &choice);
    
    BigInt *answer;
    int ans;

    switch(choice) {
        case 1: answer = addI(num1, num2); break;
        case 2: answer = subI(num1, num2); break;
        case 3: answer = mulI(num1, num2); break;
        case 4: answer = divI(num1, num2); break;
        case 5: answer = powI(num1, num2); break;
        case 6: answer = gcdI(num1, num2); break;
        case 7: answer = lcmI(num1, num2); break;
        case 8: answer = modI(num1, num2); break;
        case 9: ans = absCompI(num1, num2); break;
        case 10: ans = compI(num1, num2); break;
        default: printf("Invalid choice!\n");
    }
    
    if(choice > 8) printf("%d", ans);
    else printI(answer);
    
    return 0;
}