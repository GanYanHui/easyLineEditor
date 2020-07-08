#include <stdio.h>
#include <stdlib.h>//system
#include <string.h>
struct list {
	char string[80];
	struct list *next;
	struct list *pre;
	int flag;//flag=1表示结点有内容
};

FILE *fp1, *fp2;
struct list *head;
int page = 1;

struct list *my_createlist();//建立链表
void my_initFile();//初始化文件
struct list *my_display();//显示活区
struct list *my_change();//活区切换
struct list *my_save();//保存
struct list *my_insert();//行插入
struct list *my_delete_one();//单行删除
struct list *my_delete_more();//多行删除
void my_look();//显示链表内容
struct list *my_replace();//字符串替换
void my_search();//字符串查找

int main()
{
	int choice;
	int choice2;
	struct list *p;
	p = head = (struct list*)malloc(sizeof(struct list));

	while (1)
	{
		printf("***------------------------------------***\n");
		printf("###请选择：1.初始化文件 2.显示链表内容 ###\n");
		printf("###        3.活区显示   4.活区切换     ###\n");
		printf("###        5.行插入     6.行删除       ###\n");
		printf("###        7.字符串查找 8.字符串替换   ###\n");
		printf("###        9.保存       0.退出         ###\n");
		printf("***------------------------------------***\n");
		scanf("%d", &choice);
		while ((!fp1 || !fp2) && choice != 1)
		{
			printf("文件未打开，请重新输入选项:");
			scanf("%d", &choice);
		}
		while (fp1 && fp2 && choice == 1)
		{
			printf("文件已打开，请重新输入选项:");
			scanf("%d", &choice);
		}
		switch (choice) {
		case 1:my_initFile(); break;
		case 2:my_look(); break;
		case 3:my_display(); break;
		case 4:my_change(); break;
		case 5:my_insert(); break;
		case 6:
			printf("请选择：1：单行删除 2：多行删除\n");
			scanf("%d", &choice2);
			if (choice2 == 1) {
				my_delete_one(); break;
			}
			else {
				my_delete_more(); break;
			}
		case 7: my_search(); break;
		case 8: my_replace(); break;
		case 9: my_save(); break;
		case 0:
			for (p = head; head != NULL;)//释放链表空间
			{
				head = p->next;
				free(p);
				p = head;
			}
			fclose(fp1);
			fclose(fp2);
			system("PAUSE");
			exit(0); break;
		}
	}
	return 0;
}
void my_initFile()//初始化
{//打开文件1，创建文件2
	char fname[30], out_fname[30];
	int choice = 2;
	system("cls");
	printf("请先打开文件\n");
	printf("请输入要(打开)的文件名<可包括盘符、路径、不超过30个字符>：(如d:\\f12-1.txt)\n");
	scanf("%s", fname);
	while (!(fp1 = fopen(fname, "r+")))
	{
		printf("文件打开错误，请输入选择： 1：重新输入2：返回主界面\n");
		scanf("%d", &choice);
		if (choice == 2) {
			return;
		}
		else {
			printf("请输入文件名：");
			scanf("%s", fname);
		}
	}
	printf("打开成功!\n");
	printf("请输入要(写入)的文件名<可包括盘符、路径、不超过30个字符>：(如d:\\f12-1.txt)\n");
	scanf("%s", out_fname);
	while (!(fp2 = fopen(out_fname, "w+")))
	{
		printf("文件打开错误，请输入选择： 1：重新输入2：返回主界面\n");
		scanf("%d", &choice);
		if (choice == 2) {
			return;
		}
		else {
			printf("请输入文件名：");
			scanf("%s", out_fname);
		}
	}
	system("cls");
	printf("文件创建成功!将自动创建链表...\n");
	system("PAUSE");
	head = my_createlist();
	system("cls");
	printf("创建链表成功!\n");
	system("PAUSE");
}
struct list *my_createlist()//建立链表
{
	int i;
	struct list *p, *now;
	if (!fp1 || !fp2)
	{
		printf("发生了不可预期的错误，请先初始化文件\n");
		system("PAUSE");
		return head;
	}
	p = (struct list*)malloc(sizeof(struct list));
	head = NULL;
	p->next = NULL;
	p->flag = 0;
	for (i = 0; i < 20; i++)
	{
		if (i == 0)
		{
			head = p;
		}
		else
		{
			p->next = now;
			now->pre = p;
			p = now;
			now = p->next;
		}
		now = (struct list*)malloc(sizeof(struct list));
		now->flag = 0;
		now->next = NULL;
	}
	now = NULL;
	return head;
}
void my_look()//显示链表内容
{
	int i, sum = 0;
	struct list *p;

	if (!fp1 || !fp2)//文件未打开
	{
		printf("发生了不可预期的错误，请先初始化文件\n");
		system("PAUSE");
		return;
	}
	for (p = head; p != NULL && p->flag == 1; p = p->next)//计算链表有多少结点
	{
		sum++;
	}
	p = head;
	if (sum == 0 || head == NULL) {//链表为空
		printf("链表为空\n");
	}
	else
	{
		for (i = 1; i <= sum; i++)
		{
			printf("%4d :%s", i, p->string);
			printf("\n");
			p = p->next;
		}
	}
}
struct list *my_display()//显示活区
{//将文件内容读入链表中

