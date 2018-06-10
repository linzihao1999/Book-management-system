#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define LOGIN_FAILED 0
#define LOGIN_SUCCESS 1
#define LOGIN_SUCCESS_ADMIN 2
#define EXIT_SYSTEM 3
#define SIGNUP_SUCCESS 4

#define ADMIN_NAME "admin"
#define ADMIN_PASSWORD "admin"

struct User {
    char name[100];
    char password[100];
    int age;
    struct User *next;
} *uhead;
int usersize;
char current_user[100];

struct Book {
    char name[100];
    char author[100];
    char belong[100];
    int borrow;
    int isbn;
    double price;
    struct Book *next;
} *bhead;
int booksize;

void push_user(struct User t) {
    usersize++;
    struct User *p = (struct User *) malloc(sizeof(struct User));
    p->next = NULL;
    strcpy(p->name, t.name);
    strcpy(p->password, t.password);
    p->age = t.age;
    p->next = uhead->next;
    uhead->next = p;
}

void push_book(struct Book t) {
    booksize++;
    struct Book *p = (struct Book *) malloc(sizeof(struct Book));
    p->next = NULL;
    strcpy(p->name, t.name);
    strcpy(p->author, t.author);
    strcpy(p->belong, t.belong);
    p->borrow = t.borrow;
    p->isbn = t.isbn;
    p->price = t.price;
    p->next = bhead->next;
    bhead->next = p;
}

int initial() {
    uhead = (struct User *) malloc(sizeof(struct User));
    bhead = (struct Book *) malloc(sizeof(struct Book));
    uhead->next = NULL;
    bhead->next = NULL;
    FILE *fin = NULL;
    fin = fopen("data.txt", "r");
    if (fin == NULL) {
        printf("No data");
        return 0;
    } else {
        int n;
        fscanf(fin, "%d", &n);
        for (int i = 0; i < n; i++) {
            struct User t;
            fscanf(fin, "%s%s%d", t.name, t.password, &t.age);
            push_user(t);
        }
        fscanf(fin, "%d", &n);
        for (int i = 0; i < n; i++) {
            struct Book t;
            fscanf(fin, "%s%s%s%d%d%lf", t.name, t.author, t.belong, &t.borrow, &t.isbn, &t.price);
            push_book(t);
        }
    }
    return 0;
}


int signin() {
    while (1) {
        printf("1.登陆\n"
                       "2.注册\n"
                       "3.退出\n\n");
        int p;
        scanf("%d", &p);
        if (p == 3)return EXIT_SYSTEM;
        printf("请输入用户名：");
        char t1[100], t2[100];
        scanf("%s", t1);
        printf("请输入密码：");
        scanf("%s", t2);
        if (p == 2) {
            struct User t;
            for (int i = 0; i < 100; i++)t.name[i] = t1[i], t.password[i] = t2[i];
            int tage;
            printf("请输入年龄:");
            scanf("%d", &tage);
            t.age = tage;
            push_user(t);
            return SIGNUP_SUCCESS;
        }
        if (strcmp(t1, ADMIN_NAME) == 0 && strcmp(t2, ADMIN_PASSWORD) == 0) {
            strcpy(current_user, "admin");
            return LOGIN_SUCCESS_ADMIN;
        }

        for (struct User *i = uhead; i != NULL; i = i->next) {
            if (strcmp(t1, i->name) == 0) {
                if (strcmp(t2, i->password) == 0) {
                    printf("Welcome back %s\n", i->name);
                    strcpy(current_user, i->name);
                    return LOGIN_SUCCESS;
                } else {
                    return LOGIN_FAILED;
                }
            }
        }
        return 0;
    }
}

void print_book(struct Book *p) {
    printf("|书名 %s\n", p->name);
    printf("|作者 %s\n", p->author);
    printf("|图书号 %d\n", p->isbn);
    printf("|价格 %f\n", p->price);
    if (p->borrow == 0)printf("|可以借阅\n");
    else printf("|已被 %s 借阅\n", p->belong);
    printf("\n");
}

