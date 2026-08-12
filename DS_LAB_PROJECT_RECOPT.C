#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Order
{
    int order_id;
    char customer_name[50];
    char product_name[50];
    int quantity;
    float per_price;
    float total_price;
    float due;
    float paid_amount;
    float return_amount;
    char status[20];

    struct Order *next;
};

typedef struct Order node;

#define FIELD_WIDTH   20   
#define PROMPT_WIDTH  25   
#define LINE_WIDTH    40
#define BUFFER_SIZE   100

int   isOrderIdExists(node *head, int id);
node *insertAtBeginning(node *head, int *success);
node *insertAtEnd(node *head, int *success);
void  display(node *head);
node *searchOrder(node *head, int id);
void  updateStatus(node *head, int id);
node *deleteOrder(node *head, int id);
void  printLine(void);
void  getValidInt(const char *prompt, int *out);
void  getValidPositiveInt(const char *prompt, int *out);
void  getValidFloat(const char *prompt, float *out);
void  getValidNonNegativeFloat(const char *prompt, float *out);
void  getValidString(const char *prompt, char *out, int size);
void getValidInt(const char *prompt, int *out)
{
    char buffer[BUFFER_SIZE];
    char *endptr;
    long val;
    
    while(1)
    {
        printf("%-*s: ", PROMPT_WIDTH, prompt);

        if(fgets(buffer, sizeof(buffer), stdin) == NULL)
        {
            clearerr(stdin);
            printf("Invalid Input. Please Input The Right One.\n");
            continue;
        }

        val = strtol(buffer, &endptr, 10);

        while(*endptr == ' ' || *endptr == '\t' || *endptr == '\n' || *endptr == '\r')
        {
            endptr++;
        }

        if(endptr != buffer && *endptr == '\0')
        {
            *out = (int)val;
            return;
        }

        printf("Invalid Input. Please Input The Right One.\n");
    }
}

void getValidPositiveInt(const char *prompt, int *out)
{
    while(1)
    {
        getValidInt(prompt, out);

        if(*out > 0)
        {
            return;
        }

        printf("Invalid Input. Please Input The Right One.\n");
    }
}

void getValidFloat(const char *prompt, float *out)
{
    char buffer[BUFFER_SIZE];
    char *endptr;
    float val;

    while(1)
    {
        printf("%-*s: ", PROMPT_WIDTH, prompt);

        if(fgets(buffer, sizeof(buffer), stdin) == NULL)
        {
            clearerr(stdin);
            printf("Invalid Input. Please Input The Right One.\n");
            continue;
        }

        val = strtof(buffer, &endptr);

        while(*endptr == ' ' || *endptr == '\t' || *endptr == '\n' || *endptr == '\r')
        {
            endptr++;
        }

        if(endptr != buffer && *endptr == '\0')
        {
            *out = val;
            return;
        }

        printf("Invalid Input. Please Input The Right One.\n");
    }
}

void getValidNonNegativeFloat(const char *prompt, float *out)
{
    while(1)
    {
        getValidFloat(prompt, out);

        if(*out >= 0)
        {
            return;
        }

        printf("Invalid Input. Please Input The Right One.\n");
    }
}

void getValidString(const char *prompt, char *out, int size)
{
    char buffer[BUFFER_SIZE];
    char *start;

    while(1)
    {
        printf("%-*s: ", PROMPT_WIDTH, prompt);

        if(fgets(buffer, sizeof(buffer), stdin) == NULL)
        {
            clearerr(stdin);
            printf("Invalid Input. Please Input The Right One.\n");
            continue;
        }

        buffer[strcspn(buffer, "\n")] = '\0';

        start = buffer;
        while(*start == ' ' || *start == '\t')
        {
            start++;
        }

        if(*start == '\0')
        {
            printf("Invalid Input. Please Input The Right One.\n");
            continue;
        }

        strncpy(out, start, size - 1);
        out[size - 1] = '\0';
        return;
    }
}

int isOrderIdExists(node *head, int id)
{
    node *temp = head;

    while(temp != NULL)
    {
        if(temp->order_id == id)
        {
            return 1;
        }

        temp = temp->next;
    }

    return 0;
}

void printLine(void)
{
    int i;
    for(i = 0; i < LINE_WIDTH; i++)
    {
        putchar('=');
    }
    putchar('\n');
}

