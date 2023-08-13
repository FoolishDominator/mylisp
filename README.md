# 编译方法
1. 在项目根目录mylisp下新建build文件夹
```shell
mkdir build
```
2. 进入build目录使用cmake编译
```shell
cd build
cmake ..
make
```
3. 此时在/mylisp/bin目录下生成可执行文件mylisp
# 运行方法
mylisp可执行文件执行方法：./mylisp [filename]

例如：假设在项目根目录/mylisp下,执行如下代码能查看解释结果
```shell
./bin/mylisp test.lisp
```
# 测试文件解释
以.lisp结尾的文件即为测试文件，直接将其作为mylisp程序的参数即可进行解释

# 解释器完成情况
1. 7个基本操作：quote, atom, eq, car, cdr, cons, cond.(支持用'a代替(quote a)的简写方法)
2. 支持string
3. lambda函数调用
4. label函数调用