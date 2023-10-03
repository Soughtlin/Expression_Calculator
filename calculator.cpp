#include "calculator.h"
#include "ui_calculator.h"

Calculator::Calculator(QWidget *parent):QWidget(parent),ui(new Ui::Calculator){ui->setupUi(this);}

Calculator::~Calculator(){delete ui;}

void Calculator::init(){
    //找到所有按钮，放到一个按钮组中，更便捷地设置按钮
    buttonGroup=new QButtonGroup(this);
    auto buttonList=findChildren<QPushButton*>();    //findChildren,findchild
    for(auto btn:buttonList){
        buttonGroup->addButton(btn);
    }

    //关联按钮组click信号
    //void (QButtonGroup::*buttonClick)(QAbstractButton*)=&QButtonGroup::buttonClicked;
    QObject::connect(buttonGroup,QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked),this,&Calculator::buttonSlot);
}

//这里一定要QAbstractButton指针做参数,信号和槽的参数必须兼容(connect检查)，信号参数个数>=槽且要求对应参数类型相同
//buttonClicked的参数类型是QAbstractButton
void Calculator::buttonSlot(QAbstractButton* btn){
    //qInfo("%s",btn->text().toLatin1().data());   //toLatin1转ASCII
    QString name=btn->text();
    if(name=="AC"){
        ui->line_expression->clear();
        ui->line_result->clear();
    }
    else if(name=="del"){
        ui->line_expression->setCursorPosition(ui->line_expression->cursorPosition()-1);
        ui->line_expression->del();
    }
    else if(name=="<-"){
        ui->line_expression->setCursorPosition(ui->line_expression->cursorPosition()-1);
    }
    else if(name=="->"){
        ui->line_expression->setCursorPosition(ui->line_expression->cursorPosition()+1);
    }
    else if(name=="="){
        QString expression = ui->line_expression->text();
        ui->line_result->clear();
        ui->line_result->setText(QString::number(compute(expression)));
    }
    else if(name=='.'){}  //小数运算暂未实现
    else{
        ui->line_expression->insert(name);
    }
}

//利用双栈计算表达式
//操作数直接进操作数栈
//操作符进栈：空栈、优先级高于栈顶操作符、左括号，
double Calculator::compute(QString exp){
    QStack<char> op_stack;
    op_stack.clear();
    QStack<double> num_stack;
    num_stack.clear();
    for(int i=0;i<exp.length();i++){
        //操作符栈操作
        if(exp.at(i)=='+'||exp.at(i)=='-'||exp.at(i)=='*'||exp.at(i)=='/'||exp.at(i)=='('||exp.at(i)==')'){
            //栈为空时
            if(op_stack.isEmpty()){
                op_stack.push(exp.at(i).unicode());
                continue;
            }
            //栈不为空
            char opi=exp.at(i).unicode();
            switch(opi){
            case '+':
                if(op_stack.top()=='('){
                    op_stack.push(opi);
                }
                else{
                    char op=op_stack.pop();
                    double num1=num_stack.pop();
                    double num2=num_stack.pop();
                    switch(op){
                    case'+':
                        num_stack.push(num2+num1);  //先出来放后面
                        break;
                    case'*':
                        num_stack.push(num2*num1);
                        break;
                    case'/':
                        num_stack.push(num2/num1);
                        break;
                    case'-':
                        num_stack.push(num2-num1);
                        break;
                    }
                    op_stack.push(opi);
                }
                break;
            case '-':
                if(op_stack.top()=='('){
                    op_stack.push(opi);
                }
                else{
                    char op=op_stack.pop();
                    double num1=num_stack.pop();
                    double num2=num_stack.pop();
                    switch(op){
                    case'+':
                        num_stack.push(num2+num1);
                        break;
                    case'*':
                        num_stack.push(num2*num1);
                        break;
                    case'/':
                        num_stack.push(num2/num1);
                        break;
                    case'-':
                        num_stack.push(num2-num1);
                        break;
                    }
                    op_stack.push(opi);
                }
                break;
            case '*':
                if(op_stack.top()=='+'||op_stack.top()=='-'||op_stack.top()=='('){
                    op_stack.push(opi);
                }
                else{
                    char op=op_stack.pop();
                    double num1=num_stack.pop();
                    double num2=num_stack.pop();
                    switch(op){     //不删了，说不定以后还有用
                    case'+':
                        num_stack.push(num2+num1);
                        break;
                    case'*':
                        num_stack.push(num2*num1);
                        break;
                    case'/':
                        num_stack.push(num2/num1);
                        break;
                    case'-':
                        num_stack.push(num2-num1);
                        break;
                    }
                    op_stack.push(opi);
                }
                break;
            case '/':
                if(op_stack.top()=='+'||op_stack.top()=='-'||op_stack.top()=='('){
                    op_stack.push(opi);
                }
                else{
                    char op=op_stack.pop();
                    double num1=num_stack.pop();
                    double num2=num_stack.pop();
                    switch(op){
                    case'+':
                        num_stack.push(num2+num1);
                        break;
                    case'*':
                        num_stack.push(num2*num1);
                        break;
                    case'/':
                        num_stack.push(num2/num1);
                        break;
                    case'-':
                        num_stack.push(num2-num1);
                        break;
                    }
                    op_stack.push(opi);
                }
                break;
            case'(':
                op_stack.push(opi);
                break;
            case')':
                char flag=op_stack.top();
                while(!op_stack.isEmpty()&&flag!='('){
                    double num1=num_stack.pop();
                    double num2=num_stack.pop();
                    switch(flag){
                    case'+':
                        num_stack.push(num2+num1);
                        break;
                    case'*':
                        num_stack.push(num2*num1);
                        break;
                    case'/':
                        num_stack.push(num2/num1);
                        break;
                    case'-':
                        num_stack.push(num2-num1);
                        break;
                    }
                    if(!op_stack.isEmpty()) op_stack.pop();
                    if(!op_stack.isEmpty()) flag=op_stack.top();
                }
                if(!op_stack.isEmpty()) op_stack.pop();
                break;
            }
        }
        else if(exp.at(i)==' '||exp.at(i)=='.'){continue;}
        //操作数栈操作
        else{
            int j=i;
            //个位数处理，包含此个位数是表达式中最后一个数
            if(j==exp.length()-1||exp.at(j+1).unicode()<'0'||exp.at(j+1).unicode()>'9'){
                num_stack.push(exp.at(j).unicode()-'0');
            }
            //非个位数处理
            else{
                double ret=0;
                char num=exp.at(j).unicode();
                while(j<exp.length()&&num>='0'&&num<='9'){
                    ret=ret*10+num-'0';
                    j++;
                    if(j<exp.length()) num=exp.at(j).unicode();
                    else break;
                }

                num_stack.push(ret);
                i=j-1;  //这一点刚开始没想到:j已扫描完的同一个整数的字符，i必须跳过，否则后面for循环会不断扫描到这个数的低位，stack中存入了许多不应该的数，用stack的倒数第二层验证是很有效的debug方法
            }
        }
    }
    //最后将操作数栈中剩余操作符进行运算
    while(1){
        if(op_stack.isEmpty()) break;
        char op=op_stack.pop();
        double num1=num_stack.pop();
        double num2=num_stack.pop();
        switch(op){
        case'+':
            num_stack.push(num2+num1);
            break;
        case'-':
            num_stack.push(num2-num1);
            break;
        case'*':
            num_stack.push(num2*num1);
            break;
        case'/':
            num_stack.push(num2/num1);
            break;
        }
    }
    //num_stack.pop();
    return num_stack.top();
}









