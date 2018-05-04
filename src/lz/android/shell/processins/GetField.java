package lz.android.shell.processins;

import lz.android.shell.advmpformat.StringItem;

public class GetField {
    private short classNameIdx;
    private  short typeIdx;
    private  int fieldNameIdx;
    public void setClassNameIdx(short classNameIdx)
    {
        this.classNameIdx=classNameIdx;
    }
    public  void setTypeIdx(short typeIdx)
    {
        this.typeIdx=typeIdx;
    }
    public void setfiledNameIdx(int fieldNameIdx)
    {
        this.fieldNameIdx=fieldNameIdx;
    }
    public  short getClassNameIdx()
    {
        return  classNameIdx;
    }
    public  short getTypeIdx()
    {
        return  typeIdx;
    }
    public int getFieldNameIdx() {
        return fieldNameIdx;
    }
    public StringItem className;
    public  StringItem typeName;
    public StringItem filed;
}
