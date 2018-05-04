package lz.android.shell.advmpformat;
import lz.util.BitConverter;

public class StringItem {

    /**
     * 字符串中字符个数。
     */
    public int size;

    /**
     * 字符数组。
     */
    public byte[] str;

    /**
     * 用于测试
     * @return
     */
    public  String strs;
    public  byte[] getBytes()
    {
        byte[] result = new byte[size +4];//用前4字节存储字符串大小
        result[0] = (byte) (size);
        result[1] = (byte) (size >> 8);
        result[2] = (byte) (size >> 16);
        result[3] = (byte) (size >> 24);
        for(int i=0;i<size;i++)
        {
            result[4+i]=str[i];
        }
        return result;
    }

}
