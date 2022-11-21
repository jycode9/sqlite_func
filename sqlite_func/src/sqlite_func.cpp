#include <iostream>
#include "../sqlite_func.h"

void functest() {

    sqlfunc my_sql("../src/test.db");

    //做成单例之后，使用宏定义将column、value进行简化
    //my_sql.insertData("USER", my_sql.Columns("ID", "NAME"), my_sql.Values("4", "'WENJY4'"));
    my_sql.insertData("USER", my_sql.Values("5", "'WENJY5'"));

    my_sql.updateData("USER", my_sql.Columns("NAME"), my_sql.Values("'LILI'"));
    //my_sql.updateData("USER", my_sql.Columns("NAME"), my_sql.Values("LILI"), my_sql.Where("ID==1"), my_sql.And("AGE==23"));

    //my_sql.deleteData("USER", "NAME", "'Jam'");

    //newchar是一个指针变量。它指向了内存中“NAMETEST”这个常量的地址。
    //当我们要修改newchar的值的时候，是无法直接修改NAMETEST的，这个数据已经固定


    //strcat的原理应该是进行数组类型的拼接，相当于，在原来的字符后面pushback东西。
    //但是newchar本身的值和NAMETEST绑定了，强行修改会造成错误的。
    //想要修改，可以这样
    //strcat(newchar, " LLLL");



}


int main()
{
    std::cout << "start main" << std::endl;

    functest();


    std::cout << "end main" << std::endl;

    

}


