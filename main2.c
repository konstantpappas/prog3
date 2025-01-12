#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>


struct products {
    char description[40];
    int stock;
    float price;
};





int main() {
    int fd[2], fd2[2], status, i,k,j;
    int value; // i tixaia timi
    int result; // to apotelesma
    int count2 = 0, count3 = 0, count4 = 0;
    float all_total = 0; // Synoliki timi olwn twn proiontwn
    float total = 0; //Synoliki Timi Pelati

    struct products product[20] = {
        {"Water", 2, 1.5},
        {"Chips", 2, 1.5},
        {"Chocolate", 2, 1.0},
        {"Biscuits", 2, 1.5},
        {"Alcohol", 2, 1.0},
        {"Cola", 2, 1.0},
        {"Fish", 2, 1.5},
        {"Beef", 2, 4.0},
        {"Lemonade", 2, 1.5},
        {"Juice", 2, 1.5},
        {"Bread", 2, 1.5},
        {"Coffee", 2, 1.5},
        {"Beans", 2, 1.5},
        {"Carrots", 2, 1.5},
        {"Onions", 2, 1.5},
        {"Tomatoes", 2, 2.5},
        {"Oregano", 2, 1.0},
        {"Salt", 2, 1.0},
        {"Pepper", 2, 0.5},
        {"Milk", 2, 1.5}
    };

    if (pipe(fd) < 0) {
        printf("Error number %d\n", errno);
        perror("Error description");
        return -1;
    }

    if (pipe(fd2) < 0) {
        printf("Error number %d\n", errno);
        perror("Error description");
        return -1;
    }

    // O Arithmos ton pelatwn
    for (i = 0; i<5; i++) {
        pid_t cpid = fork();  // Create fork()

        if (cpid > 0) {
            // Patriki diergasia pou diaxeirizetai to eshop
            close(fd[1]);  // Close writing pipe1
            close(fd2[0]); // Close reading pipe2
            
            sleep(1);//1 deuterolepto anamoni metaxy kathe diergasias
            for (j = 0; j<5; j++) {
                for (k = 0; k<10; k++) {
                    read(fd[0], &value, sizeof(value)); //Pairnoume apo to read to product apo to child

                    // Elenxos an yparxei se apothema to product
                    if (product[value].stock > 0) {
                        product[value].stock--;
                        result = 1;  // Success
                        all_total = all_total+product[value].price;
                        count4++;
                    }else{
                        result = 0;  // To proion den yparxei
                        count3++;
                    }

                    // Grafoume me write an to product yparxei i oxi
                    write(fd2[1], &result, sizeof(result));
                }
            }

            close(fd[0]);  // Close read end pipe1
            close(fd2[1]); // Close write end pipe2
        } else if(cpid == 0) {
            // Client
            srand(time(NULL));
            total = 0;  // total tou kathe pelati

            for (j=0; j<10; j++){
                value = rand() % 20;  //i tixaia timi
                close(fd[0]);  // Close read pipe 1
                write(fd[1], &value, sizeof(value));  // Apostoli paraggelias sto e-shop.
            }

            close(fd[1]);  // Close pipe1 an otan apostaloun oles oi paraggelies

            // Apotelesmata paraggelion kathe pelati
            for (j=0; j<10; j++) {
                read(fd2[0], &result, sizeof(result));  //To Result an yparxei to product i oxi.
                if (result == 1) {//An ena i paraggelia einai epityxis,an 0 to product den yparxei
                    total = total+product[value].price;  
                    count2++;
                } else {
                    count3++; //plithos apotyximenwn paraggelion
                }
            }

            printf("Client [%d], purchase complete, your total is: %.2f\n", i, total);
            close(fd2[0]);  // close read of pipe 2
            exit(0);  // 
        } else {
            perror("Fork failed");
            return -1;
        }
    }

    printf("\nTotal orders processed: 50 - successful orders: %d - unsuccessful orders: %d\n", count4, count3);
    printf("%d requests were made. %d products were bought, totaling %.2f euros.\n", count2, count2, all_total);

    return 0;
}


