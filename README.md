# balancecar

#### 项目介绍
平衡车代码+MATLAB上位机代码
本来是想做一个小车只需采集并发送数据，接收并执行simulink的命令，用simulink来做姿态解算和PID平衡控制的项目，但是在调试了三天之后以失败告终，主要原因是蓝牙模块的传输速度太慢了，所以就转而求其次，用matlab写了个上位机做姿态显示并且发送一些指令控制小车前进后退，当然也可以用手机蓝牙串口发送指令来控制。


![1](https://gitee.com/cuitsj/balancecar/blob/master/png/sch.png)

#### 软件架构
1. 上位机的代码和exe程序在MATLABGUI文件夹。
2. 平衡小车代码在balance_car文件夹。


#### 相关说明

1.  我的主控芯片是STM32F103C8T6。
2.  电机驱动模块用的L298N。
3.  OLED使用的是4线SPI通讯协议，3线SPI应该亦可以。
4.  PID参数我没有认真调，最近比较忙调了一个上午感觉能用就没再调了，如果车体跟我一样的话不用调或者微调就可以跑起来。
5.  主控板我是参照我画的原理图用洞洞板搭的，有兴趣的小伙伴可以画板制作PCB，如果你是L298N的话请直接使用我的管脚连接方案，这也是我看手册规划出来的，毕竟在程序里面改管脚是比较麻烦的而且容易出现BUG。
6.  没有做航向角的原因是MPU6050没有磁力计，计算出来的航向角偏移的很厉害，而且对于平衡车来说没有航向角对控制没有影响，而且好的磁力计不便宜。有兴趣的话可以加个磁力计，并且MPU也支持外扩磁力计很方便，这样的话就可以做惯性导航。



#### 特技

1.  使用 Readme\_XXX.md 来支持不同的语言，例如 Readme\_en.md, Readme\_zh.md
2.  Gitee 官方博客 [blog.gitee.com](https://blog.gitee.com)
3.  你可以 [https://gitee.com/explore](https://gitee.com/explore) 这个地址来了解 Gitee 上的优秀开源项目
4.  [GVP](https://gitee.com/gvp) 全称是 Gitee 最有价值开源项目，是综合评定出的优秀开源项目
5.  Gitee 官方提供的使用手册 [https://gitee.com/help](https://gitee.com/help)
6.  Gitee 封面人物是一档用来展示 Gitee 会员风采的栏目 [https://gitee.com/gitee-stars/](https://gitee.com/gitee-stars/)