node *insertAtBeginning(node *head, int *success)
{
    node *newnode = (node*)malloc(sizeof(node));
    int id;

    *success = 0;

    if(newnode == NULL)
    {
        printf("Memory Allocation Failed\n");
        return head;
    }

    getValidInt("Enter Order ID", &id);

    if(isOrderIdExists(head, id))
    {
        printf("Order ID Already Exists\n");
        free(newnode);
        return head;
    }

    newnode->order_id = id;

    getValidString("Enter Customer Name", newnode->customer_name, sizeof(newnode->customer_name));
    getValidString("Enter Product Name", newnode->product_name, sizeof(newnode->product_name));
    getValidPositiveInt("Enter Quantity", &newnode->quantity);
    getValidNonNegativeFloat("Enter Per Product Price", &newnode->per_price);

    newnode->total_price = newnode->quantity * newnode->per_price;
    newnode->due = newnode->total_price;
    newnode->paid_amount = 0;
    newnode->return_amount = 0;
    strcpy(newnode->status, "Due");

    newnode->next = head;

    *success = 1;
    return newnode;
}

node *insertAtEnd(node *head, int *success)
{
    node *newnode = (node*)malloc(sizeof(node));
    node *temp;
    int id;

    *success = 0;

    if(newnode == NULL)
    {
        printf("Memory Allocation Failed\n");
        return head;
    }

    getValidInt("Enter Order ID", &id);

    if(isOrderIdExists(head, id))
    {
        printf("Order ID Already Exists\n");
        free(newnode);
        return head;
    }

    newnode->order_id = id;

    getValidString("Enter Customer Name", newnode->customer_name, sizeof(newnode->customer_name));
    getValidString("Enter Product Name", newnode->product_name, sizeof(newnode->product_name));
    getValidPositiveInt("Enter Quantity", &newnode->quantity);
    getValidNonNegativeFloat("Enter Per Product Price", &newnode->per_price);

    newnode->total_price = newnode->quantity * newnode->per_price;
    newnode->due = newnode->total_price;
    newnode->paid_amount = 0;
    newnode->return_amount = 0;
    strcpy(newnode->status, "Due");
    newnode->next = NULL;

    *success = 1;

    if(head == NULL)
    {
        return newnode;
    }

    temp = head;

    while(temp->next != NULL)
    {
        temp = temp->next;
    }

    temp->next = newnode;

    return head;
}

void display(node *head)
{
    node *temp = head;

    if(head == NULL)
    {
        printf("No Order Available\n");
        return;
    }

    printLine();

    while(temp != NULL)
    {
        printf("%-*s: %d\n",   FIELD_WIDTH, "Order ID", temp->order_id);
        printf("%-*s: %s\n",   FIELD_WIDTH, "Customer Name", temp->customer_name);
        printf("%-*s: %s\n",   FIELD_WIDTH, "Product Name", temp->product_name);
        printf("%-*s: %d\n",   FIELD_WIDTH, "Quantity", temp->quantity);
        printf("%-*s: %.2f\n", FIELD_WIDTH, "Per Product Price", temp->per_price);
        printf("%-*s: %.2f\n", FIELD_WIDTH, "Total Price", temp->total_price);
        printf("%-*s: %s\n",   FIELD_WIDTH, "Status", temp->status);
        printf("%-*s: %.2f\n", FIELD_WIDTH, "Due Amount", temp->due);
        printf("%-*s: %.2f\n", FIELD_WIDTH, "Paid Amount", temp->paid_amount);

        if(temp->return_amount > 0)
        {
            printf("%-*s: %.2f\n", FIELD_WIDTH, "Change Returned", temp->return_amount);
        }

        printLine();

        temp = temp->next;
    }
}

node *searchOrder(node *head, int id)
{
    node *temp = head;

    while(temp != NULL)
    {
        if(temp->order_id == id)
        {
            return temp;
        }

        temp = temp->next;
    }

    return NULL;
}

