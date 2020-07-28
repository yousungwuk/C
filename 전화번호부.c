#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXWORD	100
//#define MAXNUM	100		// maximum number of phonebook entries
//#define EOL		'.'		// define end of input

struct tnode {
	char *name;
	struct plist {
		int id;				// 전화부 id
		char *item;			// 전화부 item
		struct plist *next;
	} *pb;
	struct tnode *left;
	struct tnode *right;
};

/* 전화부 ID */
enum PID { MOBILE = 1, HOME, OFFICE, FAX, ETC, EMAIL, ADDR, MEMO };

char *sItem[] = {
	"휴대폰", "집전화", "사무실전화", "팩스", "기타", "이메일", "주소", "메모"
};

struct tnode *createNode();		// create node and initialize
struct plist *createList();		// create list and initialize
struct tnode *addTree(struct tnode *, struct tnode *);	// add a node into binary tree
void addList(struct plist **pstart, struct plist *pj);	// add a list in increasing order of id
void treeprint(struct tnode *);
int dispNode(struct tnode *p, int opt);
int getmnum();
char *strdupl(char *);
// Requirement #3
struct tnode *search(struct tnode *root, char *key) {
	struct tnode *p;
	p = root;
	while (p != NULL) {
		if (strcmp(p->name, key) > 0) {
			p = p->left;
			if (p == NULL) {
				printf("찾는사람이 존재하지않습니다.\n");
				return p;
			}
		}
		if (strcmp(p->name, key) < 0) {
			p = p->right;
			if (p == NULL) {
				printf("찾는사람이 존재하지않습니다.\n");
				return p;
			}
		}
		if (strcmp(p->name, key) == 0) {
			return p;
		}
	}
	return p;
}

int main()
{
	struct tnode *root, *pn;
	struct plist *pi, **pstart;
	char word[MAXWORD];
	char name[100] = { '\0' };
	int m, n;
	root = NULL;
	do {
		printf("\n\t===전화번호부====\n\n"
			"\t1. 추가\n"
			"\t2. 전체보기\n"
			"\t3. 검색\n"
			"\t4. 종료\n");
		while ((m = getmnum()) < 0 || 4 < m);
		switch (m) {
		case 1:	// 추가
			pn = createNode();
			printf("\n이름을 입력하세요: ");
			scanf(" %[^\n]", word);			// get line (after skipping white space)
			pn->name = strdupl(word);		// alloc and copy name
			pn->pb = NULL;
			pstart = &pn->pb;				// list start
			do {
				printf("\n선택하세요\n(0:종료 1:휴대폰 2:집전화 3:사무실전화 "
					"4:팩스 5:기타 6:이메일 7:주소 8:메모): ");
				scanf("%d", &n);
				if (n < 0 || 8 < n) continue;
				if (!n) break;
				pi = createList();
				pi->id = n;
				printf("\n%s 내용을 입력하세요: ", sItem[n - 1]);
				scanf(" %[^\n]", word);
				pi->item = strdupl(word);
				addList(pstart, pi);
			} while (n);
			root = addTree(root, pn);
			break;
		case 2:	// 전체보기
			treeprint(root);
			printf("\n");
			break;
		case 3: //검색 추가해야함//
			pn = createNode();
			printf("찾고자 하는 이름을 입력하시오.: ");
			scanf(" %[^\n]", name);
			pn = search(root, name);
			if (pn == NULL) {
				break;
			}
			dispNode(pn, 0);
			break;
		case 4:
			return 0;
			break;
		default:
			break;
		}
	} while (m);

	return 0;
}

/* getmnum: get menu number */
int getmnum() {
	int m;
	printf("\n\t원하는 메뉴를 선택하세요: ");
	scanf("%d", &m);
	return m;
}

/* addTree: add a node at or below p
print error message if there is same name */
struct tnode *addTree(struct tnode *p, struct tnode *r) {
	struct tnode *tem;
	tem = p;
	if (p == NULL) {
		p = (struct tnode *)malloc(sizeof(struct tnode));
		p = r;
		p->left = NULL;
		p->right = NULL;
		return p;
	}
	else if (strcmp(p->name, r->name) == 0) {
		printf("같은 이름이 존재합니다.\n");
		return p;
	}
	else if (strcmp(p->name, r->name) > 0) {
		p->left = addTree(p->left, r);
	}
	else if (strcmp(p->name, r->name) < 0) {
		p->right = addTree(p->right, r);
	}
	return p;
}

/* addList: add a list at or next pi in increasing order of id */
void addList(struct plist **pstart, struct plist *pj) {
	struct plist *after, *before, *temp = pj, *pi;
	if (*pstart == NULL) {
		pi = createList();
		pi->id = -1;
		pi->next = pj;
		*pstart = pi;
	}
	else {
		before = *pstart;
		after = before->next;
		while (after != NULL) {
			if (after->id < temp->id) {
				before = after;
				after = after->next;
			}
			else if (after->id > temp->id) {
				before->next = temp;
				temp->next = after->next;
				after->next = NULL;
				temp = after;
				before = before->next;
				after = before->next;
			}
			else {
				printf("이미입력된 아이템 입니다.\n");
				return;
			}
		}
		before->next = temp;
	}

}

/* treeprint: in-order print of tree p */
void treeprint(struct tnode *p) {
	if (p != NULL) {
		treeprint(p->left);
		dispNode(p, 0);
		treeprint(p->right);
	}
}

/* dispNode: display a node including list
(opt=1) display with line number, return number of items */
int dispNode(struct tnode *p, int opt) {
	struct plist *pcurrent;
	int i = 1;
	printf("이름: %s\n", p->name);
	if(!opt) {
		pcurrent = p->pb;
		while (pcurrent != NULL) {
			if (pcurrent->id == -1) {
				pcurrent = pcurrent->next;
				continue;
			}
			else {
				printf("%s: %s\n", sItem[(pcurrent->id) - 1], pcurrent->item);
				pcurrent = pcurrent->next;
			}
		}
		return 0;
	}
	return 0;
}

/* createNode: make and initialize a tnode */
struct tnode *createNode() {
	struct tnode *p;
	p = (struct tnode *) malloc(sizeof(struct tnode));
	p->left = p->right = NULL;
	return p;
}

/* createList: make and initialize a list */
struct plist *createList() {
	struct plist *p;
	p = (struct plist *) malloc(sizeof(struct plist));
	p->next = NULL;
	return p;
}

/* strdupl: make a duplicate of a string */
char *strdupl(char *s) {
	char *p;
	p = (char *)malloc(strlen(s) + 1);		// +1 for '\0'
	if (p != NULL)
		strcpy(p, s);
	return p;
}