void search_book() {
    while (1) {
        printf("1.搜索读者\n"
                       "2.搜索图书名\n"
                       "3.搜索图书号\n"
                       "4.搜索作者\n"
                       "5.搜索价格范围\n"
                       "6.返回\n\n");
        int p;
        scanf("%d", &p);
        char t[100];
        int l = 0, r = 0, isbn = 0;
        switch (p) {
            case 1: {
                printf("请输入读者名：");
                scanf("%s", t);
                break;
            }
            case 2: {
                printf("请输入图书名:");
                scanf("%s", t);
                break;
            }
            case 3: {
                printf("请输入图书号:");
                scanf("%d", &isbn);
                break;
            }
            case 4: {
                printf("请输入作者:");
                scanf("%s", t);
            }
            case 5: {
                printf("请输入最低价格:");
                scanf("%d", &l);
                printf("请输入最高价格:");
                scanf("%d", &r);
            }
            case 6:
                return;
            default:
                break;
        }
        int flag = 0;
        printf("符合条件的书籍有------------\n");
        for (struct Book *i = bhead; i != NULL; i = i->next) {
            switch (p) {
                case 1: {
                    if (strcmp(t, i->belong) == 0 && i->borrow == 1)
                        print_book(i), flag = 1;
                    break;
                }
                case 2: {
                    if (strcmp(t, i->name) == 0)
                        print_book(i), flag = 1;
                    break;
                }
                case 3: {
                    if (isbn == i->isbn)
                        print_book(i), flag = 1;
                    break;
                }
                case 4: {
                    if (strcmp(t, i->author) == 0)
                        print_book(i), flag = 1;
                    break;
                }
                case 5: {
                    if (l <= i->price && i->price <= r)
                        print_book(i), flag = 1;
                    break;
                }
                default:
                    break;
            }
        }
        if (flag == 0)printf("None\n\n");
        printf("--------------------------\n\n");
    }
}

void borrow_book() {
    printf("可借阅的书籍有----\n");
    int num = 1;
    for (struct Book *i = bhead->next; i != NULL; i = i->next)
        if (i->borrow == 0) {
            printf("%d.%s\n", num++, i->name);
        }
    printf("---------------\n\n");
    int p;
    scanf("%d", &p);
    num = 1;
    for (struct Book *i = bhead->next; i != NULL; i = i->next, num++) {
        if (p == num) {
            print_book(i);
            if (i->borrow == 0) {
                printf("确认借阅(Y/n)?\n");
                char t[10];
                scanf("%s", t);
                if (t[0] == 'Y' || t[0] == 'y') {
                    i->borrow = 1;
                    strcpy(i->belong, current_user);
                }
                printf("借阅成功\n\n");
            }
            break;
        }
    }

}

void return_book() {
    printf("---------------\n");
    int num = 1, flag = 0;
    for (struct Book *i = bhead->next; i != NULL; i = i->next) {
        if (strcmp(i->belong, current_user) == 0 && i->borrow == 1) {
            printf("%d.\n", num++);
            flag = 1;
            print_book(i);
        }
    }
    if (flag == 0) {
        printf("无可归还书籍\n\n");
        return;
    }
    printf("您选择要归还的书籍有\n");
    int p;
    scanf("%d", &p);
    num = 1;
    flag = 0;
    for (struct Book *i = bhead->next; i != NULL; i = i->next, num++) {
        if (num == p && strcmp(i->belong, current_user) == 0 && i->borrow == 1) {
            i->borrow = 0;
            flag = 1;
            break;
        }
    }
    if (!flag)printf("归还失败\n\n");
    else printf("归还成功\n\n");
}

void insert_book() {
    struct Book t;
    printf("请输入书名:");
    scanf("%s", t.name);
    printf("请输入作者:");
    scanf("%s", t.author);
    printf("请输入图书号:");
    scanf("%d", &t.isbn);
    printf("请输入价格:");
    scanf("%lf", &t.price);
    t.borrow = 0;
    t.belong[0] = '0';
    push_book(t);
    printf("添加成功\n\n");
}

void delete_book() {
    int num = 1;
    printf("请选择要删除的图书------\n");
    printf("输入0退出\n");
    for (struct Book *i = bhead->next; i != NULL; i = i->next, num++) {
        printf("%d.\n", num);
        print_book(i);
    }
    int p;
    scanf("%d", &p);
    if (p == 0)return;
    num = 0;
    int flag = 0;
    for (struct Book *i = bhead; i != NULL; i = i->next, num++) {
        if (num + 1 == p) {
            struct Book *t = i->next;
            flag = 1;
            i->next = i->next->next;
            free(t);
            break;
        }
    }
    if (flag)printf("删除成功\n");
    else printf("删除失败\n");
}

