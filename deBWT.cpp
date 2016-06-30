#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <utility>
#include <ctime>
#define N 100000
using namespace std;


char t[N]="AGCTAGGGTC";     //存放从文件中读取的原串
char *str[N];               //存放原串的所有后缀子串
char BWT[N];                //存放BWT序列，用于序列片段的精确比对
int t_len;                  //原串的长度
int len;                    //加$符号后的长度

char T[N]="";               //存放根据BWT序列获得的原串
int Occ[5][N];              //记录BWT序列第j位之前字符i出现的次数
int C[5];                   //记录排在字符i之前的所有字符总和


//原串从第i位切开得到后缀子串
char *Substr(char *s,int i)
{
    s = s+i;
    return s;
}

//比较两个字符串大小
bool cmp(char *t1,char *t2)
{
    if(strcmp(t1,t2)<0)                       //t1比t2小
        return true;
    else
        return false;
}

//得到BWT序列
void get_BWT()
{
    int i;
    for(i = 0; i<len; i++)
    {
        BWT[i]=t[(len-strlen(str[i])-1+len)%len];
    }
}

//求碱基对应的C数组的下标
int num(char c)
{
    if(c == 'A' || c == 'a')
        return 0;
    else if(c == 'C' || c == 'c')
        return 1;
    else if(c == 'G' || c == 'g')
        return 2;
    else if(c == 'T' || c == 't')
        return 3;
    else
    {
        printf("文件中含有非法字符!");
        exit(0);
    }
}

//找到前一个字符在BWT序列中的下标
int LF(int r)
{
    char c = BWT[r];
    int i = num(c);
    return C[i]+Occ[i][r];
}

//还原原序列
void UNPERMUTE()
{
    int r = 0;
    int l = t_len;                                  //从最后一位开始回溯
    while(BWT[r]!= '$')
    {
        l--;
        T[l] = BWT[r];
        r = LF(r);                                  //得到前一个字符在BWT序列的下标
    }
}

//找到前一个字符在BWT序列中的下标
int LFC(int r,char c)
{
    return C[num(c)]+Occ[num(c)][r];
}
//实现精确匹配，找出片段在原串中的位置
pair<int,int> Exactmatch(char *p)
{
    int sp,ep,l;                              //其实下标，终止下标，片段长度
    l = strlen(p)-1;
    char c = p[l];
    sp = C[num(c)];                           //末尾字符的起始下标
    ep = C[num(c)+1];                         //末尾字符的终止下标
    l -= 1;                                   //前一个字符
    while(sp<ep && l>=0)
    {
        c = p[l--];
        sp = LFC(sp,c);                       //字符的起始下标
        ep = LFC(ep,c);                       //字符的终止下标
    }
    return make_pair(sp,ep);
}
int main()
{
    int i,j;
    pair<int,int> pa;

    cout<<"原序列为:"<<t<<endl;


    //求BWT序列
    t_len=strlen(t);
    t[t_len]='$';                                                    //在原串末尾添加$符号
    len = t_len+1;
    for( i=0; i<len; i++)
    {
        str[i] = Substr(t,i);
    }
    sort(str,str+len,cmp);
    get_BWT();
    printf("得到的BWT序列:%s\n",BWT);


    for(i=1; i<=len; i++)
    {
        for(j=0; j<4; j++)
            Occ[j][i] += Occ[j][i-1];
        if(BWT[i-1] != '$')
        {
            C[num(BWT[i-1])+1]+=1;                                          //计算C数组
            Occ[num(BWT[i-1])][i] +=1;                                          //计算Occ
        }
    }
    C[0]=1;
    C[1]+=C[0];
    C[2]+=C[1];
    C[3]+=C[2];
    C[4]+=C[3];

    /*UNPERMUTE();                                                                //根据BWT还原字符串
    printf("得到的原串:\n%s\n",T);
    */

    char p[1000]="CTAGGG";
    cout<<"搜索片段:"<<p<<endl;
    pa = Exactmatch(p);
    if(pa.first<pa.second)
    {
        printf("该片段出现在原串的:");
        for(i = pa.first; i<pa.second; i++)
            printf("第%d位\t",(len-strlen(str[i])-1+len+2)%len);
        printf("\n");
    }
    else
        printf("找不到该片段出现的位置!\n");

    return 0;
}


