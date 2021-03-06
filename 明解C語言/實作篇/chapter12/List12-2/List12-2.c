/*
    使用陣列的線性清單的實作範例
*/

#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>

#define Null    -1          /* 空編號 */

typedef enum {
    Term, Insert, Append, Delete, Print, Clear
} Menu;

typedef int     Index;      /* 編號型 */

/*--- 節點 ---*/
typedef struct {
    int    no;          /* 會員編號 */
    char   name[10];        /* 名稱 */
    Index  next;            /* 指向後續元素的編號 */
    Index  Dnext;           /* 刪除清單指標 */
} Node;

/*--- 線性清單 ---*/
typedef struct {
    Node*   n;              /* 清單本體(陣列) */
    Index  top;             /* 指向清單前頭元素的編號 */
    Index  max;             /* 指向使用記錄的最大編號 */
    Index  deleted;         /* 指向刪除清單的前頭元素的編號 */
} List;

#define Top     (list->top)             /* 清單的前頭 */

#define Second  (list->n[Top].next)     /* 清單第二個元素 */

#define Next(x) (list->n[(x)].next)     /* 清單第x個元素 */

/*--- 對清單的各元素設定值 ----*/
void SetNode(Node* x, int no, char* name, Index next) {
    x->no   = no;
    x->next = next;
    strcpy(x->name, name);
}

/*--- 傳回要插入的記錄的編號 ---*/
int GetIndex(List* list) {
    if (list->deleted == Null) {        /* 無刪除記錄時 */
        return (++(list->max));
    } else {
        Index  rec = list->deleted;
        list->deleted = list->n[rec].Dnext;
        return (rec);
    }
}

/*--- 將指定的記錄登錄到刪除清單 ---*/
void DeleteIndex(List* list, Index idx) {
    if (list->deleted == Null) {        /* 無刪除記錄時 */
        list->deleted = idx;
        list->n[idx].Dnext = Null;
    } else {
        Index  ptr = list->deleted;
        list->deleted = idx;
        list->n[idx].Dnext = ptr;
    }
}

/*--- 在前頭插入節點 ---*/
void InsertNode(List* list, int no, char* name) {
    Index  ptr = Top;
    Top = GetIndex(list);
    SetNode(&list->n[Top], no, name, ptr);
}

/*--- 在末尾插入節點 ---*/
void AppendNode(List* list, int no, char* name) {
    if (Top == Null) {                      /* 若清單為空的話 */
        InsertNode(list, no, name);    /* 則插入至前頭 */
    } else {
        Index  ptr = Top;

        while (Next(ptr) != Null) {         /* 找出末尾節點 */
            ptr = Next(ptr);
        }

        Next(ptr) = GetIndex(list);
        SetNode(&list->n[Next(ptr)], no, name, Null);
    }
}

/*--- 將前頭節點刪除 ---*/
void DeleteNode(List* list) {
    if (Top != Null) {
        Index  ptr = Second;
        DeleteIndex(list, Top);
        Top = ptr;
    }
}

/*--- 將全節點刪除 ---*/
void ClearList(List* list) {
    while (Top != Null) {               /* 刪除前頭節點 */
        DeleteNode(list);    /* 直到清單全空 */
    }
}

/*--- 顯示所有的節點 ---*/
void PrintList(List* list) {
    Index  ptr = Top;

    puts("【一覽表】");

    while (ptr != Null) {
        printf("%5d %-10.10s\n", list->n[ptr].no, list->n[ptr].name);
        ptr = Next(ptr);
    }
}

/*--- 將線性清單初始化 ---*/
void InitList(List* list, int size) {
    /* 一次確保住元素型為Node且元素數為size的陣列 */
    list->n = calloc(size, sizeof(Node));
    list->top = list->max = list->deleted = Null;
}

/*--- 線性清單使用結束 ---*/
void TermList(List* list) {
    ClearList(list);                /* 刪除全部的節點 */
    free(list->n);
}

/*--- 輸入資料 ---*/
Node Read(char* message) {
    Node  temp;

    printf("請輸入要%s的資料。\n", message);

    printf("號碼 : ");
    scanf("%d", &temp.no);
    printf("名稱 : ");
    scanf("%s", temp.name);

    return (temp);
}

/*--- 選單選項 ---*/
Menu SelectMenu(void) {
    int  ch;

    do {
        printf("\n(1)在前頭插入節點	(2)在末尾插入節點\n");
        printf("(3)刪除前頭的節點	(4)顯示全部的節點\n");
        printf("(5)刪除全部的節點	(0)結　束　處　理 : ");
        scanf("%d", &ch);
    } while (ch < Term  ||  ch > Clear);

    return ((Menu)ch);
}

/*--- 主程式 ---*/
int main(void) {
    Menu  menu;
    List  list;

    InitList(&list, 100);               /* �鑒怳j節點數為100個 */

    do {
        Node  x;

        switch (menu = SelectMenu()) {
        case Insert:
            x = Read("插入");
            InsertNode(&list, x.no, x.name);
            break;

        case Append:
            x = Read("插入");
            AppendNode(&list, x.no, x.name);
            break;

        case Delete:
            DeleteNode(&list);
            break;

        case Print :
            PrintList(&list);
            break;

        case Clear :
            ClearList(&list);
            break;
        }
    } while (menu != Term);

    TermList(&list);
    return (0);
}