void change_book() {
    printf("请选择要修改的书籍------\n");
    printf("输入0退出\n");
    int num = 1;
    for (struct Book *i = bhead->next; i != NULL; i = i->next, num++) {
        printf("%d.\n", num);
        print_book(i);
    }
    printf("------------------\n");
    int p;
    scanf("%d", &p);
    if (p == 0)return;
    num = 1;
    for (struct Book *i = bhead->next; i != NULL; i = i->next, num++) {
        if (num == p) {
            printf("请选择要修改的信息\n"
                           "1.书名 %s\n"
                           "2.作者 %s\n"
                           "3.借阅者 %s\n"
                           "4.图书号 %d\n"
                           "5.价格 %lf\n"
                           "6.返回\n", i->name, i->author, i->borrow == 1 ? i->belong : "None", i->isbn, i->price);
            int p1;
            scanf("%d", &p1);
            switch (p1) {
                case 1: {
                    printf("请输入书名:");
                    char t[100];
                    scanf("%s", t);
                    strcpy(i->name, t);
                    break;
                }
                case 2: {
                    printf("请输入作者:");
                    char t[100];
                    scanf("%s", t);
                    strcpy(i->author, t);
                    break;
                }
                case 3: {
                    if (i->borrow) {
                        printf("1.取消借阅\n"
                                       "2.修改借阅者\n");
                        int p2;
                        scanf("%d", &p2);
                        if (p2 == 1)i->borrow = 0;
                        if (p2 == 2) {
                            printf("请输入读者:");
                            char t[100];
                            scanf("%s", t);
                            strcpy(i->belong, t);
                        }
                    } else {
                        printf("请输入作者:");
                        char t[100];
                        scanf("%s", t);
                        strcpy(i->author, t);
                        i->borrow = 1;
                    }
                    break;
                }
                case 4: {
                    printf("请输入图书号:\n");
                    int t;
                    scanf("%d", &t);
                    i->isbn = t;
                }
                case 5: {
                    printf("请输入价格:\n");
                    int t;
                    scanf("%lf", &t);
                    i->price = t;
                }
                case 6: {
                    return;
                }
                default:
                    break;
            }
        }
    }
}

void search_user() {
    while (1) {
        printf("1.搜索姓名\n"
                       "2.搜索年龄\n"
                       "3.返回\n");
        int p;
        scanf("%d", &p);
        switch (p) {
            case 1: {
                printf("请输入姓名:");
                char t[100];
                scanf("%s", t);
                printf("%s 借阅的书有---------\n", t);
                for (struct User *i = uhead->next; i != NULL; i = i->next) {
                    if (strcmp(t, i->name) == 0) {
                        for (struct Book *j = bhead->next; j != NULL; j = j->next) {
                            if (strcmp(t, j->belong) == 0 && j->borrow == 1) {
                                print_book(j);
                            }
                        }
                        printf("--------------\n");
                        break;
                    }
                }
            }
            case 2: {
                int t;
                printf("请输入年龄:");
                scanf("%d", &t);
                printf("符合此年龄的读者有\n");
                for (struct User *i = uhead->next; i != NULL; i = i->next) {
                    if (i->age == t) {
                        printf("%s\n", i->name);
                    }
                }
                printf("\n");
            }
            case 3: {
                return;
            }
            default:
                break;
        }
    }
}

void admin_menu() {
    printf("Welcome admin\n");
    while (1) {
        int p;
        printf("1.增加图书\n"
                       "2.删除图书\n"
                       "3.修改图书信息\n"
                       "4.退出\n");
        scanf("%d", &p);
        switch (p) {
            case 1: {
                insert_book();
                break;
            }
            case 2: {
                delete_book();
                break;
            }
            case 3: {
                change_book();
                break;
            }
            case 4: {
                return;
            }
            default:
                break;
        }
    }
}

void user_menu() {
    while (1) {
        int p;
        printf("1.查询书籍\n"
                       "2.借书\n"
                       "3.还书\n"
                       "4.查询用户\n"
                       "5.退出\n\n");
        scanf("%d", &p);
        if (p == 1)search_book();//done
        if (p == 2)borrow_book();//done
        if (p == 3)return_book();//done
        if (p == 4)search_user();
        if (p == 5)break;
    }
}

void save_file() {
    FILE *fout = NULL;
    fout = fopen("data.txt", "w");
    fprintf(fout, "%d\n", usersize);
    struct User *p = uhead->next;
    for (int i = 0; i < usersize; i++) {
        fprintf(fout, "%s\n%s\n%d\n", p->name, p->password, p->age);
        p = p->next;
    }
    fprintf(fout, "%d\n", booksize);
    struct Book *p1 = bhead->next;
    for (int i = 0; i < booksize; i++) {
        fprintf(fout, "%s\n%s\n%s\n%d\n%d\n%lf\n", p1->name, p1->author, p1->belong, p1->borrow, p1->isbn, p1->price);
        p1 = p1->next;
    }
}

int main() {
    initial();
    int num = 0;
    while (1) {
        int t = signin();
        if (t == LOGIN_FAILED) {
            ++num;
            printf("Login failed\n");
        }
        if (t == LOGIN_SUCCESS)
            user_menu();
        if (t == LOGIN_SUCCESS_ADMIN)
            admin_menu();
        if (t == SIGNUP_SUCCESS) {
            printf("注册成功\n\n");
        }
        if (t == EXIT_SYSTEM)break;
    }
    save_file();
    return 0;
}