	int i, choice = 1, j;
	struct list *p;

	p = (struct list*)malloc(sizeof(struct list));
	p = head;

	if (!fp1 || !fp2)//文件未打开
	{
		printf("显示错误，请先打开文件");
		system("PAUSE");
		return head;
	}
	if (feof(fp1))//文件已结束
	{
		printf("文件已经结束\n");
		system("PAUSE");
		return head;
	}
	while ((!feof(fp1)) && choice == 1)
	{
		p = head;
		printf("第%d页：\n", page++);
		for (i = 1, j = 1; i <= 20 && (!feof(fp1)); i++)
		{//从文件1里读取内容到链表中
			if (fgets(p->string, sizeof(p->string), fp1))
			{
				printf("%4d :", i);
				puts(p->string);
				p->flag = 1;//此结点有内容
				p = p->next;
			}
		}
		printf("请选择: 1：下一页 0：返回主页面\n");
		scanf("%d", &choice);
		if (feof(fp1) && choice == 1)
		{
			printf("文件已经结束\n");
			system("PAUSE");
			return head;
		}
	}
	return head;
}
struct list *my_change()//活区切换
{//将链表里的内容存入文件2
 //从文件1中读取内容到链表
	int i, choice = 1, j;
	struct list *p;

	p = (struct list*)malloc(sizeof(struct list));
	if (!fp1 || !fp2)
	{
		printf("发生了不可预期的错误，请先打开文件\n");
		system("PAUSE");
		return head;
	}
	if (feof(fp1))
	{
		printf("文件已经结束\n");
		system("PAUSE");
		return head;
	}
	while (!feof(fp1) && choice == 1)
	{
		p = head;
		for (i = 1; i <= 20; i++)//把链表里的内容输出到fp2中
		{
			fputs(p->string, fp2);//向文件写入一个字符串
			p->flag = 0;//此结点无内容
			p = p->next;
		}
		p = head;
		if (!feof(fp1))
		{
			printf("第%d页 :\n", page++);
		}
		for (i = 1, j = 1; i <= 20 && (!feof(fp1)); i++)//将文件内容读入链表中
		{
			if (fgets(p->string, sizeof(p->string), fp1))
			{
				printf("%4d :", j++);
				puts(p->string);
				p->flag = 1;//此结点有内容
				p = p->next;
			}
		}
		printf("切换成功!");
		printf("内容已保存,请选择: 1：继续切换 2：返回主页面\n");
		scanf("%d", &choice);
		if (feof(fp1) && choice == 1)
		{
			printf("文件已经结束\n");
			system("PAUSE");
			return head;
		}
	}
	return head;
}
struct list *my_insert()//行插入
{
	int i, n, j, sum;
	struct list *pre, *now, *p, *p2;
	char q[81];

