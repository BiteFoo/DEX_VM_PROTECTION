package lz.android.shell.processins;

import lz.android.shell.advmpformat.StringItem;

/**
 * Created by lz on 2017/11/2.
 * 用来存储旧的method结构
 */
public class Method {
    private short classNameIdx;
    private  short protoIdx;
    private  int methodNameIdx;
    public void setClassNameIdx(short classNameIdx)
    {
        this.classNameIdx=classNameIdx;
    }
    public  void setProtoIdx(short protoIdx)
    {
        this.protoIdx=protoIdx;
    }
    public void setMethodNameIdx(int methodNameIdx)
    {
        this.methodNameIdx=methodNameIdx;
    }
    public  short getClassNameIdx()
    {
        return  classNameIdx;
    }
    public  short getProtoIdx()
    {
        return  protoIdx;
    }
    public int getMethodNameIdx() {
        return methodNameIdx;
    }
    public  StringItem className;
    public  StringItem methodName;
    public  StringItem proto;
}