void updateStatus(node *head, int id)
{
    node *temp = head;

    while(temp != NULL)
    {
        if(temp->order_id == id)
        {
            float payment;

            printLine();
            printf("%-*s: %d\n",   FIELD_WIDTH, "Order ID", temp->order_id);
            printf("%-*s: %.2f\n", FIELD_WIDTH, "Total Price", temp->total_price);
            printf("%-*s: %.2f\n", FIELD_WIDTH, "Current Due", temp->due);
            printLine();

            getValidNonNegativeFloat("Enter Paid Amount", &payment);

            if(payment >= temp->due)
            {
                temp->return_amount = payment - temp->due;
                temp->paid_amount = temp->total_price;
                temp->due = 0;
                strcpy(temp->status, "All Paid");
            }
            else
            {
                temp->paid_amount += payment;
                temp->due -= payment;
                strcpy(temp->status, "Due");
            }

            printf("Payment Updated Successfully\n");
            printLine();
            printf("%-*s: %.2f\n", FIELD_WIDTH, "Paid Amount", temp->paid_amount);
            printf("%-*s: %.2f\n", FIELD_WIDTH, "Remaining Due", temp->due);
            printf("%-*s: %s\n",   FIELD_WIDTH, "Status", temp->status);

            if(temp->return_amount > 0)
            {
                printf("%-*s: %.2f\n", FIELD_WIDTH, "Change Returned", temp->return_amount);
            }

            printLine();

            return;
        }

        temp = temp->next;
    }

    printf("Order Not Found\n");
}

node *deleteOrder(node *head, int id)
{
    node *temp = head;
    node *prev = NULL;

    if(head == NULL)
    {
        printf("No Order Available\n");
        return head;
    }

    if(head->order_id == id)
    {
        temp = head;
        head = head->next;
        free(temp);

        printf("Order Deleted Successfully\n");
        return head;
    }

    while(temp != NULL && temp->order_id != id)
    {
        prev = temp;
        temp = temp->next;
    }

    if(temp == NULL)
    {
        printf("Order Not Found\n");
        return head;
    }

    prev->next = temp->next;
    free(temp);

    printf("Order Deleted Successfully\n");

    return head;
}

int main()
{
    node *head = NULL;

    int choice;
    int id;

    while(1)
    {
        printf("\n");
        printLine();
        printf("   E-COMMERCE ORDER MANAGEMENT SYSTEM\n");
        printLine();
        printf("1. Add Order at Beginning\n");
        printf("2. Add Order at End\n");
        printf("3. Display All Orders\n");
        printf("4. Search Order\n");
        printf("5. Update Order Status\n");
        printf("6. Delete Order\n");
        printf("7. Exit\n");
        printLine();

        getValidInt("Enter Your Choice", &choice);

        switch(choice)
        {
        case 1:
        {
            int success;
            head = insertAtBeginning(head, &success);
            if(success)
            {
                printf("Order Added Successfully\n");
            }
            break;
        }

        case 2:
        {
            int success;
            head = insertAtEnd(head, &success);
            if(success)
            {
                printf("Order Added Successfully\n");
            }
            break;
        }

        case 3:
            display(head);
            break;

        case 4:
        {
            node *result;

            getValidInt("Enter Order ID", &id);

            result = searchOrder(head, id);

            if(result != NULL)
            {
                printLine();
                printf("%-*s: %d\n",   FIELD_WIDTH, "Order ID", result->order_id);
                printf("%-*s: %s\n",   FIELD_WIDTH, "Customer Name", result->customer_name);
                printf("%-*s: %s\n",   FIELD_WIDTH, "Product Name", result->product_name);
                printf("%-*s: %d\n",   FIELD_WIDTH, "Quantity", result->quantity);
                printf("%-*s: %.2f\n", FIELD_WIDTH, "Per Product Price", result->per_price);
                printf("%-*s: %.2f\n", FIELD_WIDTH, "Total Price", result->total_price);
                printf("%-*s: %s\n",   FIELD_WIDTH, "Status", result->status);
                printf("%-*s: %.2f\n", FIELD_WIDTH, "Due Amount", result->due);
                printf("%-*s: %.2f\n", FIELD_WIDTH, "Paid Amount", result->paid_amount);

                if(result->return_amount > 0)
                {
                    printf("%-*s: %.2f\n", FIELD_WIDTH, "Change Returned", result->return_amount);
                }

                printLine();
            }
            else
            {
                printf("Order Not Found\n");
            }

            break;
        }

        case 5:
            getValidInt("Enter Order ID", &id);
            updateStatus(head, id);
            break;

        case 6:
            getValidInt("Enter Order ID", &id);
            head = deleteOrder(head, id);
            break;

        case 7:
            printf("Thank You!\n");
            exit(0);

        default:
            printf("Invalid Input. Please Input The Right One.\n");
        }
    }

    return 0;
}