	if (!fp1 || !fp2)
	{
		printf("发生了不可预期的错误，请先打开文件\n");
		system("PAUSE");
		return head;
	}
	sum = 0;
	for (p = head; p != NULL && p->flag == 1; p = p->next)//sum用来计算有内容的结点个数
	{
		sum++;
	}
	//此时sum＝结点个数
	printf("请输入要插入的行号（0-%d）：\n", sum);
	scanf("%d", &n);
	while (n < 0 || n>sum)
	{
		printf("行号应在0-%d之间,请重新输入：\n", sum);
		scanf("%d", &n);
	}
	printf("请输入内容：\n");
	scanf("\n");
	gets(q);
	strcat(q, "\n");
	if (sum < 20 && n >= sum)//直接插在末尾
	{
		p = head;
		for (j = 1; j < sum; j++)
		{
			p = p->next;
		}//此时p指向末尾，第sum个
		strcpy(p->next->string, q);
		p->next->flag = 1;
	}
	else if (sum < 20 && n < sum)//新建结点并插入到d之后，删除有内容的结点末尾后面一个以保证结点个数恒定
	{
		p2 = (struct list*)malloc(sizeof(struct list));
		strcpy(p2->string, q);
		p2->flag = 1;

		if (n == 0)//新建一个结点作为头，插在原本头前面
		{
			p2->next = head;
			head->pre = p2;
			head = p2;
		}
		else {//新建一个结点插在第d个之后
			p = head;
			for (j = 1; j < n; j++)
			{
				p = p->next;
			}//此时p指向第d个
			p2->next = p->next;
			p2->pre = p;
		}
		p = head;
		while (p->flag == 1)
		{
			p = p->next;
		}//p指向最后一个结点的后面一个，此结点无内容
		p->pre->next = p->next;
		p->next->pre = p->pre;
		free(p);
	}
	else if (n == 0 && sum == 20)//直接将q存入文件中
	{
		fputs(q, fp2);
	}
	else//把第一行读入文件，插入的行之前（包括这行）的内容往前移动一位
	{//再把要插入的内容插入
		pre = head;
		now = pre->next;
		fputs(pre->string, fp2);
		pre->flag = 0;

		for (i = 1; i < n; i++)//把插入行前的内容往前推一位
		{
			strcpy(pre->string, now->string);
			pre->flag = 1;
			now->flag = 0;

			pre = now;
			now = pre->next;
		}
		strcpy(pre->string, q);//把内容读入该插入行
		pre->flag = 1;
	}
	printf("插入成功!\n");
	system("PAUSE");
	my_look();
	system("PAUSE");
	return head;
}
struct list *my_delete_one()//单行删除
{
	int d, i, sum = 0;
	struct list *p1, *p2, *p3, *p;
	p1 = p2 = p3 = (struct list*)malloc(sizeof(struct list));
	p1 = p2 = p3 = head;
    //p1记录p2前一个位置，p3记录p2下一个位置
	if (!fp1 || !fp2)//文件未打开
	{
		printf("发生了不可预期的错误，请先打开文件\n");
		system("PAUSE");
		return head;
	}
	for (p = head; p != NULL && p->flag == 1; p = p->next) {//计算有内容的结点个数
		sum++;
	}
	p = head;
	if (sum == 0 || head == NULL)//链表为空
	{
		printf("链表为空，无可删除行\n");
		system("PAUSE");
		return head;
	}
	printf("请输入你想删除的行号(1-%d):\n", sum);
	scanf("%d", &d);
	while (d < 1 || d > sum)
	{
		printf("删除的行号不对，请重新输入(1-%d):\n", sum);
		scanf("%d", &d);
	}
	for (i = 1; i < d; i++)
	{
		p2 = p2->next;
	}
	//此时p2指向要删除的行
	if (d == 1)//删除的是头节点
	{
		p3 = p2->next;
		free(p2);
		head = p3;
	}
	else//删除非头节点
	{
		p1 = p2->pre;
		p3 = p2->next;
		p1->next = p3;
		p3->pre = p1;
		free(p2);
	}
	p2 = (struct list*)malloc(sizeof(struct list));//删除完后新建一个结点插在末尾，以保证结点个数恒定
	p2 = head;
	while (p2->next)//找末尾
	{
		p2 = p2->next;
	}
	//此时p2指向链表最后一个
	//再创建一个结点p
	//使p2后一个是p
	p = (struct list*)malloc(sizeof(struct list));
	p->flag = 0;
	p->next = NULL;
	p->pre = p2;
	p2->next = p;
	printf("删除成功!\n");
	system("PAUSE");
	my_look();
	system("PAUSE");
	return head;
}
struct list *my_delete_more()//多行删除
{
	int start, end, i, num = 0/*要删除的行数*/, sum = 0;
	struct list *p1, *p2, *p3, *p/*创建新的结点*/;
	//p1记录p2前一个位置，p3记录p2下一个位置

