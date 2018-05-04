package lz.android.shell.processins;


import lz.android.shell.advmpformat.StringItem;


import java.util.ArrayList;
import java.util.List;

/**
 * Created by lz on 2017/11/3.
 */
public class DexFileFormat {
//    public class StringData
//    {
//        List<StringItem> stringItems = new ArrayList<StringItem>();
//    }
//    public class TypeItemDate
//    {
//        List<Integer> typeItems=new ArrayList<Integer>();
//    }
//    public  class MethodItemDate
//    {
//        List<Integer> classNameIdx=new ArrayList<Integer>();
//        List<Integer> protoIdx=new ArrayList<Integer>();
//        List<Integer> methodNameIdx=new ArrayList<Integer>();
//    }
public static class StringData
{
    public StringItem stringItems;
}
    public static class TypeItemDate
    {
        public int typeItem;
    }
    public static class MethodItemDate
    {

       public int classNameIdx;
       public int protoIdx;
        public int methodNameIdx;
    }
    public static  class FieldItemDate
    {
        public int classNameIdx;
        public int fieldNameIdx;
        public int typeIdx;

    }
}
