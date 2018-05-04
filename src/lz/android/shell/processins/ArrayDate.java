package lz.android.shell.processins;

public class ArrayDate {
    public int pos;//记录新数组的修改位置
    public int oldpos; //记录原始数组的位置
    public int size=1;//记录修改长度，应该为1个short
    public int off;  //记录修改位需要加上的偏移
    public int  keyvalue; //记录修改位值
}
