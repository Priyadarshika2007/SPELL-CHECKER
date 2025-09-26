#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100
#define LIMIT 5

typedef struct Node {
    char word[MAX];
    struct Node *l, *r;
    int h;
} Node;

int max(int a, int b) { return a > b ? a : b; }
int height(Node* n) { return n ? n->h : 0; }

Node* newNode(const char* w) {
    Node* n = (Node*)malloc(sizeof(Node));
    strcpy(n->word, w);
    n->l = n->r = NULL;
    n->h = 1;
    return n;
}

Node* rotRight(Node* y) {
    Node *x = y->l, *T = x->r;
    x->r = y; y->l = T;
    y->h = max(height(y->l), height(y->r)) + 1;
    x->h = max(height(x->l), height(x->r)) + 1;
    return x;
}

Node* rotLeft(Node* x) {
    Node *y = x->r, *T = y->l;
    y->l = x; x->r = T;
    x->h = max(height(x->l), height(x->r)) + 1;
    y->h = max(height(y->l), height(y->r)) + 1;
    return y;
}

int balance(Node* n) { return n ? height(n->l) - height(n->r) : 0; }

Node* insert(Node* n, const char* w) {
    if (!n) return newNode(w);
    int cmp = strcmp(w, n->word);
    if (cmp < 0) n->l = insert(n->l, w);
    else if (cmp > 0) n->r = insert(n->r, w);
    else return n;

    n->h = 1 + max(height(n->l), height(n->r));
    int b = balance(n);

    if (b > 1 && strcmp(w, n->l->word) < 0) return rotRight(n);
    if (b < -1 && strcmp(w, n->r->word) > 0) return rotLeft(n);
    if (b > 1 && strcmp(w, n->l->word) > 0) { n->l = rotLeft(n->l); return rotRight(n); }
    if (b < -1 && strcmp(w, n->r->word) < 0) { n->r = rotRight(n->r); return rotLeft(n); }

    return n;
}

int search(Node* root, const char* w) {
    if (!root) return 0;
    int cmp = strcmp(w, root->word);
    if (cmp == 0) return 1;
    return cmp < 0 ? search(root->l, w) : search(root->r, w);
}

void suggest(Node* root, const char* pre, int* count) {
    if (!root || *count >= LIMIT) return;
    suggest(root->l, pre, count);
    if (strncmp(root->word, pre, strlen(pre)) == 0) {
        printf("  Suggestion: %s\n", root->word);
        (*count)++;
    }
    suggest(root->r, pre, count);
}

void freeTree(Node* root) {
    if (!root) return;
    freeTree(root->l);
    freeTree(root->r);
    free(root);
}

int main() {
    const char* dict[] = {
        "hello", "world", "spell", "checker", "tree",
        "data", "structure", "check", "cheese", "choice",
        "trick", "trie"
    };
    int n = sizeof(dict) / sizeof(dict[0]);
    Node* root = NULL;
    for (int i = 0; i < n; i++) root = insert(root, dict[i]);

    char word[MAX];
    printf("Enter words (type 'exit' to quit):\n");
    while (1) {
        printf("\nWord: ");
        scanf("%s", word);
        if (strcmp(word, "exit") == 0) break;
        if (search(root, word)) printf("Correct\n");
        else {
            printf("Incorrect\n");
            int count = 0;
            suggest(root, word, &count);
            if (!count) printf("  No suggestions found.\n");
        }
    }

    freeTree(root);
    return 0;
}