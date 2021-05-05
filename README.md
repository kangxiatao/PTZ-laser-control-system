## 激光打靶系统源码

好几年前写的程序了，在b站发过激光打靶的视频，有不少人找我要源码，今天就特意找了一下上传到GitHub

## 硬件结构

 - 主控芯片为stm32f103ze
 - 云台由水平和垂直两个舵机搭建
 - 云台上用一块stm32f103z8t6采集陀螺仪数据通过无线串口传至主控


## 代码结构

主函数中做显示和触摸屏输入处理，控制部分全部放在中断函数中
 - Control中为主要的控制代码
 - FWlib为固件库
 - Hardware文件夹中为硬件驱动程序
 - Output为编译器输出文件
 - System为正点原子和stm32官方提供的部分系统文件
 - User_epic文件夹中主要为主函数、UI部分和自己的一些帮助函数
 
## 演示视频

[【电赛】激光打靶控制系统（各种尬吹）](https://www.bilibili.com/video/BV18t411K7cr#reply4517737285)

## 还真是蛮怀恋电赛那段时光的，太美好了，各位且行且珍惜！