	if (!fp1 || !fp2)//文件未打开
	{
		printf("发生了不可预期的错误，请先打开文件\n");
		system("PAUSE");
		return head;
	}
	for (p = head; p != NULL && p->flag == 1; p = p->next)//计算有内容的结点的个数
	{
		sum++;
	}
	if (sum == 0 || head == NULL)//链表为空
	{
		printf("链表为空，无可删除行\n");
		system("PAUSE");
		return head;
	}
	printf("请输入你想删除的起始行(1-%d):\n", sum);
	scanf("%d", &start);
	while (start < 1 || start > sum)
	{
		printf("输入错误，请重新输入你想删除的起始行(1-%d):\n", sum);
		scanf("%d", &start);
	}
	printf("请输入你想删除的终止行(%d-%d):\n", start, sum);
	scanf("%d", &end);
	while (end < start || end > sum)
	{
		printf("输入错误，请重新输入你想删除的终止行(%d-%d):\n", start, sum);
		scanf("%d", &end);
	}
	num = end - start + 1;//num为要删除的行数
	p2 = head;
	for (i = 1; i < start; i++)//找到删除的起始行
	{
		p2 = p2->next;//i=start时，p2指向第start行
	}
	//此时p2指向删除的起始行
	for (i = 0; i < num; i++)//删除num行
	{
		if (start == 1)//从第一行开始删
		{
			p3 = p2->next;
			free(p2);
			head = p3;
			p2 = p3;
		}
		else//从中间开始删
		{
			p1 = p2->pre;
			p3 = p2->next;
			p1->next = p3;
			p3->pre = p1;
			free(p2);
			p2 = p3;
		}
	}
	//此时p1等于起始行的前一行
	//此时p2,p3都等于终止行的下一行
	p2 = head;
 	while (p2->next!=NULL)//找末尾
	{
		p2 = p2->next;
	}
	//此时p2指向最后一行
	for (i = 0; i < num; i++)//创建num个结点在末尾,以保证结点个数恒定
	{
		p = (struct list*)malloc(sizeof(struct list));
		p->flag = 0;
		p->next = NULL;
		p->pre = p2;
		p2->next = p;
		p2 = p;//p2再次指向末尾
	}
	printf("删除成功!\n");
	system("PAUSE");
	my_look();
	system("PAUSE");
	return head;
}
struct list *my_save()//保存
{//将链表里的内容存入文件中
	int i;
	struct list *p = head;
	if (!fp1 || !fp2)
	{
		printf("发生了预期之外的错误，请先打开文件");
		system("PAUSE");
		return head;
	}
	for (i = 0; i < 20 && p->flag == 1; i++)
	{
		fputs(p->string, fp2);
		p->flag = 0;
		p = p->next;
	}
	while (!feof(fp1))//如果文件1没有结尾，则将文件1剩余内容全部读入文件2
	{
		p = (struct list*)malloc(sizeof(struct list));
		fgets(p->string, sizeof(p->string), fp1);
		fputs(p->string, fp2);
	}
	printf("保存成功！\n");
	system("PAUSE");
	return head;
}
struct list *my_replace()//字符串替换
{
	char q[81], t[81], m[81], flag[20], flag2[20];//q为待替换字符串,即m为母串，q为子串，q将替换为t, flag用于决定是否替换， flag2用于决定是否继续替换
	int count = 0, i, j, k, n1, n2, n3, sign = 0, sum = 0;//sign用来表示是否找到
	struct list *pre, *now;

	if (!fp1 || !fp2)
	{
		printf("发生了预期之外的错误，请先打开文件");
		system("PAUSE");
		return head;
	}
	for (pre = head; pre != NULL && pre->flag == 1; pre = pre->next) {//计算链表的有内容的结点个数
		sum++;
	}
	if (sum == 0 || head == NULL) {
		printf("链表为空，无可替换字符\n");
		system("PAUSE");
		return head;
	}
	printf("请输入待替换的字符串：");
	scanf("%s", q);
	printf("替换为(暂时只能接受相同字符数量):");
	scanf("%s", t);
	pre = now = head;
	k = 1;//行
	while (now != NULL && k < sum) {//一行一行比较
		n1 = strlen(now->string);//这行内容大小
		n2 = strlen(q);//待替换的字符串大小
		n3 = strlen(t);//替换成的字符串大小
		strcpy(m, now->string);//m用来暂时储存该行的数据
		/*在m中找q*/
		for (i = 0; i < n1; i++) {//在一行内查找
			j = 0;//刷新j
			strcpy(flag, "N");
			strcpy(flag2, "N");
			if (m[i] == q[0]) {//m的第i个字符与q第0个字符比较
				for (j = 0; j < n2; j++) {//第一个相等，判断后续是否也相等
					if (m[i + j] != q[j]) {//m的第i+j个字符与q的第j个字符比较
						break;
					}
				}
			}
			if (j == n2) {//正常退出循环，说明全部相等，找到了
				printf("第%d行：%s\n", k, m);
				count++; sign = 1;
				printf("第%d个字符处找到。是否替换<Y/N>?", count);
				scanf(" ");
				gets(flag);
			}
			if (strcmp(flag, "Y") == 0) {//要替换
				for (j = 0; j < n3; j++) {//把m中的q替换成t
					m[i + j] = t[j];
				}
				strcpy(now->string, m);//暂时只能替换相同数量的
				printf("替换成功,继续替换吗<Y/N>?");
				scanf(" ");
				gets(flag2);
			}
			if (strcmp(flag2, "N") == 0 && strcmp(flag, "Y") == 0) {//退出for循环
				break;
			}
			else {//继续替换接着找 或者 没找到接着找 或者 不替换接着找
				sign = 0;
			}
		}
		if (strcmp(flag2, "N") == 0 && strcmp(flag, "Y") == 0) {//退出while循环
			break;
		}
		pre = now;
		now = pre->next;
		k++;
	}
	if (sign == 0) {
		printf("没找到\n");
	}
	return head;
}
void my_search()//字符串查找
{
	char q[81], m[81], flag[20] = "Y";//q为待查找字符串,即m为母串，q为子串
	int count = 0, i, j, k, n1, n2, sign = 0, sum = 0;
	struct list *pre, *now;

	if (!fp1 || !fp2)
	{
		printf("发生了预期之外的错误，请先打开文件");
		system("PAUSE");
		return ;
	}
	for (pre = head; pre != NULL && pre->flag == 1; pre = pre->next) {//计算有内容的结点个数
		sum++;
	}
	if (sum == 0 || head == NULL) {
		printf("链表为空，无可查找字符\n");
	}
	else {
		printf("请输入待查找的字符串：\n");
		scanf("\n");
		gets(q);
		pre = now = head;
		k = 1;//行
		while (now != NULL && strcmp(flag, "N") != 0 && k < sum) {//一行一行比较
			n1 = strlen(now->string);//行的长度
			n2 = strlen(q);//要查找的字符串长度
			strcpy(m, now->string);
			/*在m中找q*/
			for (i = 0; i < n1; i++) {//在一行内查找
				j = 0;//刷新j
				if (m[i] == q[0]) {//m的第i个字符与q第0个字符比较
					for (j = 0; j < n2; j++) {//第一个相等，判断后续是否也相等
						if (m[i + j] != q[j]) {//m的第i+j个字符与q的第j个字符比较
							break;
						}
					}
				}
				if (j == n2) {//正常退出循环，说明全部相等，找到了
					printf("第%d行：%s\n", k, m);
					count++; sign = 1;
					printf("第%d个字符处找到。继续查找吗<Y/N>?", count);
					gets(flag);
				}
				if (strcmp(flag, "N") == 0) {//flag等于N
					break;
				}
				else {
					sign = 0;
				}
			}
			pre = now;
			now = pre->next;
			k++;
		}
		if (sign == 0) {
			printf("没找到\n");
		}
	}
}
